#include "mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , popUp(new MaskPopUp(this))
    , videoRecorder(new VideoRecorder(this))
{

    ui->setupUi(this);

    ui->toolBar->addWidget(cameraComboBox);
    ui->toolBar->addWidget(filterComboBox);

    cameraComboBox->addItems(videoRecorder->getCameraList());
    filterComboBox->addItems(videoRecorder->getMaskList());

    ui->captureFrameButton->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->playVideoButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->setPathButton->setIcon(style()->standardIcon(QStyle::SP_DirLinkIcon));

    ui->videoPlayerWidget->setVisible(false);
    ui->playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

    //Połączenia z VideoRecorder

    connect(videoRecorder, &VideoRecorder::frameChanged,
            this, &MainWindow::updateFrame);

    connect(videoRecorder, &VideoRecorder::recordingStatusChanged,
            this, &MainWindow::updateRecordingStatus);

    connect(videoRecorder, &VideoRecorder::startPauseChanged,
            this, &MainWindow::updatePlayerStatus);

    connect(videoRecorder, &VideoRecorder::cameraListChanged,
            this, &MainWindow::updateCameraList);

    connect(videoRecorder, &VideoRecorder::setPlayerFalse,
            this, &MainWindow::setPlayerOff);

    connect(videoRecorder, &VideoRecorder::videoDurationChanged,
            this, &MainWindow::updateVidDuration);

    connect(videoRecorder, &VideoRecorder::videoPositionChanged,
            this, &MainWindow::updateVidPosition);


    //Połączenia UI

    connect(ui->videoSlider, &QSlider::sliderMoved,
            this, &MainWindow::handleSliderMoved);

    connect(cameraComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            videoRecorder, &VideoRecorder::setCamera);

    connect(ui->captureFrameButton, &QAction::triggered,
            videoRecorder, &VideoRecorder::captureFrame);

    connect(ui->captureVideoButton, &QAction::triggered,
            videoRecorder, &VideoRecorder::startStopRecording);

    connect(ui->playPauseButton, &QPushButton::clicked,
            videoRecorder, &VideoRecorder::playPauseVideo);

    connect(ui->setCustomMask, &QAction::triggered,
            this, &MainWindow::openPopUp);

    connect(ui->setCustomMask, &QAction::triggered,
            this, &MainWindow::openPopUp);

    connect(filterComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            videoRecorder, &VideoRecorder::setMask);

    connect(ui->setPathButton, &QAction::triggered,
            this, [this]() {
                QString dir = QFileDialog::getExistingDirectory(this, "Select Directory", videoRecorder->getCurrentPath());

                if (!dir.isEmpty()) {
                    videoRecorder->setCurrentPath(dir);
                }
            });

    connect(ui->playVideoButton, &QAction::triggered,
            this, [this]() {
                if(videoRecorder->getPlayerState() == false){

                    QString file = QFileDialog::getOpenFileName(this,"Select Directory",videoRecorder->getCurrentPath());
                    if (!file.isEmpty()) {
                        videoRecorder->playVideo(file);
                        cameraComboBox->setEnabled(false);
                        ui->captureFrameButton->setEnabled(false);
                        ui->captureVideoButton->setEnabled(false);
                        ui->setCustomMask->setEnabled(false);
                        filterComboBox->setEnabled(false);
                        ui->playVideoButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
                        ui->videoPlayerWidget->setVisible(true);

                    }
                }else{
                    videoRecorder->playVideo("");
                    cameraComboBox->setEnabled(true);
                    ui->captureFrameButton->setEnabled(true);
                    ui->captureVideoButton->setEnabled(true);
                    ui->setCustomMask->setEnabled(true);
                    filterComboBox->setEnabled(true);
                    ui->playVideoButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
                    ui->videoPlayerWidget->setVisible(false);

                }
            });

    if (cameraComboBox->count() > 0) {
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

    if(is_recording){
        cameraComboBox->setEnabled(false);
        filterComboBox->setEnabled(false);
        ui->playVideoButton->setEnabled(false);
        ui->setCustomMask->setEnabled(false);
    }else{
        cameraComboBox->setEnabled(true);
        filterComboBox->setEnabled(true);
        ui->playVideoButton->setEnabled(true);
        ui->setCustomMask->setEnabled(true);
    }
}

void MainWindow::updatePlayerStatus(bool is_paused)
{
    if(is_paused){
        ui->playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }else{
        ui->playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
}

void MainWindow::setPlayerOff()
{
    cameraComboBox->setEnabled(true);
    ui->captureFrameButton->setEnabled(true);
    ui->captureVideoButton->setEnabled(true);
    ui->setCustomMask->setEnabled(true);
    filterComboBox->setEnabled(true);
    ui->playVideoButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->videoPlayerWidget->setVisible(false);

}

void MainWindow::updateCameraList()
{
    cameraComboBox->clear();
    cameraComboBox->addItems(videoRecorder->getCameraList());
}

void MainWindow::updateVidPosition(qint64 position){

    ui->videoSlider->setValue(position);
}

void MainWindow::updateVidDuration(qint64 duration){

    ui->videoSlider->setRange(0,duration);
}

void MainWindow::handleSliderMoved(int slid_position){
    videoRecorder->setPlayerPosition(slid_position);
}

void MainWindow::openPopUp()
{
   popUp->show();
}


