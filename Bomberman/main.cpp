#include <iostream>
#include <MainGame.hpp>
#include <menu.hpp>

void    checkKeys(MainGame *game, std::vector<void *> params)
{
	(void)params;
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
	Menu        menu;

	nanogui::init();
	Menu::loadOptions();
	menu.initMenu(&game);
	game.initGame(menu.getGlfwWindow(), Menu::windowWidth, Menu::windowHeight, 70.0f);
	menu.buildMenuWindows();
	srand((unsigned int)time(nullptr));
	game.gameLoop();
	nanogui::shutdown();
	return 0;
}