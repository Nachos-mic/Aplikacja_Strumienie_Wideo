#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "videorecorder.h"
#include "maskpopup.h"
#include <QComboBox>
#include <QIcon>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateFrame(const QString& frame);
    void updateRecordingStatus(bool is_recording);
    void updatePlayerStatus(bool is_paused);
    void setPlayerOff();
    void updateCameraList();

    void updateVidDuration(qint64 duration);
    void updateVidPosition(qint64 position);
    void handleSliderMoved(int slid_position);

    void openPopUp();

private:
    Ui::MainWindow *ui;
    MaskPopUp *popUp;
    VideoRecorder *videoRecorder;
    QComboBox* cameraComboBox = new QComboBox;
    QComboBox* filterComboBox = new QComboBox;
};
#endif
