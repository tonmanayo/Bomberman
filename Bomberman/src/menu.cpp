#include <menu.hpp>

Menu*                       Menu::activeMenu = nullptr;
irrklang::ISoundSource*     Menu::_menuMusic;
irrklang::ISoundSource*     Menu::_playGameReady;
irrklang::ISoundSource*     Menu::_playGameReady1;
irrklang::ISoundSource*		Menu::_bombExplosionSound;
irrklang::ISoundSource*		Menu::_bombPlacementSound;
irrklang::ISoundSource*		Menu::_mapLevelUp;
irrklang::ISoundSource*		Menu::_playerHurtSound;
irrklang::ISoundSource*		Menu::_enemyHurtSound;

irrklang::ISoundSource*		Menu::_playerWalkingSound;
irrklang::ISoundSource*		Menu::_playPowerUp;
irrklang::ISoundSource*		Menu::_level1Song;
irrklang::ISoundSource*		Menu::_level2Song;
irrklang::ISoundSource*		Menu::_level3Song;
irrklang::ISoundSource*		Menu::_level4Song;
irrklang::ISoundSource*		Menu::_level5Song;
irrklang::ISoundSource*		Menu::_level6Song;
irrklang::ISoundSource*		Menu::_playIlly;

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

Menu::Menu(const Menu &rhs) {
	(void)rhs;
	*this = rhs;
}

