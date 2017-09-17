#include <menu.hpp>

Menu*                       Menu::activeMenu = nullptr;
irrklang::ISoundSource*     Menu::_menuMusic;
bool                        Menu::isFullScreen = false;
int                         Menu::windowWidth = 1280;
int                         Menu::windowHeight = 760;
int                         Menu::difficulty = 2;
nanogui::Label              *Menu::title = nullptr;
MainMenu                    Menu::mainMenu = MainMenu();
StoryModeMenu               Menu::storyModeMenu = StoryModeMenu();
NewGameMenu                 Menu::newGameMenu = NewGameMenu();
PauseMenu                   Menu::pauseMenu = PauseMenu();
EndGameMenu                 Menu::endGameMenu = EndGameMenu();
LoadGameMenu                Menu::loadGameMenu = LoadGameMenu();
OptionMenu                  Menu::optionMenu;
float                       Menu::textStartTime = 0;
Gui                         Menu::gui;
Options                     Menu::options;
Options                     Menu::tmpOptions;

Menu::Menu(MainGame *mainGame)
{
	Menu::loadOptions();
	initMenu(mainGame);
}

Menu::Menu(const Menu &rhs) { *this = rhs; }

Menu& Menu::operator=(const Menu &rhs)
{
	return *this;
}

void Menu::loadOptions()
{
	/// creating sound engine
	MainGame::soundEngine = irrklang::createIrrKlangDevice();
	/// getting primary monitor video mode for RGB bits and refreshRate
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	//Menu::windowWidth = mode->width;
	//Menu::windowHeight = mode->height;
	/// getting all video modes with primary monitor video mode RGB bits & refreshRate
	int count = 0;
	const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
	int index = 0;
	/// loading them into my resolution list
	for (int i = 0; i < count - 1; i++){
		if (mode->refreshRate == modes[i].refreshRate && mode->redBits == modes[i].redBits &&
				mode->greenBits == modes[i].greenBits && mode->blueBits == modes[i].blueBits)
		{
			Menu::options.resolutionList.insert(std::pair<int, std::vector<int>>(index, {modes[i].width, modes[i].height, modes[i].refreshRate}));
			index++;
		}
	}
	/// todo: load options from config file
	/// loading options
	Menu::options.musicVolume = 1.0f;
	Menu::options.soundVolume = 1.0f;
	Menu::options.mute = false;
	Menu::options.resolutionIndex = (int)Menu::options.resolutionList.size() - 1;
	Menu::options.fullScreen = false;
	Menu::copyOptions(Menu::tmpOptions, Menu::options);
}

void Menu::copyOptions(Options &dest, Options &src)
{
	dest.musicVolume = src.musicVolume;
	dest.soundVolume = src.soundVolume;
	dest.mute = src.mute;
	dest.resolutionIndex = src.resolutionIndex;
	dest.fullScreen = src.fullScreen;
	dest.resolutionList = src.resolutionList;
}

bool Menu::initMenu(MainGame *mainGame)
{
	_mainGame = mainGame;
	Menu::activeMenu = this;
	/// creating glfw window using nanogui screen
    _screen = new nanogui::Screen({Menu::windowWidth, Menu::windowHeight}, "Bomberman", false, Menu::isFullScreen, 8, 8, 24, 8, 4, 4, 1);
	_screen->setVisible(true);
	/// setting callbacks
	Zion::Input::mouseCallback2 = Menu::mouseCallback;
	Zion::Input::cursorPositionCallback2 = Menu::cursorPositionCallback;
	Zion::Input::keyCallback2 = Menu::keyCallback;
	std::vector<void *> params;
	params.push_back(this);
	/// add menuUpdate function to gameLoop
	MainGame::functions.insert(std::pair<const char *, Func>("menuUpdate", {Menu::updateMenu, params}));
	/// creating background music
	if (MainGame::soundEngine == nullptr)
		_menuMusic = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/breakout.mp3");
	return true;
}

bool Menu::buildMenuWindows()
{
    createBackground();
	createMainMenu();
    createStoryMenu();
	createExitWindow();
	createNewGameMenu();
	createLoadGameMenu();
	createPauseGameMenu();
	createEndGameMenu();
	createOptionsMenu();
	return true;
}

GLFWwindow* Menu::getGlfwWindow()
{
	return _screen->glfwWindow();
}

