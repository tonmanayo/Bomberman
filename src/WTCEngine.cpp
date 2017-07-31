//
// Created by Tony MACK on 2017/07/31.
//

#include "../inc/WTCEngine.hpp"
namespace WTCEngine {
    int init() {
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        return 0;
    }
}