#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    window.setWindowTitle("VideoRecorder 1.0");
    window.setMinimumHeight(480);
    window.setMinimumWidth(640);
    window.show();
    return a.exec();
}
