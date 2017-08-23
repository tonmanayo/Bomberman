//
// Created by tonmanayo on 2017/08/06.
//

#include "../inc/Agent.hpp"
#include "../inc/Level.hpp"

#include <algorithm>

Agent::Agent() {}


Agent::~Agent() {}

bool Agent::collideWithLevel(const std::vector<std::string>& levelData) {

    std::vector<glm::vec2> collideTilePositions;                                                                    // Check the four corners
    checkTilePosition(levelData, collideTilePositions, _position.x, _position.y);                                   // 1st corner
    checkTilePosition(levelData, collideTilePositions, _position.x + AGENT_WIDTH, _position.y);                     // 2nd Corner
    checkTilePosition(levelData, collideTilePositions, _position.x, _position.y + AGENT_WIDTH);                     // 3rd Corner
    checkTilePosition(levelData, collideTilePositions, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH);       // 4th Corner

    if (collideTilePositions.empty()) {                                                                             // No collision
        return false;
    }
    for (int i = 0; i < collideTilePositions.size(); i++) {                                                         // Do the collision
        collideWithTile(collideTilePositions[i]);
    }
    return true;
}

// Circular collision
bool Agent::collideWithAgent(Agent* agent) {

    const float MIN_DISTANCE = AGENT_RADIUS * 2.0f;                                 // Minimum distance before there is a collision
    glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);                     // Center position of this agent
    glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);          // Center position of the parameter agent
    glm::vec2 distVec = centerPosA - centerPosB;                                    // Distance vector between the two agents
    float distance = glm::length(distVec);                                          // Length of the distance vector
    float collisionDepth = MIN_DISTANCE - distance;                                 // Depth of the collision

    if (collisionDepth > 0) {                                                       // If collision depth > 0 then we did collide
        glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;     // push them away from each other
        _position += collisionDepthVec / 2.0f;                                      // Push them in opposite directions
        agent->_position -= collisionDepthVec / 2.0f;
        return true;
    }
    return false;
}

void Agent::draw(WTCEngine::SpriteBatch& _spriteBatch) {

    const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
    glm::vec4       destRect(_position.x, _position.y, AGENT_WIDTH, AGENT_WIDTH);

    _spriteBatch.draw(destRect, uvRect, _textureID, 0.0f, _color, _direction);
}

bool Agent::applyDamage(float damage) {

    _health -= damage;
    if (_health <= 0) {
        return true;
    }
    return false;
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData,
                              std::vector<glm::vec2>& collideTilePositions,
                              float x,
                              float y) {

    glm::vec2 gridPos = glm::vec2(std::floor(x / (float)TILE_WIDTH), std::floor(y / (float)TILE_WIDTH));            // Get the position of this corner in grid
    if (gridPos.x < 0 || gridPos.x >= levelData[0].size() ||
        gridPos.y < 0 || gridPos.y >= levelData.size())                                                             // If we are outside the world, just return
        return;
    if (levelData[gridPos.y][gridPos.x] != '.') {                                                                   // If this is not an air tile, we should collide with it
        collideTilePositions.push_back(gridPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
    }
}

void Agent::collideWithTile(glm::vec2 tilePos) {                    // Axis Aligned Bounding Box collision

    const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;             // The minimum distance before a collision
    const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;
    glm::vec2 centerAgentPos = _position + glm::vec2(AGENT_RADIUS); // Center position of the agent
    glm::vec2 distVec = centerAgentPos - tilePos;                   // Vector from the agent to the tile
    float xDepth = MIN_DISTANCE - std::abs(distVec.x);              // Depth of the collision
    float yDepth = MIN_DISTANCE - std::abs(distVec.y);
    if (xDepth > 0 && yDepth > 0) {                                 // If both the depths are > 0, then we collided
        if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {      // Which collision depth is less
            if (distVec.x < 0) {                                    // Push in X direction
                _position.x -= xDepth;
            } else {
                _position.x += xDepth;
            }
        } else {
            if (distVec.y < 0) {                                    // Push in Y direction
                _position.y -= yDepth;
            } else {
                _position.y += yDepth;
            }
        }
    }
}

glm::vec2 Agent::getPosition() const {
    return _position;
}
