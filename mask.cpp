#include "mask.h"

#define mask_size 3

Mask::Mask(QObject *parent)
{



}

Mask::~Mask(){

}

void Mask::applyMaskToFrame(QImage &frame){

    qDebug() << "Mask is Applied";

    // QImage result = frame.copy();

    // QImage gray_image = frame.convertToFormat(QImage::Format_Grayscale8);
    // const uchar *ptr_src = gray_image.constBits();
    // uchar *ptr_dst = result.bits();

    // int width = frame.width();
    // int height = frame.height();
    // int offset = mask_size / 2;

    // for(int x = offset ; x < height ; ++x){
    //     for(int y = offset ; y < width ; ++y){

    //     }
    // }

}


