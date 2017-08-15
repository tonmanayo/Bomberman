//
// Created by tonmanayo on 2017/08/06.
//



#include "../inc/Level.hpp"


Level::Level(const std::string& fileName) {

    try {
        std::ifstream file;
        file.open(fileName);

        if (file.fail()) {
            WTCEngine::ErrorHandle("Failed to open " + fileName);
        }

        // Throw away the first string in tmp
        std::string tmp;

        file >> tmp >> _numHumans;

        std::getline(file, tmp); // Throw away the rest of the first line

        // Read the level data
        while (std::getline(file, tmp)) {
            _levelData.emplace_back(tmp);
        }

        _spriteBatch.init();
        _spriteBatch.begin();

        glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

        WTCEngine::Color whiteColor(255, 255, 255, 255);

        // Render all the map
        for (int y = 0; y < _levelData.size(); y++) {
            for (int x = 0; x < _levelData[y].size(); x++) {

                char tile = _levelData[y][x];
                // Get dest rect
                glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

                // Process the tile
                switch (tile) {
                    case 'B':
                    case 'R':
                        _spriteBatch.draw(destRect,
                                          uvRect,
                                          WTCEngine::ResourceManager::getTexture("Textures/red_bricks.png").id,
                                          0.0f,
                                          whiteColor);
                        break;
                    case 'G':
                        _spriteBatch.draw(destRect,
                                          uvRect,
                                          WTCEngine::ResourceManager::getTexture("Textures/glass.png").id,
                                          0.0f,
                                          whiteColor);
                        break;
                    case 'L':
                        _spriteBatch.draw(destRect,
                                          uvRect,
                                          WTCEngine::ResourceManager::getTexture("Textures/light_bricks.png").id,
                                          0.0f,
                                          whiteColor);
                        break;
                    case '@':
                        _levelData[y][x] = '.'; // So we dont collide with a @
                        _startPlayerPos.x = x * TILE_WIDTH;
                        _startPlayerPos.y = y * TILE_WIDTH;
                        break;
                    case 'Z':
                        _levelData[y][x] = '.'; // So we dont collide with a Z
                        _zombieStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
                        break;
                    case '.':
                        break;
                    default:
                        std::cout << "Unexpected symbol " << tile << "at: " <<  x << " " <<  y << std::endl;
                        break;
                }
            }
        }

        _spriteBatch.end();

    } catch (WTCEngine::ErrorHandle &errorhandle) {
        std::cout << errorhandle.what() << std::endl;
    }
}

Level::~Level() {}


void Level::draw() {
    _spriteBatch.renderBatch();
}

int Level::getWidth() const {
    return static_cast<int>(_levelData[0].size());
}

int Level::getHeight() const {
    return static_cast<int>(_levelData.size());
}