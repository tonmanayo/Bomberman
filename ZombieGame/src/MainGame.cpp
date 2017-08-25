//
// Created by tonmanayo on 2017/08/06.
//

#include "../inc/MainGame.hpp"

#include <stdio.h>


const float HUMAN_SPEED = 5.0f;
const float ZOMBIE_SPEED = 5.3f;
const float PLAYER_SPEED = 10.0f;

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
    for (auto &_breakableBricks : _breakableBricks) {
        delete _breakableBricks;
    }
}

void MainGame::run() {

    initSystems();
    initLevel();
    gameLoop();
}

void MainGame::initSystems() {
    // Initialize the game engine
    WTCEngine::init();

    // Initialize sound, must happen after WTCEngine::init
   // _audioEngine.init();

    // Create our window
    _window.create("BomberMan", _screenWidth, _screenHeight, 0);

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
    _player->init(PLAYER_SPEED, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera, &_bullets, &_bomb);

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
    //add bricks
    const std::vector<glm::vec2>& breakableBrickPositions = _levels[_currentLevel]->getBreakableBrickStartPositions();
    for (int i = 0; i < breakableBrickPositions.size(); i++) {
        _breakableBricks.push_back(new BreakableBricks);
        _breakableBricks.back()->init(breakableBrickPositions[i]);
    }

    // Set up the players guns //todo use this gun when getting a power up increasde explosion size

    const float BULLET_SPEED = 20.0f;
    _player->addGun(new Gun("Magnum", 10, 1, 0.0f, 30, BULLET_SPEED));

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
    const float DESIRED_FPS = 60.0f;                                    // FPS the game is designed to run at
    const int MAX_PHYSICS_STEPS = 6;                                    // Max number of physics steps per frame
    const float MS_PER_SECOND = 1000;                                   // Number of milliseconds in a second
    const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;        // The desired frame time per frame
    const float MAX_DELTA_TIME = 1.0f;                                  // Maximum size of deltaTime

    WTCEngine::FpsLimiter fpsLimiter;                                   // Used to cap the FPS
    fpsLimiter.setMaxFPS(60.0f);

    const float CAMERA_SCALE = 1.0f / 3.0f;                             // Zoom out the camera by 3x
    _camera.setScale(CAMERA_SCALE);

    float previousTicks = SDL_GetTicks();                               // Start our previousTicks variable
    static float total;

    while (_gameState == GameState::PLAY) {                             // Main loop
        fpsLimiter.begin();

        float newTicks = SDL_GetTicks();                                // Calculate the frameTime in milliseconds
        float frameTime = newTicks - previousTicks;
        previousTicks = newTicks;                                       // Store newTicks in previousTicks so we can use it next frame
        float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

        checkVictory();

        _inputManager.update();

        processInput();

        int i = 0;                                                      // This counter makes sure we don't spiral to death!
        while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {        // Loop while we still have steps to process.
            float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME); // The deltaTime should be the the smaller of the totalDeltaTime and MAX_DELTA_TIME
            total += deltaTime / 100;                                   // Update all physics here and pass in deltaTime
            updateAgents(deltaTime);
            updateBullets(total);
            _particleEngine.update(deltaTime);                          // Since we just took a step that is length deltaTime, subtract from totalDeltaTime
            totalDeltaTime -= deltaTime;                                // Increment our frame counter so we can limit steps to MAX_PHYSICS_STEPS
            i++;
        }
        _camera.setPosition(_player->getPosition());                    // Make sure the camera is bound to the player position
        _camera.update();
        _hudCamera.update();

        drawGame();
        _fps = fpsLimiter.end();                                        // End the frame, limit the FPS, and get the current FPS.
        std::cout << _fps << std::endl;
    }
}

