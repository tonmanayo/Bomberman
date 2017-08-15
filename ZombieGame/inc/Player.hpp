//
// Created by tonmanayo on 2017/08/06.
//

#ifndef BOMBERMAN_PLAYER_H
#define BOMBERMAN_PLAYER_H


#include "Human.hpp"

#include "Human.hpp"
#include "../../inc/InputManager.hpp"
#include "../../inc/Camera2D.hpp"
#include "Bullet.hpp"

#include <SDL2/SDL.h>
#include "../../inc/ResourceManager.hpp"
#include "Gun.hpp"

class Gun;

class Player : public Human
{
public:
    Player();
    virtual ~Player();

    void init(float speed, glm::vec2 pos, WTCEngine::InputManager* inputManager, WTCEngine::Camera2D* camera, std::vector<Bullet>* bullets);

    void addGun(Gun* gun);

    void update(const std::vector<std::string> &levelData,
                std::vector<Human*>& humans,
                std::vector<Zombie*>& zombies,
                float deltaTime) override;
private:
    WTCEngine::InputManager* _inputManager;

    std::vector<Gun*> _guns;
    int _currentGunIndex;

    WTCEngine::Camera2D* _camera;
    std::vector<Bullet>* _bullets;

};


#endif //BOMBERMAN_PLAYER_H
