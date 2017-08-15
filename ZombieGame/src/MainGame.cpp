//
// Created by tonmanayo on 2017/08/06.
//

#include "../inc/MainGame.hpp"

#include <stdio.h>


const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;
const float PLAYER_SPEED = 5.0f;

MainGame::MainGame()  :
        _screenWidth(1600),
        _screenHeight(900),
        _gameState(GameState::PLAY),
        _fps(0),
        _player(nullptr),
        _numHumansKilled(0),
        _numZombiesKilled(0) {
    // Empty

}

MainGame::~MainGame() {
    // Don't forget to delete the levels!
    for (auto &_level : _levels) {
        delete _level;
    }
    // Don't forget to delete the humans and zombies!
    for (auto &_human : _humans) {
        delete _human;
    }
    for (auto &_zombie : _zombies) {
        delete _zombie;
    }
}

void MainGame::run() {

    initSystems();

    initLevel();

   // WTCEngine::Music music = _audioEngine.loadMusic("Sound/XYZ.ogg");
    //music.play(-1);

    gameLoop();
}

void MainGame::initSystems() {
    // Initialize the game engine
    WTCEngine::init();

    // Initialize sound, must happen after WTCEngine::init
   // _audioEngine.init();

    // Create our window
    _window.create("ZombieGame", _screenWidth, _screenHeight, 0);

    // Grey background color
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    // Set up the shaders
    initShaders();

    // Initialize our spritebatch
    _agentSpriteBatch.init();
    _hudSpriteBatch.init();

    // Initialize sprite font
  //  _spriteFont = new WTCEngine::SpriteFont("/home/tonmanayo/Projects/Bomberman/Fonts/chintzy.ttf", 64);

    // Set up the camera
    _camera.init(_screenWidth, _screenHeight);
    _hudCamera.init(_screenWidth, _screenHeight);
    _hudCamera.setPosition(glm::vec2(_screenWidth / 2, _screenHeight / 2));

    // Initialize particles
    _bloodParticleBatch = new WTCEngine::ParticleBatch2D;

    // Initialize the particle batch and use a lambda function to define the update
    _bloodParticleBatch->init(1000, 0.05f,
                               WTCEngine::ResourceManager::getTexture("Textures/particle.png"),
                               [](WTCEngine::Particle2D& particle, float deltaTime) {
                                   particle.position += particle.velocity * deltaTime;
                                   particle.color.a = (GLubyte)(particle.life * 255.0f);
                               });

    _particleEngine.addParticleBatch(_bloodParticleBatch);

}

void MainGame::initLevel() {
    // Level 1
    _levels.push_back(new Level("Levels/level1.txt"));
    _currentLevel = 0;

    _player = new Player();
    _player->init(PLAYER_SPEED, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera, &_bullets);

    _humans.push_back(_player);

    std::mt19937 randomEngine;
    randomEngine.seed(time(nullptr));

    std::uniform_int_distribution<int> randX(2, _levels[_currentLevel]->getWidth() - 2);
    std::uniform_int_distribution<int> randY(2, _levels[_currentLevel]->getHeight() - 2);

    // Add all the random humans
    for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++) {
        _humans.push_back(new Human);
        glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
        _humans.back()->init(HUMAN_SPEED, pos);
    }

    // Add the zombies
    const std::vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->getZombieStartPositions();
    for (int i = 0; i < zombiePositions.size(); i++) {
        _zombies.push_back(new Zombie);
        _zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
    }

    // Set up the players guns

    const float BULLET_SPEED = 20.0f;
//    _player->addGun(new Gun("Magnum", 10, 1, 5.0f, 30, BULLET_SPEED, _audioEngine.loadSoundEffect("Sound/shots/pistol.wav")));
//    _player->addGun(new Gun("Shotgun", 30, 12, 20.0f, 4, BULLET_SPEED, _audioEngine.loadSoundEffect("Sound/shots/shotgun.wav")));
//    _player->addGun(new Gun("MP5", 2, 1, 10.0f, 20, BULLET_SPEED, _audioEngine.loadSoundEffect("Sound/shots/cg1.wav")));

}

void MainGame::initShaders() {
    // Compile our color shader
    _textureProgram.compileShaders("shaders/colorShading.vert", "shaders/colorShading.frag");
    _textureProgram.addAttribute("vertexPosition");
    _textureProgram.addAttribute("vertexColor");
    _textureProgram.addAttribute("vertexUV");
    _textureProgram.linkShaders();
}

