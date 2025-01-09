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

QVideoFrame applyMaskToFrame(const QVideoFrame &frame);
void setFilterMask(int index);
// void setCustomMask(std::vector<float> set_mask){mask = set_mask;}


#endif // MASK_H
