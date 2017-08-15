//
// Created by tonmanayo on 2017/08/06.
//

#include "../inc/Human.hpp"
#include "../../inc/ResourceManager.hpp"


#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>


Human::Human() :
        _frames(0) {}


Human::~Human() {}

void Human::init(float speed, glm::vec2 pos) {

    static std::mt19937 randomEngine(time(nullptr));
    static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

    _health = 20;

    _color.r = 255;
    _color.g = 255;
    _color.b = 255;
    _color.a = 255;

    _speed = speed;
    _position = pos;
    // Get random direction
    _direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
    // Make sure direction isn't zero
    if (_direction.length() == 0) _direction = glm::vec2(1.0f, 0.0f);

    _direction = glm::normalize(_direction);

    _textureID = WTCEngine::ResourceManager::getTexture("Textures/human.png").id;
}

void Human::update(const std::vector<std::string>& levelData,
                   std::vector<Human*>& humans,
                   std::vector<Zombie*>& zombies,
                   float deltaTime) {

    std::random_device rd;
    static std::mt19937 randomEngine(rd());
    static std::uniform_real_distribution<float> randRotate(-4, 4);

    _position += _direction * _speed * deltaTime;

    // Randomly change direction every 20 frames
//    if (_frames == 100) {
//        _direction = glm::rotate(_direction, randRotate(randomEngine));
//        _frames = 0;
//    } else {
//        _frames++;
//    }

    if (!collideWithLevel(levelData)) {
        _direction = glm::rotate(_direction, randRotate(randomEngine));
    } else {
        _direction = _direction;
    }
}