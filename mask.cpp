#include "mask.h"

#define mask_size 3

Mask::Mask(QObject *parent)
{



}

Mask::~Mask(){

}

const std::vector<float>& Mask::getMask() {
    static std::vector<float> mask;
    switch(chosen_mask) {
    case 1:
        mask = {
            -1, -2, -1,
            0,  0,  0,
            1,  2,  1
        };
        break;

    case 2:
        mask = {
            -1, 0, 1,
            -2, 0, 2,
            -1, 0, 1
        };
        break;

    case 3:
        mask = {
            -1, -1, -1,
            -1,  8, -1,
            -1, -1, -1
        };
        break;

    case 4:
        mask = {
            -2, -1,  0,
            -1,  1,  1,
            0,  1,  2
        };
        break;
    }

    return mask;
}

QImage Mask::applyMaskToFrame(QImage &frame){

    qDebug() << "Mask is Applied";

    QImage result = frame.copy();

    QImage gray_image = frame.convertToFormat(QImage::Format_Grayscale8);
    const uchar *ptr_src = gray_image.constBits();
    uchar *ptr_dst = result.bits();

    int width = frame.width();
    int height = frame.height();
    int offset = mask_size / 2;

    const auto&  mask = getMask();
    std::vector<uchar> temp_buff(width * height);

    for(int y = offset ; y < height ; ++y){
        for(int x = offset ; x < width ; ++x){

            float sum = 0.0f;

            for (int i = -offset; i <= offset; ++i) {
                for (int j = -offset; j <= offset; ++j) {
                    int x_src = x + j;
                    int y_src = y + i;
                    int index_mask = (i + offset) * mask_size + (j + offset);

                    sum += static_cast<float>(ptr_src[y_src * width + x_src]) * mask[index_mask];
                }
            }

            sum = std::clamp(sum, 0.0f, 255.0f);
            temp_buff[y * width + x] = static_cast<uchar>(sum);
        }
    }

    for (int i = 0; i < width * height; ++i) {
        ptr_dst[i * 4] = temp_buff[i];
        ptr_dst[i * 4 + 1] = temp_buff[i];
        ptr_dst[i * 4 + 2] = temp_buff[i];
        ptr_dst[i * 4 + 3] = 255;
    }

    return result;
}



