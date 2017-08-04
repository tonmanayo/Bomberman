//
// Created by Tony MACK on 2017/08/04.
//

#ifndef BOMBERMAN_BULLET_HPP
#define BOMBERMAN_BULLET_HPP


#include <glm/vec2.hpp>
#include "SpriteBatch.hpp"

class Bullet {
public:
    Bullet();
    virtual ~Bullet();
    Bullet(glm::vec2 position, glm::vec2 direction,float speed, int lifetime);
    void draw(WTCEngine::SpriteBatch &spriteBatch);
    bool update();
private:
    int             _liftime;
    float           _speed;
    glm::vec2       _direction;
    glm::vec2       _position;
};


#endif //BOMBERMAN_BULLET_HPP