void MainGame::updateAgents(float deltaTime) {
    try {
        for (auto &_human : _humans) {
            _human->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
        }
        for (auto &_breakableBrick : _breakableBricks) {
            _breakableBrick->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
        }

        for (int i = 1; i < _humans.size(); i++) {
            if (_humans[i]->collideWithAgent(_player)) {
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
    for (int i = 0; i < _bullets.size(); i++) {
        _bullets[i].update(_levels[_currentLevel]->getLevelData());
        if (_bullets[i].getTime() > 2){
            _levels[_currentLevel]->setLevelData(_bullets[i].getPosition(), '.');
            //addBlood(_bullets[i].getPosition(), 10);
            _player->bomb(_bullets[i].getPosition());
            _bullets[i] = _bullets.back();
            _bullets.pop_back();
        }

        for (int j = 0; j < _bomb.size(); j++) {
           //addBlood(_bomb[j].getPosition(), 10);
        }
    }

    bool wasBulletRemoved;

    for (int i = 0; i < _bomb.size(); i++) {
        wasBulletRemoved = false;
        if (!wasBulletRemoved) {
                for (int j = 0; j < _breakableBricks.size();) {
                    // Check collision
                    if (_bomb[i].collideWithBreakableBrick(_breakableBricks[j])) {
                        // Add blood
                     //   addBlood(_breakableBricks[j]->getPosition(), 5);
                       _levels[_currentLevel]->setLevelData(_breakableBricks[j]->getPosition(), '.');
                        delete _breakableBricks[j];
                        _breakableBricks[j] = _breakableBricks.back();
                        _breakableBricks.pop_back();
                        _bomb[i] = _bomb.back();
                        _bomb.pop_back();
                        wasBulletRemoved = true;
                        i--;
                        break;
                    } else {
                        j++;
                    }
                }
            }
        // Loop through humans
        if (!wasBulletRemoved) {
            for (int j = 1; j < _humans.size(); ) {
                // Check collision
                if (_bomb[i].collideWithAgent(_humans[j])) {
                    // Add blood
                 //   addBlood(_bomb[i].getPosition(), 5);
                    // Damage human, and kill it if its out of health
                    if (_humans[j]->applyDamage(_bomb[i].getDamage())) {
                        // If the human died, remove him
                        delete _humans[j];
                        _humans[j] = _humans.back();
                        _humans.pop_back();
                    } else {
                        j++;
                    }
                    _bomb[i] = _bomb.back();
                    _bomb.pop_back();
                    _numHumansKilled++;
                    i--;
                    break;
                } else {
                    j++;
                }
            }
        }
    }
    _bomb.clear();
}


void MainGame::checkVictory() {
    // TODO: Support for multiple levels!
    // _currentLevel++; initLevel(...);

//    // If all zombies are dead we win!
//    if (_zombies.empty()) {
//        // Print victory message
//        std::cout << "*** You win! ***\n You killed: " << _numHumansKilled << " humans and " << _numZombiesKilled << " zombies. There are " << _humans.size() - 1 <<  " civilians remaining\n" ;
//        _gameState = GameState::EXIT;
//    }
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

    // Draw the bricks
    for (int i = 0; i < _breakableBricks.size(); i++) {
        if (_camera.isBoxInView(_breakableBricks[i]->getPosition(), agentDims)) {
            _breakableBricks[i]->drawBrick(_agentSpriteBatch);
        }
    }

    // Draw the bullets
    for (int i = 0; i < _bullets.size(); i++) {
        _bullets[i].draw(_agentSpriteBatch);
       // _levels[_currentLevel]->setLevelData(_bullets[i].getPosition(), 'R');
    }
    //draw bomb
    for (int i = 0; i < _bomb.size(); i++) {
        _bomb[i].draw(_agentSpriteBatch);
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

//    std::cout  << " Num Humans " << _humans.size() << std::endl;
//    //_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(0, 0),
//    //                   glm::vec2(0.5), 0.0f, WTCEngine::Color(255, 255, 255, 255));
//
//    std::cout  << " Num Zombies " << _zombies.size() << std::endl;
//   // _spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(0, 36),
//     //                  glm::vec2(0.5), 0.0f, WTCEngine::Color(255, 255, 255, 255));

    _hudSpriteBatch.end();
    _hudSpriteBatch.renderBatch();
}

void MainGame::addBlood(const glm::vec2& position, int numParticles) {

    static std::mt19937 randEngine(time(nullptr));
    static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);

    glm::vec2 vel(2.0f, 0.0f);
    WTCEngine::Color col(255, 255, 255, 255);

    glm::vec2 newpos = position;

    for (int i = 0; i < numParticles; i++) {
        _bloodParticleBatch->addParticle(position, glm::rotate(vel, randAngle(randEngine)), col, TILE_WIDTH);
    }
}