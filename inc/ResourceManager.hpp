//
// Created by Tony MACK on 2017/07/31.
//

#ifndef BOMBERMAN_RESOURCEMANAGER_HPP
#define BOMBERMAN_RESOURCEMANAGER_HPP


#include "TextureCache.hpp"

class ResourceManager {
public:
    static GLTexture getTexture(const std::string &textureString);
private:
    static TextureCache _textureCache;
};


#endif //BOMBERMAN_RESOURCEMANAGER_HPP
