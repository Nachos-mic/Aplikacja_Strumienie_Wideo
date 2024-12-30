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
    QVideoFrame mod_frame = input_frame;
    if (!mod_frame.map(QVideoFrame::ReadOnly))
        return input_frame;
    QImage frame = mod_frame.toImage();
    mod_frame.unmap();

    int width = frame.width();
    int height = frame.height();
    int offset = mask_size / 2;

    // Nakładanie maski w kolorze musi się odbywać poprzez nakładanie jej na kanały RGB osobno , dlatego różne bufory tymczasowe

    std::vector<uchar> temp_buff_r(width * height);
    std::vector<uchar> temp_buff_g(width * height);
    std::vector<uchar> temp_buff_b(width * height);

    QImage img_frame = frame.convertToFormat(QImage::Format_RGB888);
    const uchar *ptr_src = img_frame.constBits();

    for(int y = offset; y < height - offset; ++y) {
        for(int x = offset; x < width - offset; ++x) {
            float sum_r = 0.0f;
            float sum_g = 0.0f;
            float sum_b = 0.0f;

            for (int i = -offset; i <= offset; ++i) {
                for (int j = -offset; j <= offset; ++j) {
                    int x_src = x + j;
                    int y_src = y + i;
                    int src_pixel = (y_src * width + x_src) * 3;  // 3 kanały koloru
                    int index_mask = (i + offset) * mask_size + (j + offset);

                    sum_r += static_cast<float>(ptr_src[src_pixel]) * mask[index_mask];
                    sum_g += static_cast<float>(ptr_src[src_pixel + 1]) * mask[index_mask];
                    sum_b += static_cast<float>(ptr_src[src_pixel + 2]) * mask[index_mask];
                }
            }
            int pixel_pos = y * width + x;
            temp_buff_r[pixel_pos] = static_cast<uchar>(std::clamp(sum_r, 0.0f, 255.0f));
            temp_buff_g[pixel_pos] = static_cast<uchar>(std::clamp(sum_g, 0.0f, 255.0f));
            temp_buff_b[pixel_pos] = static_cast<uchar>(std::clamp(sum_b, 0.0f, 255.0f));
        }
    }

    QVideoFrame result(QVideoFrameFormat(frame.size(), QVideoFrameFormat::Format_BGRA8888));
    if (!result.map(QVideoFrame::WriteOnly))
        return input_frame;

    uchar *ptr_dst = result.bits(0);
    for (int i = 0; i < width * height; ++i) {
        ptr_dst[i * 4] = temp_buff_b[i];     // Kanał Niebieski
        ptr_dst[i * 4 + 1] = temp_buff_g[i]; // Kanał Zielony
        ptr_dst[i * 4 + 2] = temp_buff_r[i]; // Kanał Czerwony
        ptr_dst[i * 4 + 3] = 255;            // Kanał Alpha
    }

    result.unmap();
    return result;
}



