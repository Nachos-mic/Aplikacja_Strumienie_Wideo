#include "videorecorder.h"
#include "mask.h"

VideoRecorder::VideoRecorder(QObject *parent)
    : QObject(parent)
    , ptr_camera(nullptr)
    , ptr_video_sink(new QVideoSink(this))
    , ptr_frame_timer(new QTimer(this))
    , is_recording(false)
{
    //Podłączenie metod dotyczących klatek wideo do wykrytej zmiany klatki wideo przez QVideoSink::videoFrameChanged

    connect(ptr_video_sink, &QVideoSink::videoFrameChanged,
            this, &VideoRecorder::handleFrameChanged);

    ptr_frame_timer->setInterval(33);
    connect(ptr_frame_timer, &QTimer::timeout, this, &VideoRecorder::updateFrame);
    ptr_frame_timer->start();

    //Podłączenie metody do aktualizacji listy kamer

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &VideoRecorder::updateCameraList);
    timer->start(500);

    //Ustawianie captureSession do pokazywania feedu z kamery

    capture_session.setVideoSink(ptr_video_sink);
    updateCameraList();

    if (!tab_camera_devices.isEmpty()) {
        setCamera(0);
    }
}

VideoRecorder::~VideoRecorder()
{
    if (ptr_camera) {
        ptr_camera->stop();
        delete ptr_camera;
    }
    ptr_frame_timer->stop();
}

QString VideoRecorder::getFrame() const
{
    return frame;
}

void VideoRecorder::handleFrameChanged(const QVideoFrame &frame)
{
    last_frame = filter_set ?  applyMaskToFrame(frame) : frame;
}

void VideoRecorder::updateFrame()
{
    if (last_frame.isValid()) {
        if (!last_frame.map(QVideoFrame::ReadOnly))
            return;

        QImage image = last_frame.toImage();
        last_frame.unmap();

        if (image.isNull())
            return;

        QByteArray byte_arr;
        {
            QBuffer buffer(&byte_arr);
            if (buffer.open(QIODevice::WriteOnly)) {
                image.save(&buffer, "JPEG", 30);
            }
        }

        QString new_frame = QString("data:image/jpeg;base64,%1").arg(QString::fromLatin1(byte_arr.toBase64()));
        frame = new_frame;
        emit frameChanged(frame);
    }
}

void VideoRecorder::updateCameraList()
{
    tab_camera_devices = QMediaDevices::videoInputs();
    tab_camera_list.clear();

    for (const QCameraDevice &device : tab_camera_devices) {
        tab_camera_list.append(device.description());
    }

    if(tab_camera_list.size() != camera_list_size){
        camera_list_size = tab_camera_list.size();
        setCamera(0);
        emit cameraListChanged();
    }
}

void VideoRecorder::setCamera(int index)
{
    if (index >= 0 && index < tab_camera_devices.size()) {
        if (ptr_camera) {
            ptr_camera->stop();
            delete ptr_camera;
        }

        ptr_frame_timer->stop();

       //Ustawianie rozdzielczości kamery.

        QList<QCameraFormat> camera_formats = tab_camera_devices.at(index).videoFormats();

        QCameraFormat selected_format;

        for (const QCameraFormat &format : camera_formats) {
            if (format.resolution().width() == 640 &&
                format.resolution().height() == 480) {
                selected_format = format;
                qDebug() << format.resolution();
                break;
            }
        }

        ptr_camera = new QCamera(tab_camera_devices.at(index), this);

        ptr_camera->setCameraFormat(selected_format);

        capture_session.setCamera(ptr_camera);

        //Uruchamianie kamery

        QTimer::singleShot(100, this, [this]() {
            if (ptr_camera) {
                ptr_camera->start();
            }
        });

        //Uruchamianie obsługi klatek na nowo

        QTimer::singleShot(200, this, [this]() {
            ptr_frame_timer->start();
        });
    }
}


