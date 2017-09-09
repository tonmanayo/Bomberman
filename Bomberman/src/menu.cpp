#include <menu.hpp>


Menu*   Menu::activeMenu = nullptr;

Menu::Menu(float width, float height, MainGame *mainGame, bool fullScreen, bool resizable)
{
	initMenu(width, height, mainGame, fullScreen, resizable);
}

Menu::Menu(const Menu &rhs)
{
	*this = rhs;
}

Menu& Menu::operator=(const Menu &rhs)
{
	if (this != &rhs)
	{
		_screen = rhs._screen;
		_startGameMenu = rhs._startGameMenu;
		_startMenu = rhs._startMenu;
		_pauseGameMenu = rhs._pauseGameMenu;
		_mainGame = rhs._mainGame;
	}
	return *this;
}

bool Menu::initMenu(float width, float height, MainGame *mainGame, bool fullScreen, bool resizable)
{
	_mainGame = mainGame;
	Menu::activeMenu = this;

	this->config = YAML::LoadFile("../config.yaml");

	_screen = new nanogui::Screen({width, height}, "Bomberman", resizable, fullScreen,
		8, 8, 24, 8, 4, 4, 1);
	_screen->setVisible(true);
	Zion::Input::mouseCallback2 = Menu::mouseCallback;
	Zion::Input::cursorPositionCallback2 = Menu::cursorPositionCallback;
	Zion::Input::keyCallback2 = Menu::keyCallback;
	std::vector<void *> params;
	params.push_back(this);
	MainGame::functions.insert(std::pair<const char *, Func>("menuUpdate", {Menu::updateMenu, params}));
	return true;
}

bool Menu::buildMenuWindows(float width, float height)
{
	_createStartMenu(width, height);
	_createStartGameMenu(width, height);
	_createOptionsMenu(width, height);
	_createPauseGameMenu(width, height);
	_createBackground(width, height);
	_createExitWindow(width, height);
	_screen->performLayout();
	return true;
}

void Menu::_createStartMenu(float width, float height)
{
	_startMenu = new nanogui::Window(_screen, "Menu");
	_startMenu->setLayout(new nanogui::GroupLayout());
	_startMenu->setWidth(250);
	_startMenu->setHeight(250);
	_startMenu->setVisible(true);
	_startMenu->center();
	/// start game button
	nanogui::Button *startButton = new nanogui::Button(_startMenu, "Start Game");
	startButton->setPosition({50, 40});
	startButton->setSize({150, 50});
	startButton->setCallback([]{
		activeMenu->_mainGame->setGameState(GAMESTATE::GAME);
		activeMenu->_startGameMenu->setVisible(false);
		activeMenu->_pauseGameMenu->setVisible(true);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		activeMenu->_scene = new Scene(activeMenu->_mainGame, activeMenu->_mainGame->getMap("map1"), 5);
	});
	/// options button
	nanogui::Button *optionsButton = new nanogui::Button(_startMenu, "Options");
	optionsButton->setPosition({50, 110});
	optionsButton->setSize({150, 50});
	optionsButton->setCallback([] {
		activeMenu->_startMenu->setVisible(false);
		activeMenu->_optionsMenu->setVisible(true);
	});
	/// exit button
	nanogui::Button *exitButton = new nanogui::Button(_startMenu, "Exit");
	exitButton->setPosition({50, 180});
	exitButton->setSize({150, 50});
	exitButton->setCallback([]{ Menu::exitButtonCallBack();});
}

void Menu::_createStartGameMenu(float width, float height)
{
	_startGameMenu = new nanogui::Window(_screen, "Start Game");
	_startGameMenu->setVisible(false);
}

void Menu::_createOptionsMenu(float width, float height)
{
	_optionsMenu = new nanogui::Window(_screen, "Options");

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
	});

}

void Menu::_createPauseGameMenu(float width, float height)
{
	_pauseGameMenu = new nanogui::Window(_screen, "Pause");
	_pauseGameMenu->setVisible(false);
	_pauseGameMenu->setSize({250, 250});
	_pauseGameMenu->center();
	/// resume game button
	nanogui::Button *resume = new nanogui::Button(_pauseGameMenu, "Resume");
	resume->setPosition({50, 40});
	resume->setSize({150, 50});
	resume->setCallback([]{
		activeMenu->_mainGame->setGameState(GAMESTATE::GAME);
		activeMenu->_pauseGameMenu->setVisible(false);
	});
}

void Menu::_createExitWindow(float width, float height)
{
	_exitWindow = new nanogui::Window(_screen, "");
	_exitWindow->setLayout(new nanogui::GroupLayout());
	_exitWindow->setWidth(400);
	_exitWindow->setHeight(150);
	_exitWindow->setVisible(false);
	_exitWindow->center();
	/// text label
	nanogui::Label *label = new nanogui::Label(_exitWindow, "Do you want to exit game?", "sans", 30);
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
		activeMenu->_startMenu->setVisible(true);
		activeMenu->_exitWindow->setVisible(false);
	});
}

void Menu::_createBackground(float width, float height)
{
	_menuBg = new Zion::SquareSprite(*_mainGame->getShader("basic"), 0, 0, 8, 5);
	_menuBg->addTextureFromFile("resource/images/menu_bg.jpg");
	_menuTitle = new Zion::SquareSprite(*_mainGame->getShader("basic"), 0, 0, 3, 1.5);
	_menuTitle->addTextureFromFile("resource/images/title1.png");
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
		menu->_menuBg->render(glm::translate(glm::mat4(), {0, 0, -1}));
		menu->_menuTitle->render(glm::translate(glm::mat4(), {0, 1, 0}));
		menu->_screen->drawWidgets();
	}
	if (state == GAMESTATE::PAUSE)
	{
		//menu->_menuBg->render(glm::translate(glm::mat4(), {0, 0, -1}));
		if (menu->_mainGame->getGameWindow().isKeyPressed(GLFW_KEY_ESCAPE))
		{
			game->setGameState(GAMESTATE::GAME);
			return;
		}
		menu->_screen->drawWidgets();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
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

void Menu::exitButtonCallBack()
{
	activeMenu->_startMenu->setVisible(false);
	activeMenu->_exitWindow->setVisible(true);
}

void Menu::backButtonCallBack()
{
	activeMenu->_optionsMenu->setVisible(false);
	activeMenu->_exitWindow->setVisible(true);
}

//void Menu::applyButtonCallBack()
//{
//	this->config["sound"]["volume"] = std::to_string(volumeSlider->value());
//	this->config["sound"]["mute"] = muteCb->checked();
//}