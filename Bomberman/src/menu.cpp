#include <menu.hpp>

Menu*                       Menu::activeMenu = nullptr;
irrklang::ISoundSource*     Menu::_menuMusic;
bool                        Menu::isFullScreen = true;
int                         Menu::windowWidth = 1280;
int                         Menu::windowHeight = 760;
int                         Menu::difficulty = 2;
nanogui::Label              *Menu::title = nullptr;
MainMenu                    Menu::mainMenu = MainMenu();
StoryModeMenu               Menu::storyModeMenu = StoryModeMenu();
NewGameMenu                 Menu::newGameMenu = NewGameMenu();
float                       Menu::textStartTime = 0;
Gui                         Menu::gui;

Menu::Menu(float width, float height, MainGame *mainGame, bool fullScreen, bool resizable)
{
	initMenu(width, height, mainGame, fullScreen, resizable);
}

Menu::Menu(const Menu &rhs) { *this = rhs; }

Menu& Menu::operator=(const Menu &rhs)
{
	return *this;
}

bool Menu::initMenu(float width, float height, MainGame *mainGame, bool fullScreen, bool resizable)
{
	_mainGame = mainGame;
	Menu::activeMenu = this;
    this->config = YAML::LoadFile("../config.yaml");
    _screen = new nanogui::Screen({width, height}, "Bomberman", resizable, fullScreen, 8, 8, 24, 8, 4, 4, 1);
	_screen->setVisible(true);
	Zion::Input::mouseCallback2 = Menu::mouseCallback;
	Zion::Input::cursorPositionCallback2 = Menu::cursorPositionCallback;
	Zion::Input::keyCallback2 = Menu::keyCallback;
	std::vector<void *> params;
	params.push_back(this);
	MainGame::functions.insert(std::pair<const char *, Func>("menuUpdate", {Menu::updateMenu, params}));
	if (MainGame::soundEngine)
		_menuMusic = MainGame::soundEngine->addSoundSourceFromFile("resource/sounds/breakout.mp3");
    /// getting the screen width and height
    if (Menu::isFullScreen)
    {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetWindowMonitor(_screen->glfwWindow()));
        Menu::windowWidth = mode->width;
        Menu:windowHeight = mode->height;
    }
    else
        glfwGetWindowSize(_screen->glfwWindow(), &Menu::windowWidth, &Menu::windowHeight);
	return true;
}

bool Menu::buildMenuWindows()
{
    createBackground();
	createMainMenu();
    createStoryMenu();
	createExitWindow();
	createNewGameMenu();
	//createOptionsMenu();
	//createPauseGameMenu();
	//createLoadGameMenu();
	//_screen->performLayout();
	return true;
}

