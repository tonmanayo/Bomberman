#include <scene.hpp>

void Scene::updatePlayer(MainGame *game, Scene *scene)
{
	if (game->getGameWindow().isKeyPressed(GLFW_KEY_S))
	{
		if (!worldCollisionDown(scene->_player->getPosition(), {0, 0, 0.02f}, scene))
			scene->_player->changePosZ(0.02f);
		scene->_player->rotate(glm::radians(0.0f), {0, 1, 0});
		MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(),  scene->_player->getId(), scene->_player->getTransformation());
		glm::vec3 pos = scene->_player->getPosition();
		scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
		scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
		scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
	}
	if (game->getGameWindow().isKeyPressed(GLFW_KEY_W))
	{
		if (!worldCollisionUp(scene->_player->getPosition(), {0, 0, -0.2f}, scene))
			scene->_player->changePosZ(-0.02f);
		scene->_player->rotate(glm::radians(180.0f), {0, 1, 0});
		MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(), scene->_player->getId(), scene->_player->getTransformation());
		glm::vec3 pos = scene->_player->getPosition();
		scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
		scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
		scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
	}
	if (game->getGameWindow().isKeyPressed(GLFW_KEY_A))
	{
		if (!worldCollisionLeft(scene->_player->getPosition(), {-0.1f, 0, 0}, scene))
			scene->_player->changePosX(-0.02f);
		scene->_player->rotate(glm::radians(-90.0f), {0, 1, 0});
		MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(), scene->_player->getId(), scene->_player->getTransformation());
		glm::vec3 pos = scene->_player->getPosition();
		scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
		scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
		scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
	}
	if (game->getGameWindow().isKeyPressed(GLFW_KEY_D))
	{
		if (!worldCollisionRight(scene->_player->getPosition(), {0.1f, 0, 0}, scene))
			scene->_player->changePosX(0.02f);
		scene->_player->rotate(glm::radians(90.0f), {0, 1, 0});
		MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(), scene->_player->getId(), scene->_player->getTransformation());
		glm::vec3 pos = scene->_player->getPosition();
		scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
		scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
		scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
	}
	if (game->getGameWindow().isKeyPressed(GLFW_KEY_SPACE) && scene->_nbBombs < 1)
	{
            scene->_addBomb(scene->_player->getPosition().x, scene->_player->getPosition().z);
            scene->_nbBombs++;
	}
}

void Scene::bombExplode(Scene *scene, const Bomb &bomb) {
    //todo add for loop for power up
    if (scene->breakableBrickCollisionDown(bomb.getPosition(), scene))
    {
        MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[scene->getWorldy(bomb.getPosition().z) - 1][scene->getWorldx(bomb.getPosition().x)]->getId());
        scene->_blocks[scene->getWorldy(bomb.getPosition().z) - 1][scene->getWorldx(bomb.getPosition().x)] = nullptr;
    }
    if (scene->breakableBrickCollisionUp(bomb.getPosition(), scene))
    {
        MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[scene->getWorldy(bomb.getPosition().z) + 1][scene->getWorldx(bomb.getPosition().x)]->getId());
        scene->_blocks[scene->getWorldy(bomb.getPosition().z) + 1][scene->getWorldx(bomb.getPosition().x)] = nullptr;
    }
    if (scene->breakableBrickCollisionLeft(bomb.getPosition(), scene))
    {
        MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[scene->getWorldy(bomb.getPosition().z)][scene->getWorldx(bomb.getPosition().x) - 1]->getId());
        scene->_blocks[scene->getWorldy(bomb.getPosition().z)][scene->getWorldx(bomb.getPosition().x) - 1] = nullptr;
    }
    if (scene->breakableBrickCollisionRight(bomb.getPosition(), scene))
    {
        MainGame::renderer.removeFromRender("breakBlock", scene->_blocks[scene->getWorldy(bomb.getPosition().z)][scene->getWorldx(bomb.getPosition().x) + 1]->getId());
        scene->_blocks[scene->getWorldy(bomb.getPosition().z)][scene->getWorldx(bomb.getPosition().x) + 1] = nullptr;
    }

}

void Scene::updateBomb(MainGame *game, Scene *scene) {
	for (int i = 0; i < scene->_bomb.size(); ++i) {
		if (scene->_bomb[i].explodeTime())
		{
            scene->_nbBombs--;
            bombExplode(scene, scene->_bomb[i]);
            if(scene->PlayerExplosionCollision(scene->_bomb[i].getPosition(), scene))
            {
                std::cout << "dead\n";
                scene->_player->reset();
                glm::vec3 pos = scene->_player->getPosition();
                scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
                scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
                scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
                MainGame::renderer.applyTransformationToRenderable("player", scene->_player->getId(), scene->_player->getTransformation());
            }
			MainGame::renderer.removeFromRender("bomb", scene->_bomb[i].getId());
			delete scene->_blocks[scene->getWorldy(scene->_bomb[i].getPosition().z)][scene->getWorldx(scene->_bomb[i].getPosition().x)];
			scene->_blocks[scene->getWorldy(scene->_bomb[i].getPosition().z)][scene->getWorldx(scene->_bomb[i].getPosition().x)] = nullptr;
			scene->_bomb[i] = scene->_bomb.back();
			scene->_bomb.pop_back();
		}
	}
}


void Scene::updateEnemy(MainGame *game, Scene *scene) {
    for (int i = 0; i < scene->_enemies.size(); ++i) {
        scene->_enemies[i].changePosZ(0.02f);
        //std::cout << scene->_enemies[i].getPosition().z << std::endl;
    }
}