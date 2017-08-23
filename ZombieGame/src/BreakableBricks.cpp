//
// Created by Tony MACK on 2017/08/17.
//

#include "../inc/BreakableBricks.hpp"
#include "../inc/Level.hpp"
void BreakableBricks::init(glm::vec2 pos) {
    _position = pos;
    _health = 1;
    _color = WTCEngine::Color(255,255,255,255);
    _speed = 1;
    _textureID = WTCEngine::ResourceManager::getTexture("Textures/glass.png").id;   // bind it to the right texture
}

BreakableBricks::~BreakableBricks() {}

BreakableBricks::BreakableBricks() {}

void BreakableBricks::drawBrick(WTCEngine::SpriteBatch &_spriteBatch) {
    const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);                                 // draw the breakable bricks
    glm::vec4       destRect(_position.x, _position.y, TILE_WIDTH, TILE_WIDTH);
    _spriteBatch.draw(destRect, uvRect, _textureID, 0.0f, _color);
}