void Menu::createMainMenu()
{
    Menu::mainMenu.buttonTheme = new nanogui::Theme(_screen->nvgContext());
    Menu::mainMenu.buttonTheme->mButtonFontSize = 35;
    Menu::mainMenu.buttonTheme->mFontNormal = 30;
    Menu::mainMenu.buttonTheme->mFontBold = 0;
    Menu::mainMenu.buttonTheme->mTextColor = {230, 230, 230, 255};
    Menu::mainMenu.buttonTheme->mButtonGradientTopUnfocused = {69, 82, 130, 255};
    Menu::mainMenu.buttonTheme->mButtonGradientBotUnfocused = {40, 48, 76, 255};
    Menu::mainMenu.buttonTheme->mButtonGradientTopFocused = {248, 96, 73, 255};
    Menu::mainMenu.buttonTheme->mButtonGradientBotFocused = {176, 33, 26, 255};
    Menu::mainMenu.buttonTheme->mButtonGradientTopPushed = {193, 39, 24, 255};
    Menu::mainMenu.buttonTheme->mButtonGradientBotPushed = {186, 36, 26, 255};
    Menu::mainMenu.buttonTheme->mButtonCornerRadius = 10;
    Menu::mainMenu.buttonTheme->mBorderDark = {0, 0, 0, 0};
    Menu::mainMenu.buttonTheme->mBorderLight = {0, 0, 0, 0};
    Menu::mainMenu.buttonTheme->mBorderMedium = {0, 0, 0, 0};
    Menu::mainMenu.buttonTheme->mDropShadow = {0, 0, 0, 255};

    /// dimensions and positions for buttons
    int     posY = Menu::windowHeight / 2;
    int     buttonHeight = 50;
    int     buttonWidth = Menu::windowWidth / 3;
    buttonWidth = (buttonWidth > 450) ? 450 : buttonWidth;
    int     offset = ((Menu::windowWidth / 2) - buttonWidth) / 2;
    int     labelOffset = (Menu::windowWidth / 2) / 3;
    int     posX = (Menu::windowWidth / 2) - (buttonWidth + offset);

    Menu::title = new nanogui::Label(_screen, "Main Menu", "sans-bold", 45);
    Menu::title->setSize({labelOffset, 40});
    Menu::title->setPosition({labelOffset, posY - 90});
    Menu::title->setVisible(true);
    Menu::title->setColor({15, 3, 8, 255});

    Menu::mainMenu.start = new nanogui::Button(_screen, "CAMPAIGN");
    Menu::mainMenu.start->setSize({buttonWidth, buttonHeight});
    Menu::mainMenu.start->setPosition({posX, posY});
    Menu::mainMenu.start->setTheme(Menu::mainMenu.buttonTheme);
	Menu::mainMenu.start->setCursor(nanogui::Cursor::Hand);
    Menu::mainMenu.start->setCallback([]{
        Menu::mainMenu.changeView(false);
        Menu::storyModeMenu.changeView(true);
        Menu::title->setCaption("CAMPAIGN");
    });

    posY += 60;
    Menu::mainMenu.options = new nanogui::Button(_screen, "OPTIONS");
    Menu::mainMenu.options->setSize({buttonWidth, buttonHeight});
    Menu::mainMenu.options->setPosition({posX, posY});
    Menu::mainMenu.options->setTheme(Menu::mainMenu.buttonTheme);
	Menu::mainMenu.options->setCursor(nanogui::Cursor::Hand);

    posY += 60;
    Menu::mainMenu.exit = new nanogui::Button(_screen, "EXIT");
    Menu::mainMenu.exit->setSize({buttonWidth, buttonHeight});
    Menu::mainMenu.exit->setPosition({posX, posY});
    Menu::mainMenu.exit->setTheme(Menu::mainMenu.buttonTheme);
	Menu::mainMenu.exit->setCursor(nanogui::Cursor::Hand);
    Menu::mainMenu.exit->setCallback([]{
        Menu::mainMenu.changeView(false);
        activeMenu->_exitWindow->setVisible(true);
    });

    Menu::mainMenu.changeView(true);
}

void Menu::createStoryMenu() {
    Menu::storyModeMenu.buttonTheme = Menu::mainMenu.buttonTheme;

    /// dimensions and positions for buttons
    int     posY = Menu::windowHeight / 2;
    int     buttonHeight = 50;
    int     buttonWidth = Menu::windowWidth / 4;
    buttonWidth = (buttonWidth > 250) ? 250 : buttonWidth;
    int     posX = ((Menu::windowWidth / 2) - (buttonWidth * 2)) / 3;

    Menu::storyModeMenu.newGame = new nanogui::Button(_screen, "NEW GAME");
    Menu::storyModeMenu.newGame->setSize({buttonWidth, buttonHeight});
    Menu::storyModeMenu.newGame->setPosition({posX, posY});
    Menu::storyModeMenu.newGame->setTheme(Menu::storyModeMenu.buttonTheme);
	Menu::storyModeMenu.newGame->setCallback([]{
		Menu::storyModeMenu.changeView(false);
		Menu::newGameMenu.changeView(true);
	});

    Menu::storyModeMenu.loadGame = new nanogui::Button(_screen, "LOAD GAME");
    Menu::storyModeMenu.loadGame->setSize({buttonWidth, buttonHeight});
    Menu::storyModeMenu.loadGame->setPosition({posX + buttonWidth + posX, posY});
    Menu::storyModeMenu.loadGame->setTheme(Menu::storyModeMenu.buttonTheme);

    int backPosX = ((Menu::windowWidth / 2) / 2) - (buttonWidth / 2);
    Menu::storyModeMenu.back = new nanogui::Button(_screen, "BACK");
    Menu::storyModeMenu.back->setSize({buttonWidth, buttonHeight});
    Menu::storyModeMenu.back->setPosition({backPosX, posY + 80});
    Menu::storyModeMenu.back->setTheme(Menu::storyModeMenu.buttonTheme);
    Menu::storyModeMenu.back->setCallback([]{
        Menu::mainMenu.changeView(true);
        Menu::storyModeMenu.changeView(false);
        Menu::title->setCaption("MAIN MENU");
    });

    Menu::storyModeMenu.changeView(false);
}

