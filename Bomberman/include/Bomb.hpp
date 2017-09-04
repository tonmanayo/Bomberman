//
// Created by Tony MACK on 2017/08/29.
//

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "../include/player.hpp"

#ifndef GAME_BOMB_HPP
#define GAME_BOMB_HPP

class Bomb {
private:
    glm::vec3   _position;
    glm::vec2   _direction;
    float       _time;
    int         _id;
public:
    Bomb(glm::vec3  position, int id);
    virtual         ~Bomb();
    bool            explodeTime();
    void            draw();
    float           getTime() const;
    int             getId() const;
    glm::vec3       getPosition() const;

};

#endif //GAME_BOMB_HPP
