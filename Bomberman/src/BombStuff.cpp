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

void Scene::renderExplosion(Scene *scene, Bomb &bomb, MainGame *game) {
    int x = scene->getGridx(bomb.getPosition().x);
    int y = scene->getGridy(bomb.getPosition().z);

    int xx = scene->getWorldx(bomb.getPosition().x);
    int yy = scene->getWorldy(bomb.getPosition().z);

    glm::vec3   bombPosition = bomb.getPosition();

    bool r = true, l = true, u = true, d = true;
    Zion::Renderable *model;

	float   quarter = (float)HALF_GRID_BLOCK_SIZE / 2.0f;

	glm::vec3 middle = glm::vec3(x, 0, y);
	// renderFlameExplosion(middle);
	MainGame::explosionUp->generateParticles(middle, true);
	MainGame::explosionDown->generateParticles(middle, true);
	MainGame::explosionLeft->generateParticles(middle, true);
	MainGame::explosionRight->generateParticles(middle, true);

    for (int i = 0; i < 1 + scene->_player->getPowerExplosion(); ++i ) {
	    if (i > bomb.getExplosionCount())
		    return bomb.increaseExplosionCount(i);
        glm::vec3 matup = glm::vec3(x, 0,  y - ((i + 1) * GRID_BLOCK_SIZE));
        glm::vec3 matdown = glm::vec3(x, 0,  y + (i + 1) * GRID_BLOCK_SIZE);
        glm::vec3 matleft = glm::vec3(x - ((i + 1)  * GRID_BLOCK_SIZE), 0,  y);
        glm::vec3 matright = glm::vec3(x + (i + 1) * GRID_BLOCK_SIZE, 0,  y);
        if (model != nullptr)
        {
            if (u)
            {
	            MainGame::explosionUp->generateParticles(matup, true);
	            MainGame::explosionUp->generateParticles(matup + quarter, true);
	            MainGame::explosionUp->generateParticles(matup - quarter, true);
	            //renderFlameExplosion(matup);
            }
            if (d)
            {
	            MainGame::explosionDown->generateParticles(matdown, true);
	            MainGame::explosionDown->generateParticles(matdown + quarter, true);
	            MainGame::explosionDown->generateParticles(matdown - quarter, true);
	            //renderFlameExplosion(matdown);
            }
            if (l)
            {
	            MainGame::explosionLeft->generateParticles(matleft, true);
	            MainGame::explosionLeft->generateParticles(matleft + quarter, true);
	            MainGame::explosionLeft->generateParticles(matleft - quarter, true);
	            //renderFlameExplosion(matleft);
            }
            if (r)
            {
	            MainGame::explosionRight->generateParticles(matright, true);
	            MainGame::explosionRight->generateParticles(matright + quarter, true);
	            MainGame::explosionRight->generateParticles(matright - quarter, true);
	            //renderFlameExplosion(matright);
            }

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
        }
    }
}


void    Scene::renderFlameExplosion(glm::vec3 flamePosition)
{
    float   half = (float)HALF_GRID_BLOCK_SIZE;
    float   quarter = half / 2.0f;

    //MainGame::explosionLeft->generateParticles(flamePosition, true);
    //top left
    //MainGame::explosionParticles->generateParticles({flamePosition.x - quarter, flamePosition.y, flamePosition.z - quarter}, true);
    //MainGame::explosionParticles->generateParticles({flamePosition.x - half, flamePosition.y, flamePosition.z - half}, true);
    //down left
    //MainGame::explosionParticles->generateParticles({flamePosition.x - quarter, flamePosition.y, flamePosition.z + quarter}, true);
    //MainGame::explosionParticles->generateParticles({flamePosition.x - half, flamePosition.y, flamePosition.z + half}, true);
    //down right
    //MainGame::explosionParticles->generateParticles({flamePosition.x + quarter, flamePosition.y, flamePosition.z + quarter}, true);
    //MainGame::explosionParticles->generateParticles({flamePosition.x + half, flamePosition.y, flamePosition.z + half}, true);
    //top right
   // MainGame::explosionParticles->generateParticles({flamePosition.x + quarter, flamePosition.y, flamePosition.z - quarter}, true);
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