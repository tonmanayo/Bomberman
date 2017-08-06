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



const int TILE_WIDTH = 64;

class Level
{
public:
    // Load the level
    Level(const std::string& fileName);
    ~Level();

    void draw();

    // Getters
    int getWidth() const { return _levelData[0].size(); }
    int getHeight() const { return _levelData.size(); }
    int getNumHumans() const { return _numHumans; }
    const std::vector<std::string>& getLevelData() const { return _levelData; }
    glm::vec2 getStartPlayerPos() const { return _startPlayerPos; }
    const std::vector<glm::vec2>& getZombieStartPositions() const { return _zombieStartPositions; }

private:
    std::vector<std::string> _levelData;
    int _numHumans;
    WTCEngine::SpriteBatch _spriteBatch;

    glm::vec2 _startPlayerPos;
    std::vector<glm::vec2> _zombieStartPositions;
};
#endif //BOMBERMAN_LEVEL_H
