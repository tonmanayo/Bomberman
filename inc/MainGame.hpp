//
// Created by Tony Mack on 2017/07/28.
//

#ifndef BOMBERMAN_MAINGAME_HPP
#define BOMBERMAN_MAINGAME_HPP

#include <SDL.h>

#include "../inc/Sprite.hpp"
#include "GLSLProgram.hpp"
#include <OpenGL/gl3.h>
#include "GLTexture.hpp"
#include "Window.hpp"
#include "Camera2D.hpp"
#include "SpriteBatch.hpp"

enum GameState {
        PLAY, EXIT
    };

    class MainGame {

    public:
        MainGame();

        virtual ~MainGame();

        void run();

    private:
        WTCEngine::Window _window;
        unsigned int                     _ScreenWidth;
        unsigned int                     _ScreenHeight;
        GameState                        _gameState;
        WTCEngine::GLSLProgram           _colorProgram;
        float                            _time;
        float                            _fps;
        float                            _frameTime;
        float                            _maxFPS;
        WTCEngine::Camera2D              _camera2D;
        WTCEngine::SpriteBatch           _spriteBatch;

        void initGame();

        void initShaders();

        void processInput();

        void gameLoop();

        void drawGame();

        void calculateFPS();
    };


#endif //BOMBERMAN_MAINGAME_HPP
