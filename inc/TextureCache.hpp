//
// Created by Tony MACK on 2017/07/31.
//

#ifndef BOMBERMAN_TEXTURECACHE_HPP
#define BOMBERMAN_TEXTURECACHE_HPP


#include <map>
#include "GLTexture.hpp"
#include <string>

namespace WTCEngine {
    class TextureCache {
    private:
        std::map<std::string, GLTexture> _texttureMap;
    public:
        GLTexture getTexture(std::string);

    };
}

#endif //BOMBERMAN_TEXTURECACHE_HPP
