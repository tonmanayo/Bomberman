//
// Created by Tony MACK on 2017/08/04.
//

#include "../inc/Bullet.hpp"
#include "../inc/GLTexture.hpp"
#include "../inc/ResourceManager.hpp"

Bullet::Bullet() : _position(0), _direction(0), _speed(0), _liftime(0){

}

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float speed, int lifeTime) {
    _position = position;
    _liftime = lifeTime;
    _speed = speed;
    _direction = direction;
}

Bullet::~Bullet() {

}

void Bullet::draw(WTCEngine::SpriteBatch &spriteBatch) {
    WTCEngine::Color color(255, 255, 255, 255);

    glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
    static WTCEngine::GLTexture texture = WTCEngine::ResourceManager::getTexture("resources/sprites/Enemys/Enemy_Snowman1.png");

    glm::vec4 posAndSize = glm::vec4(_position.x, _position.y, 30, 30);
    spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);
}

bool Bullet::update() {
    _position += _direction * _speed;
    _liftime--;
    if (_liftime == 0) {
        return true;
    } else
        return false;

}
