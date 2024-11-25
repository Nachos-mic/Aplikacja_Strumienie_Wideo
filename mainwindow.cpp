#include "mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , videoRecorder(new VideoRecorder(this))
{
    ui->setupUi(this);

    ui->cameraComboBox->addItems(videoRecorder->getCameraList());

    connect(videoRecorder, &VideoRecorder::frameChanged,
            this, &MainWindow::updateFrame);

    connect(videoRecorder, &VideoRecorder::recordingStatusChanged,
            this, &MainWindow::updateRecordingStatus);

    connect(ui->cameraComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            videoRecorder, &VideoRecorder::setCamera);

    connect(ui->captureFrameBUtton, &QPushButton::clicked,
            videoRecorder, &VideoRecorder::captureFrame);

    connect(ui->captureVideoButton, &QPushButton::clicked,
            videoRecorder, &VideoRecorder::startStopRecording);

    connect(ui->filterButton, &QPushButton::clicked,
            videoRecorder, &VideoRecorder::changeFilterState);

    connect(ui->setPathButton, &QPushButton::clicked,
            this, [this]() {
                QString dir = QFileDialog::getExistingDirectory(this,
                                                                "Select Directory",
                                                                videoRecorder->getCurrentPath());
                if (!dir.isEmpty()) {
                    videoRecorder->setCurrentPath(dir);
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

void MainWindow::updateFrame(const QString& frameData)
{
    QString base64Data = frameData.mid(frameData.indexOf(',') + 1);
    QByteArray imageData = QByteArray::fromBase64(base64Data.toLatin1());
    QPixmap pixmap;
    pixmap.loadFromData(imageData);

    ui->label->setPixmap(pixmap.scaled(ui->label->size(),
                                       Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::updateRecordingStatus(bool isRecording)
{
    ui->captureVideoButton->setText(isRecording ? "Stop Recording" : "Capture Video");
}
