//
// Created by tonmanayo on 2017/08/06.
//

#include "../inc/Bullet.hpp"
#include "../inc/Level.hpp"


Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed, float time) :
        _position(position),
        _direction(direction),
        _damage(damage),
        _speed(speed),
        _time(time)
        {

}

Bullet::~Bullet() {}

float Bullet::getTime() const{
    return _time;
}

bool Bullet::update(const std::vector<std::string>& levelData) {

    if (_speed != 0 && _time < 0.02f)                           // Dont move the bomb
        _position += _direction * _speed ;                      // Speed = d / t
    _time += 0.01;
    return collideWithWorld(levelData);
}

void Bullet::draw(WTCEngine::SpriteBatch& spriteBatch) {        //draw the bomb / bullets
    glm::vec4 destRect(_position.x,
                       _position.y,
                       BULLET_RADIUS * 2,
                       BULLET_RADIUS * 2);
    const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
    WTCEngine::Color color(75, 75, 75, 75);
    spriteBatch.draw(destRect, uvRect, WTCEngine::ResourceManager::getTexture("Textures/circle.png").id, 0.0f, color);
}

bool Bullet::collideWithAgent(Agent* agent) {
    const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;                            // Min dist to collide

    glm::vec2 centerPosA = _position;
    glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);
    glm::vec2 distVec = centerPosA - centerPosB;                                        // distance from agent to bullet

    float distance = glm::length(distVec);
    float collisionDepth = MIN_DISTANCE - distance;
    if (collisionDepth > 0) {                                                           // less than 0 means collision
        return true;
    }
    return false;
}

bool Bullet::collideWithBreakableBrick(BreakableBricks *breakableBricks) {              //TODO change to template
    const float MIN_DISTANCE = TILE_WIDTH;


    glm::vec2 centerPosA = _position;
    glm::vec2 centerPosB = breakableBricks->getPosition() + glm::vec2(TILE_WIDTH / 2);
    glm::ivec2 gridPosition;


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
    gridPosition.x = static_cast<int>(floor(_position.x / (float)TILE_WIDTH));          // Convert to world coords
    gridPosition.y = static_cast<int>(floor(_position.y / (float)TILE_WIDTH));

    if (gridPosition.x < 0 || gridPosition.x >= levelData[0].size() ||                  // Outside world check
        gridPosition.y < 0 || gridPosition.y >= levelData.size()) {
        return true;
    }
    return (levelData[gridPosition.y][gridPosition.x] != '.');                          // check map ,true, false
}

float Bullet::getDamage() const {
    return _damage;
}

glm::vec2 Bullet::getPosition() const {
    return _position;
}
