#include <scene.hpp>

void Scene::bombExplode(Scene *scene, const Bomb &bomb) {

    int x = scene->getWorldx(bomb.getPosition().x);
    int y = scene->getWorldy(bomb.getPosition().z);

    bool r = true, l = true, u = true, d = true;
    for (int i = 0; i <= scene->_player->getPowerExplosion(); ++i) {

        if (scene->_blocks[y - 1 - i][x] != nullptr && scene->_blocks[y - 1 - i][x]->getCollision() && d) {
            if (scene->_blocks[y - 1 - i][x]->isBreakable()) {
                MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y - 1 - i][x]->getId());
                scene->_blocks[y - 1 - i][x]->setCollision(false);
            }
            d = false;
        }
        if (scene->_blocks[y + 1 + i][x] != nullptr && scene->_blocks[y + 1 + i][x]->getCollision() && u) {
            if (scene->_blocks[y + 1 + i][x]->isBreakable()) {
                MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y + 1 + i][x]->getId());
                scene->_blocks[y + 1 + i][x]->setCollision(false);
            }
            u = false;
        }
        if (scene->_blocks[y][x - 1 - i] != nullptr && scene->_blocks[y][x - 1 - i]->getCollision() && l) {
            if (scene->_blocks[y][x - 1 - i]->isBreakable() ) {
                MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y][x - 1 - i]->getId());
                scene->_blocks[y][x - 1 - i]->setCollision(false);
            }
            l = false;
        }
        if (scene->_blocks[y][x + 1 + i] != nullptr && scene->_blocks[y][x + 1 + i]->getCollision() && r) {
            if (scene->_blocks[y][x + 1 + i]->isBreakable()) {
                MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y][x + 1 + i]->getId());
                scene->_blocks[y][x + 1 + i]->setCollision(false);
            }
            r = false;
        }
    }
}

void Scene::renderExplosion(Scene *scene, Bomb &bomb, MainGame *game) {
	(void)game;
    int x = static_cast<int>(scene->getGridx(bomb.getPosition().x));
    int y = static_cast<int>(scene->getGridy(bomb.getPosition().z));

    int xx = scene->getWorldx(bomb.getPosition().x);
    int yy = scene->getWorldy(bomb.getPosition().z);

    bool r = true, l = true, u = true, d = true;

	float   quarter = (float)HALF_GRID_BLOCK_SIZE / 2.0f;

	glm::vec3 middle = glm::vec3(x, 0, y);
	// renderFlameExplosion(middle);
	MainGame::explosionY->generateParticles(middle, true);

    for (int i = 0; i < 1 + scene->_player->getPowerExplosion(); ++i ) {

        glm::vec3 matup = glm::vec3(x, 0,  y - ((i + 1) * GRID_BLOCK_SIZE));
        glm::vec3 matdown = glm::vec3(x, 0,  y + (i + 1) * GRID_BLOCK_SIZE);
        glm::vec3 matleft = glm::vec3(x - ((i + 1)  * GRID_BLOCK_SIZE), 0,  y);
        glm::vec3 matright = glm::vec3(x + (i + 1) * GRID_BLOCK_SIZE, 0,  y);

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

        if (u)
        {
            MainGame::explosionY->generateParticles(matup, true);
            MainGame::explosionY->generateParticles(matup + quarter, true);
            MainGame::explosionY->generateParticles(matup - quarter, true);
        }
        if (d)
        {
            MainGame::explosionY->generateParticles(matdown, true);
            MainGame::explosionY->generateParticles(matdown + quarter, true);
            MainGame::explosionY->generateParticles(matdown - quarter, true);
        }
        if (l)
        {
            MainGame::explosionY->generateParticles(matleft, true);
            MainGame::explosionY->generateParticles(matleft + quarter, true);
            MainGame::explosionY->generateParticles(matleft - quarter, true);
        }
        if (r)
        {
            MainGame::explosionY->generateParticles(matright, true);
            MainGame::explosionY->generateParticles(matright + quarter, true);
            MainGame::explosionY->generateParticles(matright - quarter, true);
        }
    }

}