void Menu::createExitWindow()
{
    /// dimensions and positions for buttons
    int     posY = Menu::windowHeight / 2;
    int     windowHeight = 150;
    int     windowWidth = Menu::windowWidth / 3;
    windowWidth = (windowWidth > 450) ? 450 : windowWidth;
    int     offset = ((Menu::windowWidth / 2) - windowWidth) / 2;
    int     posX = (Menu::windowWidth / 2) - (windowWidth + offset);

    _exitWindow = new nanogui::Window(_screen, "");
    _exitWindow->setSize({windowWidth, windowHeight});
    _exitWindow->setVisible(false);
    _exitWindow->setPosition({posX, posY});
    _exitWindow->theme()->mTransparent = {0, 0, 0, 100};
    _exitWindow->theme()->mWindowCornerRadius = 5;
    _exitWindow->theme()->mWindowFillFocused = {0, 0, 0, 100};
    _exitWindow->theme()->mWindowFillUnfocused = {0, 0, 0, 100};
    /// text label
    nanogui::Label *label = new nanogui::Label(_exitWindow, "Do you want to exit game?", "sans", 25);
    label->setPosition({70, 20});
    label->setSize({300, 30});
    /// yes button
    nanogui::Button *yes = new nanogui::Button(_exitWindow, "Yes");
    yes->setSize({70, 30});
    yes->setPosition({80, 90});
    yes->setCallback([]{
        glfwSetWindowShouldClose(activeMenu->_screen->glfwWindow(), (int)true);
    });
    /// no button
    nanogui::Button *no = new nanogui::Button(_exitWindow, "No");
    no->setSize({70, 30});
    no->setPosition({250, 90});
    no->setCallback([]{
        activeMenu->_exitWindow->setVisible(false);
        Menu::mainMenu.changeView(true);
    });
}