void MainGame::gameLoop() {

    // Some helpful constants.
    const float DESIRED_FPS = 60.0f; // FPS the game is designed to run at
    const int MAX_PHYSICS_STEPS = 6; // Max number of physics steps per frame
    const float MS_PER_SECOND = 1000; // Number of milliseconds in a second
    const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS; // The desired frame time per frame
    const float MAX_DELTA_TIME = 1.0f; // Maximum size of deltaTime

    // Used to cap the FPS
    WTCEngine::FpsLimiter fpsLimiter;
    fpsLimiter.setMaxFPS(60.0f);

    // Zoom out the camera by 3x
    const float CAMERA_SCALE = 1.0f / 3.0f;
    _camera.setScale(CAMERA_SCALE);

    // Start our previousTicks variable
    float previousTicks = SDL_GetTicks();

    // Main loop
    while (_gameState == GameState::PLAY) {
        fpsLimiter.begin();

        // Calculate the frameTime in milliseconds
        float newTicks = SDL_GetTicks();
        float frameTime = newTicks - previousTicks;
        previousTicks = newTicks; // Store newTicks in previousTicks so we can use it next frame
        // Get the total delta time
        float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

        checkVictory();

        _inputManager.update();

        processInput();

        int i = 0; // This counter makes sure we don't spiral to death!
        // Loop while we still have steps to process.
        while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
            // The deltaTime should be the the smaller of the totalDeltaTime and MAX_DELTA_TIME
            float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
            // Update all physics here and pass in deltaTime
            updateAgents(deltaTime);
            updateBullets(deltaTime);
            _particleEngine.update(deltaTime);
            // Since we just took a step that is length deltaTime, subtract from totalDeltaTime
            totalDeltaTime -= deltaTime;
            // Increment our frame counter so we can limit steps to MAX_PHYSICS_STEPS
            i++;
        }

        // Make sure the camera is bound to the player position
        _camera.setPosition(_player->getPosition());
        _camera.update();
        _hudCamera.update();

        drawGame();

        // End the frame, limit the FPS, and get the current FPS.
        _fps = fpsLimiter.end();
        std::cout << _fps << std::endl;
    }
}

void MainGame::updateAgents(float deltaTime) {
    try {
        for (auto &_human : _humans) {
            _human->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
        }
        for (auto &_zombie : _zombies) {
            _zombie->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
        }
        for (int i = 0; i < _zombies.size(); i++) {
            for (int j = i + 1; j < _zombies.size(); j++) {
                _zombies[i]->collideWithAgent(_zombies[j]);
            }
            for (int j = 1; j < _humans.size(); j++) {
                if (_zombies[i]->collideWithAgent(_humans[j])) {
                    _zombies.push_back(new Zombie);
                    _zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
                    delete _humans[j];
                    _humans[j] = _humans.back();
                    _humans.pop_back();
                }
            }
            if (_zombies[i]->collideWithAgent(_player)) {
               throw WTCEngine::ErrorHandle("YOU LOSE");
            }
        }
        for (int i = 0; i < _humans.size(); i++) {
            for (int j = i + 1; j < _humans.size(); j++) {
                _humans[i]->collideWithAgent(_humans[j]);
            }
        }
    } catch (WTCEngine::ErrorHandle errorHandle) {
        std::cout << errorHandle.what() << std::endl;
        _gameState =  GameState::EXIT;
    }
}

void MainGame::updateBullets(float deltaTime) {
    // Update and collide with world
    for (int i = 0; i < _bullets.size(); ) {
        // If update returns true, the bullet collided with a wall
        if (_bullets[i].update(_levels[_currentLevel]->getLevelData(), deltaTime)) {
            _bullets[i] = _bullets.back();
            _bullets.pop_back();
        } else {
            i++;
        }
    }

    bool wasBulletRemoved;

    for (int i = 0; i < _bullets.size(); i++) {
        wasBulletRemoved = false;
        // Loop through zombies
        for (int j = 0; j < _zombies.size(); ) {
            // Check collision
            if (_bullets[i].collideWithAgent(_zombies[j])) {
                // Add blood
                addBlood(_bullets[i].getPosition(), 5);

                if (_zombies[j]->applyDamage(_bullets[i].getDamage())) {
                    // If the zombie died, remove him
                    delete _zombies[j];
                    _zombies[j] = _zombies.back();
                    _zombies.pop_back();
                    _numZombiesKilled++;
                } else {
                    j++;
                }

                _bullets[i] = _bullets.back();
                _bullets.pop_back();
                wasBulletRemoved = true;
                i--;
                break;
            } else {
                j++;
            }
        }
        // Loop through humans
        if (!wasBulletRemoved) {
            for (int j = 1; j < _humans.size(); ) {
                // Check collision
                if (_bullets[i].collideWithAgent(_humans[j])) {
                    // Add blood
                    addBlood(_bullets[i].getPosition(), 5);
                    // Damage human, and kill it if its out of health
                    if (_humans[j]->applyDamage(_bullets[i].getDamage())) {
                        // If the human died, remove him
                        delete _humans[j];
                        _humans[j] = _humans.back();
                        _humans.pop_back();
                    } else {
                        j++;
                    }

                    _bullets[i] = _bullets.back();
                    _bullets.pop_back();
                    _numHumansKilled++;
                    i--;
                    break;
                } else {
                    j++;
                }
            }
        }
    }
}

