#include <iostream>
#include <MainGame.hpp>
#include <menu.hpp>

int     main(void)
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