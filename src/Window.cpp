//
// Created by Tony MACK on 2017/07/31.
//

#include <iostream>
#include "../inc/Window.hpp"
#include "../inc/ErrorHandle.hpp"
namespace WTCEngine {
    Window::Window() {


    }

    Window::~Window() {

    }

    int Window::create(std::string windowName, int ScreenWidth, int ScreenHeight, unsigned int currentFlags) {
        try {


            Uint32 flags = SDL_WINDOW_OPENGL;

            if (currentFlags & INVISABLE)
                flags |= SDL_WINDOW_HIDDEN;
            if (currentFlags & FULLSCREEN)
                flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
            if (currentFlags & BOARDERLESS)
                flags |= SDL_WINDOW_BORDERLESS;

            _SDLwindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          ScreenWidth,
                                          ScreenHeight, flags);

            if (_SDLwindow == nullptr)
                throw ErrorHandle("Error creating window");

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

            SDL_GLContext glContext = SDL_GL_CreateContext(_SDLwindow);

            GLuint vertexArrayID;
            glGenVertexArrays(1, &vertexArrayID);
            glBindVertexArray(vertexArrayID);

            if (glContext == nullptr)
                throw ErrorHandle("could not create SDL GL context");


            //background colour
            glClearColor(1.0f, 1.0f, 0.0f, 1.0);
            //set vsync
            SDL_GL_SetSwapInterval(0);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            std::printf("*** OpenGL Version: %s\n", glGetString(GL_VERSION));

            return 0;


        } catch (ErrorHandle errorHandle) {
            std::cout << errorHandle.what() << std::endl;
            return -1;
        }

    }

    void Window::swapBuffer() {
        SDL_GL_SwapWindow(_SDLwindow);
    }
}