void MainGame::checkVictory() {
    // TODO: Support for multiple levels!
    // _currentLevel++; initLevel(...);

    // If all zombies are dead we win!
    if (_zombies.empty()) {
        // Print victory message
        std::cout << "*** You win! ***\n You killed: " << _numHumansKilled << " humans and " << _numZombiesKilled << " zombies. There are " << _humans.size() - 1 <<  " civilians remaining\n" ;
        _gameState = GameState::EXIT;
    }
}

void MainGame::processInput() {
    SDL_Event evnt;
    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                _gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                _inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                _inputManager.pressKey(evnt.key.keysym.sym);
                if (evnt.key.keysym.sym == SDLK_ESCAPE)
                    _gameState = GameState::EXIT;
                break;
            case SDL_KEYUP:
                _inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                _inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                _inputManager.releaseKey(evnt.button.button);
                break;
        }
    }
}

void MainGame::drawGame() {
    
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _textureProgram.use();
    
    glActiveTexture(GL_TEXTURE0);
    GLint textureUniform = _textureProgram.getUiformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    
    glm::mat4 projectionMatrix = _camera.getcamMatrix();
    GLint pUniform = _textureProgram.getUiformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Draw the level
    _levels[_currentLevel]->draw();

    // Begin drawing agents
    _agentSpriteBatch.begin();

    const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

    // Draw the humans
    for (int i = 0; i < _humans.size(); i++) {
        if (_camera.isBoxInView(_humans[i]->getPosition(), agentDims)) {
            _humans[i]->draw(_agentSpriteBatch);
        }
    }

    // Draw the zombies
    for (int i = 0; i < _zombies.size(); i++) {
        if (_camera.isBoxInView(_zombies[i]->getPosition(), agentDims)) {
            _zombies[i]->draw(_agentSpriteBatch);
        }
    }

    // Draw the bullets
    for (int i = 0; i < _bullets.size(); i++) {
        _bullets[i].draw(_agentSpriteBatch);
    }

    _agentSpriteBatch.end();

    _agentSpriteBatch.renderBatch();

    // Render the particles
    _particleEngine.draw(&_agentSpriteBatch);

    // Render the heads up display
    drawHud();

    // Unbind the program
    _textureProgram.unuse();

    // Swap our buffer and draw everything to the screen!
    _window.swapBuffer();
}

void MainGame::drawHud() {
    char buffer[256];

    glm::mat4 projectionMatrix = _hudCamera.getcamMatrix();
    GLint pUniform = _textureProgram.getUiformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    _hudSpriteBatch.begin();

    std::cout << buffer << " Num Humans " << _humans.size() << std::endl;
    //_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(0, 0),
    //                   glm::vec2(0.5), 0.0f, WTCEngine::Color(255, 255, 255, 255));

    std::cout << buffer << " Num Zombies " << _zombies.size() << std::endl;
   // _spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(0, 36),
     //                  glm::vec2(0.5), 0.0f, WTCEngine::Color(255, 255, 255, 255));

    _hudSpriteBatch.end();
    _hudSpriteBatch.renderBatch();
}

void MainGame::addBlood(const glm::vec2& position, int numParticles) {

    static std::mt19937 randEngine(time(nullptr));
    static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);

    glm::vec2 vel(2.0f, 0.0f);
    WTCEngine::Color col(255, 0, 0, 255);

    for (int i = 0; i < numParticles; i++) {
        _bloodParticleBatch->addParticle(position, glm::rotate(vel, randAngle(randEngine)), col, 30.0f);
    }
}