void Menu::createNewGameMenu()
{
	Menu::newGameMenu.buttonTheme = Menu::mainMenu.buttonTheme;

	/// dimensions and position
	int     posY = Menu::windowHeight / 2;
	int     textBoxHeight = 50;
	int     textBoxWidth = Menu::windowWidth / 3;
	textBoxWidth = (textBoxWidth > 450) ? 450 : textBoxWidth;
	int     offset = ((Menu::windowWidth / 2) - textBoxWidth) / 2;
	int     posX = (Menu::windowWidth / 2) - (textBoxWidth + offset);
	int     buttonHeight = 50;
	int     buttonWidth = Menu::windowWidth / 4;
	buttonWidth = (buttonWidth > 250) ? 250 : buttonWidth;
	int     buttonPosX = ((Menu::windowWidth / 2) - (buttonWidth * 2)) / 3;

	Menu::newGameMenu.textLabel = new nanogui::Label(_screen, "Enter Profile Name", "sans", 30);
	Menu::newGameMenu.textLabel->setSize({textBoxWidth, textBoxHeight});
	Menu::newGameMenu.textLabel->setPosition({posX + 80, posY});
	Menu::newGameMenu.textLabel->setColor({50, 50, 50, 255});

	posY += 60;
	Menu::newGameMenu.profileNameBox = new nanogui::TextBox(_screen, "");
	Menu::newGameMenu.profileNameBox->setSize({textBoxWidth, textBoxHeight});
	Menu::newGameMenu.profileNameBox->setPosition({posX, posY});
	Menu::newGameMenu.profileNameBox->setEditable(true);
	Menu::newGameMenu.profileNameBox->theme()->mTextColor = {40, 40, 40, 255};
	Menu::newGameMenu.profileNameBox->setFontSize(35);

	posY += 70;
	int     otherButtonWidth = textBoxWidth / 3;
	int     otherPosX = posX + 10;

	nanogui::Theme  *tmp = new nanogui::Theme(_screen->nvgContext());
	tmp->mFontBold = 1;
	tmp->mFontNormal = 0;
	tmp->mStandardFontSize = 35;
	tmp->mTextColor = {40, 40, 40, 255};

	Menu::newGameMenu.easy = new nanogui::CheckBox(_screen, "Easy");
	Menu::newGameMenu.easy->setSize({otherButtonWidth, buttonHeight});
	Menu::newGameMenu.easy->setPosition({otherPosX, posY});
	Menu::newGameMenu.easy->setTheme(tmp);
	Menu::newGameMenu.easy->setCallback([](bool state){
		Menu::difficulty = 1;
		if (state){
			Menu::newGameMenu.normal->setChecked(false);
			Menu::newGameMenu.hard->setChecked(false);
		}else
			Menu::newGameMenu.easy->setChecked(true);
	});

	otherPosX += otherButtonWidth;
	Menu::newGameMenu.normal = new nanogui::CheckBox(_screen, "Normal");
	Menu::newGameMenu.normal->setSize({otherButtonWidth, buttonHeight});
	Menu::newGameMenu.normal->setPosition({otherPosX, posY});
	Menu::newGameMenu.normal->setTheme(tmp);
	Menu::newGameMenu.normal->setChecked(true);
	Menu::newGameMenu.normal->setCallback([](bool state){
		Menu::difficulty = 2;
		if (state){
			Menu::newGameMenu.easy->setChecked(false);
			Menu::newGameMenu.hard->setChecked(false);
		}else
			Menu::newGameMenu.normal->setChecked(true);
	});

	otherPosX += otherButtonWidth + 10;
	Menu::newGameMenu.hard = new nanogui::CheckBox(_screen, "Hard");
	Menu::newGameMenu.hard->setSize({otherButtonWidth, buttonHeight});
	Menu::newGameMenu.hard->setPosition({otherPosX, posY});
	Menu::newGameMenu.hard->setTheme(tmp);
	Menu::newGameMenu.hard->setCallback([](bool state){
		Menu::difficulty = 3;
		if (state){
			Menu::newGameMenu.normal->setChecked(false);
			Menu::newGameMenu.easy->setChecked(false);
		}else
			Menu::newGameMenu.hard->setChecked(true);
	});

	posY += 80;
	Menu::newGameMenu.createGame = new nanogui::Button(_screen, "Create Game");
	Menu::newGameMenu.createGame->setSize({buttonWidth, buttonHeight});
	Menu::newGameMenu.createGame->setPosition({buttonPosX, posY});
	Menu::newGameMenu.createGame->setTheme(Menu::newGameMenu.buttonTheme);
	Menu::newGameMenu.createGame->setCallback([]{
		if (Menu::newGameMenu.profileNameBox->value().empty())
			return;
		Menu::newGameMenu.changeView(false);
		activeMenu->_saveFileName = Menu::newGameMenu.profileNameBox->value();
		activeMenu->scene = new Scene();
		activeMenu->scene->setDifficulty(Menu::difficulty);
		activeMenu->scene->newGame(activeMenu->_mainGame, "map2");
		activeMenu->scene->saveGame(activeMenu->_saveFileName);
		activeMenu->_mainGame->setGameState(GAMESTATE::START);
		Menu::stopMenuMusic();
	});

	Menu::newGameMenu.cancel = new nanogui::Button(_screen, "Cancel");
	Menu::newGameMenu.cancel->setSize({buttonWidth, buttonHeight});
	Menu::newGameMenu.cancel->setPosition({buttonPosX + buttonWidth + buttonPosX, posY});
	Menu::newGameMenu.cancel->setTheme(Menu::newGameMenu.buttonTheme);
	Menu::newGameMenu.cancel->setCallback([]{
		Menu::storyModeMenu.changeView(true);
		Menu::newGameMenu.changeView(false);
	});

	Menu::newGameMenu.changeView(false);
}

