//
// Created by Tony Mack on 2017/07/28.
//

#ifndef BOMBERMAN_MAINGAME_HPP
#define BOMBERMAN_MAINGAME_HPP

#include <sdl.h>
#include <GL/glew.h>

enum GameState {PLAY, EXIT};

class MainGame {

public:
    MainGame();
    virtual ~MainGame();

    void run();


private:
    SDL_Window *_window;
    unsigned int _ScreenWidth;
    unsigned int _ScreenHeight;
    GameState _gameState;

    void initGame();
    void processInput();
    void gameLoop();
    void drawGame();




};


#endif //BOMBERMAN_MAINGAME_HPP
