//
// Created by Tony MACK on 2017/07/31.
//

#ifndef BOMBERMAN_WINDOW_HPP
#define BOMBERMAN_WINDOW_HPP

#include <GL/glew.h>
#include <SDL.h>

#include <string>

namespace WTCEngine {
    enum windowFlags {
        INVISABLE = 0x1,
        FULLSCREEN = 0x2,
        BOARDERLESS = 0x4
    };

    class Window {
    private:
        SDL_Window *_SDLwindow;
        int _screenWidth;
        int _screenHeight;
    public:
        Window();

        virtual ~Window();

        void swapBuffer();

        int create(std::string windowName, int ScreenWidth, int ScreenHeight, unsigned int currentFlags);
    };

}
#endif //BOMBERMAN_WINDOW_HPP
