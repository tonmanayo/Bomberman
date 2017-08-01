//
// Created by Tony Mack on 2017/07/28.
//

#include <iostream>
#include "../inc/MainGame.hpp"
#include "../inc/ErrorHandle.hpp"
#include "../inc/WTCEngine.hpp"
#include "../inc/ResourceManager.hpp"

    MainGame::MainGame() : _time(0), _ScreenHeight(768), _ScreenWidth(1024), _gameState(PLAY), _maxFPS(60) {
        _camera2D.init(_ScreenWidth, _ScreenHeight);
    }

    MainGame::~MainGame() {}

    void MainGame::run() {
        initGame();
        gameLoop();
    }

    void MainGame::initGame() {
        try {

            WTCEngine::init();
            _window.create("Bomberman", _ScreenWidth, _ScreenHeight, 0);
            initShaders();
            _spriteBatch.init();


        } catch (WTCEngine::ErrorHandle errorHandle) {
            std::cout << errorHandle.what() << std::endl;
        }

    }

    void MainGame::processInput() {

        const float CAMERA_SPEED = 20.0f;
        const float SCALE_SPEED = 0.10f;

        SDL_Event e;

        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    _gameState = EXIT;
                    break;
                case SDL_MOUSEMOTION :
                    //std::cout << "x: " << e.motion.x << " y: " << e.motion.y << std::endl;
                    break;
                case SDL_KEYDOWN :
                    switch (e.key.keysym.sym) {
                        case SDLK_w :
                            _camera2D.setPosition(_camera2D.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
                            break ;
                        case SDLK_s:
                            _camera2D.setPosition(_camera2D.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
                            break ;
                        case SDLK_a :
                            _camera2D.setPosition(_camera2D.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
                            break ;
                        case SDLK_d:
                            _camera2D.setPosition(_camera2D.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
                            break ;
                        case SDLK_q:
                            _camera2D.setScale(_camera2D.getScale() + SCALE_SPEED);
                            break ;
                        case SDLK_e:
                            _camera2D.setScale(_camera2D.getScale() - SCALE_SPEED);
                            break ;
                        case SDLK_ESCAPE:
                            _gameState = EXIT;
                            break ;
                    }
            }
        }
    }

    void MainGame::gameLoop() {
        while (_gameState != EXIT) {
            float startTicks = SDL_GetTicks();

            processInput();
            _time += 0.01;
            _camera2D.update();
            drawGame();
            calculateFPS();
            static int frameCounter = 0;
            if (frameCounter++ == 10) {
                std::cout << _fps << std::endl;
                frameCounter = 0;
            }
            float frameTicks = SDL_GetTicks() - startTicks;
            if (1000.00f / _maxFPS > frameTicks) {
                SDL_Delay(1000.0f / _maxFPS - frameTicks);
            }
        }
    }

    void MainGame::drawGame() {
        glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _colorProgram.use();
        glActiveTexture(GL_TEXTURE0);
        GLint texturelocation = _colorProgram.getUiformLocation("mySampler");
        glUniform1i(texturelocation, 0);

        GLint timeLocation = _colorProgram.getUiformLocation("time");
        glUniform1f(timeLocation, _time);

        GLint pLocation = _colorProgram.getUiformLocation("P");
        glm::mat4 cameraMatrix = _camera2D.getcamMatrix();

        glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

        _spriteBatch.begin();

        glm::vec4 position(0.0f, 0.0f, 50.0f, 50.0f);
        glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
        static WTCEngine::GLTexture texture = WTCEngine::ResourceManager::getTexture("resources/sprites/Enemys/Enemy_Snowman1.png");
        WTCEngine::Color color;
        color.r = 255;
        color.g = 1;
        color.b = 1;
        color.a = 255;
        _spriteBatch.draw(position, uv, texture.id, 0.0f, color);
        for (int i = 0; i < 10000; ++i) {
            _spriteBatch.draw(position + glm::vec4(50.0f,0.0f,00.0f,00.0f), uv, texture.id, 0.0f, color);
        }

        _spriteBatch.end();
        _spriteBatch.renderBatch();


        glBindTexture(GL_TEXTURE_2D, 0);
        _colorProgram.unuse();
        _window.swapBuffer();
    }

    void MainGame::initShaders() {
        _colorProgram.compileShaders("shaders/colorShading.vert", "shaders/colorShading.frag");
        _colorProgram.addAttribute("vertexPosition");
        _colorProgram.addAttribute("vertexColor");
        _colorProgram.addAttribute("vertexUV");
        _colorProgram.linkShaders();
    }

    void MainGame::calculateFPS() {
        static const int NUM_SAMPLES = 10;
        static float frameTimes[NUM_SAMPLES];
        static int curFrame = 0;

        static float prevTicks = SDL_GetTicks();
        float curTicks = SDL_GetTicks();

        _frameTime = curTicks - prevTicks;
        prevTicks = curTicks;
        frameTimes[curFrame % NUM_SAMPLES] = _frameTime;

        int count = 0;
        if (curFrame < NUM_SAMPLES) {
            count = curFrame;
        } else
            count = NUM_SAMPLES;
        float frameTimeAvg = 0;
        for (int i = 0; i < count; ++i) {
            frameTimeAvg += frameTimes[i];
        }
        if (count > 0)
            frameTimeAvg /= count;

        if (frameTimeAvg > 0)
            _fps = 1000.0f / frameTimeAvg;
        else
            _fps = 60;
        curFrame++;
    }

