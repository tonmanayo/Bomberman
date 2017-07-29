//
// Created by Tony Mack on 2017/07/28.
//

#include <iostream>
#include "../inc/MainGame.hpp"
#include "../inc/ErrorHandle.hpp"
#include "../inc/imageLoader.hpp"

MainGame::MainGame(): _time(0), _window(nullptr), _ScreenHeight(768), _ScreenWidth(1024), _gameState(PLAY) {}

MainGame::~MainGame() {}

void MainGame::run() {
    initGame();
    _sprite.init(-1.0f, -1.0f, 2.0f, 2.0f);
    _PlayerTexture = imageLoader::loadPNG("resources/sprites/Enemys/Enemy_Snowman1.png");
    gameLoop();
}

void MainGame::initGame() {

    try {
        SDL_Init(SDL_INIT_EVERYTHING);
        _window = SDL_CreateWindow("Bomb Squad", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _ScreenWidth,
                                   _ScreenHeight, SDL_WINDOW_OPENGL);

        if (_window == nullptr)
            throw ErrorHandle("Error creating window");

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GLContext glContext = SDL_GL_CreateContext(_window);

        GLuint vertexArrayID;
        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);

        if (glContext == nullptr)
            throw ErrorHandle("could not create SDL GL context");

//        GLenum error = glewInit();
//        if (error != GLEW_OK)
//            throw ErrorHandle("could not init Glew");

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        //background colour
        glClearColor(1.0f, 1.0f, 0.0f, 1.0);
        initShaders();


    } catch (ErrorHandle errorHandle){
        std::cout << errorHandle.what() << std::endl;
    }

}

void MainGame::processInput(){

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
        _time += 0.01;
        drawGame();
    }
}

void MainGame::drawGame() {
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _colorProgram.use();

    glActiveTexture(GL_TEXTURE0);
    GLint texturelocation = _colorProgram.getUiformLocation("mySampler");
    glUniform1i(texturelocation, 0);
    glBindTexture(GL_TEXTURE_2D, _PlayerTexture.id);

   // GLint timeLocation = _colorProgram.getUiformLocation("time");
  //  glUniform1f(timeLocation, _time);

    _sprite.draw();

    glBindTexture(GL_TEXTURE_2D, 0);

    _colorProgram.unuse();
    SDL_GL_SwapWindow(_window);
}

void MainGame::initShaders() {
    _colorProgram.compileShaders("shaders/colorShading.vert", "shaders/fragShader.frag");
    _colorProgram.addAttribute("vertexPosition");
    _colorProgram.addAttribute("vertexColor");
    _colorProgram.addAttribute("vertexUV");
    _colorProgram.linkShaders();
}