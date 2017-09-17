#include <scene.hpp>

void Scene::bombExplode(Scene *scene, const Bomb &bomb) {

    int x = scene->getWorldx(bomb.getPosition().x);
    int y = scene->getWorldy(bomb.getPosition().z);

    bool r = true, l = true, u = true, d = true;
    bool r1 = true, l1 = true, u1 = true, d1 = true;
    for (int i = 0; i <= scene->_player->getPowerExplosion(); ++i) {

        if (scene->_blocks[y - 1 - i][x] != nullptr && scene->_blocks[y - 1 - i][x]->isBreakable() && d && d1) {
            d = false;
            MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y - 1 - i][x]->getId());
            scene->_blocks[y - 1 - i][x]->setCollision(false);
        }
        if (scene->_blocks[y + 1 + i][x] != nullptr && scene->_blocks[y + 1 + i][x]->isBreakable() && u && u1) {
            u = false;
            MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y + 1 + i][x]->getId());
            scene->_blocks[y + 1 + i][x]->setCollision(false);
        }
        if (scene->_blocks[y][x - 1 - i] != nullptr && scene->_blocks[y][x - 1 - i]->isBreakable() && l && l1) {
            l = false;
            MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y][x - 1 - i]->getId());
            scene->_blocks[y][x - 1 - i]->setCollision(false);
        }
        if (scene->_blocks[y][x + 1 + i] != nullptr && scene->_blocks[y][x + 1 + i]->isBreakable() && r && r1) {
            r = false;
            MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y][x + 1 + i]->getId());
            scene->_blocks[y][x + 1 + i]->setCollision(false);
        }
        if (scene->_blocks[y - 1 - i][x] != nullptr && d1) {
            d1 = false;
        }
        if (scene->_blocks[y + 1 + i][x] != nullptr && u1) {
            u1 = false;
        }
        if (scene->_blocks[y][x - 1 - i] != nullptr && l1) {
            l1 = false;
        }
        if (scene->_blocks[y][x + 1 + i] != nullptr && r1) {
            r1 = false;
        }
    }
}

void Scene::renderExplosion(Scene *scene, Bomb &bomb, MainGame *game) {
    int x = static_cast<int>(scene->getGridx(bomb.getPosition().x));
    int y = static_cast<int>(scene->getGridy(bomb.getPosition().z));

    int xx = scene->getWorldx(bomb.getPosition().x);
    int yy = scene->getWorldy(bomb.getPosition().z);

    glm::vec3   bombPosition = bomb.getPosition();

    bool r = true, l = true, u = true, d = true;
    Zion::Renderable *model;

	float   quarter = (float)HALF_GRID_BLOCK_SIZE / 2.0f;

	glm::vec3 middle = glm::vec3(x, 0, y);
	// renderFlameExplosion(middle);
	MainGame::explosionUp->generateParticles(middle, true);
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
            }
            if (d)
            {
	            MainGame::explosionDown->generateParticles(matdown, true);
	            MainGame::explosionDown->generateParticles(matdown + quarter, true);
	            MainGame::explosionDown->generateParticles(matdown - quarter, true);
            }
            if (l)
            {
	            MainGame::explosionLeft->generateParticles(matleft, true);
	            MainGame::explosionLeft->generateParticles(matleft + quarter, true);
	            MainGame::explosionLeft->generateParticles(matleft - quarter, true);
            }
            if (r)
            {
	            MainGame::explosionRight->generateParticles(matright, true);
	            MainGame::explosionRight->generateParticles(matright + quarter, true);
	            MainGame::explosionRight->generateParticles(matright - quarter, true);
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
