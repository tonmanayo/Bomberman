#include <scene.hpp>

void Scene::bombExplode(Scene *scene, const Bomb &bomb) {

    int x = scene->getWorldx(bomb.getPosition().x);
    int y = scene->getWorldy(bomb.getPosition().z);
    //todo add for loop for power up
    if (scene->breakableBrickCollisionDown(bomb.getPosition(), scene))
    {
        MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y - 1][x]->getId());
        scene->_blocks[y - 1][x]->setCollision(false);
    }
    if (scene->breakableBrickCollisionUp(bomb.getPosition(), scene))
    {
        MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y + 1][x]->getId());
        scene->_blocks[y + 1][x]->setCollision(false);
    }
    if (scene->breakableBrickCollisionLeft(bomb.getPosition(), scene))
    {
        MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y][x - 1]->getId());
        scene->_blocks[y][x - 1]->setCollision(false);
    }
    if (scene->breakableBrickCollisionRight(bomb.getPosition(), scene))
    {
        MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y][x + 1]->getId());
        scene->_blocks[y][x + 1]->setCollision(false);
    }

}

void Scene::renderExplosion(Scene *scene, const Bomb &bomb, MainGame *game) {
    int x = scene->getGridx(bomb.getPosition().x);
    int y = scene->getGridy(bomb.getPosition().z);

    Zion::Renderable *model;
    model = game->getModel("explosion");

    for (int j = 1; j < 2; ++j ) {
            glm::mat4 middle = glm::translate(glm::mat4(), glm::vec3(x, 0, y));
            glm::mat4 matup = glm::translate(glm::mat4(), glm::vec3(x, 0,  y - (j * GRID_BLOCK_SIZE)));
            glm::mat4 matdown = glm::translate(glm::mat4(), glm::vec3(x, 0,  y + j * GRID_BLOCK_SIZE));
            glm::mat4 matleft = glm::translate(glm::mat4(), glm::vec3(x - (j * GRID_BLOCK_SIZE), 0,  y));
            glm::mat4 matright = glm::translate(glm::mat4(), glm::vec3(x + j * GRID_BLOCK_SIZE, 0,  y));
        if (model != nullptr)
        {
            MainGame::renderer.addToRender("explosion", bomb.getId() + 1, model, middle);
            MainGame::renderer.addToRender("explosion", bomb.getId() + 2, model, matup);
            MainGame::renderer.addToRender("explosion", bomb.getId() + 3, model, matdown);
            MainGame::renderer.addToRender("explosion", bomb.getId() + 4, model, matleft);
            MainGame::renderer.addToRender("explosion", bomb.getId() + 5, model, matright);
        }
    }
}