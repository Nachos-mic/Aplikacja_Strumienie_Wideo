#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "videorecorder.h"
#include <QComboBox>

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
    QComboBox* cameraComboBox = new QComboBox;
    QComboBox* filterComboBox = new QComboBox;
    QString box_style_settings = "QComboBox {background: white; border: 1px solid gray; border-radius: 3px; padding: 1px 1px 1px 1px; min-width: 6em; margin-right: 3px;} "
                                 "QComboBox QListView {background: white; border: 1px solid gray;} "
                                 "QComboBox QListView::item {background: white; color: black;}"
                                 "QComboBox QListView::item:selected {background: lightblue;}";

    QString toolButtons_style_settings ="QToolButton {background: white ;border-style: outset; width: 100px; border-width: 1px; border-radius: 4px; border-color: black; margin-right: 3px; }";
};
#endif
