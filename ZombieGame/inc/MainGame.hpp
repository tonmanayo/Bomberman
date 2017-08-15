//
// Created by tonmanayo on 2017/08/06.
//

#ifndef BOMBERMAN_MAINGAME_H
#define BOMBERMAN_MAINGAME_H

#include "../../inc/Window.hpp"
#include "../../inc/GLSLProgram.hpp"
#include "../../inc/Camera2D.hpp"
#include "../../inc/InputManager.hpp"
#include "../../inc/SpriteBatch.hpp"
#include "../../inc/SpriteFont.hpp"
//#include "../../inc/AudioEngine.hpp"
#include "../../inc/ParticleEngine2D.hpp"
#include "../../inc/ParticleBatch2D.hpp"

#include "../../inc/WTCEngine.hpp"
#include "../../inc/FpsLimiter.hpp"
#include "../../inc/ErrorHandle.hpp"
#include "../../inc/ResourceManager.hpp"
#include <random>
#include <ctime>
#include <algorithm>

#include <SDL.h>
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

#include "Gun.hpp"
#include "Zombie.hpp"
#include "Player.hpp"
#include "Level.hpp"
#include "Bullet.hpp"

class Zombie;

enum class GameState {
    PLAY,
    EXIT
};

class MainGame
{
public:
    MainGame();
    virtual ~MainGame();

    void run();

private:
    void initSystems();
    void initLevel();
    void initShaders();
    void gameLoop();
    void updateAgents(float deltaTime);
    void updateBullets(float deltaTim);
    void checkVictory();
    void processInput();
    void drawGame();
    void drawHud();
    void addBlood(const glm::vec2& position, int numParticles);

    WTCEngine::Window _window;
    WTCEngine::GLSLProgram _textureProgram;
    WTCEngine::InputManager _inputManager;
    WTCEngine::Camera2D _camera;
    WTCEngine::Camera2D _hudCamera;
    WTCEngine::SpriteBatch _agentSpriteBatch;
    WTCEngine::SpriteBatch _hudSpriteBatch;
    WTCEngine::ParticleEngine2D _particleEngine;
    WTCEngine::ParticleBatch2D* _bloodParticleBatch;

    WTCEngine::SpriteFont* _spriteFont;
   // WTCEngine::AudioEngine _audioEngine;

    std::vector<Level*> _levels;

    int _screenWidth = 1024;
    int _screenHeight = 768;

    float _fps;

    int _currentLevel;

    Player* _player;
    std::vector<Human*> _humans;
    std::vector<Zombie*> _zombies;
    std::vector<Bullet> _bullets;

    int _numHumansKilled;
    int _numZombiesKilled;

    GameState _gameState;
};

#endif //BOMBERMAN_MAINGAME_H
