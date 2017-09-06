#include <scene.hpp>

void Scene::bombExplode(Scene *scene, const Bomb &bomb) {

    int x = scene->getWorldx(bomb.getPosition().x);
    int y = scene->getWorldy(bomb.getPosition().z);
    //todo add for loop for power up
    if (scene->breakableBrickCollisionDown(bomb.getPosition(), scene))
    {
        MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y - 1][x]->getId());
        scene->_blocks[y - 1][x] = nullptr;
    }
    if (scene->breakableBrickCollisionUp(bomb.getPosition(), scene))
    {
        MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y + 1][x]->getId());
        scene->_blocks[y + 1][x] = nullptr;
    }
    if (scene->breakableBrickCollisionLeft(bomb.getPosition(), scene))
    {
        MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y][x - 1]->getId());
        scene->_blocks[y][x - 1] = nullptr;
    }
    if (scene->breakableBrickCollisionRight(bomb.getPosition(), scene))
    {
        MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[y][x + 1]->getId());
        scene->_blocks[y][x + 1] = nullptr;
    }

}

void Scene::renderExplosion(Scene *scene, const Bomb &bomb, MainGame *game) {
    int x = scene->getGridx(bomb.getPosition().x);
    int y = scene->getGridy(bomb.getPosition().z);

    Zion::Renderable *model;
    model = game->getModel("explosion");

    for (int j = 0; j < 2; ++j ) {
            glm::mat4 matup = glm::translate(glm::mat4(), glm::vec3(x, 0,  y - (j * GRID_BLOCK_SIZE)));
            glm::mat4 matdown = glm::translate(glm::mat4(), glm::vec3(x, 0,  y + j * GRID_BLOCK_SIZE));
            glm::mat4 matleft = glm::translate(glm::mat4(), glm::vec3(x - (j * GRID_BLOCK_SIZE), 0,  y));
            glm::mat4 matright = glm::translate(glm::mat4(), glm::vec3(x + j * GRID_BLOCK_SIZE, 0,  y));
        if (model != nullptr)
        {
            MainGame::renderer.addToRender("explosion", 0, model, matup);
         //   MainGame::renderer.addToRender("explosion", 0, model, matdown);
          //  MainGame::renderer.addToRender("explosion", 0, model, matleft);
           // MainGame::renderer.addToRender("explosion", 0, model, matright);
        }
    }
}