void VideoRecorder::captureFrame()
{
    if (!last_frame.isValid()) return;

    QImage image = last_frame.toImage();

    if (image.isNull()) return;

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString filename = Utils::getMediaPath() + "/captured_img_" + timestamp + ".png";

    QDir().mkpath(Utils::getMediaPath());

    if (image.save(filename)) {
        qDebug() << "Frame saved to:" << filename;
    }
}



void VideoRecorder::startStopRecording() {

    if (is_recording) {
        is_recording = false;
        writer.release();
        emit recordingStatusChanged(false);
        qDebug() << "Stopped Recording";
        return;
    }

    qDebug() << "Started Recording";

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString filename = Utils::getMediaPath() + "/video_" + timestamp + ".mp4";

    if (!writer.open(filename.toStdString(),cv::VideoWriter::fourcc('m','p','4','v'),fps,cv::Size(last_frame.width(), last_frame.height()),true)) {
        qDebug() << "Failed to open video writer";
        return;
    }

    is_recording = true;
    emit recordingStatusChanged(true);

    //Obniżenie fps w wypadku nałożenia filtracji

    fps = filter_set ? 25 : fps;

    //Osobny wątek do nagrywania wideo (bez tego , program się posypie)

    QThread* recording_thread = QThread::create([this]() {
        while (is_recording && last_frame.isValid()) {
            QImage image = last_frame.toImage();
            if (image.format() != QImage::Format_RGB888) {
                image = image.convertToFormat(QImage::Format_RGB888);
            }

            cv::Mat mat(image.height(),image.width(),CV_8UC3,const_cast<uchar*>(image.bits()),image.bytesPerLine());

            cv::Mat corr_mat;
            cv::cvtColor(mat, corr_mat, cv::COLOR_RGB2BGR);
            writer.write(corr_mat);

            QThread::msleep(1000/fps);
        }
    });

    recording_thread->start();
}

void VideoRecorder::playVideo(QString path){

    if(is_video == false){

        filter_set = false;

        if (ptr_camera) {
            ptr_camera->stop();
            ptr_frame_timer->stop();
        }

        ptr_media_player = new QMediaPlayer(this);
        ptr_video_player_sink = new QVideoSink(this);

        ptr_media_player->setVideoSink(ptr_video_player_sink);

        connect(ptr_video_player_sink, &QVideoSink::videoFrameChanged,
                this, [this](const QVideoFrame &frame) {
                    if (!frame.isValid()) return;

                    QVideoFrame copy_frame = frame;
                    if (!copy_frame.map(QVideoFrame::ReadOnly))
                        return;

                    QImage image = copy_frame.toImage();
                    copy_frame.unmap();

                    if (image.isNull())
                        return;

                    QByteArray byte_arr;
                    QBuffer buffer(&byte_arr);
                    if (buffer.open(QIODevice::WriteOnly)) {
                        image.save(&buffer, "JPEG", 85);
                    }

                    QString new_frame = QString("data:image/jpeg;base64,%1").arg(QString::fromLatin1(byte_arr.toBase64()));
                    this->frame = new_frame;
                    emit frameChanged(this->frame);
                });

        connect(ptr_media_player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
            if (status == QMediaPlayer::EndOfMedia) {
                ptr_media_player->stop();
                is_video = false;
                ptr_media_player->setVideoSink(nullptr);
                delete ptr_media_player;
                ptr_media_player = nullptr;

                if (ptr_camera) {
                    ptr_camera->start();
                    ptr_frame_timer->start();
                }

                emit setPlayerFalse();
            }
        });

        is_video = true;
        ptr_media_player->setSource(QUrl::fromLocalFile(path));
        ptr_media_player->play();

    }else{

        ptr_media_player->stop();
        is_video = false;
        ptr_media_player->setVideoSink(nullptr);
        delete ptr_media_player;
        ptr_media_player = nullptr;

        if (ptr_camera) {
            ptr_camera->start();
            ptr_frame_timer->start();
        }
    }
}

void VideoRecorder::setMask(int index){
    if(index > 0){
        filter_set = true;
        setFilterMask(index);
    }else{
        filter_set = false;
    }

}

