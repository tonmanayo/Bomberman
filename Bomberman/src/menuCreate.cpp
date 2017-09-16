#include <menu.hpp>

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

void Menu::createStoryMenu()
{
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
		Menu::title->setCaption("NEW GAME");
	});

	Menu::storyModeMenu.loadGame = new nanogui::Button(_screen, "LOAD GAME");
	Menu::storyModeMenu.loadGame->setSize({buttonWidth, buttonHeight});
	Menu::storyModeMenu.loadGame->setPosition({posX + buttonWidth + posX, posY});
	Menu::storyModeMenu.loadGame->setTheme(Menu::storyModeMenu.buttonTheme);
	Menu::storyModeMenu.loadGame->setCallback([]{
		Menu::storyModeMenu.changeView(false);
		Menu::loadGameMenu.changeView(true);
		Menu::title->setCaption("LOAD GAME");
	});

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
		Menu::title->setVisible(false);
		Menu::pauseMenu.changeView(true);
		Menu::createNewGame(1, Menu::difficulty, Menu::newGameMenu.profileNameBox->value());
		Menu::newGameMenu.profileNameBox->setValue("");
		Menu::loadSaveDirectory();
	});

	Menu::newGameMenu.cancel = new nanogui::Button(_screen, "Cancel");
	Menu::newGameMenu.cancel->setSize({buttonWidth, buttonHeight});
	Menu::newGameMenu.cancel->setPosition({buttonPosX + buttonWidth + buttonPosX, posY});
	Menu::newGameMenu.cancel->setTheme(Menu::newGameMenu.buttonTheme);
	Menu::newGameMenu.cancel->setCallback([]{
		Menu::storyModeMenu.changeView(true);
		Menu::newGameMenu.changeView(false);
		Menu::title->setCaption("CAMPAIGN");
	});

	Menu::newGameMenu.changeView(false);
}

