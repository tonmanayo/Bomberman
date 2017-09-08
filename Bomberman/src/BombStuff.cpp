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

void Scene::renderExplosion(Scene *scene, const Bomb &bomb, MainGame *game) {
    int x = scene->getGridx(bomb.getPosition().x);
    int y = scene->getGridy(bomb.getPosition().z);

    Zion::Renderable *model;
    model = game->getModel("explosion");

    for (int j = 1; j < 2 + scene->_player->getPowerExplosion(); ++j ) {
            glm::mat4 middle = glm::translate(glm::mat4(), glm::vec3(x, 0, y));
            glm::mat4 matup = glm::translate(glm::mat4(), glm::vec3(x, 0,  y - (j * GRID_BLOCK_SIZE)));
            glm::mat4 matdown = glm::translate(glm::mat4(), glm::vec3(x, 0,  y + j * GRID_BLOCK_SIZE));
            glm::mat4 matleft = glm::translate(glm::mat4(), glm::vec3(x - (j * GRID_BLOCK_SIZE), 0,  y));
            glm::mat4 matright = glm::translate(glm::mat4(), glm::vec3(x + j * GRID_BLOCK_SIZE, 0,  y));
        if (model != nullptr)
        {
            MainGame::renderer.addToRender("explosion", bomb.getId() , model, middle);
            MainGame::renderer.addToRender("explosion1", bomb.getId() + j - 1, model, matup);
            MainGame::renderer.addToRender("explosion2", bomb.getId() + j - 1, model, matdown);
            MainGame::renderer.addToRender("explosion3", bomb.getId() + j - 1, model, matleft);
            MainGame::renderer.addToRender("explosion4", bomb.getId() + j - 1, model, matright);
        }
    }
}