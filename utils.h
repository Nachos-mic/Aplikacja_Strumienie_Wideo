#ifndef UTILS_H
#define UTILS_H
#include <QObject>
#include <QStandardPaths>

class Utils {
private:
    static QString media_path;

public:
    Utils();
    static QString getMediaPath();
    static void setMediaPath(QString path);
};

#endif // UTILS_H