void Menu::createPauseGameMenu()
{
	/// window theme
	nanogui::Theme  *winTheme = new nanogui::Theme(_screen->nvgContext());
	winTheme->mWindowCornerRadius = 0;
	winTheme->mWindowFillFocused = {30, 30, 30, 255};
	winTheme->mWindowFillUnfocused = {30, 30, 30, 255};

	/// dimensions and positions for buttons
	int     posY = Menu::windowHeight / 3;
	int     winSizeX = Menu::windowWidth / 5;
	winSizeX = (winSizeX < 350) ? 350 : winSizeX;
	int     xOffset = winSizeX / 5;
	int     buttonWidth = xOffset * 3;

	Menu::pauseMenu.window = new nanogui::Window(_screen, "");
	Menu::pauseMenu.window->setSize({winSizeX, Menu::windowHeight});
	Menu::pauseMenu.window->setPosition({0, 0});
	Menu::pauseMenu.window->setTheme(winTheme);

	/// resume button
	Menu::pauseMenu.resume = new nanogui::Button(Menu::pauseMenu.window, "Resume");
	Menu::pauseMenu.resume->setSize({buttonWidth, 50});
	Menu::pauseMenu.resume->setPosition({xOffset, posY});
	Menu::pauseMenu.resume->setCallback([]{
		if (Menu::pauseMenu.quitWindow->visible())
			return;
		Menu::pauseMenu.saveLabel->setVisible(false);
		activeMenu->_mainGame->setGameState(GAMESTATE::GAME);
		Menu::pauseMenu.quitWindow->setVisible(false);
	});

	posY += 70;
	/// save button
	Menu::pauseMenu.save = new nanogui::Button(Menu::pauseMenu.window, "Save");
	Menu::pauseMenu.save->setSize({buttonWidth, 50});
	Menu::pauseMenu.save->setPosition({xOffset, posY});
	Menu::pauseMenu.save->setCallback([]{
		if (Menu::pauseMenu.quitWindow->visible())
			return;
		activeMenu->scene->saveGame(activeMenu->_saveFileName);
		activeMenu->createLoadGameMenu();
		Menu::pauseMenu.saveLabel->setVisible(true);
		Menu::pauseMenu.quitWindow->setVisible(false);
	});

	/// save label
	Menu::pauseMenu.saveLabel = new   nanogui::Label(Menu::pauseMenu.window, "Game Saved", "sans", 25);
	Menu::pauseMenu.saveLabel->setSize({buttonWidth, 50});
	Menu::pauseMenu.saveLabel->setPosition({xOffset * 2, Menu::windowHeight - 70});
	Menu::pauseMenu.saveLabel->setColor({136, 224, 74, 255});
	Menu::pauseMenu.saveLabel->setVisible(false);

	posY += 70;
	/// load button
	Menu::pauseMenu.load = new nanogui::Button(Menu::pauseMenu.window, "Load");
	Menu::pauseMenu.load->setSize({buttonWidth, 50});
	Menu::pauseMenu.load->setPosition({xOffset, posY});

	posY += 70;
	/// quit button
	Menu::pauseMenu.quit = new nanogui::Button(Menu::pauseMenu.window, "Quit");
	Menu::pauseMenu.quit->setSize({buttonWidth, 50});
	Menu::pauseMenu.quit->setPosition({xOffset, posY});
	Menu::pauseMenu.quit->setCallback([]{
		if (Menu::pauseMenu.quitWindow->visible())
			return;
		Menu::pauseMenu.quitWindow->setVisible(true);
	});

	/// quit window
	int  quitWinOffset = winSizeX / 3;
	int  padding = quitWinOffset / 3;
	Menu::pauseMenu.quitWindow = new nanogui::Window(_screen, "Do you want to quit game?");
	Menu::pauseMenu.quitWindow->setSize({winSizeX, 75});
	Menu::pauseMenu.quitWindow->setPosition({(Menu::windowWidth / 2) - (winSizeX / 2), (Menu::windowHeight / 2)});
	Menu::pauseMenu.quitWindow->setTheme(winTheme);
	Menu::pauseMenu.quitWindow->setVisible(false);

	/// yes button
	Menu::pauseMenu.yes = new nanogui::Button(Menu::pauseMenu.quitWindow, "Yes");
	Menu::pauseMenu.yes->setSize({quitWinOffset, 30});
	Menu::pauseMenu.yes->setPosition({padding, 35});
	Menu::pauseMenu.yes->setCallback([]{
		Menu::pauseMenu.quitWindow->setVisible(false);
		Menu::pauseMenu.saveLabel->setVisible(false);
		Menu::pauseMenu.changeView(false);
		Menu::mainMenu.changeView(true);
		Menu::title->setVisible(true);
		Menu::title->setCaption("MAIN MENU");
		activeMenu->_mainGame->setGameState(GAMESTATE::MENU);
		Menu::destroyGame();
	});

	/// no button
	Menu::pauseMenu.no = new nanogui::Button(Menu::pauseMenu.quitWindow, "No");
	Menu::pauseMenu.no->setSize({quitWinOffset, 30});
	Menu::pauseMenu.no->setPosition({2 * padding + quitWinOffset, 35});
	Menu::pauseMenu.no->setCallback([]{
		Menu::pauseMenu.quitWindow->setVisible(false);
		Menu::pauseMenu.saveLabel->setVisible(false);
	});

	Menu::pauseMenu.changeView(false);
}

void Menu::createEndGameMenu()
{
	/// window theme
	nanogui::Theme  *winTheme = new nanogui::Theme(_screen->nvgContext());
	winTheme->mWindowCornerRadius = 0;
	winTheme->mWindowFillFocused = {30, 30, 30, 255};
	winTheme->mWindowFillUnfocused = {30, 30, 30, 255};

	/// dimensions and positions for buttons
	int     winSizeX = Menu::windowWidth / 5;
	winSizeX = (winSizeX < 350) ? 350 : winSizeX;
	int     xOffset = winSizeX / 5;

	/// end window
	int  quitWinOffset = winSizeX / 3;
	int  padding = quitWinOffset / 3;
	Menu::endGameMenu.window = new nanogui::Window(_screen, "Do you want to retry?");
	Menu::endGameMenu.window->setSize({winSizeX, 75});
	Menu::endGameMenu.window->setPosition({(Menu::windowWidth / 2) - (winSizeX / 2), (Menu::windowHeight / 2)});
	Menu::endGameMenu.window->setTheme(winTheme);

	/// yes button
	Menu::endGameMenu.yes = new nanogui::Button(Menu::endGameMenu.window, "Yes");
	Menu::endGameMenu.yes->setSize({quitWinOffset, 30});
	Menu::endGameMenu.yes->setPosition({padding, 35});
	Menu::endGameMenu.yes->setCallback([]{
		int level = activeMenu->scene->getLevel();
		int difficulty = activeMenu->scene->getDifficultyValue();
		destroyGame();
		Menu::endGameMenu.changeView(false);
		Menu::mainMenu.changeView(false);
		Menu::pauseMenu.changeView(true);
		createNewGame(level, difficulty, activeMenu->_saveFileName);
		Menu::textStartTime = 0;
	});

	/// no button
	Menu::endGameMenu.no = new nanogui::Button(Menu::endGameMenu.window, "No");
	Menu::endGameMenu.no->setSize({quitWinOffset, 30});
	Menu::endGameMenu.no->setPosition({2 * padding + quitWinOffset, 35});
	Menu::endGameMenu.no->setCallback([]{
		destroyGame();
		Menu::pauseMenu.changeView(false);
		Menu::title->setVisible(true);
		Menu::endGameMenu.changeView(false);
		Menu::mainMenu.changeView(true);
		activeMenu->_mainGame->setGameState(GAMESTATE::MENU);
		Menu::textStartTime = 0;
	});

	Menu::endGameMenu.changeView(false);
}