void Menu::createLoadGameMenu()
{
	/*_loadGameMenu = new nanogui::Window(_screen, "Load Game");
	_loadGameMenu->setLayout(new nanogui::GroupLayout());
	_loadGameMenu->setSize({width / 3, height / 3});
	_loadGameMenu->setVisible(false);
	_loadGameMenu->center();
	auto *panel = new nanogui::VScrollPanel(_loadGameMenu);
	panel->setSize({(width / 3) - 10, (height / 3) - 90});
	panel->setPosition({10, 35});
	auto *panelView = new nanogui::Window(panel, "");
	panelView->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical, nanogui::Alignment::Fill, 20, 5));
	panelView->setSize({(width / 3) - 10, (height / 3) - 90});
	panelView->setPosition({10, 35});

	DIR     *dir = opendir("save");
	struct dirent *file;
	if (dir)
	{
		int i = 0;
		while ((file = readdir(dir)))
		{
			if (file->d_name[0] == '.')
				continue;
			nanogui::Button *lab = new nanogui::Button(panelView, file->d_name);
			lab->setSize({(width / 3) - 30, 30});
			lab->setPosition({5, i * 33 + 3});
			lab->setCallback([lab]{
				std::cout << "load : " << lab->caption() << std::endl;
				activeMenu->_mainGame->setGameState(GAMESTATE::GAME);
				activeMenu->_loadGameMenu->setVisible(false);
				activeMenu->_pauseGameMenu->setVisible(true);
				activeMenu->_saveFileName = lab->caption();
				activeMenu->_scene = new Scene();
				activeMenu->_scene->loadGame(activeMenu->_mainGame, activeMenu->_saveFileName);
				Menu::stopMenuMusic();
			});
			i++;
		}
	}else{
		perror("directory error");
	}
	closedir(dir);
	nanogui::Button *back = new nanogui::Button(_loadGameMenu, "back");
	back->setPosition({(width / 3) - 100, (height / 3) - 50});
	back->setSize({70, 30});
	back->setCallback([]{
		activeMenu->_loadGameMenu->setVisible(false);
		activeMenu->_startMenu->setVisible(true);
	});*/
}

void Menu::createOptionsMenu()
{
	/*_optionsMenu = new nanogui::Window(_screen, "Options");

	_optionsMenu->setLayout(new nanogui::GroupLayout());
	_optionsMenu->setWidth(300);
	_optionsMenu->setHeight(500);
	_optionsMenu->setVisible(false);
	_optionsMenu->center();

	nanogui::GridLayout *layout =
			new nanogui::GridLayout(nanogui::Orientation::Horizontal, 2,
						   nanogui::Alignment::Middle, 15, 5);
	layout->setColAlignment(
			{ nanogui::Alignment::Maximum, nanogui::Alignment::Fill });
	layout->setSpacing(0, 10);
	_optionsMenu->setLayout(layout);

	//Volume Slider
	nanogui::Slider *volumeSlider = new nanogui::Slider(this->_optionsMenu);
	volumeSlider->setValue(this->config["sound"]["volume"].as<float>());
//	volumeSlider->setPosition({10, 50});
//	volumeSlider->setSize({200, 20});
	volumeSlider->setVisible(true);

	//Text box to show value of Volume Slider
	nanogui::TextBox *volumeTxtBox = new nanogui::TextBox(this->_optionsMenu);
//	volumeTxtBox->setPosition({200, 50});
//	volumeTxtBox->setSize({20, 20});
	volumeTxtBox->setAlignment(nanogui::TextBox::Alignment::Right);
	volumeTxtBox->setVisible(true);
	volumeTxtBox->setValue(std::to_string(volumeSlider->value()));
	volumeTxtBox->setUnits("%");
	volumeSlider->setCallback([volumeTxtBox](float value) {
		volumeTxtBox->setValue(std::to_string(static_cast<int>(value * 100)));
	});
	volumeSlider->setFinalCallback([&](float value) {
		std::cout << "Final volumeSlider value: " << static_cast<int>(value) << std::endl;
	});

	//temporary spacer
	nanogui::Widget *muteSpacer = new nanogui::Widget(this->_optionsMenu);

	nanogui::CheckBox *muteCb = new nanogui::CheckBox(this->_optionsMenu, "Mute",
													  [](bool state) { std::cout << "Mute: " << state << std::endl; }
	);
//	volumeTxtBox->setPosition({50, 100});
//	volumeTxtBox->setSize({150, 50});
	volumeTxtBox->setVisible(true);

	//temporary spacer
	nanogui::Widget *resSpacer = new nanogui::Widget(this->_optionsMenu);

	std::vector<std::string> resList = this->config["graphics"]["resolutionList"].as<std::vector<std::string>>();
	nanogui::ComboBox *resCoBo = new nanogui::ComboBox(this->_optionsMenu, this->config["graphics"]["resolutionList"].as<std::vector<std::string>>());




	nanogui::Button *applyButton = new nanogui::Button(this->_optionsMenu, "Apply");
//	applyButton->setPosition({50, 180});
//	applyButton->setSize({150, 50});
	applyButton->setCallback([=]{
		this->config["sound"]["volume"] = std::to_string(volumeSlider->value());
		this->config["sound"]["mute"] = muteCb->checked();
		this->config["graphics"]["resolution"] = this->config["graphics"]["resolutionList"][resCoBo->selectedIndex()];
	});



	nanogui::Button *backButton = new nanogui::Button(this->_optionsMenu, "Back");
//	backButton->setPosition({50, 180});
	backButton->setSize({150, 50});
	backButton->setCallback([]{
		activeMenu->_optionsMenu->setVisible(false);
		activeMenu->_startMenu->setVisible(true);
	});*/

}

