#include <scene.hpp>
#include <random>

void Scene::updatePlayer(MainGame *game, std::vector<void *> params)
{
	auto *scene = (Scene *)params[0];

	if (game->getGameWindow().isKeyPressed(GLFW_KEY_S))
	{
		if (!worldCollisionDown(scene->_player->getPosition(), {0, 0, 0.02f}, scene))
			scene->_player->changePosZ(0.02f);
		scene->_player->rotate(glm::radians(0.0f), {0, 1, 0});
		MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(),
		              scene->_player->getId(), scene->_player->getTransformation());
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
		MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(),
		              scene->_player->getId(), scene->_player->getTransformation());
		glm::vec3 pos = scene->_player->getPosition();
		scene->_game->getGameCamera().setCameraPosition(
				glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
		scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
		scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
	}
	if (game->getGameWindow().isKeyPressed(GLFW_KEY_A))
	{
		if (!worldCollisionLeft(scene->_player->getPosition(), {-0.1f, 0, 0}, scene))
			scene->_player->changePosX(-0.02f);
		scene->_player->rotate(glm::radians(-90.0f), {0, 1, 0});
		MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(),
		              scene->_player->getId(), scene->_player->getTransformation());
		glm::vec3 pos = scene->_player->getPosition();
		scene->_game->getGameCamera().setCameraPosition(
				glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
		scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
		scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
	}
	if (game->getGameWindow().isKeyPressed(GLFW_KEY_D))
	{
		if (!worldCollisionRight(scene->_player->getPosition(), {0.1f, 0, 0}, scene))
			scene->_player->changePosX(0.02f);
		scene->_player->rotate(glm::radians(90.0f), {0, 1, 0});
		MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(),
		              scene->_player->getId(), scene->_player->getTransformation());
		glm::vec3 pos = scene->_player->getPosition();
		scene->_game->getGameCamera().setCameraPosition(
				glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
		scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
		scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
	}
	if (game->getGameWindow().isKeyPressed(GLFW_KEY_SPACE) && scene->_nbBombs < 1)
	{
            scene->_addBomb(scene->_player->getPosition().x, scene->_player->getPosition().z);
            scene->_nbBombs++;
	}
}

