#include <random>
#include <menu.hpp>

void Scene::updatePlayer(MainGame *game, Scene *scene) {

	float   velocity =  (1.0f + scene->_player->getPowerSpeed() / 3.5f) * Zion::Renderable::deltaTime;
	float   positionChange = (5.0f + scene->_player->getPowerSpeed()) * Zion::Renderable::deltaTime;

    if (!scene->_endLevel) {

            for (int j = 0; j < (int)scene->_mapLength; ++j) {
                for (int k = 0; k < (int)scene->_mapWidth; ++k) {
                    if (scene->_blocks[j][k] != nullptr && scene->_blocks[j][k]->isBreakable() &&
                            !scene->_blocks[j][k]->getPowerUp()) {
                        scene->_blocks[j][k]->setEndMap(true);
                        scene->_blocks[j][k]->setPosition(k * (int)GRID_BLOCK_SIZE, 0, -j * (int)GRID_BLOCK_SIZE);
                        scene->_endLevel = true;
                        Zion::Renderable *endLevel = scene->_game->getModel("floor2");
                        if (endLevel != nullptr)
                        {
                            glm::mat4 mat1 = glm::translate(glm::mat4(), glm::vec3(k * (int)GRID_BLOCK_SIZE, -1, -j * (int)GRID_BLOCK_SIZE));
                            MainGame::renderer.addToRender("endLevel", 0, endLevel, mat1);
                        }
                        break ;
                    }
                }
            }
    }
    if (game->getGameWindow().isKeyPressed(Menu::options.moveDown.glfwValue)) {
        Menu::playPlayerWalking();
        if (!worldCollisionDown(scene->_player, {0.0f, 0.0f, positionChange}, scene))
            scene->_player->changePosZ(positionChange);
        scene->_player->rotate(glm::radians(0.0f), {0, 1, 0});
        MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(), scene->_player->getId(),
                                                           scene->_player->getTransformation());
	    game->renderer.increaseAnimeTime("player", 0, velocity);
        glm::vec3 pos = scene->_player->getPosition();
        scene->_player->setDirection('D');
        scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x, pos.y + 11, pos.z + 6));
        scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
        scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
    }
    if (game->getGameWindow().isKeyPressed(Menu::options.moveUp.glfwValue)) {
        Menu::playPlayerWalking();
        if (!worldCollisionUp(scene->_player, {0, 0, -positionChange}, scene))
            scene->_player->changePosZ(-positionChange);
        scene->_player->rotate(glm::radians(180.0f), {0, 1, 0});
        MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(), scene->_player->getId(),
                                                           scene->_player->getTransformation());
	    game->renderer.increaseAnimeTime("player", 0, velocity);
        scene->_player->setDirection('U');
        glm::vec3 pos = scene->_player->getPosition();
        scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x, pos.y + 11, pos.z + 6));
        scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
        scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
    }
    if (game->getGameWindow().isKeyPressed(Menu::options.moveLeft.glfwValue)) {
        Menu::playPlayerWalking();
        if (!worldCollisionLeft(scene->_player, {-positionChange, 0, 0}, scene))
            scene->_player->changePosX(-positionChange);
        scene->_player->rotate(glm::radians(-90.0f), {0, 1, 0});
        MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(), scene->_player->getId(),
                                                           scene->_player->getTransformation());
	    game->renderer.increaseAnimeTime("player", 0, velocity);
        scene->_player->setDirection('L');
        glm::vec3 pos = scene->_player->getPosition();
        scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x, pos.y + 11, pos.z + 6));
        scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
        scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
    }
    if (game->getGameWindow().isKeyPressed(Menu::options.moveRight.glfwValue)) {
        Menu::playPlayerWalking();
        if (!worldCollisionRight(scene->_player, {positionChange, 0, 0}, scene))
            scene->_player->changePosX(positionChange);
        scene->_player->rotate(glm::radians(90.0f), {0, 1, 0});
        MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(), scene->_player->getId(),
                                                           scene->_player->getTransformation());
	    game->renderer.increaseAnimeTime("player", 0, velocity);
        scene->_player->setDirection('R');
        glm::vec3 pos = scene->_player->getPosition();
        scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x, pos.y + 11, pos.z + 6));
        scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
        scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
    }
    if (game->getGameWindow().isKeyPressed(Menu::options.placeBomb.glfwValue) && (int)scene->_bomb.size() < 1 + scene->_player->getPowerBombNbr()) {
        Menu::playBombPlacement();
        scene->_addBomb(scene->_player->getPosition().x, scene->_player->getPosition().z);
    }

	if (scene->worldEndLevel(scene->_player->getPosition(), scene)) {
        if (scene->_enemies.empty())
        {
            MainGame::soundEngine->stopAllSounds();
            Menu::playLevelUp();
            scene->_levelCompleted = true;
	        game->setGameState(GAMESTATE::END);
        }
    }
    worldGetPower(scene->_player->getPosition(), scene);
    if (scene->getLevel() == 6) {

        int ix = 0;
        int iy = 0;
        if (scene->_enemies[0]->getDirection() == 'R') { ix = (int)-HALF_GRID_BLOCK_SIZE; iy = 0; }
        if (scene->_enemies[0]->getDirection() == 'L') {ix = (int)HALF_GRID_BLOCK_SIZE; iy = 0;}
        if (scene->_enemies[0]->getDirection() == 'U') {ix = 0; iy = (int)HALF_GRID_BLOCK_SIZE;}
        if (scene->_enemies[0]->getDirection() == 'D') {ix = 0; iy = (int)-HALF_GRID_BLOCK_SIZE;}
        glm::vec3 matup = glm::vec3(scene->_enemies[0]->getPosition().x + ix, 0,  scene->_enemies[0]->getPosition().z + iy);
        MainGame::explosionY->generateParticles(matup, true);
    }
}

