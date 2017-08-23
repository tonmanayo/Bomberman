//
// Created by tonmanayo on 2017/08/06.
//

#ifndef BOMBERMAN_ZOMBIE_HPP
#define BOMBERMAN_ZOMBIE_HPP

// todo make it enemy 2

#include <glm/vec2.hpp>
#include "Agent.hpp"

#include "../../inc/ResourceManager.hpp"
#include "Human.hpp"

class Zombie : public Agent
{
public:
    Zombie();
    ~Zombie();

    void init(float speed, glm::vec2 pos);

    virtual void update(const std::vector<std::string>& levelData,
                        std::vector<Human*>& humans,
                        std::vector<Zombie*>& zombies,
                        float deltaTime) override;
private:

    Human* getNearestHuman(std::vector<Human*>& humans);
};

#endif //BOMBERMAN_ZOMBIE_HPP
