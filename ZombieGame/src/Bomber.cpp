//
// Created by tonmanayo on 2017/08/06.
//

#include "../inc/Bomber.hpp"
#include "../inc/Level.hpp"


Bomber::Bomber(glm::vec2 position, glm::vec2 direction, float damage, float speed, float time) :
        _position(position),
        _direction(direction),
        _damage(damage),
        _speed(speed),
        _time(time)
        {}

Bomber::~Bomber() {}

float Bomber::getTime() const{
    return _time;
}

bool Bomber::update(const std::vector<std::string>& levelData) {

    if (_speed != 0 && _time < 0.02f)                           // Dont move the bomb
        _position += _direction * _speed ;                      // Speed = d / t
    _time += 0.01;
    return collideWithWorld(levelData);
}

void Bomber::draw(WTCEngine::SpriteBatch& spriteBatch) {        //draw the bomb / bullets

    float x = _position.x, y = _position.y;

    x =  std::trunc(_position.x / TILE_WIDTH);
    y =  std::trunc(_position.y / TILE_WIDTH);

    x *= TILE_WIDTH;
    y *= TILE_WIDTH;

  //  x += TILE_WIDTH / 2;
   // y += TILE_WIDTH / 2;

    x += (30);
    y += (30);

//    x = sqrt(pow((x + TILE_WIDTH), 2) - pow(x, 2));
//    y = sqrt(pow((y + TILE_WIDTH), 2) - pow(y, 2));
    //y += TILE_WIDTH / 2;

    glm::vec4 destRect(x,
                       y,
                       BULLET_RADIUS * 2,
                       BULLET_RADIUS * 2);
    const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

    WTCEngine::Color color(75, 75, 75, 75);
    spriteBatch.draw(destRect, uvRect, WTCEngine::ResourceManager::getTexture("Textures/circle.png").id, 0.0f, color);
}

bool Bomber::collideWithAgent(Agent* agent) {
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

bool Bomber::collideWithBreakableBrick(BreakableBricks *breakableBricks) {              //TODO change to template
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
bool Bomber::collideWithWorld(const std::vector<std::string>& levelData) {
    glm::ivec2 gridPosition;
    gridPosition.x = static_cast<int>(floor(_position.x / (float)TILE_WIDTH));          // Convert to world coords
    gridPosition.y = static_cast<int>(floor(_position.y / (float)TILE_WIDTH));

    if (gridPosition.x < 0 || gridPosition.x >= levelData[0].size() ||                  // Outside world check
        gridPosition.y < 0 || gridPosition.y >= levelData.size()) {
        return true;
    }
    return (levelData[gridPosition.y][gridPosition.x] != '.');                          // check map ,true, false
}

float Bomber::getDamage() const {
    return _damage;
}

glm::vec2 Bomber::getPosition() const {
    return _position;
}