void Menu::createNewGame(int level, int difficulty, std::string saveName)
{
	activeMenu->_saveFileName = saveName;
	activeMenu->scene = new Scene();
	activeMenu->scene->setDifficulty(difficulty);
	activeMenu->scene->setLevel(level);
	if (level == 1)
		activeMenu->scene->newGame(activeMenu->_mainGame, "map2");
	else
		activeMenu->scene->newGame(activeMenu->_mainGame, "map2");
	activeMenu->scene->saveGame(activeMenu->_saveFileName);
	activeMenu->createLoadGameMenu();
	activeMenu->_mainGame->setGameState(GAMESTATE::START);
	Menu::textStartTime = 0;
	Menu::stopMenuMusic();
}

void Menu::destroyGame()
{
	delete activeMenu->scene;
	MainGame::functions.erase("sceneUpdate");
	MainGame::renderer.removeAll();
}

void Menu::loadSaveDirectory()
{
	DIR     *dir = opendir("save");
	struct dirent *file;
	if (dir)
	{
		Menu::loadGameMenu.fileNames.clear();
		while ((file = readdir(dir)))
		{
			if (file->d_name[0] == '.')
				continue;
			Menu::loadGameMenu.fileNames.push_back(std::string(file->d_name));
		}
	}else{
		perror("directory error");
	}
	closedir(dir);
}

void Menu::updateGameStateStart(MainGame *game, Menu *menu, GAMESTATE state)
{
	float halfHeight = (float)Menu::windowHeight / 3;
	float halfWidth = (float)Menu::windowWidth / 2;
	float xOffset = (float)Menu::windowWidth / 5;

	if (Menu::textStartTime == 0.0f)
		Menu::textStartTime = (float)glfwGetTime();

	Menu::gui.whiteBanner->render(glm::translate(glm::mat4(), {0, 0.3, 0}));

	float changeTime = (float)glfwGetTime() - Menu::textStartTime;
	float offset;
	if (changeTime <= 1.4f) {
		int stage = menu->scene->getLevel();
		if (changeTime <= 0.9f)
			offset = (changeTime / 0.9f) * (halfWidth + xOffset);
		else
			offset = (halfWidth + xOffset);
		MainGame::fontRenderer1->renderText("STAGE " + std::to_string(stage), (float)Menu::windowWidth - (offset - halfWidth / 3), halfHeight, 1.5f, {0.8, 0.8, 0.8});
		MainGame::fontRenderer1->renderText("Defeat All Enemies", (float)Menu::windowWidth - offset, halfHeight + 55, 2.0f, {0.8, 0.8, 0.8});
	}else if (changeTime <= 2.6f) {
		float tmp = changeTime - 1.4f;
		if (changeTime <= 2.3)
			offset = (tmp / 0.9f) * (halfWidth + xOffset - halfWidth / 4);
		else
			offset = (halfWidth + xOffset - halfWidth / 4);
		MainGame::fontRenderer1->renderText("READY", (float)Menu::windowWidth - offset, halfHeight, 3.0f, {0.8, 0.8, 0.8});
	}else {
		float tmp = changeTime - 2.8f;
		if (changeTime <= 3.5f)
			offset = (tmp / 0.7f) * (halfWidth + xOffset - halfWidth / 3);
		else
			offset = (halfWidth + xOffset - halfWidth / 3);
		MainGame::fontRenderer1->renderText("GO", (float)Menu::windowWidth - offset, halfHeight, 3.0f, {0.8, 0.8, 0.8});
	}
	if (changeTime >= 3.9f)
	{
		menu->_mainGame->setGameState(GAMESTATE::GAME);
		Menu::textStartTime = 0;
	}
}

void Menu::updateGameStateEnd(MainGame *game, Menu *menu, GAMESTATE state)
{
	float halfHeight = (float)Menu::windowHeight / 3;
	float halfWidth = (float)Menu::windowWidth / 2;
	float xOffset = (float)Menu::windowWidth / 5;

	if (Menu::textStartTime == 0.0f)
		Menu::textStartTime = (float)glfwGetTime();

	float changeTime = (float)glfwGetTime() - Menu::textStartTime;
	if (menu->scene->isLevelCompleted())
	{
		float offset;
		if (changeTime <= 0.9f)
			offset = (changeTime / 0.9f) * (halfWidth + xOffset);
		else
			offset = (halfWidth + xOffset);
		Menu::gui.whiteBanner->render(glm::translate(glm::mat4(), {0, 0.3, 0}));
		MainGame::fontRenderer1->renderText("Stage Cleared", (float)Menu::windowWidth - offset, halfHeight + 55, 2.0f, {0.8, 0.8, 0.8});
		if (changeTime >= 1.7f){
			int level = menu->scene->getLevel() + 1;
			int difficulty = menu->scene->getDifficultyValue();
			destroyGame();
			createNewGame(level, difficulty, menu->_saveFileName);
		}
	}else{
		Menu::gui.whiteBanner->render(glm::translate(glm::mat4(), {0, 0.4, 0}));
		MainGame::fontRenderer1->renderText("You  Died !!!", halfWidth - xOffset, halfHeight - 15, 2.0f, {0.54, 0.027, 0.027});
		if (changeTime > 0.6f)
		{
			if (Menu::pauseMenu.window->visible())
				Menu::pauseMenu.changeView(false);
			if (!Menu::endGameMenu.window->visible())
				Menu::endGameMenu.changeView(true);
			menu->_screen->drawWidgets();
		}
	}

}

