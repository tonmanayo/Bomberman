#include <iostream>
#include <MainGame.hpp>
#include <scene.hpp>

void    checkKeys(MainGame *game, std::vector<void *> params)
{
	if (game->getGameWindow().isKeyPressed(GLFW_KEY_UP))
		game->getGameCamera().changeCameraYPos(2.5);
	if (game->getGameWindow().isKeyPressed(GLFW_KEY_DOWN))
		game->getGameCamera().changeCameraYPos(-2.5f);
	if (game->getGameWindow().isKeyPressed(GLFW_KEY_LEFT))
		game->getGameCamera().changeCameraXPos(-2.5f);
	if (game->getGameWindow().isKeyPressed(GLFW_KEY_RIGHT))
		game->getGameCamera().changeCameraXPos(2.5f);
	if (game->getGameWindow().isKeyPressed(GLFW_KEY_PAGE_UP))
		game->getGameCamera().changeCameraZPos(-2.5f);
	if (game->getGameWindow().isKeyPressed(GLFW_KEY_PAGE_DOWN))
		game->getGameCamera().changeCameraZPos(2.5f);
}

int     main(int ac, char **av)
{
	MainGame    game;
	Scene       *scene;

	game.initGame(1280.0f, 760.0f, 70.0f);
	game.setupGameCamera(glm::vec3(5, 13, -6), -80.0f, -90.0f);

	scene = new Scene(&game, game.getMap("map1"), 5);

	MainGame::functions.insert(std::pair<const char *, Func>("checkKeys", {checkKeys, std::vector<void *>()}));

	game.gameLoop();
	return 0;
}