void Menu::createLoadGameMenu()
{
	Menu::loadGameMenu.buttonTheme = Menu::mainMenu.buttonTheme;

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

	Menu::loadSaveDirectory();

	/// filenames panel
	nanogui::Theme  *panelTheme = new nanogui::Theme(_screen->nvgContext());
	panelTheme->mTransparent = {10, 10, 10, 255};
	panelTheme->mButtonFontSize = 20;
	panelTheme->mFontBold = 0;
	panelTheme->mButtonGradientTopUnfocused = {48, 55, 66, 230};
	panelTheme->mButtonGradientBotUnfocused = {21, 24, 29, 230};
	panelTheme->mBorderDark = {10, 10, 10, 255};
	panelTheme->mBorderMedium = {10, 10, 10, 255};
	panelTheme->mBorderLight = {10, 10, 10, 255};
	panelTheme->mWindowFillUnfocused = {10, 10, 10, 255};
	panelTheme->mWindowFillFocused = {10, 10, 10, 255};

	Menu::loadGameMenu.panel = new nanogui::VScrollPanel(_screen);
	Menu::loadGameMenu.panel->setSize({textBoxWidth, textBoxHeight * 3});
	Menu::loadGameMenu.panel->setPosition({posX, posY});

	auto *panelView = new nanogui::Widget(Menu::loadGameMenu.panel);
	panelView->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical, nanogui::Alignment::Fill, 20, 5));
	panelView->setSize({textBoxWidth, textBoxHeight * 4});
	panelView->setTheme(panelTheme);

	int     namePosY = 10;
	for (std::string& fileName : Menu::loadGameMenu.fileNames){
		auto *nameButton = new nanogui::Button(panelView, fileName);
		nameButton->setSize({textBoxWidth - 10, 20});
		nameButton->setPosition({5, namePosY});
		nameButton->setTextColor({230, 230, 230, 255});
		nameButton->setTheme(panelTheme);
		nameButton->setCallback([fileName]{
			Menu::loadGameMenu.changeView(false);
			Menu::title->setVisible(false);
			Menu::pauseMenu.changeView(true);
			activeMenu->_saveFileName = fileName;
			activeMenu->scene = new Scene();
			activeMenu->scene->loadGame(activeMenu->_mainGame, fileName);
			activeMenu->_mainGame->setGameState(GAMESTATE::START);
			Menu::textStartTime = 0;
			Menu::stopMenuMusic();
		});
		namePosY += 40;
	}

	Menu::loadGameMenu.cancel = new nanogui::Button(_screen, "Cancel");
	Menu::loadGameMenu.cancel->setSize({buttonWidth, buttonHeight});
	Menu::loadGameMenu.cancel->setPosition({posX + textBoxWidth / 2, posY + textBoxHeight * 3 + 10});
	Menu::loadGameMenu.cancel->setTheme(Menu::loadGameMenu.buttonTheme);
	Menu::loadGameMenu.cancel->setCallback([]{
		Menu::storyModeMenu.changeView(true);
		Menu::loadGameMenu.changeView(false);
		Menu::title->setCaption("CAMPAIGN");
	});

	Menu::loadGameMenu.changeView(false);
}