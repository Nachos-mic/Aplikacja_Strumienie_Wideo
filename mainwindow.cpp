#include "mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , videoRecorder(new VideoRecorder(this))
{

    ui->setupUi(this);

    ui->cameraComboBox->addItems(videoRecorder->getCameraList());
    ui->filterComboBox->addItems(videoRecorder->getMaskList());


    connect(videoRecorder, &VideoRecorder::frameChanged,
            this, &MainWindow::updateFrame);

    connect(videoRecorder, &VideoRecorder::recordingStatusChanged,
            this, &MainWindow::updateRecordingStatus);

    connect(videoRecorder, &VideoRecorder::cameraListChanged,
            this, &MainWindow::updateCameraList);

    connect(videoRecorder, &VideoRecorder::setPlayerFalse,
            this, &MainWindow::setPlayerOff);

    connect(ui->cameraComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            videoRecorder, &VideoRecorder::setCamera);

    connect(ui->captureFrameButton, &QPushButton::clicked,
            videoRecorder, &VideoRecorder::captureFrame);

    connect(ui->captureVideoButton, &QPushButton::clicked,
            videoRecorder, &VideoRecorder::startStopRecording);

    connect(ui->filterComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            videoRecorder, &VideoRecorder::setMask);

    connect(ui->setPathButton, &QPushButton::clicked,
            this, [this]() {
                QString dir = QFileDialog::getExistingDirectory(this, "Select Directory", videoRecorder->getCurrentPath());

                if (!dir.isEmpty()) {
                    videoRecorder->setCurrentPath(dir);
                }
            });

    connect(ui->playVideoButton, &QPushButton::clicked,
            this, [this]() {
                if(videoRecorder->getPlayerState() == false){

                    QString file = QFileDialog::getOpenFileName(this,"Select Directory",videoRecorder->getCurrentPath());
                    if (!file.isEmpty()) {
                        videoRecorder->playVideo(file);
                        ui->cameraComboBox->setEnabled(false);
                        ui->captureFrameButton->setEnabled(false);
                        ui->captureVideoButton->setEnabled(false);
                        ui->filterComboBox->setEnabled(false);
                        ui->playVideoButton->setText("Stop playing");
                    }
                }else{
                    videoRecorder->playVideo("");
                    ui->cameraComboBox->setEnabled(true);
                    ui->captureFrameButton->setEnabled(true);
                    ui->captureVideoButton->setEnabled(true);
                    ui->filterComboBox->setEnabled(true);
                    ui->playVideoButton->setText("Play Video");
                }
            });

    if (ui->cameraComboBox->count() > 0) {
        videoRecorder->setCamera(0);
    }

    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateFrame(const QString& frame)
{
    QString base64_frame = frame.mid(frame.indexOf(',') + 1);
    QByteArray image_frame = QByteArray::fromBase64(base64_frame.toLatin1());
    QPixmap pixmap;
    pixmap.loadFromData(image_frame);

    ui->label->setPixmap(pixmap.scaled(ui->label->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::updateRecordingStatus(bool is_recording)
{

    ui->captureVideoButton->setText(is_recording ? "Stop Recording" : "Capture Video");

    if(is_recording){
        ui->cameraComboBox->setEnabled(false);
        ui->filterComboBox->setEnabled(false);
        ui->playVideoButton->setEnabled(false);
    }else{
        ui->cameraComboBox->setEnabled(true);
        ui->filterComboBox->setEnabled(true);
        ui->playVideoButton->setEnabled(true);
    }
}

void MainWindow::setPlayerOff()
{
    ui->cameraComboBox->setEnabled(true);
    ui->captureFrameButton->setEnabled(true);
    ui->captureVideoButton->setEnabled(true);
    ui->filterComboBox->setEnabled(true);
    ui->playVideoButton->setText("Play Video");
}

void MainWindow::updateCameraList()
{
    ui->cameraComboBox->clear();
    ui->cameraComboBox->addItems(videoRecorder->getCameraList());
}
