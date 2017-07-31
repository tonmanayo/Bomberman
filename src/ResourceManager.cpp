//
// Created by Tony MACK on 2017/07/31.
//

#include "../inc/ResourceManager.hpp"

TextureCache ResourceManager::_textureCache;

GLTexture ResourceManager::getTexture(const std::string &textureString) {
    return _textureCache.getTexture(textureString);
}
