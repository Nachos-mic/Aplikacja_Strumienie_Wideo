#ifndef MASK_H
#define MASK_H

#include <QObject>
#include <QVideoSink>
#include <QVideoFrame>
#include <QImage>
#include <QDebug>
#include <memory>

class Mask : public QObject
{
    Q_OBJECT
public:
    explicit Mask(QObject *parent = nullptr);
    ~Mask();

public slots:
    static void applyMaskToFrame(QImage &frame);


};

#endif // MASK_H
