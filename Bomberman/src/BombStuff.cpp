#include <scene.hpp>

void Scene::bombExplode(Scene *scene, const Bomb &bomb) {

    int x = scene->getWorldx(bomb.getPosition().x);
    int y = scene->getWorldy(bomb.getPosition().z);

    bool r = true, l = true, u = true, d = true;
    for (int i = 0; i <= scene->_player->getPowerExplosion(); ++i) {

        if (scene->_blocks[y - 1 - i][x] != nullptr && scene->_blocks[y - 1 - i][x]->isBreakable() && d) {
            d = false;
            MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y - 1 - i][x]->getId());
            scene->_blocks[y - 1 - i][x]->setCollision(false);
        }
        if (scene->_blocks[y + 1 + i][x] != nullptr && scene->_blocks[y + 1 + i][x]->isBreakable() && u) {
            u = false;
            MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y + 1 + i][x]->getId());
            scene->_blocks[y + 1 + i][x]->setCollision(false);
        }
        if (scene->_blocks[y][x - 1 - i] != nullptr && scene->_blocks[y][x - 1 - i]->isBreakable() && l) {
            l = false;
            MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y][x - 1 - i]->getId());
            scene->_blocks[y][x - 1 - i]->setCollision(false);
        }
        if (scene->_blocks[y][x + 1 + i] != nullptr && scene->_blocks[y][x + 1 + i]->isBreakable() && r) {
            r = false;
            MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y][x + 1 + i]->getId());
            scene->_blocks[y][x + 1 + i]->setCollision(false);
        }
    }
}

/*
 * MainGame::explosionParticles->generateParticles({x, 0, y}, true);
            MainGame::explosionParticles->generateParticles({x - halfBlockSize, 0, y - halfBlockSize}, true);
            MainGame::explosionParticles->generateParticles({x + halfBlockSize, 0, y - halfBlockSize}, true);
            MainGame::explosionParticles->generateParticles({x + halfBlockSize, 0, y + halfBlockSize}, true);
            MainGame::explosionParticles->generateParticles({x - halfBlockSize, 0, y + halfBlockSize}, true);

             if (scene->_blocks[yy - 1 - i][xx] != nullptr && d) {
                d = false;
            }
            if (scene->_blocks[yy + 1 + i][xx] != nullptr && u) {
                u = false;
            }
            if (scene->_blocks[yy][xx - 1 - i] != nullptr && l) {
                l = false;
            }
            if (scene->_blocks[yy][xx + 1 + i] != nullptr && r) {
                r = false;
            }
             int x = scene->getGridx(bomb.getPosition().x);
    int y = scene->getGridy(bomb.getPosition().z);

    int xx = scene->getWorldx(bomb.getPosition().x);
    int yy = scene->getWorldy(bomb.getPosition().z);
 */

void    Scene::renderFlameExplosion(glm::vec3 flamePosition)
{
    float   half = (float)HALF_GRID_BLOCK_SIZE;
    float   quarter = half / 2.0f;

    MainGame::explosionParticles->generateParticles(flamePosition, true);
    //top left
    MainGame::explosionParticles->generateParticles({flamePosition.x - quarter, flamePosition.y, flamePosition.z - quarter}, true);
    //MainGame::explosionParticles->generateParticles({flamePosition.x - half, flamePosition.y, flamePosition.z - half}, true);
    //down left
    MainGame::explosionParticles->generateParticles({flamePosition.x - quarter, flamePosition.y, flamePosition.z + quarter}, true);
    //MainGame::explosionParticles->generateParticles({flamePosition.x - half, flamePosition.y, flamePosition.z + half}, true);
    //down right
    MainGame::explosionParticles->generateParticles({flamePosition.x + quarter, flamePosition.y, flamePosition.z + quarter}, true);
    //MainGame::explosionParticles->generateParticles({flamePosition.x + half, flamePosition.y, flamePosition.z + half}, true);
    //top right
    MainGame::explosionParticles->generateParticles({flamePosition.x + quarter, flamePosition.y, flamePosition.z - quarter}, true);
    //MainGame::explosionParticles->generateParticles({flamePosition.x + half, flamePosition.y, flamePosition.z - half}, true);
    //top
    //MainGame::explosionParticles->generateParticles({flamePosition.x, flamePosition.y, flamePosition.z - half}, true);
    //down
    //MainGame::explosionParticles->generateParticles({flamePosition.x, flamePosition.y, flamePosition.z + half}, true);
    //MainGame::explosionParticles->generateParticles({flamePosition.x + quarter, flamePosition.y, flamePosition.z + half}, true);
    //MainGame::explosionParticles->generateParticles({flamePosition.x - quarter, flamePosition.y, flamePosition.z + half}, true);
    //left
    //MainGame::explosionParticles->generateParticles({flamePosition.x - half, flamePosition.y, flamePosition.z}, true);
    //right
    //MainGame::explosionParticles->generateParticles({flamePosition.x + half, flamePosition.y, flamePosition.z}, true);

}

void Scene::renderExplosion(Scene *scene, const Bomb &bomb, MainGame *game) {
    glm::vec3   bombPosition = bomb.getPosition();

    int bombGridX = abs((int)scene->getGridx(bomb.getPosition().x) / 2);
    int bombGridY = abs((int)scene->getGridy(bomb.getPosition().z) / 2);
    int explosionLength = scene->getPlayer()->getPowerExplosion() + 1;

    std::cout << bombGridX << "  " << bombGridY << std::endl;

    renderFlameExplosion(bombPosition);
    /// draw vertical explosion
    for (int i = 1; i <= explosionLength; i++)
    {
        /// up vertical
        int  gridPosY = bombGridY - i;
        if (gridPosY > 0 && gridPosY < scene->_mapLength && scene->_blocks[gridPosY][bombGridX] == nullptr)
            renderFlameExplosion({bombPosition.x, bombPosition.y, bombPosition.z - (GRID_BLOCK_SIZE * i)});
        /// down vertical
        gridPosY = bombGridY + i;
        if (gridPosY > 0 && gridPosY < scene->_mapLength && scene->_blocks[gridPosY][bombGridX] == nullptr)
            renderFlameExplosion({bombPosition.x, bombPosition.y, bombPosition.z + (GRID_BLOCK_SIZE * i)});
    }

    /// draw horizontal explosion
    for (int i = 1; i <= explosionLength; i++)
    {
        /// left horizontal
        int  gridPosX = bombGridX - i;
        if (gridPosX > 0 && gridPosX < scene->_mapWidth && scene->_blocks[bombGridY][gridPosX] == nullptr)
            renderFlameExplosion({bombPosition.x - (GRID_BLOCK_SIZE * i), bombPosition.y, bombPosition.z});
        /// right horizontal
        gridPosX = bombGridX + i;
        if (gridPosX > 0 && gridPosX < scene->_mapWidth && scene->_blocks[bombGridY][gridPosX] == nullptr)
            renderFlameExplosion({bombPosition.x + (GRID_BLOCK_SIZE * i), bombPosition.y, bombPosition.z});
    }
}