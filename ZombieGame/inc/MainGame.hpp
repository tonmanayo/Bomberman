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
#include "../../inc/AudioEngine.hpp"
#include "../../inc/ParticleEngine2D.hpp"
#include "../../inc/ParticleBatch2D.hpp"

#include "../../inc/WTCEngine.hpp"
#include "../../inc/FpsLimiter.hpp"
#include "../../inc/ErrorHandle.hpp"
#include "../../inc/ResourceManager.hpp"
#include <random>
#include <ctime>
#include <algorithm>

#include <SDL/SDL.h>
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
    ~MainGame();

    /// Runs the game
    void run();

private:
    /// Initializes the core systems
    void initSystems();

    /// Initializes the level and sets up everything
    void initLevel();

    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();

    /// Updates all agents
    void updateAgents(float deltaTime);

    /// Updates all bullets
    void updateBullets(float deltaTim);

    /// Checks the victory condition
    void checkVictory();

    /// Handles input processing
    void processInput();

    /// Renders the game
    void drawGame();

    /// Draws the HUD
    void drawHud();

    /// Adds blood to the particle engine
    void addBlood(const glm::vec2& position, int numParticles);

    /// Member Variables
    WTCEngine::Window m_window; ///< The game window

    WTCEngine::GLSLProgram m_textureProgram; ///< The shader program

    WTCEngine::InputManager m_inputManager; ///< Handles input

    WTCEngine::Camera2D m_camera; ///< Main Camera
    WTCEngine::Camera2D m_hudCamera; ///< Hud Camera

    WTCEngine::SpriteBatch m_agentSpriteBatch; ///< Draws all agents
    WTCEngine::SpriteBatch m_hudSpriteBatch;

    WTCEngine::ParticleEngine2D m_particleEngine;
    WTCEngine::ParticleBatch2D* m_bloodParticleBatch;

    std::vector<Level*> m_levels; ///< vector of all levels

    int m_screenWidth = 1024;
    int m_screenHeight = 768;

    float m_fps;

    int m_currentLevel;

    Player* m_player;
    std::vector<Human*> m_humans; ///< Vector of all humans
    std::vector<Zombie*> m_zombies; ///< Vector of all zombies
    std::vector<Bullet> m_bullets;

    int m_numHumansKilled; ///< Humans killed by player
    int m_numZombiesKilled; ///< Zombies killed by player

    WTCEngine::SpriteFont* m_spriteFont;

    WTCEngine::AudioEngine m_audioEngine;

    GameState m_gameState;
};

#endif //BOMBERMAN_MAINGAME_H
