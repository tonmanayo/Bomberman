#include <scene.hpp>
#include <random>

void Scene::updatePlayer(MainGame *game, Scene *scene) {

	float   velocity =  (1.0f + scene->_player->getPowerSpeed()) * Zion::Renderable::deltaTime;
	float   positionChange = (5.0f + scene->_player->getPowerSpeed()) * Zion::Renderable::deltaTime;

    if (game->getGameWindow().isKeyPressed(GLFW_KEY_S)) {
        if (!MainGame::soundEngine->isCurrentlyPlaying("resource/sounds/run.wav"))
            MainGame::soundEngine->play2D("resource/sounds/run.wav");
       // Menu::playPlayerWalking(); //todo cant get this like this
        if (!worldCollisionDown(scene->_player->getPosition(), {0, 0, positionChange}, scene))
            scene->_player->changePosZ(positionChange);
        scene->_player->rotate(glm::radians(0.0f), {0, 1, 0});
        MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(), scene->_player->getId(),
                                                           scene->_player->getTransformation());
	    game->renderer.increaseAnimeTime("player", 0, velocity);
        glm::vec3 pos = scene->_player->getPosition();
        scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x, pos.y + 10, pos.z + 6));
        scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
        scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
    }
    if (game->getGameWindow().isKeyPressed(GLFW_KEY_W)) {
        if (!MainGame::soundEngine->isCurrentlyPlaying("resource/sounds/run.wav"))
            MainGame::soundEngine->play2D("resource/sounds/run.wav");
        if (!worldCollisionUp(scene->_player->getPosition(), {0, 0, -positionChange}, scene))
            scene->_player->changePosZ(-positionChange);
        scene->_player->rotate(glm::radians(180.0f), {0, 1, 0});
        MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(), scene->_player->getId(),
                                                           scene->_player->getTransformation());
	    game->renderer.increaseAnimeTime("player", 0, velocity);
        glm::vec3 pos = scene->_player->getPosition();
        scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x, pos.y + 10, pos.z + 6));
        scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
        scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
    }
    if (game->getGameWindow().isKeyPressed(GLFW_KEY_A)) {
        if (!MainGame::soundEngine->isCurrentlyPlaying("resource/sounds/run.wav"))
            MainGame::soundEngine->play2D("resource/sounds/run.wav");
        if (!worldCollisionLeft(scene->_player->getPosition(), {-positionChange, 0, 0}, scene))
            scene->_player->changePosX(-positionChange);
        scene->_player->rotate(glm::radians(-90.0f), {0, 1, 0});
        MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(), scene->_player->getId(),
                                                           scene->_player->getTransformation());
	    game->renderer.increaseAnimeTime("player", 0, velocity);
        glm::vec3 pos = scene->_player->getPosition();
        scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x, pos.y + 10, pos.z + 6));
        scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
        scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
    }
    if (game->getGameWindow().isKeyPressed(GLFW_KEY_D)) {
        if (!MainGame::soundEngine->isCurrentlyPlaying("resource/sounds/run.wav"))
            MainGame::soundEngine->play2D("resource/sounds/run.wav");
        ;
        if (!worldCollisionRight(scene->_player->getPosition(), {positionChange, 0, 0}, scene))
            scene->_player->changePosX(positionChange);
        scene->_player->rotate(glm::radians(90.0f), {0, 1, 0});
        MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(), scene->_player->getId(),
                                                           scene->_player->getTransformation());
	    game->renderer.increaseAnimeTime("player", 0, velocity);
        glm::vec3 pos = scene->_player->getPosition();
        scene->_game->getGameCamera().setCameraPosition(glm::vec3(pos.x, pos.y + 10, pos.z + 6));
        scene->_game->getGameCamera().setCameraTarget(scene->_player->getPosition());
        scene->_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
    }
    if (game->getGameWindow().isKeyPressed(GLFW_KEY_SPACE) && scene->_bomb.size() < 1 + scene->_player->getPowerBombNbr()) {
        if (!MainGame::soundEngine->isCurrentlyPlaying("resource/sounds/bombDrop.wav"))
            MainGame::soundEngine->play2D("resource/sounds/bombDrop.wav");
        scene->_addBomb(scene->_player->getPosition().x, scene->_player->getPosition().z);
    }

	if (scene->worldEndLevel(scene->_player->getPosition(), scene)) {
        if (scene->_enemies.empty())
        {
            MainGame::soundEngine->play2D("resource/sounds/levelUp.wav");
            scene->_levelCompleted = true;
	        game->setGameState(GAMESTATE::END);
        }
    }

    if (scene->_player->getHP() == 0) {
        game->setGameState(GAMESTATE::END);
    }
    worldGetPower(scene->_player->getPosition(), scene);
}

void Scene::updateBomb(MainGame *game, Scene *scene) {

	for (int i = 0; i < scene->_bomb.size(); ++i) {
		if (scene->_bomb[i].explodeTime() && !scene->_bomb[i].getExploded())
		{
            scene->_bomb[i].setExploded(true);
            bombExplode(scene, scene->_bomb[i]);
            renderExplosion(scene, scene->_bomb[i], game);
            enemiesExplosionCollision(scene->_bomb[i].getPosition(), scene);
            if (scene->PlayerExplosionCollision(scene->_bomb[i].getPosition(), scene)) {
                scene->_player->decHP(scene->getDifficulty());
                MainGame::soundEngine->play2D("resource/sounds/playerInjured.wav");
                std::cout << "hurt\n";
            }
            MainGame::soundEngine->play2D("resource/sounds/explosion.wav");
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