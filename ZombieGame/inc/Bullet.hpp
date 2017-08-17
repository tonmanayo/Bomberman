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

class Human;
class Zombie;
class Agent;

const int BULLET_RADIUS = 10;

class Bullet
{
public:
    Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
    virtual ~Bullet();

    bool update(const std::vector<std::string>& levelData, float deltaTime);
    void draw(WTCEngine::SpriteBatch& spriteBatch);
    bool collideWithAgent(Agent* agent);
    bool collideWithBreakableBrick(BreakableBricks* breakableBricks, const std::vector<std::string>& levelData);
    float getDamage() const ;
    glm::vec2 getPosition() const ;

private:

    bool collideWithWorld(const std::vector<std::string>& levelData);
    glm::vec2 _position;
    glm::vec2 _direction;
    float _damage;
    float _speed;
};


#endif //BOMBERMAN_BULLET_HPP
