#include <menu.hpp>

Menu*                       Menu::activeMenu = nullptr;
irrklang::ISoundSource*     Menu::_menuMusic;
irrklang::ISoundSource*     Menu::_playGameReady;
irrklang::ISoundSource*		Menu::_bombExplosionSound;
irrklang::ISoundSource*		Menu::_bombPlacementSound;
irrklang::ISoundSource*		Menu::_mapLevelUp;
irrklang::ISoundSource*		Menu::_playerHurtSound;
irrklang::ISoundSource*		Menu::_enemyHurtSound;
irrklang::ISoundSource*		Menu::_playerWalkingSound;
irrklang::ISoundSource*		Menu::_playPowerUp;
irrklang::ISoundSource*		Menu::_gameMusic;
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
bool                        Menu::isKeyBind = false;
std::string                 Menu::keyBind;

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
	Menu::loadOptionsConfig();
	if (Menu::options.resolutionIndex + 1 >= Menu::options.resolutionList.size())
		Menu::options.resolutionIndex = (int)Menu::options.resolutionList.size() - 1;
	Menu::copyOptions(Menu::tmpOptions, Menu::options);
	Menu::windowWidth = Menu::options.resolutionList[Menu::options.resolutionIndex][0];
	Menu::windowHeight = Menu::options.resolutionList[Menu::options.resolutionIndex][1];
	Menu::isFullScreen = Menu::options.fullScreen;
}

void Menu::copyOptions(Options &dest, Options &src)
{
	dest.musicVolume = src.musicVolume;
	dest.soundVolume = src.soundVolume;
	dest.mute = src.mute;
	dest.resolutionIndex = src.resolutionIndex;
	dest.fullScreen = src.fullScreen;
	dest.resolutionList = src.resolutionList;
	dest.moveUp = src.moveUp;
	dest.moveDown = src.moveDown;
	dest.moveLeft = src.moveLeft;
	dest.moveRight = src.moveRight;
	dest.placeBomb = src.placeBomb;
	dest.pause = src.pause;
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
	if (MainGame::soundEngine != nullptr) {
		_menuMusic = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/breakout.mp3");
		_enemyHurtSound = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/enemieDies.wav");
		_playerHurtSound = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/playerInjured.wav");
		_playerWalkingSound = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/run.wav");
		_bombExplosionSound = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/explosion.wav");
		_bombPlacementSound = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/bombDrop.wav");
		_gameMusic = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/game_song.wav");
		_mapLevelUp = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/levelUp.wav");
		_playPowerUp = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/powerUp.wav");
		_playGameReady = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/gameStart.wav");
	}
	Menu::updateSoundOptions();
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
	if (level < 4)
	{
		activeMenu->scene->newGame(activeMenu->_mainGame, "stage" + std::to_string(level));
	}
	activeMenu->scene->saveGame(activeMenu->_saveFileName);
	activeMenu->createLoadGameMenu();
	activeMenu->_mainGame->setGameState(GAMESTATE::START);
	Menu::textStartTime = 0;
	MainGame::soundEngine->stopAllSounds();
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
			Menu::loadGameMenu.fileNames.emplace_back(file->d_name);
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
        Menu::playGameReady();
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
		Menu::playGameSong();
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
	MainGame::fontRenderer1->renderText("x" + std::to_string(bomb), 80,
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

	///time
	auto i = static_cast<int>(std::trunc(scene->getLevelTime()));                //todo fix with resolution
	Menu::gui.timeBack->render(glm::translate(glm::mat4(), {0.0f, 1.94f, 0.0f}));
	MainGame::fontRenderer1->renderText(std::to_string(i), (Menu::windowWidth / 2) - 32,
										25 , 1.0f,
										{0.8, 0.8, 0.8});
}

bool Menu::mouseCallback(int button, int action, int mod)
{
	if (Menu::isKeyBind)
		return true;
	if (activeMenu != nullptr)
		activeMenu->_screen->mouseButtonCallbackEvent(button, action, mod);
	return true;
}

bool Menu::cursorPositionCallback(int xpos, int ypos)
{
	if (Menu::isKeyBind)
		return true;
	if (activeMenu != nullptr)
		activeMenu->_screen->cursorPosCallbackEvent(xpos, ypos);
	return true;
}

bool Menu::keyCallback(int key, int scancode, int action, int mods)
{
	if (Menu::isKeyBind){
		Menu::keyPressKeyBindings(key);
		return true;
	}
	if (activeMenu != nullptr)
		activeMenu->_screen->keyCallbackEvent(key, scancode, action, mods);
	return true;
}

void Menu::playMenuMusic()
{
	if (MainGame::soundEngine)
		MainGame::soundEngine->play2D(_menuMusic, true);
}

void 	Menu::playBombPlacement() {
	if (MainGame::soundEngine && !MainGame::soundEngine->isCurrentlyPlaying(_bombPlacementSound))
		MainGame::soundEngine->play2D(_bombPlacementSound);
}
void 	Menu::playPlayerHurt() {
	if (MainGame::soundEngine && !MainGame::soundEngine->isCurrentlyPlaying(_playerHurtSound))
		MainGame::soundEngine->play2D(_playerHurtSound);
}
void 	Menu::playEnemyHurt() {
	if (MainGame::soundEngine && !MainGame::soundEngine->isCurrentlyPlaying(_enemyHurtSound))
		MainGame::soundEngine->play2D(_enemyHurtSound);
}
void 	Menu::playPlayerWalking() {
	if (MainGame::soundEngine && !MainGame::soundEngine->isCurrentlyPlaying(_playerWalkingSound))
		MainGame::soundEngine->play2D(_playerWalkingSound);
}
void	Menu::playGameSong() {
	if (MainGame::soundEngine && !MainGame::soundEngine->isCurrentlyPlaying(_gameMusic))
		MainGame::soundEngine->play2D(_gameMusic);
}

void	Menu::playLevelUp() {
	if (MainGame::soundEngine && !MainGame::soundEngine->isCurrentlyPlaying(_mapLevelUp))
		MainGame::soundEngine->play2D(_mapLevelUp);
}

void	Menu::playPowerUp() {
	if (MainGame::soundEngine && !MainGame::soundEngine->isCurrentlyPlaying(_playPowerUp))
		MainGame::soundEngine->play2D(_playPowerUp);
}

void 	Menu::playBombExplosion() {
	if (MainGame::soundEngine && !MainGame::soundEngine->isCurrentlyPlaying(_bombExplosionSound))
		MainGame::soundEngine->play2D(_bombExplosionSound);
}

void 	Menu::playGameReady() {
    if (MainGame::soundEngine && !MainGame::soundEngine->isCurrentlyPlaying(_playGameReady))
        MainGame::soundEngine->play2D(_playGameReady);
}