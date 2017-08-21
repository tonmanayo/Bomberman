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
        {
    // Empty
}


Gun::~Gun() {
    // Empty
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime) {
    _frameCounter += 1.0f * deltaTime;
    // After a certain number of frames has passed we fire our gun
    if (_frameCounter >= _fireRate && isMouseDown) {
        fire(direction, position, bullets);
        _frameCounter = 0;
    }
}

void Gun::fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>& bullets) {

    static std::mt19937 randomEngine(time(nullptr));
    // For offsetting the accuracy
    std::uniform_real_distribution<float> randRotate(-_spread, _spread);

    glm::vec2 newpos = position;
    glm::vec2 newdir = position;
    newpos.x += TILE_WIDTH;
    //newpos.x += TILE_WIDTH;

    for (int i = 0; i < _bulletsPerShot; i++) {
        // Add a new bullet

        //static
        bullets.emplace_back(position - glm::vec2(BULLET_RADIUS),
                             glm::vec2(0, 0),
                             _bulletDamage,
                             0, 0);

        //right bullet


    }
}

void Gun::Bomb(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet> &bombs) {


    glm::vec2 newpos = position;
    glm::vec2 newdir = position;
    newpos.x += TILE_WIDTH;
    //newpos.x += TILE_WIDTH;

    for (int i = 0; i < _bulletsPerShot; i++) {
        // Add a new bullet



        //right bullet

        newdir.x = -newdir.x;
        bombs.emplace_back(newpos - glm::vec2(BULLET_RADIUS),
                             glm::normalize(glm::vec2(1, 0)),
                             _bulletDamage,
                             _bulletSpeed,
                             0);
        newpos.x -= TILE_WIDTH * 2;

        bombs.emplace_back(newpos - glm::vec2(BULLET_RADIUS),
                             glm::normalize(glm::vec2(-1, 0)),
                             _bulletDamage,
                             _bulletSpeed,
                             0);
        newpos.x = position.x;
        newpos.y -= TILE_WIDTH;

        bombs.emplace_back(newpos - glm::vec2(BULLET_RADIUS),
                             glm::normalize(glm::vec2(0, -1)),
                             _bulletDamage,
                             _bulletSpeed,
                             0);

        newpos.y += TILE_WIDTH * 2;
        bombs.emplace_back(newpos - glm::vec2(BULLET_RADIUS),
                             glm::normalize(glm::vec2(0, 1)),
                             _bulletDamage,
                             _bulletSpeed,
                             0);
    }
}