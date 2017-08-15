//
// Created by tonmanayo on 2017/08/06.
//

#include "../inc/Bullet.hpp"


#include "../../inc/ResourceManager.hpp"

#include "../inc/Agent.hpp"
#include "../inc/Human.hpp"
#include "../inc/Zombie.hpp"
#include "../inc/Level.hpp"


Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed) :
        _position(position),
        _direction(direction),
        _damage(damage),
        _speed(speed) {
}


Bullet::~Bullet() {}


bool Bullet::update(const std::vector<std::string>& levelData, float deltaTime) {
    _position += _direction * _speed * deltaTime;
    return collideWithWorld(levelData);
}

void Bullet::draw(WTCEngine::SpriteBatch& spriteBatch) {
    glm::vec4 destRect(_position.x,
                       _position.y,
                       BULLET_RADIUS * 2,
                       BULLET_RADIUS * 2);
    const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

    WTCEngine::Color color;
    color.r = 75;
    color.g = 75;
    color.b = 75;
    color.a = 255;

    spriteBatch.draw(destRect, uvRect, WTCEngine::ResourceManager::getTexture("Textures/circle.png").id, 0.0f, color);
}

bool Bullet::collideWithAgent(Agent* agent) {
    const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

    glm::vec2 centerPosA = _position;
    glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

    glm::vec2 distVec = centerPosA - centerPosB;

    float distance = glm::length(distVec);

    float collisionDepth = MIN_DISTANCE - distance;
    if (collisionDepth > 0) {
        return true;
    }
    return false;
}

bool Bullet::collideWithWorld(const std::vector<std::string>& levelData) {
    glm::ivec2 gridPosition;
    gridPosition.x = static_cast<int>(floor(_position.x / (float)TILE_WIDTH));
    gridPosition.y = static_cast<int>(floor(_position.y / (float)TILE_WIDTH));

    // If we are outside the world, just return
    if (gridPosition.x < 0 || gridPosition.x >= levelData[0].size() ||
        gridPosition.y < 0 || gridPosition.y >= levelData.size()) {
        return true;
    }

    return (levelData[gridPosition.y][gridPosition.x] != '.');
}

float Bullet::getDamage() const {
    return _damage;
}

glm::vec2 Bullet::getPosition() const {
    return _position;
}