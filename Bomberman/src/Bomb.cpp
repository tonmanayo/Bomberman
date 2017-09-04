//
// Created by Tony MACK on 2017/08/29.
//


#include <Bomb.hpp>

Bomb::Bomb(glm::vec3 position, int id) :
        _position(position), _id(id)
{
    _time = static_cast<float>(glfwGetTime());
}

Bomb::~Bomb() {}

float   Bomb::getTime() const{
    return _time;
}

bool    Bomb::explodeTime(){
    return glfwGetTime() - _time > 1.5;
}

int     Bomb::getId() const
{
    return _id;
}

glm::vec3 Bomb::getPosition() const
{
    return _position;
}