Menu& Menu::operator=(const Menu &rhs)
{
	(void)rhs;
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
	for (int i = 0; i < count - 1; i++)
	{
		if (modes[i].refreshRate == mode->refreshRate && mode->redBits == modes[i].redBits && mode->greenBits == modes[i].greenBits && mode->blueBits == modes[i].blueBits)
		{
			Menu::options.resolutionList.insert(std::pair<int, std::vector<int>>(index, {modes[i].width, modes[i].height, modes[i].refreshRate}));
			index++;
		}
	}
	/// loading options
	Menu::loadOptionsConfig();
	if (Menu::options.resolutionIndex + 1 >= (int)Menu::options.resolutionList.size())
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
		_level1Song = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/stageMusic/level1.wav");
		_level2Song = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/stageMusic/level2.wav");
		_level3Song = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/stageMusic/level3.wav");
		_level4Song = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/stageMusic/level4.wav");
		_level5Song = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/stageMusic/level5.wav");
		_level6Song = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/stageMusic/level6.wav");
		_mapLevelUp = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/levelUp.wav");
		_playPowerUp = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/powerUp.wav");
		_playGameReady = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/gameStart.wav");
		_playGameReady1 = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/gameStart1.wav");
		_playIlly = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/illy_sound.mp3");
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

void Menu::createNewGame(int level, int difficulty, std::string saveName, int hp, int bombs, float speed, int explode)
{
	(void)level;
	(void)difficulty;
	activeMenu->_saveFileName = saveName;
	activeMenu->scene = new Scene();
	activeMenu->scene->setDifficulty(difficulty);
	activeMenu->scene->setLevel(level);
	if (level < 7)
	{
		activeMenu->scene->newGame(activeMenu->_mainGame, "stage" + std::to_string(level));
		activeMenu->scene->getPlayer()->setHp(hp + activeMenu->scene->getPlayer()->getHP());
		activeMenu->scene->getPlayer()->setPowerExplosion(explode + activeMenu->scene->getPlayer()->getPowerExplosion());
		activeMenu->scene->getPlayer()->setPowerBombNbr(bombs + activeMenu->scene->getPlayer()->getPowerBombNbr());
		activeMenu->scene->getPlayer()->setPowerSpeed(speed + activeMenu->scene->getPlayer()->getPowerSpeed());
		activeMenu->scene->saveGame(activeMenu->_saveFileName);
		activeMenu->createLoadGameMenu();
		activeMenu->_mainGame->setGameState(GAMESTATE::START);
		Menu::textStartTime = 0;
		if (activeMenu->scene->getLevel() == 6) {
			MainGame::soundEngine->stopAllSounds();
			Menu::playIlly();
		}
	}else{
		delete activeMenu->scene;
	 	activeMenu->_mainGame->setGameState(GAMESTATE::CREDITS);
		Menu::pauseMenu.changeView(false);
		Menu::textStartTime = 0.0f;
	}
}

void Menu::destroyGame()
{
//	if (activeMenu->scene != nullptr) {
//		delete activeMenu->scene;
//		activeMenu->scene = nullptr;
//	}
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
	(void)game;
	(void)state;
	glDisable(GL_DEPTH_TEST);
	if (Menu::textStartTime == 0.0f)
		Menu::textStartTime = (float)glfwGetTime();

	Menu::gui.whiteBanner->render(glm::translate(glm::mat4(), {0, 0.0, 0}));

	float changeTime = (float)glfwGetTime() - Menu::textStartTime;
	float posY1;
	float posY2;
	float posX1;
	float posX2;
	float offSet;
	float textSize = ((float)Menu::windowWidth / 1600.0f) * 2.0f;

	if (changeTime <= 1.4f)
	{
		int stage = Menu::activeMenu->scene->getLevel();
		posX1 = (0.80f * Menu::windowWidth) / 2.0f;
		posX2 = (0.57f * Menu::windowWidth) / 2.0f;
		posY1 = (0.80f * Menu::windowHeight) / 2.0f;
		posY2 = (1.0f * Menu::windowHeight) / 2.0f;
		if (changeTime <= 1.0f){
			offSet = (changeTime / 0.7f) * posX1;
			float newPosX = (float)Menu::windowWidth - offSet;
			MainGame::fontRenderer1->renderText("STAGE " + std::to_string(stage), newPosX, posY1, textSize, {0.8, 0.8, 0.8});
			offSet = (changeTime / 0.45f) * posX2;
			newPosX = (float)Menu::windowWidth - offSet;
			MainGame::fontRenderer1->renderText("Defeat All Enemies", newPosX, posY2, textSize, {0.8, 0.8, 0.8});
		}else {
			MainGame::fontRenderer1->renderText("STAGE " + std::to_string(stage), posX1, posY1, textSize, {0.8, 0.8, 0.8});
			MainGame::fontRenderer1->renderText("Defeat All Enemies", posX2, posY2, textSize, {0.8, 0.8, 0.8});
			Menu::playGameReady1();
		}
	}
	else if (changeTime <= 2.6f)
	{
		posX1 = (0.90f * Menu::windowWidth) / 2.0f;
		posY1 = (0.90f * Menu::windowHeight) / 2.0f;
		float tmp = changeTime - 1.4f;
		if (changeTime <= 2.3){
			offSet = (tmp / 0.8f) * posX1;
			float newPosX = (float)Menu::windowWidth - offSet;
			MainGame::fontRenderer1->renderText("READY", newPosX, posY1, textSize, {0.8, 0.8, 0.8});
		}else{
			MainGame::fontRenderer1->renderText("READY", posX1, posY1, textSize, {0.8, 0.8, 0.8});
			Menu::playGameReady1();
		}
	}else{
		posX1 = (0.98f * Menu::windowWidth) / 2.0f;
		posY1 = (0.90f * Menu::windowHeight) / 2.0f;
		float tmp = changeTime - 2.8f;
		if (changeTime <= 3.5f){
			offSet = (tmp / 0.75f) * posX1;
			float newPosX = (float)Menu::windowWidth - offSet;
			MainGame::fontRenderer1->renderText("GO", newPosX, posY1, textSize, {0.8, 0.8, 0.8});
		} else{
			Menu::playGameReady();
			MainGame::fontRenderer1->renderText("GO", posX1, posY1, textSize, {0.8, 0.8, 0.8});
		}
	}
	if (changeTime >= 3.9f)
	{
		menu->_mainGame->setGameState(GAMESTATE::GAME);
		Menu::textStartTime = 0;
		MainGame::game->soundEngine->stopAllSounds();
		Menu::playGameSong();
	}
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Menu::updateGameStateEnd(MainGame *game, Menu *menu, GAMESTATE state)
{
	(void)game;
	(void)state;
	glDisable(GL_DEPTH_TEST);
	if (Menu::textStartTime == 0.0f)
		Menu::textStartTime = (float)glfwGetTime();

	Menu::gui.whiteBanner->render(glm::translate(glm::mat4(), {0, 0.0, 0}));

	float changeTime = (float)glfwGetTime() - Menu::textStartTime;
	float textSize = ((float)Menu::windowWidth / 1600.0f) * 3.0f;
	float posY;
	float posX;

	if (menu->scene->isLevelCompleted()){
		posY = (0.90f * Menu::windowHeight) / 2.0f;
		posX = (0.70f * Menu::windowWidth) / 2.0f;
		textSize = ((float)Menu::windowWidth / 1600.0f) * 2.5f;
		MainGame::fontRenderer1->renderText("Stage Cleared", posX, posY, textSize, {0.8, 0.8, 0.8});
		if (changeTime >= 1.7f){
			int level = menu->scene->getLevel() + 1;
			int difficulty = menu->scene->getDifficultyValue();
			int explode = menu->scene->getPlayer()->getPowerExplosion();
			int hp = menu->scene->getPlayer()->getHP() - 100;
			hp = (hp < 0) ? 0 : hp;
			int bombs = menu->scene->getPlayer()->getPowerBombNbr();
			float speed = menu->scene->getPlayer()->getPowerSpeed();
			Zion::ParticleMaster::clearAll();
			destroyGame();
			createNewGame(level, difficulty, menu->_saveFileName, hp, bombs, speed, explode);
		}
	}else{
		posY = (0.90f * Menu::windowHeight) / 2.0f;
		posX = (0.65f * Menu::windowWidth) / 2.0f;
		MainGame::fontRenderer1->renderText("You  Died !!!", posX, posY, textSize, {0.54, 0.027, 0.027});
		if (changeTime > 0.7f)
		{
			if (Menu::pauseMenu.window->visible())
				Menu::pauseMenu.changeView(false);
			if (!Menu::endGameMenu.window->visible())
				Menu::endGameMenu.changeView(true);
			menu->_screen->drawWidgets();
		}
	}
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Menu::updateGameCredits(MainGame *game, Menu *menu, GAMESTATE state)
{
	static float posX1 = 0.0f;
	static float posY1 = 0.0f;
	static float dist = 0.0f;
	(void)game;
	(void)state;
	(void)menu;
	glDisable(GL_DEPTH_TEST);
	float textSize = ((float)Menu::windowWidth / 1600.0f) * 1.5f;
	float textSize1 = ((float)Menu::windowWidth / 1600.0f) * 1.5f;
	float textSize2 = ((float)Menu::windowWidth / 1600.0f) * 1.0f;
	if (posX1 == 0.0f){
		posY1 = (1.95f * Menu::windowHeight) / 2.0f;
		posX1 = (0.73f * Menu::windowWidth) / 2.0f;
		dist = (0.2f * Menu::windowHeight) / 2.0f;
	}
	MainGame::fontRenderer2->renderText("CREDITS", posX1, posY1, textSize, {0.1, 0.1, 0.1});
	/// main programmers
	MainGame::fontRenderer1->renderText("Game Logic - Tony Mack", (0.70f * Menu::windowWidth) / 2.0f, posY1 + (0.6f * dist), textSize2, {0.1, 0.1, 0.1});
	MainGame::fontRenderer1->renderText("Model Design - Miriam Lamarre", (0.63f * Menu::windowWidth) / 2.0f, posY1 + (1.0f * dist), textSize2, {0.1, 0.1, 0.1});
	MainGame::fontRenderer1->renderText("Animations - Miriam Lamarre", (0.68f * Menu::windowWidth) / 2.0f, posY1 + (1.4f * dist), textSize2, {0.1, 0.1, 0.1});
	MainGame::fontRenderer1->renderText("Music - Tony Mack", (0.82f * Menu::windowWidth) / 2.0f, posY1 + (1.8f * dist), textSize2, {0.1, 0.1, 0.1});
	MainGame::fontRenderer1->renderText("Graphics Engine - Stephen Asiedu", (0.57f * Menu::windowWidth) / 2.0f, posY1 + (2.2f * dist), textSize2, {0.1, 0.1, 0.1});
	MainGame::fontRenderer1->renderText("Graphical User Interface(GUI) - Angus Burroughs", (0.23f * Menu::windowWidth) / 2.0f, posY1 + (2.6f * dist), textSize2, {0.1, 0.1, 0.1});
	MainGame::fontRenderer1->renderText("Sound Engine - Nikolaus Gebhardt", (0.64f * Menu::windowWidth) / 2.0f, posY1 + (3.0f * dist), textSize2, {0.1, 0.1, 0.1});
	/// technologies and tools
	MainGame::fontRenderer2->renderText("Tools And Technology", (0.45f * Menu::windowWidth) / 2.0f, posY1 + (3.8f * dist), textSize1, {0.1, 0.1, 0.1});
	MainGame::fontRenderer1->renderText("Open Graphics Library(OpenGL)", (0.65f * Menu::windowWidth) / 2.0f, posY1 + (4.3f * dist), textSize2, {0.1, 0.1, 0.1});
	MainGame::fontRenderer1->renderText("GLFW", (0.95f * Menu::windowWidth) / 2.0f, posY1 + (4.7f * dist), textSize2, {0.1, 0.1, 0.1});
	MainGame::fontRenderer1->renderText("IrrKlang", (0.90f * Menu::windowWidth) / 2.0f, posY1 + (5.1f * dist), textSize2, {0.1, 0.1, 0.1});
	MainGame::fontRenderer1->renderText("Blender", (0.91f * Menu::windowWidth) / 2.0f, posY1 + (5.5f * dist), textSize2, {0.1, 0.1, 0.1});
	MainGame::fontRenderer1->renderText("Nanogui", (0.91f * Menu::windowWidth) / 2.0f, posY1 + (5.9f * dist), textSize2, {0.1, 0.1, 0.1});
	MainGame::fontRenderer1->renderText("GL Transmission Format", (0.70f * Menu::windowWidth) / 2.0f, posY1 + (6.3f * dist), textSize2, {0.1, 0.1, 0.1});
	if (posY1 > (0.20f * Menu::windowHeight) / 2.0f)
	{
		posY1 -= Zion::Renderable::deltaTime * 80.0f;
	}else{
		if (Menu::textStartTime == 0.0f)
			Menu::textStartTime = (float)glfwGetTime();
		if ((float)glfwGetTime() - Menu::textStartTime > 3.9f)
		{
			menu->_mainGame->setGameState(GAMESTATE::MENU);
			Menu::textStartTime = 0;
			destroyGame();
			Menu::mainMenu.changeView(true);
			Menu::pauseMenu.changeView(false);
			Menu::title->setVisible(true);
			Menu::title->setCaption("MAIN MENU");
			posX1 = 0.0f;
			posY1 = 0.0f;
			dist = 0.0f;
		}
	}
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
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
		float posX = (0.80f * Menu::windowWidth) / 2.0f;
		float posY = (0.80f * Menu::windowHeight) / 2.0f;
		float textSize = ((float)Menu::windowWidth / 1600.0f) * 2.0f;
		MainGame::fontRenderer2->renderText("Paused", posX, posY, textSize, {0.8, 0.8, 0.8});
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
	else if (state == GAMESTATE::CREDITS)
		Menu::updateGameCredits(game, menu, state);
}

void Menu::renderGui()
{
	/// render life
	int  life = scene->getPlayer()->getHP() / scene->getDifficulty();
	auto  enemyCount = (int)scene->getEnemyCount();
	int  bomb = scene->getPlayer()->getPowerBombNbr() + 1;
	int  explode = scene->getPlayer()->getPowerExplosion() + 1;
	auto speed = (int)(scene->getPlayer()->getPowerSpeed() / 1.5f);

	float posX = 0.96f;
	float posY = 0.91f;
	float heartPosX = posX * -1;
	glDisable(GL_DEPTH_TEST);
	for (int i = 0; i < life; i++)
	{
		Menu::gui.heart->render({glm::translate(glm::mat4(), {heartPosX, 0.93, 0})});
		heartPosX += 0.06;
	}
	/// bomb
	Menu::gui.bomb->render(glm::translate(glm::mat4(), {-0.94f, -0.78f, 0}));
	float textPosY = (1.75f * Menu::windowHeight) / 2.0f;
	float textPosX = (0.1f * Menu::windowWidth) / 2.0f;
	float textSize = (float)Menu::windowWidth / 1600.0f;
	MainGame::fontRenderer1->renderText("x" + std::to_string(bomb), textPosX, textPosY, textSize, {0.8, 0.8, 0.8});
	/// speed
	textPosY = (1.48f * Menu::windowHeight) / 2.0f;
	Menu::gui.speed->render(glm::translate(glm::mat4(), {-0.94f, -0.52f, 0}));
	MainGame::fontRenderer1->renderText(std::string("x") + std::to_string(speed), textPosX, textPosY, textSize, {0.8, 0.8, 0.8});
	/// explode
	textPosY = (1.88f * Menu::windowHeight) / 2.0f;
	Menu::gui.explode->render(glm::translate(glm::mat4(), {-0.94f, -0.91f, 0}));
	MainGame::fontRenderer1->renderText(std::string("x") + std::to_string(explode), textPosX, textPosY, textSize, {0.8, 0.8, 0.8});
	/// enemy
	textPosY = (1.62f * Menu::windowHeight) / 2.0f;
	Menu::gui.enemy2->render(glm::translate(glm::mat4(), {-0.94f, -0.65f, 0}));
	MainGame::fontRenderer1->renderText(std::string("x") + std::to_string(enemyCount), textPosX, textPosY, textSize, {0.8, 0.8, 0.8});
	/// level
	int level = Menu::activeMenu->scene->getLevel();
	textPosX = (0.87f * Menu::windowWidth) / 2.0f;
	textPosY = (0.06f * Menu::windowHeight) / 2.0f;
	Menu::gui.timeBack->render(glm::translate(glm::mat4(), {0.0f, posY, 0.0f}));
	MainGame::fontRenderer2->renderText(std::string("Stage ") + std::to_string(level), textPosX, textPosY, textSize, {0.8, 0.8, 0.8});
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
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
		if (MainGame::soundEngine && !MainGame::soundEngine->isCurrentlyPlaying(_level1Song) &&
				Menu::activeMenu->scene->getLevel() == 1)
			MainGame::soundEngine->play2D(_level1Song);
		else if (MainGame::soundEngine && !MainGame::soundEngine->isCurrentlyPlaying(_level2Song) &&
				Menu::activeMenu->scene->getLevel() == 2)
			MainGame::soundEngine->play2D(_level2Song);
		else if (MainGame::soundEngine && !MainGame::soundEngine->isCurrentlyPlaying(_level3Song) &&
				Menu::activeMenu->scene->getLevel() == 3)
			MainGame::soundEngine->play2D(_level3Song);
		else if (MainGame::soundEngine && !MainGame::soundEngine->isCurrentlyPlaying(_level4Song) &&
				Menu::activeMenu->scene->getLevel() == 4)
			MainGame::soundEngine->play2D(_level4Song);
		else if (MainGame::soundEngine && !MainGame::soundEngine->isCurrentlyPlaying(_level5Song) &&
				Menu::activeMenu->scene->getLevel() == 5)
			MainGame::soundEngine->play2D(_level5Song);
		else if (MainGame::soundEngine && !MainGame::soundEngine->isCurrentlyPlaying(_level6Song) &&
				Menu::activeMenu->scene->getLevel() == 6)
			MainGame::soundEngine->play2D(_level6Song);
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

void 	Menu::playGameReady1() {
	if (MainGame::soundEngine && !MainGame::soundEngine->isCurrentlyPlaying(_playGameReady1))
		MainGame::soundEngine->play2D(_playGameReady1);
}
void 	Menu::playIlly() {
	if (MainGame::soundEngine && !MainGame::soundEngine->isCurrentlyPlaying(_playIlly))
		MainGame::soundEngine->play2D(_playIlly);
}