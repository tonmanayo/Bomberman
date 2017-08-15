//
// Created by Tony MACK on 2017/07/29.
//

#include <iostream>
#include "../inc/imageLoader.hpp"
#include "../inc/picoPNG.hpp"
#include "../inc/IOManager.hpp"
#include "../inc/ErrorHandle.hpp"
namespace WTCEngine {
    GLTexture imageLoader::loadPNG(std::string filepath) {
        GLTexture texture = {};
        std::vector<unsigned char> out;
        std::vector<unsigned char> in;

        unsigned long width, height;

        try {
            if (IOManager::readFileToBuffer(filepath, in) == false) {
                throw ErrorHandle("Failed to load PNG to buffer");
            }

            int err = decodePNG(out, width, height, &(in[0]), in.size(), true);
            if (err != 0)
                throw ErrorHandle("decode failed with error: " + std::to_string(err));

            glGenTextures(1, &(texture.id));

            glBindTexture(GL_TEXTURE_2D, texture.id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            //really cool distance shit
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);

            texture.width = width;
            texture.height = height;
            return texture;

        } catch (ErrorHandle errorHandle) {
            std::cout << errorHandle.what() << std::endl;
            return texture;
        }
    }
}
