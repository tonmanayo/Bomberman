//
// Created by Tony MACK on 2017/07/29.
//

#ifndef BOMBERMAN_PICOPNG_HPP
#define BOMBERMAN_PICOPNG_HPP

#include <vector>
namespace WTCEngine {
    extern int decodePNG(std::vector<unsigned char> &out_image, unsigned long &image_width, unsigned long &image_height,
                         const unsigned char *in_png, size_t in_size, bool convert_to_rgba32 = true);
}
#endif //BOMBERMAN_PICOPNG_HPP
