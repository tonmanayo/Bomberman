//
// Created by tonmanayo on 2017/08/06.
//

#include "../inc/Player.hpp"


Player::Player() :
        _currentGunIndex(-1) {
    // Empty
}

Player::~Player() {
    // Empty
}

void Player::init(float speed, glm::vec2 pos, WTCEngine::InputManager* inputManager, WTCEngine::Camera2D* camera, std::vector<Bomber>* bullets, std::vector<Bomber>* bombs) {
    _speed = speed;
    _position = pos;
    _inputManager = inputManager;
    _bullets = bullets;
    _bombs = bombs;
    _camera = camera;
    _color = WTCEngine::Color(255,255,255,255);
    _health = 150;
    _textureID = WTCEngine::ResourceManager::getTexture("Textures/player.png").id;
}

void Player::addGun(Gun* gun) {
    // Add the gun to his inventory
    _guns.push_back(gun);

    // If no gun equipped, equip gun.
    if (_currentGunIndex == -1) {
        _currentGunIndex = 0;
    }
}

void Player::update(const std::vector<std::string>& levelData,
                    std::vector<Human*>& humans,
                    std::vector<Zombie*>& zombies,
                    float deltaTime) {

    if (_inputManager->isKeyDown(SDLK_w)) {
        _direction = glm::normalize(glm::vec2(0.0f, 1.0f));
        _position.y += _speed * deltaTime;
    } else if (_inputManager->isKeyDown(SDLK_s)) {
        _direction = glm::normalize(glm::vec2(0.0f, -1.0f));
        _position.y -= _speed * deltaTime;
    }
    if (_inputManager->isKeyDown(SDLK_a)) {
        _direction = glm::normalize(glm::vec2(-1.0f, 0.0f));
        _position.x -= _speed * deltaTime;
    } else if (_inputManager->isKeyDown(SDLK_d)) {
        _direction = glm::normalize(glm::vec2(1.0f, 0.0f));
        _position.x += _speed * deltaTime;
    }

    if (_inputManager->isKeyDown(SDLK_1) && _guns.size() >= 0) {
        _currentGunIndex = 0;
    } else if (_inputManager->isKeyDown(SDLK_2) && _guns.size() >= 1) {
        _currentGunIndex = 1;
    } else if (_inputManager->isKeyDown(SDLK_3) && _guns.size() >= 2) {
        _currentGunIndex = 2;
    } else if (_inputManager->isKeyDown(SDLK_ESCAPE)) {
        return;
    }

    //glm::vec2 mouseCoords = _inputManager->getMouseCoords();
    //mouseCoords = _camera->convertScreenToWorld(mouseCoords);


    glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);

  //  _direction = glm::normalize(mouseCoords - centerPosition);

    if (_currentGunIndex != -1) {

        _guns[_currentGunIndex]->update(_inputManager->isKeyDown(SDLK_SPACE),
                                        centerPosition,
                                        _direction,
                                        *_bullets,
                                        deltaTime);

    }
    collideWithLevel(levelData);
}

void Player::bomb(const glm::vec2 &pos){
    _guns[_currentGunIndex]->explosion(_direction, pos, *_bombs);
}