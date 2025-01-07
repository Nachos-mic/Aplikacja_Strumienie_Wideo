#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile style_sheet_file("style/MyWatch.qss");
    style_sheet_file.open(QFile::ReadOnly);
    QString style_sheet = QLatin1String(style_sheet_file.readAll());
    app.setStyleSheet(style_sheet);


    MainWindow window;
    window.setWindowTitle("VideoRecorder 1.0");
    window.setMinimumHeight(480);
    window.setMinimumWidth(640);
    window.show();
    return app.exec();
}
