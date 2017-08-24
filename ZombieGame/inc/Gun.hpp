//
// Created by tonmanayo on 2017/08/06.
//

#ifndef BOMBERMAN_GUN_HPP
#define BOMBERMAN_GUN_HPP


#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Bomber.hpp"
#include "../../inc/AudioEngine.hpp"
class Bomber;

class Gun
{
public:
    Gun(std::string name,
        int fireRate,
        int bulletsPerShot,
        float spread,
        float bulletDamage,
        float bulletSpeed
    );
    ~Gun();

    void update(bool isMouseDown,
                const glm::vec2& position,
                const glm::vec2& direction,
                std::vector<Bomber>& bullets,
                float deltaTime
    );
    void explosion(const glm::vec2& direction, const glm::vec2& position, std::vector<Bomber>& bullets);

private:

    void placeBomb(const glm::vec2& direction, const glm::vec2& position, std::vector<Bomber>& bullets);

    std::string     _name;
    int             _fireRate;          // Fire rate in terms of frames
    int             _bulletsPerShot;
    float           _spread;            // Accuracy
    float           _bulletSpeed;
    float             _bulletDamage;
    float           _frameCounter;

};


#endif //BOMBERMAN_GUN_HPP
