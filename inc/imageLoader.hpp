//
// Created by Tony MACK on 2017/07/29.
//

#ifndef BOMBERMAN_IMAGELOADER_HPP
#define BOMBERMAN_IMAGELOADER_HPP


#include <string>
#include "GLTexture.hpp"
#include <GL/glew.h>

namespace WTCEngine {
    class imageLoader {
    public:

        static GLTexture loadPNG(std::string filepath);
    };

}
#endif //BOMBERMAN_IMAGELOADER_HPP
