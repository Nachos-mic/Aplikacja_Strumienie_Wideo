#ifndef VIDEORECORDER_H
#define VIDEORECORDER_H

#include <QObject>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QMediaDevices>
#include <QVideoSink>
#include <QVideoFrame>
#include <QImage>
#include <QDateTime>
#include <QDir>
#include <QStandardPaths>
#include <QTimer>
#include <QBuffer>
#include <QMediaRecorder>
#include <QMediaFormat>
#include <QUrl>
#include <QMediaPlayer>

#include "utils.h"
#include "mask.h"

class VideoRecorder : public QObject
{
    Q_OBJECT

public:
    explicit VideoRecorder(QObject *parent = nullptr);
    ~VideoRecorder();

    QStringList getCameraList() const { return tab_camera_list; }
    QString getFrame() const;
    bool isRecording() const { return is_recording; }

    QString getCurrentPath() const { return Utils::getMediaPath(); }
    void setCurrentPath(const QString &path) {qDebug() << path; Utils::setMediaPath(path);};
    void playVideo(const QString path);

    bool getPlayerState(){return is_video;};
    void setPlayerState(bool state){is_video = state;};

    QStringList getMaskList() const { return tab_mask_list;}

public slots:
    void setCamera(int index);
    void setMask(int index);
    void captureFrame();
    void startStopRecording();
    // void changeFilterState();

signals:
    void cameraListChanged();
    void frameChanged(const QString& frame);
    void recordingStatusChanged(bool is_recording);


private slots:
    void handleFrameChanged(const QVideoFrame &frame);
    void updateFrame();

private:
    void updateCameraList();
    bool configureMediaRecorder();
    void recordVideo();

    QMediaCaptureSession capture_session;
    QMediaCaptureSession video_session;

    QString frame;
    QVideoFrame last_frame;

    QMediaRecorder* ptr_media_recorder;
    QCamera* ptr_camera;
    QVideoSink* ptr_video_sink;

    QTimer* ptr_frame_timer;
    QFile* ptr_video_file;

    QMediaPlayer*ptr_media_player;
    QVideoSink* ptr_video_player_sink;

    QStringList tab_camera_list;
    QList<QCameraDevice> tab_camera_devices;
    QStringList tab_mask_list = {"None" , "Blur" , "Sharpen" , "Left Sobel" , "Right Sobel" , "Outline","Emboss"  };

    bool is_recording = true;
    bool is_video = false;
    bool filter_set = false;

    int camera_list_size = 0;

};

#endif // VIDEORECORDER_H
