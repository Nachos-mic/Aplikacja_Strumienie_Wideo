#include "mask.h"

#define mask_size 3

std::vector<float> mask;

void setFilterMask(int index){
    switch(index) {

    case 1:
        mask = {
            0.015625, 0.0625, 0.015625,
            0.0625, 0.6875, 0.0625,
            0.015625, 0.0625, 0.015625
        };
        break;

    case 2:
        mask = {
            0, -1, 0,
            -1, 5, -1,
            0, -1, 0
        };
        break;

    case 3:
        mask = {
            -1, -2, -1,
            0,  0,  0,
            1,  2,  1
        };
        break;

    case 4:
        mask = {
            -1, 0, 1,
            -1, 0, 1,
            -1, 0, 1
        };
        break;

    case 5:
        mask = {
            -1, -1, -1,
            -1,  8, -1,
            -1, -1, -1
        };
        break;

    case 6:
        mask = {
            -2, -1,  0,
            -1,  1,  1,
            0,  1,  2
        };
        break;
    }
}

QVideoFrame applyMaskToFrame(const QVideoFrame &input_frame) {
    qDebug() << "Mask is Applied";

    QVideoFrame mod_frame = input_frame;

    if (!mod_frame.map(QVideoFrame::ReadOnly))
        return input_frame;

    QImage frame = mod_frame.toImage();
    mod_frame.unmap();

    QImage gray_image = frame.convertToFormat(QImage::Format_Grayscale8);
    const uchar *ptr_src = gray_image.constBits();
    int width = frame.width();
    int height = frame.height();
    int offset = mask_size / 2;

    std::vector<uchar> temp_buff(width * height);

    for(int y = offset; y < height; ++y) {
        for(int x = offset; x < width; ++x) {
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

    QVideoFrame result(QVideoFrameFormat(frame.size(), QVideoFrameFormat::Format_BGRA8888));

    if (!result.map(QVideoFrame::WriteOnly))
        return input_frame;

    uchar *ptr_dst = result.bits(0);
    for (int i = 0; i < width * height; ++i) {
        ptr_dst[i * 4] = temp_buff[i];
        ptr_dst[i * 4 + 1] = temp_buff[i];
        ptr_dst[i * 4 + 2] = temp_buff[i];
        ptr_dst[i * 4 + 3] = 255;
    }

    result.unmap();
    return result;
}