void Scene::bombExplode(std::vector<void *> params, const Bomb &bomb) {
    auto *scene = (Scene *)params[0];

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

void Scene::updateBomb(MainGame *game, std::vector<void *> params) {
	auto *scene = (Scene *)params[0];
	for (int i = 0; i < scene->_bomb.size(); ++i) {
		if (scene->_bomb[i].explodeTime())
		{
            scene->_nbBombs--;
            bombExplode(params, scene->_bomb[i]);
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


void Scene::updateEnemy(MainGame *game, std::vector<void *> params) {
    auto *scene = (Scene *)params[0];
    srand(time(NULL));
    bool collision = false;

    char dir[3] = {'N','N','N'};

    for (int i = 0; i < scene->_enemies.size(); i++) {

        if (scene->enemyPlayerCollision(scene->_enemies[i]->getPosition(), scene)) {
            std::cout << "dead\n";
            scene->_player->reset();
            glm::vec3 pos = scene->_player->getPosition();
            scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
            scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
            scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
            MainGame::renderer.applyTransformationToRenderable("player", scene->_player->getId(), scene->_player->getTransformation());
        }
        int x = rand() % 3;
        if (enemyWorldCollisionDown(scene->_enemies[i]->getPosition(), {0.0f, 0.0f, 0.02f}, scene)) {
            collision = true;
            dir[0] = 'U';
            dir[1] = 'L';
            dir[2] = 'R';
            if (scene->_blocks[scene->getWorldy(scene->_enemies[i]->getPosition().z)][scene->getGridx(scene->_enemies[i]->getPosition().x - 1)] ==
                    nullptr) {
                dir[1] = 'U';
            }
            if (scene->_blocks[scene->getWorldy(scene->_enemies[i]->getPosition().z)][scene->getGridx(scene->_enemies[i]->getPosition().x + 1)] ==
                nullptr) {
                dir[2] = 'U';
            }
        }
        if (enemyWorldCollisionUp(scene->_enemies[i]->getPosition(), {0.0f, 0.0f, -0.02f}, scene)) {
            collision = true;
            dir[0] = 'D';
            dir[1] = 'L';
            dir[2] = 'R';
            if (scene->_blocks[scene->getWorldy(scene->_enemies[i]->getPosition().z)][scene->getGridx(scene->_enemies[i]->getPosition().x) - 1] !=
                nullptr) {
                dir[1] = 'D';
            }
            if (scene->_blocks[scene->getWorldy(scene->_enemies[i]->getPosition().z)][scene->getGridx(scene->_enemies[i]->getPosition().x) + 1] !=
                nullptr) {
                dir[2] = 'D';
            }
        }
        if (enemyWorldCollisionRight(scene->_enemies[i]->getPosition(), {0.01f, 0.0f, 0.0f}, scene)) {
            collision = true;
            dir[0] = 'L';
            dir[1] = 'U';
            dir[2] = 'D';
            if (scene->_blocks[scene->getWorldy(scene->_enemies[i]->getPosition().z) + 1][scene->getGridx(scene->_enemies[i]->getPosition().x)] !=
                nullptr) {
                dir[1] = 'L';
            }
            if (scene->_blocks[scene->getWorldy(scene->_enemies[i]->getPosition().z) - 1][scene->getGridx(scene->_enemies[i]->getPosition().x)] !=
                nullptr) {
                dir[2] = 'L';
            }

        }
        if (enemyWorldCollisionLeft(scene->_enemies[i]->getPosition(), {-0.01f, 0.0f, 0.0f}, scene)) {
            collision = true;
            dir[0] = 'R';
            dir[1] = 'U';
            dir[2] = 'D';
            if (scene->_blocks[scene->getWorldy(scene->_enemies[i]->getPosition().z) + 1][scene->getGridx(scene->_enemies[i]->getPosition().x)] !=
                nullptr) {
                dir[1] = 'R';
            }
            if (scene->_blocks[scene->getWorldy(scene->_enemies[i]->getPosition().z) - 1][scene->getGridx(scene->_enemies[i]->getPosition().x)] !=
                nullptr) {
                dir[2] = 'R';
            }

        }
        if (collision) {
            if (dir[x] == 'U') {
                //scene->_enemies[i]->changePosZ(-0.010f);
                scene->_enemies[i]->setDirectionx(0.0f);
                scene->_enemies[i]->setDirectiony(-0.01f);
                scene->_enemies[i]->rotate(glm::radians(-180.0f), {0, 1, 0});

            } else if (dir[x] == 'D') {
                  //scene->_enemies[i]->changePosZ(0.020f);
                scene->_enemies[i]->rotate(glm::radians(0.0f), {0, 1, 0});
                scene->_enemies[i]->setDirectionx(0.0f);
                scene->_enemies[i]->setDirectiony(0.01f);

            } else if (dir[x] == 'L') {
                //scene->_enemies[i]->changePosX(-0.01f);
                scene->_enemies[i]->rotate(glm::radians(-90.0f), {0, 1, 0});
                scene->_enemies[i]->setDirectionx(-0.01f);
                scene->_enemies[i]->setDirectiony(0.0f);

            } else if (dir[x] == 'R') {
                //scene->_enemies[i]->changePosX(0.020f);
                scene->_enemies[i]->rotate(glm::radians(90.0f), {0, 1, 0});
                scene->_enemies[i]->setDirectionx(0.01f);
                scene->_enemies[i]->setDirectiony(0.0f);

            }
            collision = false;
        }
        scene->_enemies[i]->changePosZ(scene->_enemies[i]->getDirectiony());
        scene->_enemies[i]->changePosX(scene->_enemies[i]->getDirectionx());

        MainGame::renderer.applyTransformationToRenderable(scene->_enemies[i]->getType(),
														   scene->_enemies[i]->getId(), scene->_enemies[i]->getTransformation());
    }

}