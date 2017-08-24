//
// Created by tonmanayo on 2017/08/06.
//

#include "../inc/Gun.hpp"
#include "../inc/Level.hpp"


#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>


Gun::Gun(std::string name, int fireRate, int bulletsPerShot,
         float spread, float bulletDamage, float bulletSpeed) :
        _name(name),
        _fireRate(fireRate),
        _bulletsPerShot(bulletsPerShot),
        _spread(spread),
        _bulletDamage(bulletDamage),
        _bulletSpeed(bulletSpeed),
        _frameCounter(0)
        {}

Gun::~Gun() {}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bomber>& bullets, float deltaTime) {
    _frameCounter += 1.0f * deltaTime;

    if (_frameCounter >= _fireRate && isMouseDown) {                  // frame rate fire check
        placeBomb(direction, position, bullets);
        _frameCounter = 0;
    }
}

void Gun::placeBomb(const glm::vec2& direction, const glm::vec2& position, std::vector<Bomber>& bullets) {

    if (bullets.empty())
        bullets.emplace_back(position - glm::vec2(BULLET_RADIUS),
                             glm::vec2(0, 0),
                             _bulletDamage,
                             0, 0);
}

void Gun::explosion(const glm::vec2& direction, const glm::vec2& position, std::vector<Bomber> &bombs) { // Function spread of bomb //todo make bomb bigger

    glm::vec2 newpos = position;
    glm::vec2 newdir = position;
    newpos.x += TILE_WIDTH;

    for (int i = 0; i < _bulletsPerShot; i++) {

        newdir.x = -newdir.x;                                   // Right bullet
        bombs.emplace_back(newpos - glm::vec2(BULLET_RADIUS),
                             glm::normalize(glm::vec2(1, 0)),
                             _bulletDamage,
                             _bulletSpeed,
                             0);

        newpos.x -= TILE_WIDTH * 2;                             // Left bullet
        bombs.emplace_back(newpos - glm::vec2(BULLET_RADIUS),
                             glm::normalize(glm::vec2(-1, 0)),
                             _bulletDamage,
                             _bulletSpeed,
                             0);

        newpos.x = position.x;                                  // Top bullet
        newpos.y -= TILE_WIDTH;

        bombs.emplace_back(newpos - glm::vec2(BULLET_RADIUS),
                             glm::normalize(glm::vec2(0, -1)),
                             _bulletDamage,
                             _bulletSpeed,
                             0);

        newpos.y += TILE_WIDTH * 2;                             // Down bullet
        bombs.emplace_back(newpos - glm::vec2(BULLET_RADIUS),
                             glm::normalize(glm::vec2(0, 1)),
                             _bulletDamage,
                             _bulletSpeed,
                             0);
    }
}