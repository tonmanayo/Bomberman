//
// Created by Tony MACK on 2017/07/31.
//

#include <iostream>
#include "../inc/TextureCache.hpp"
#include "../inc/imageLoader.hpp"
namespace WTCEngine {
    GLTexture TextureCache::getTexture(std::string texturePath) {
        auto mapit = _texttureMap.find(texturePath);

        if (mapit == _texttureMap.end()) {
            GLTexture newTexture = imageLoader::loadPNG(texturePath);
            _texttureMap.insert(make_pair(texturePath, newTexture));

            std::cout << "Loaded cache texture\n";
            return newTexture;
        }
        std::cout << "Loaded texture\n";

        return mapit->second;
    }
}