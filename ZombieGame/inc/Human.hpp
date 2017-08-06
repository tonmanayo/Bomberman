//
// Created by tonmanayo on 2017/08/06.
//

#ifndef BOMBERMAN_HUMAN_HPP
#define BOMBERMAN_HUMAN_HPP


#include "Agent.hpp"


class Human : public Agent
{
public:
    Human();
    virtual ~Human();

    void init(float speed, glm::vec2 pos);

    virtual void update(const std::vector<std::string>& levelData,
                        std::vector<Human*>& humans,
                        std::vector<Zombie*>& zombies,
                        float deltaTime) override;

private:
    int _frames;
};



#endif //BOMBERMAN_HUMAN_HPP