void Menu::updateMenu(MainGame *game, std::vector<void *> params)
{
	auto *menu = (Menu*)params[0];

	GAMESTATE state = menu->_mainGame->getGameState();

	if (state == GAMESTATE::MENU)
	{
		menu->_menuBg->render(glm::translate(glm::mat4(), {0, 0, 0}));
		menu->_menuTitle->render(glm::translate(glm::mat4(), {-1.7, 1.4, 0}));
		menu->_screen->drawWidgets();
	}
	else if (state == GAMESTATE::PAUSE)
	{
		float halfHeight = (float)Menu::windowHeight / 2;
		float halfWidth = (float)Menu::windowWidth / 3;
		MainGame::fontRenderer2->renderText("Paused", halfWidth, halfHeight, 2.0f, {0.8, 0.8, 0.8});
		menu->_screen->drawWidgets();
	}
	else if (state == GAMESTATE::GAME)
	{
		menu->renderGui();
	}
	else if (state == GAMESTATE::START)
		Menu::updateGameStateStart(game, menu, state);
	else if (state == GAMESTATE::END)
		Menu::updateGameStateEnd(game, menu, state);
}

void Menu::renderGui()
{
	int     halfWidth = Menu::windowWidth / 2;
	int     halfHeight = Menu::windowHeight / 2;
	/// render life
	int  life = scene->getPlayer()->getHP() / scene->getDifficulty();
	int  enenmyCount = (int)scene->getEnemyCount();
	int  bomb = scene->getPlayer()->getPowerBombNbr() + 1;
	int  explode = scene->getPlayer()->getPowerExplosion() + 1;

	float posX = -3.3f;
	for (int i = 0; i < life; i++)
	{
		Menu::gui.heart->render({glm::translate(glm::mat4(), {posX, 1.9, 0})});
		posX += 0.2;
	}
	/// bomb
	Menu::gui.bomb->render(glm::translate(glm::mat4(), {-3.3f, -1.3, 0}));
	MainGame::fontRenderer1->renderText(std::string("x") + std::to_string(bomb), 80,
	                                    Menu::windowHeight - 152, 0.5f,
	                                    {0.8, 0.8, 0.8});
	/// explode
	Menu::gui.explode->render(glm::translate(glm::mat4(), {-3.3f, -1.6, 0}));
	MainGame::fontRenderer1->renderText(std::string("x") + std::to_string(explode), 80,
	                                    Menu::windowHeight - 102, 0.5f,
	                                    {0.8, 0.8, 0.8});
	/// enemy
	Menu::gui.enemy2->render(glm::translate(glm::mat4(), {-3.3f, -1.9, 0}));
	MainGame::fontRenderer1->renderText(std::string("x") + std::to_string(enenmyCount), 80,
	                                    Menu::windowHeight - 50, 0.5f,
	                                    {0.8, 0.8, 0.8});
}

bool Menu::mouseCallback(int button, int action, int mod)
{
	if (activeMenu != nullptr)
		activeMenu->_screen->mouseButtonCallbackEvent(button, action, mod);
	return true;
}

bool Menu::cursorPositionCallback(int xpos, int ypos)
{
	if (activeMenu != nullptr)
		activeMenu->_screen->cursorPosCallbackEvent(xpos, ypos);
	return true;
}

bool Menu::keyCallback(int key, int scancode, int action, int mods)
{
	if (activeMenu != nullptr)
		activeMenu->_screen->keyCallbackEvent(key, scancode, action, mods);
	return true;
}

void Menu::playMenuMusic()
{
	if (MainGame::soundEngine)
		MainGame::soundEngine->play2D(_menuMusic, true);
}

void Menu::stopMenuMusic()
{
	if (MainGame::soundEngine)
		MainGame::soundEngine->removeSoundSource(_menuMusic);
}