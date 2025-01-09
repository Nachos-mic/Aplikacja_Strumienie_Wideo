#ifndef MASK_H
#define MASK_H

#include <QObject>
#include <QVideoSink>
#include <QVideoFrame>
#include <QImage>
#include <QDebug>
#include <stdio.h>
#include <memory>


extern std::vector<float> mask;
extern std::vector<float> custom_mask;

QVideoFrame applyMaskToFrame(const QVideoFrame &frame);
void setFilterMask(int index);
void setCustomFilterMask(std::vector<float> custom_mask);


#endif // MASK_H
