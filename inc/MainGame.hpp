//
// Created by Tony Mack on 2017/07/28.
//

#ifndef BOMBERMAN_MAINGAME_HPP
#define BOMBERMAN_MAINGAME_HPP

#include <SDL.h>
//#include <GL/glew.h>
#include "../inc/Sprite.hpp"
#include "GLSLProgram.hpp"
#include <OpenGL/gl3.h>
#include "GLTexture.hpp"
#include "Window.hpp"

enum GameState {PLAY, EXIT};

class MainGame {

public:
    MainGame();
    virtual ~MainGame();

    void run();

private:
    Window              _window;
    unsigned int        _ScreenWidth;
    unsigned int        _ScreenHeight;
    GameState           _gameState;
    std::vector<Sprite *> _sprites;
    GLSLProgram         _colorProgram;
    float               _time;
    float               _fps;
    float               _frameTime;
    float               _maxFPS;
    void initGame();
    void initShaders();
    void processInput();
    void gameLoop();
    void drawGame();
    void calculateFPS();
};


#endif //BOMBERMAN_MAINGAME_HPP
