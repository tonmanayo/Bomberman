//
// Created by tonmanayo on 2017/08/06.
//

#ifndef BOMBERMAN_AGENT_H
#define BOMBERMAN_AGENT_H


#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "../../inc/SpriteBatch.hpp"


const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

class Zombie;
class Human;

class Agent
{
public:
    Agent();
    virtual ~Agent();

    virtual void update(const std::vector<std::string>& levelData,
                        std::vector<Human*>& humans,
                        std::vector<Zombie*>& zombies,
                        float deltaTime) = 0;

    bool collideWithLevel(const std::vector<std::string>& levelData);
    bool collideWithAgent(Agent* agent);
    void draw(WTCEngine::SpriteBatch& _spriteBatch);
    bool applyDamage(float damage);
    glm::vec2 getPosition() const;

protected:

    void checkTilePosition(const std::vector<std::string>& levelData,
                           std::vector<glm::vec2>& collideTilePositions,
                           float x,
                           float y);

    void collideWithTile(glm::vec2 tilePos);

    glm::vec2 _position;
    glm::vec2 _direction = glm::vec2(1.0f, 0.0f);
    WTCEngine::Color _color;
    float _speed;
    float _health;
    GLuint _textureID;
};


#endif //BOMBERMAN_AGENT_H
