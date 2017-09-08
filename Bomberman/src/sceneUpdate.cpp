#include <scene.hpp>
#include <random>

void Scene::updatePlayer(MainGame *game, Scene *scene) {
    if (game->getGameWindow().isKeyPressed(GLFW_KEY_S)) {
        if (!worldCollisionDown(scene->_player->getPosition(), {0, 0, -0.02f}, scene))
            scene->_player->changePosZ(0.02f);
        scene->_player->rotate(glm::radians(0.0f), {0, 1, 0});
        MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(), scene->_player->getId(),
                                                           scene->_player->getTransformation());
        glm::vec3 pos = scene->_player->getPosition();
        scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
        scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
        scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
    }
    if (game->getGameWindow().isKeyPressed(GLFW_KEY_W)) {
        if (!worldCollisionUp(scene->_player->getPosition(), {0, 0, -0.2f}, scene))
            scene->_player->changePosZ(-0.02f);
        scene->_player->rotate(glm::radians(180.0f), {0, 1, 0});
        MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(), scene->_player->getId(),
                                                           scene->_player->getTransformation());
        glm::vec3 pos = scene->_player->getPosition();
        scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
        scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
        scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
    }
    if (game->getGameWindow().isKeyPressed(GLFW_KEY_A)) {
        if (!worldCollisionLeft(scene->_player->getPosition(), {-0.2f, 0, 0}, scene))
            scene->_player->changePosX(-0.02f);
        scene->_player->rotate(glm::radians(-90.0f), {0, 1, 0});
        MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(), scene->_player->getId(),
                                                           scene->_player->getTransformation());
        glm::vec3 pos = scene->_player->getPosition();
        scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
        scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
        scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
    }
    if (game->getGameWindow().isKeyPressed(GLFW_KEY_D)) {
        if (!worldCollisionRight(scene->_player->getPosition(), {0.0f, 0, 0}, scene))
            scene->_player->changePosX(0.02f);
        scene->_player->rotate(glm::radians(90.0f), {0, 1, 0});
        MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(), scene->_player->getId(),
                                                           scene->_player->getTransformation());
        glm::vec3 pos = scene->_player->getPosition();
        scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
        scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
        scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
    }
    if (game->getGameWindow().isKeyPressed(GLFW_KEY_SPACE) && scene->_nbBombs < 1 + scene->_player->getPowerBombNbr()) {
        scene->_addBomb(scene->_player->getPosition().x, scene->_player->getPosition().z);
    }
    if (scene->worldEndLevel(scene->_player->getPosition(), scene)) {
        std::cout << "FINISHED LEVEL!!!\n";
    }
    worldGetPower(scene->_player->getPosition(), scene);
}

void Scene::updateBomb(MainGame *game, Scene *scene) {

	for (int i = 0; i < scene->_bomb.size(); ++i) {
		if (scene->_bomb[i].explodeTime() && !scene->_bomb[i].getExploded())
		{
            scene->_bomb[i].setExploded(true);
            renderExplosion(scene, scene->_bomb[i], game);
            bombExplode(scene, scene->_bomb[i]);
            enemiesExplosionCollision(scene->_bomb[i].getPosition(), scene);
            if (scene->PlayerExplosionCollision(scene->_bomb[i].getPosition(), scene)) {
                std::cout << "dead\n";
                scene->_player->reset();
                glm::vec3 pos = scene->_player->getPosition();
                scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
                scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
                scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
                MainGame::renderer.applyTransformationToRenderable("player", scene->_player->getId(),
                                                                   scene->_player->getTransformation());
            }
            MainGame::renderer.removeFromRender("bomb", scene->_bomb[i].getId());
        }
        if (scene->_bomb[i].removeExplosionTime() && !scene->_bomb[i].getExplosionRemoved()) {
            scene->_bomb[i].setExplosionRemoved(true);
            MainGame::renderer.removeFromRender("explosion", scene->_bomb[i].getId());
            MainGame::renderer.removeFromRender("explosion", scene->_bomb[i].getId() + 1);
            MainGame::renderer.removeFromRender("explosion", scene->_bomb[i].getId() + 2);
            MainGame::renderer.removeFromRender("explosion", scene->_bomb[i].getId() + 3);
            MainGame::renderer.removeFromRender("explosion", scene->_bomb[i].getId() + 4);
        }
        if (scene->_bomb[i].explodeTime() && scene->_bomb[i].removeExplosionTime() && scene->_bomb[i].getExplosionRemoved() && scene->_bomb[i].getExploded()) {
            int y = scene->getWorldy(scene->_bomb[i].getPosition().z);
            int x = scene->getWorldx(scene->_bomb[i].getPosition().x);
            scene->_bomb[i] = scene->_bomb.back();
            scene->_bomb.pop_back();
            delete scene->_blocks[y][x];
            scene->_blocks[y][x] = nullptr;
            scene->_nbBombs--;
        }

	}
}