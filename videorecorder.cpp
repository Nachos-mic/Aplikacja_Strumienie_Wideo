#include "videorecorder.h"

VideoRecorder::VideoRecorder(QObject *parent)
    : QObject(parent)
    , ptr_camera(nullptr)
    , ptr_video_sink(new QVideoSink(this))
    , ptr_frame_timer(new QTimer(this))
    , ptr_media_recorder(new QMediaRecorder(this))
    , is_recording(false)
{
    connect(ptr_media_recorder, &QMediaRecorder::errorOccurred,
            this, [this](QMediaRecorder::Error error, const QString &errorString) {
                qDebug() << "Recording error:" << error << errorString;
            });

    connect(ptr_media_recorder, &QMediaRecorder::recorderStateChanged,
            this, [this](QMediaRecorder::RecorderState state) {
                qDebug() << "Recorder state changed to:" << state;
            });

    connect(ptr_video_sink, &QVideoSink::videoFrameChanged,
            this, &VideoRecorder::handleFrameChanged);

    ptr_frame_timer->setInterval(33);
    connect(ptr_frame_timer, &QTimer::timeout, this, &VideoRecorder::updateFrame);
    ptr_frame_timer->start();

    capture_session.setVideoSink(ptr_video_sink);
    updateCameraList();

    if (!tab_camera_devices.isEmpty()) {
        ptr_camera = new QCamera(tab_camera_devices.first(), this);
        capture_session.setCamera(ptr_camera);
        ptr_camera->start();
    }
    configureMediaRecorder();
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
    last_frame = frame;
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

        QByteArray byteArray;
        {
            QBuffer buffer(&byteArray);
            if (buffer.open(QIODevice::WriteOnly)) {
                image.save(&buffer, "JPEG", 25);
            }
        }

        QString new_frame = QString("data:image/jpeg;base64,%1").arg(QString::fromLatin1(byteArray.toBase64()));
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

    emit cameraListChanged();
}

void VideoRecorder::setCamera(int index)
{
    if (index >= 0 && index < tab_camera_devices.size()) {
        if (ptr_camera) {
            ptr_camera->stop();
            delete ptr_camera;
        }

        ptr_camera = new QCamera(tab_camera_devices.at(index), this);
        capture_session.setCamera(ptr_camera);

        QTimer::singleShot(100, this, [this]() {
            if (ptr_camera) {
                ptr_camera->start();
            }
        });

        ptr_frame_timer->stop();
        QTimer::singleShot(200, this, [this]() {
            ptr_frame_timer->start();
        });
    }
}


void VideoRecorder::captureFrame()
{
    if (!last_frame.isValid()) return;

    QVideoFrame frame = last_frame;
    if (!frame.map(QVideoFrame::ReadOnly)) return;

    QImage image = frame.toImage();
    frame.unmap();

    if (image.isNull()) return;

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString filename = Utils::getMediaPath() + "/captured_img_" + timestamp + ".png";

    QDir().mkpath(Utils::getMediaPath());

    if (image.save(filename)) {
        qDebug() << "Frame saved to:" << filename;
    }
}

bool VideoRecorder::configureMediaRecorder()
{
    QMediaFormat format;
    format.setFileFormat(QMediaFormat::MPEG4);
    format.setVideoCodec(QMediaFormat::VideoCodec::H264);

    ptr_media_recorder->setMediaFormat(format);
    ptr_media_recorder->setQuality(QMediaRecorder::HighQuality);
    capture_session.setRecorder(ptr_media_recorder);

    return true;
}

void VideoRecorder::startStopRecording()
{
    if (is_recording) {
        ptr_media_recorder->stop();
        is_recording = false;
        emit recordingStatusChanged(false);
    } else {
        QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
        QString filename = Utils::getMediaPath() + "/video_" + timestamp + ".mp4";
        ptr_media_recorder->setOutputLocation(QUrl::fromLocalFile(filename));

        qDebug() << "Recording to: " << filename;
        ptr_media_recorder->record();
        is_recording = true;
        emit recordingStatusChanged(true);
    }
}