void Menu::createPauseGameMenu()
{
	/*_pauseGameMenu = new nanogui::Window(_screen, "Pause");
	_pauseGameMenu->setLayout(new nanogui::GroupLayout());
	_pauseGameMenu->setSize({250, 250});
	_pauseGameMenu->setVisible(false);
	_pauseGameMenu->center();
	/// game saved Window
	_gameSaved = new nanogui::Window(_screen, "");
	_gameSaved->setSize({250, 40});
	_gameSaved->setVisible(false);
	_gameSaved->setPosition({(width / 2) - 125, height - 100});
	nanogui::Label *saveLabel = new nanogui::Label(_gameSaved, "Game Saved", "sans", 25);
	saveLabel->setPosition({70, 5});
	saveLabel->setSize({150, 30});
	saveLabel->setColor({0.0f, 0.7f, 0.0f, 1.0f});
	/// resume game button
	nanogui::Button *resume = new nanogui::Button(_pauseGameMenu, "Resume");
	resume->setPosition({50, 50});
	resume->setSize({150, 50});
	resume->setCallback([]{
		activeMenu->_mainGame->setGameState(GAMESTATE::GAME);
		activeMenu->_gameSaved->setVisible(false);
	});
	/// save game button
	nanogui::Button *save = new nanogui::Button(_pauseGameMenu, "Save");
	save->setPosition({50, 120});
	save->setSize({150, 50});
	save->setCallback([]{
		activeMenu->_scene->saveGame(activeMenu->_saveFileName);
		activeMenu->_gameSaved->setVisible(true);
	});
	/// quit game button
	nanogui::Button *quit = new nanogui::Button(_pauseGameMenu, "Quit");
	quit->setPosition({50, 190});
	quit->setSize({150, 50});
	quit->setCallback([]{
		if (activeMenu->_scene != nullptr)
		{
			delete activeMenu->_scene;
			activeMenu->_screen->removeChild(activeMenu->_loadGameMenu);
			activeMenu->_screen->removeChild(activeMenu->_newGameMenu);
			//delete activeMenu->_loadGameMenu;
			activeMenu->_createNewGameMenu(activeMenu->_mainGame->getGameWindow().getWidth(),
			                               activeMenu->_mainGame->getGameWindow().getHeight());
			activeMenu->_createLoadGameMenu(activeMenu->_mainGame->getGameWindow().getWidth(),
			                                activeMenu->_mainGame->getGameWindow().getHeight());
			std::cout << "scene deleted" << std::endl;
			std::map<const char *, Func>::iterator it;
			it = MainGame::functions.find("sceneUpdate");
			MainGame::functions.erase(it);
			MainGame::renderer.removeAll();
			activeMenu->_gameSaved->setVisible(false);
		}
		activeMenu->_mainGame->setGameState(GAMESTATE::MENU);
		activeMenu->_pauseGameMenu->setVisible(false);
		activeMenu->_startMenu->setVisible(true);
		std::cout << "game quit" << std::endl;
	});*/
}