void Scene::updateBomb(MainGame *game, Scene *scene) {

	for (int i = 0; i < (int)scene->_bomb.size(); ++i) {
		if (scene->_bomb[i].explodeTime() && !scene->_bomb[i].getExploded())
		{
            scene->_bomb[i].setExploded(true);
            enemiesExplosionCollision(scene->_bomb[i].getPosition(), scene);
            bombExplode(scene, scene->_bomb[i]);
            renderExplosion(scene, scene->_bomb[i], game);
            if (scene->PlayerExplosionCollision(scene->_bomb[i].getPosition(), scene)) {
                scene->_player->decHP(scene->getDifficulty());
                Menu::playPlayerHurt();
                std::cout << "hurt\n";
            }
            Menu::playBombExplosion();
            MainGame::renderer.removeFromRender("bomb", scene->_bomb[i].getId());
        }
		if (!scene->_bomb[i].removeExplosionTime() && scene->_bomb[i].getExploded())
		{
			renderExplosion(scene, scene->_bomb[i], game);
		}
        if (scene->_bomb[i].removeExplosionTime() && !scene->_bomb[i].getExplosionRemoved()) {
            scene->_bomb[i].setExplosionRemoved(true);
            for (int k = 0; k < (scene->_player->getPowerExplosion() + 1); ++k) {
                MainGame::renderer.removeFromRender("explosion", scene->_bomb[i].getId());
                MainGame::renderer.removeFromRender("explosion1", scene->_bomb[i].getId() + k);
                MainGame::renderer.removeFromRender("explosion2", scene->_bomb[i].getId() + k);
                MainGame::renderer.removeFromRender("explosion3", scene->_bomb[i].getId() + k);
                MainGame::renderer.removeFromRender("explosion4", scene->_bomb[i].getId() + k);
            }
        }
        if (scene->_bomb[i].explodeTime() && scene->_bomb[i].removeExplosionTime() && scene->_bomb[i].getExplosionRemoved() && scene->_bomb[i].getExploded()) {
            int y = scene->getWorldy(scene->_bomb[i].getPosition().z);
            int x = scene->getWorldx(scene->_bomb[i].getPosition().x);
            scene->_bomb[i] = scene->_bomb.back();
            scene->_bomb.pop_back();
            delete scene->_blocks[y][x];
            scene->_blocks[y][x] = nullptr;
        }
	}
}