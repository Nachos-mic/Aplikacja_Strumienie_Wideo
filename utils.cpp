#include "utils.h"

QString Utils::media_path = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/VideoRecorder";

Utils::Utils() {}

QString Utils::getMediaPath() {
    return media_path;
}

void Utils::setMediaPath(QString path) {
    media_path = path;
}