void Menu::createBackground()
{
	glm::mat4 viewMatrix = _mainGame->getGameCamera().getViewMatrix();
	_mainGame->getShader("gui")->setUniformMat4((GLchar *)"view_matrix", viewMatrix);
	/// new bg menu
	_menuBg = new Zion::SquareSprite(*_mainGame->getShader("gui"), 1.5, 0, 4, 5);
	_menuBg->addTextureFromFile("resource/images/menuBg.png");
	/// bomberman menu logo
	_menuTitle = new Zion::SquareSprite(*_mainGame->getShader("gui"), 0, 0, 2, 1);
	_menuTitle->addTextureFromFile("resource/images/menuLogo.png");
	/// adding heart logo
	Menu::gui.heart = new Zion::SquareSprite(*_mainGame->getShader("gui"), 0, 0, 0.2, 0.2);
	Menu::gui.heart->addTextureFromFile("resource/images/heart.png");
	/// adding speed
	Menu::gui.speed = new Zion::SquareSprite(*_mainGame->getShader("gui"), 0, 0, 0.2, 0.2);
	Menu::gui.speed->addTextureFromFile("resource/images/bolt.png");
	/// adding bomb logo
	Menu::gui.bomb = new Zion::SquareSprite(*_mainGame->getShader("gui"), 0, 0, 0.3, 0.3);
	Menu::gui.bomb->addTextureFromFile("resource/images/bomb.png");
	/// adding heart explode
	Menu::gui.explode = new Zion::SquareSprite(*_mainGame->getShader("gui"), 0, 0, 0.3, 0.3);
	Menu::gui.explode->addTextureFromFile("resource/images/explode.png");
	/// adding heart case
	Menu::gui.heartCase = new Zion::SquareSprite(*_mainGame->getShader("gui"), 0, 0, 0.5, 0.7);
	Menu::gui.heartCase->addTextureFromFile("resource/images/hudBg.png");
	/// add bombman
	Menu::gui.bombMan = new Zion::SquareSprite(*_mainGame->getShader("gui"), 0, 0, 2.0, 1.5);
	Menu::gui.bombMan->addTextureFromFile("resource/images/bombMan3.png");
	/// add enemy1
	Menu::gui.enemy1 = new Zion::SquareSprite(*_mainGame->getShader("gui"), 0, 0, 1.5, 1.0);
	Menu::gui.enemy1->addTextureFromFile("resource/images/enemy.png");
	/// add enemy2
	Menu::gui.enemy2 = new Zion::SquareSprite(*_mainGame->getShader("gui"), 0, 0, 0.4, 0.3);
	Menu::gui.enemy2->addTextureFromFile("resource/images/enemy2.png");
	/// add banner
	Menu::gui.whiteBanner = new Zion::SquareSprite(*_mainGame->getShader("gui"), 0, 0, 7.5, 1.2);
	Menu::gui.whiteBanner->addBaseColor({0.2, 0.2, 0.2, 0.5});
}

GLFWwindow* Menu::getGlfwWindow()
{
	return _screen->glfwWindow();
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
		menu->_screen->drawWidgets();
	}
	else if (state == GAMESTATE::GAME)
	{
		menu->renderGui();
	}
	else if (state == GAMESTATE::START)
	{
		float halfHeight = (float)Menu::windowHeight / 3;
		float halfWidth = (float)Menu::windowWidth / 2;
		float xOffset = (float)Menu::windowWidth / 5;

		if (Menu::textStartTime == 0.0f)
			Menu::textStartTime = (float)glfwGetTime();
		float changeTime = (float)glfwGetTime() - Menu::textStartTime;
		float offset;
		if (changeTime <= 1.5) {
			offset = (changeTime / 1.5f) * (halfWidth - xOffset);
		}else {
			if (changeTime > 1.7)
			{
				float diffVal = changeTime - 1.7f;
				offset = (diffVal / 1.5f) * (halfWidth - xOffset);
				offset = (halfWidth - xOffset) - offset;
			}else
				offset = (halfWidth - xOffset);
		}

		Menu::gui.whiteBanner->render(glm::translate(glm::mat4(), {0, 0.3, 0}));
		MainGame::fontRenderer1->renderText("STAGE", offset, halfHeight, 3.0f, {0.8, 0.1, 0.2});
		MainGame::fontRenderer1->renderText("1", (float)Menu::windowWidth - offset, halfHeight, 3.0f, {0.8, 0.1, 0.2});

		if (changeTime >= 3.4f)
		{
			menu->_mainGame->setGameState(GAMESTATE::GAME);
			Menu::textStartTime = 0;
		}
	}
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