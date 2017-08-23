//
// Created by tonmanayo on 2017/08/06.
//

#ifndef BOMBERMAN_LEVEL_H
#define BOMBERMAN_LEVEL_H


#include <vector>
#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <fstream>
#include <iostream>
#include "../../inc/ErrorHandle.hpp"
#include "../../inc/ResourceManager.hpp"
#include "../../inc/SpriteBatch.hpp"
#include <fstream>

const int TILE_WIDTH = 120;

class Level
{
public:
    Level(const std::string& fileName); // Load the level
    ~Level();

    void draw();
    int getWidth() const ;
    int getHeight() const ;
    int getNumHumans() const ;
    const std::vector<std::string>& getLevelData() const ;
    glm::vec2 getStartPlayerPos() const ;
    const std::vector<glm::vec2>& getZombieStartPositions() const ;
    const std::vector<glm::vec2>& getBreakableBrickStartPositions() const ;

    void setLevelData(glm::vec2 position);

private:
    std::vector<std::string> _levelData;
    int                      _numHumans;
    WTCEngine::SpriteBatch   _spriteBatch;

    glm::vec2                _startPlayerPos;
    std::vector<glm::vec2>   _zombieStartPositions;
    std::vector<glm::vec2>   _breakBrickPositions;
};
#endif //BOMBERMAN_LEVEL_H
