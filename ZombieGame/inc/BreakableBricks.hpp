//
// Created by Tony MACK on 2017/08/17.
//

#ifndef BOMBERMAN_BREAKABLEBRICKS_HPP
#define BOMBERMAN_BREAKABLEBRICKS_HPP

#include "Agent.hpp"
#include "../../inc/ResourceManager.hpp"

class BreakableBricks : public Agent {
private:
public:
    BreakableBricks();

    virtual ~BreakableBricks();

    virtual void update(const std::vector<std::string> &levelData,
                        std::vector<Human *> &humans,
                        std::vector<Zombie *> &zombies,
                        float deltaTime) override;

    void init(glm::vec2 pos);

    void drawBrick(WTCEngine::SpriteBatch &_spriteBatch);
    glm::vec2 getPosition() {return _position;};
};


#endif //BOMBERMAN_BREAKABLEBRICKS_HPP
