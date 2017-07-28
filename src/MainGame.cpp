//
// Created by Tony Mack on 2017/07/28.
//

#include <iostream>
#include "../inc/MainGame.hpp"
#include "../inc/ErrorHandle.hpp"

MainGame::MainGame(): _window(nullptr), _ScreenHeight(768), _ScreenWidth(1024), _gameState(PLAY) {}

MainGame::~MainGame() {

}

void MainGame::run() {
    initGame();
    gameLoop();
}

void MainGame::initGame() {

    try {
        SDL_Init(SDL_INIT_EVERYTHING);
        _window = SDL_CreateWindow("Bomb Squad", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _ScreenWidth,
                                   _ScreenHeight, SDL_WINDOW_OPENGL);

        if (_window == nullptr)
            throw ErrorHandle("Error creating window");

        SDL_GLContext glContext = SDL_GL_CreateContext(_window);

        if (glContext == nullptr)
            throw ErrorHandle("could not create SDL GL context");

        GLenum error = glewInit();
        if (error != GLEW_OK)
            throw ErrorHandle("could not init Glew");

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        //background colour
        glClearColor(1.0f, 1.0f, 0.0f, 1.0);


    } catch (ErrorHandle errorHandle){
        errorHandle.what();
    }

}

void MainGame::processInput() {

    SDL_Event e;

    while(SDL_PollEvent(&e)){
        switch  (e.type) {
            case SDL_QUIT:
                _gameState = EXIT;
                break ;
            case SDL_MOUSEMOTION :
            std::cout << "x: " << e.motion.x << " y: " << e.motion.y << std::endl;
                break;
        }
    }
}

void MainGame::gameLoop() {
    while (_gameState != EXIT) {
        processInput();
        drawGame();
    }
}

void MainGame::drawGame() {
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SDL_GL_SwapWindow(_window);


}
