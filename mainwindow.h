#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "videorecorder.h"

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
    void setPlayerOff();
    void updateCameraList();

private:
    Ui::MainWindow *ui;
    VideoRecorder *videoRecorder;
};
#endif
