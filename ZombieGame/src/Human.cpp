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

    static std::mt19937 randomEngine(time(nullptr));
    static std::uniform_real_distribution<float> randDir(-180.0f, 180.0f);

    float arr[6] = {-270.0f, -180.0f, -90.0f, 90.0f, 180.0f, 270.0f};
    int r = rand() % 6;

    _health = 20;

    _color.r = 255;
    _color.g = 255;
    _color.b = 255;
    _color.a = 255;

    _speed = speed;
    _position = pos;
    // Get random direction
    _direction = glm::vec2(0.0f , arr[r]);
    // Make sure direction isn't zero
    if (_direction.length() == 0) _direction = glm::vec2(-90.0f, 90.0f);

    _direction = glm::normalize(_direction);

    _textureID = WTCEngine::ResourceManager::getTexture("Textures/human.png").id;
}

void Human::update(const std::vector<std::string>& levelData,
                   std::vector<Human*>& humans,
                   std::vector<Zombie*>& zombies,
                   float deltaTime) {

    std::random_device rd;
   // static std::mt19937 randomEngine(rd());
   // static std::uniform_real_distribution<float> randRotate(-180, 180);


    _position += _direction * _speed * deltaTime;

    // Randomly change direction every 20 frames
    float arr[2] = {1.0f, -1.0f};
    int rx = rand() % 2;
    int ry = rand() % 2;
    std::cout <<"X: " << rx << " Y" << ry << std::endl;
    if (_frames == 100) {
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