#ifndef MASK_H
#define MASK_H

#include <QObject>
#include <QVideoSink>
#include <QVideoFrame>
#include <QImage>
#include <QDebug>
#include <stdio.h>
#include <memory>

class Mask : public QObject
{
    Q_OBJECT
public:
    explicit Mask(QObject *parent = nullptr);
    ~Mask();

public slots:
    static QImage applyMaskToFrame(QImage &frame);
    static void setFilterMask(int index){chosen_mask = index;};

private:
    static const std::vector<float>& getMask();
    static const int getFilterMask(){return chosen_mask;};

   static inline int chosen_mask = 0;
};

#endif // MASK_H
