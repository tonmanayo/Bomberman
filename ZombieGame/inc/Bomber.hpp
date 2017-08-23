//
// Created by tonmanayo on 2017/08/06.
//

#ifndef BOMBERMAN_BULLET_HPP
#define BOMBERMAN_BULLET_HPP


#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "../../inc/SpriteBatch.hpp"
#include "BreakableBricks.hpp"
#include "Player.hpp"

class Human;
class Zombie;
class Agent;
class Player;

const int BULLET_RADIUS = 40;

class Bomber
{
public:
    Bomber(glm::vec2 position, glm::vec2 direction, float damage, float speed, float time);
    virtual ~Bomber();

    bool update(const std::vector<std::string>& levelData);
    void draw(WTCEngine::SpriteBatch& spriteBatch);
    bool collideWithAgent(Agent* agent);
    bool collideWithBreakableBrick(BreakableBricks *breakableBricks);
    float getDamage() const;
    float getTime() const;
    glm::vec2 getPosition() const;

private:

    bool collideWithWorld(const std::vector<std::string>& levelData);
    glm::vec2 _position;
    glm::vec2 _direction;
    float _damage;
    float _speed;
    float _time;
};


#endif //BOMBERMAN_BULLET_HPP
