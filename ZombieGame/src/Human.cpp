//
// Created by tonmanayo on 2017/08/06.
//

#include "../inc/Human.hpp"
#include "../../inc/ResourceManager.hpp"


#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>


Human::Human() :
        _frames(0) {}


Human::~Human() {}

void Human::init(float speed, glm::vec2 pos) {

    float arr[6] = {-270.0f, -180.0f, -90.0f, 90.0f, 180.0f, 270.0f};                   // Give random start dir
    int r = rand() % 6;

    _health = 20;
    _color = WTCEngine::Color(255, 255, 255, 255);
    _speed = speed;
    _position = pos;
    _direction = glm::vec2(0.0f , arr[r]);                                              // Get random direction
    if (_direction.length() == 0) _direction = glm::vec2(-90.0f, 90.0f);
        _direction = glm::normalize(_direction);
    _textureID = WTCEngine::ResourceManager::getTexture("Textures/human.png").id;
}

void Human::update(const std::vector<std::string>& levelData,
                   std::vector<Human*>& humans,
                   std::vector<Zombie*>& zombies,
                   float deltaTime) {

    _position += _direction * _speed * deltaTime;

    float arr[2] = {1.0f, -1.0f};
    int rx = rand() % 2;
    int ry = rand() % 2;
    if (_frames == 100) {                                                               // Randomly change direction every 100 frames
        _direction.x = _direction.y * arr[rx];
        _direction.y = _direction.y * arr[ry];
        _frames = 0;
    } else {
        _frames++;
    }
    if (!collideWithLevel(levelData)) {
        _direction.x = -_direction.x ;
        _direction.y = -_direction.y ;
    }
}