#include <menu.hpp>

void Menu::createMainMenu()
{
	Menu::mainMenu.buttonTheme = new nanogui::Theme(_screen->nvgContext());
	Menu::mainMenu.buttonTheme->mButtonFontSize = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 30) / 1100 : 30;
	Menu::mainMenu.buttonTheme->mFontNormal = 0;
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

	int mainTitleFontSize = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 45) / 1100 : 45;
	Menu::title = new nanogui::Label(_screen, "MAIN MENU", "sans-bold", mainTitleFontSize);
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
	Menu::mainMenu.options->setCallback([]{
		Menu::mainMenu.changeView(false);
		Menu::optionMenu.changeView(true);
		Menu::optionMenu.showScreen();
		Menu::title->setCaption("OPTIONS");
		Menu::copyOptions(Menu::tmpOptions, Menu::options);
	});

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
	int     buttonWidth = (Menu::windowWidth < 1280) ? Menu::windowWidth / 5 : Menu::windowWidth / 4;
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
	int labelFontSize = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 25) / 1000 : 25;
	nanogui::Label *label = new nanogui::Label(_exitWindow, "Do you want to exit game?", "sans", labelFontSize);
	int labelPositionX = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 70) / 1280 : 70;
	int labelPositionY = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 20) / 1280 : 20;
	label->setPosition({labelPositionX, labelPositionY});
	label->setSize({300, 30});
	/// yes button
	int buttonSizeX = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 70) / 1000 : 70;
	int buttonSizeY = 30;
	int buttonPosX = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 80) / 1280 : 80;
	nanogui::Button *yes = new nanogui::Button(_exitWindow, "Yes");
	yes->setSize({buttonSizeX, buttonSizeY});
	yes->setPosition({buttonPosX, 90});
	yes->setCallback([]{
		glfwSetWindowShouldClose(activeMenu->_screen->glfwWindow(), (int)true);
	});
	/// no button
	buttonPosX = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 250) / 1280 : 250;
	nanogui::Button *no = new nanogui::Button(_exitWindow, "No");
	no->setSize({buttonSizeX, buttonSizeY});
	no->setPosition({buttonPosX, 90});
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
	int     textBoxHeight = (Menu::windowWidth > 800) ? 50 : 30;
	int     textBoxWidth = (Menu::windowWidth > 800) ? Menu::windowWidth / 3 : Menu::windowWidth / 4;
	textBoxWidth = (textBoxWidth > 450) ? 450 : textBoxWidth;
	int     offset = ((Menu::windowWidth / 2) - textBoxWidth) / 2;
	int     posX = (Menu::windowWidth / 2) - (textBoxWidth + offset);
	int     buttonHeight = 50;
	int     buttonWidth = (Menu::windowWidth > 800) ? Menu::windowWidth / 4 : Menu::windowWidth / 5;
	buttonWidth = (buttonWidth > 250) ? 250 : buttonWidth;
	int     buttonPosX = ((Menu::windowWidth / 2) - (buttonWidth * 2)) / 3;

	int profileFontSize = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 30) / 1000 : 30;
	Menu::newGameMenu.textLabel = new nanogui::Label(_screen, "Enter Profile Name", "sans", profileFontSize);
	Menu::newGameMenu.textLabel->setSize({textBoxWidth, textBoxHeight});
	int tempOffset = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 80) / 1280 : 80;
	Menu::newGameMenu.textLabel->setPosition({posX + tempOffset, posY});
	Menu::newGameMenu.textLabel->setColor({50, 50, 50, 255});

	posY += textBoxHeight + ((Menu::windowWidth > 800) ? 10 : 5);
	Menu::newGameMenu.profileNameBox = new nanogui::TextBox(_screen, "");
	Menu::newGameMenu.profileNameBox->setSize({textBoxWidth, textBoxHeight});
	Menu::newGameMenu.profileNameBox->setPosition({posX, posY});
	Menu::newGameMenu.profileNameBox->setEditable(true);
	Menu::newGameMenu.profileNameBox->theme()->mTextColor = {40, 40, 40, 255};
	int textBoxFontSize = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 35) / 1000 : 35;
	Menu::newGameMenu.profileNameBox->setFontSize(textBoxFontSize);

	posY += textBoxHeight + ((Menu::windowWidth > 800) ? 20 : 10);
	int     otherButtonWidth = (Menu::windowWidth > 800) ? textBoxWidth / 3 : textBoxWidth / 2;
	int     otherButtonHeight = (Menu::windowWidth > 800) ? buttonHeight : buttonHeight / 2;
	int     otherPosX = posX + ((Menu::windowWidth > 800) ? 10 : -15);

	nanogui::Theme  *tmp = new nanogui::Theme(_screen->nvgContext());
	tmp->mFontBold = 1;
	tmp->mFontNormal = 0;
	tmp->mStandardFontSize = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 35) / 1200 : 35;
	tmp->mTextColor = {40, 40, 40, 255};

	Menu::newGameMenu.easy = new nanogui::CheckBox(_screen, "Easy");
	Menu::newGameMenu.easy->setSize({otherButtonWidth, otherButtonHeight});
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
	Menu::newGameMenu.normal->setSize({otherButtonWidth, otherButtonHeight});
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
	Menu::newGameMenu.hard->setSize({otherButtonWidth, otherButtonHeight});
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
	int     buttonWidth = (Menu::windowWidth < 1280) ? Menu::windowWidth / 5 : Menu::windowWidth / 4;
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

void Menu::createOptionsMenu()
{
	/// creating focus tab theme
	Menu::optionMenu.focusTabTheme = new nanogui::Theme(_screen->nvgContext());
	Menu::optionMenu.focusTabTheme->mButtonCornerRadius = 0;
	Menu::optionMenu.focusTabTheme->mButtonFontSize = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 30) / 1100 : 30;
	Menu::optionMenu.focusTabTheme->mFontNormal = 0;
	Menu::optionMenu.focusTabTheme->mFontBold = 0;
	Menu::optionMenu.focusTabTheme->mTextColor = {230, 230, 230, 255};
	Menu::optionMenu.focusTabTheme->mButtonGradientTopUnfocused = {174, 17, 2, 255};
	Menu::optionMenu.focusTabTheme->mButtonGradientBotUnfocused = {177, 21, 8, 255};
	Menu::optionMenu.focusTabTheme->mButtonGradientTopFocused = {174, 17, 2, 255};
	Menu::optionMenu.focusTabTheme->mButtonGradientBotFocused = {177, 21, 8, 255};
	Menu::optionMenu.focusTabTheme->mButtonGradientTopPushed = {174, 17, 2, 255};
	Menu::optionMenu.focusTabTheme->mButtonGradientBotPushed = {177, 21, 8, 255};
	Menu::optionMenu.focusTabTheme->mBorderDark = {177, 21, 8, 255};
	Menu::optionMenu.focusTabTheme->mBorderLight = {177, 21, 8, 255};
	Menu::optionMenu.focusTabTheme->mBorderMedium = {177, 21, 8, 255};
	Menu::optionMenu.focusTabTheme->mDropShadow = {0, 0, 0, 255};

	/// creating unfocus tab theme
	Menu::optionMenu.unfocusTabTheme = new nanogui::Theme(_screen->nvgContext());
	Menu::optionMenu.unfocusTabTheme->mButtonCornerRadius = 0;
	Menu::optionMenu.unfocusTabTheme->mButtonFontSize = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 30) / 1100 : 30;
	Menu::optionMenu.unfocusTabTheme->mFontNormal = 0;
	Menu::optionMenu.unfocusTabTheme->mFontBold = 0;
	Menu::optionMenu.unfocusTabTheme->mTextColor = {10, 10, 10, 255};
	Menu::optionMenu.unfocusTabTheme->mButtonGradientTopUnfocused = {174, 17, 2, 0};
	Menu::optionMenu.unfocusTabTheme->mButtonGradientBotUnfocused = {177, 21, 8, 0};
	Menu::optionMenu.unfocusTabTheme->mButtonGradientTopFocused = {174, 17, 2, 0};
	Menu::optionMenu.unfocusTabTheme->mButtonGradientBotFocused = {177, 21, 8, 0};
	Menu::optionMenu.unfocusTabTheme->mButtonGradientTopPushed = {174, 17, 2, 0};
	Menu::optionMenu.unfocusTabTheme->mButtonGradientBotPushed = {177, 21, 8, 0};
	Menu::optionMenu.unfocusTabTheme->mBorderDark = {177, 21, 8, 255};
	Menu::optionMenu.unfocusTabTheme->mBorderLight = {177, 21, 8, 255};
	Menu::optionMenu.unfocusTabTheme->mBorderMedium = {177, 21, 8, 255};
	Menu::optionMenu.unfocusTabTheme->mDropShadow = {0, 0, 0, 0};
	
	
	Menu::optionMenu.buttonTheme = Menu::mainMenu.buttonTheme;

	/// dimensions and position
	int     posY = Menu::windowHeight / 2;
	int     textBoxHeight = (Menu::windowWidth > 800) ? 50 : 30;
	int     windowWidth = (Menu::windowWidth > 800) ? Menu::windowWidth / 3 : Menu::windowWidth / 4;
	windowWidth = (windowWidth > 450) ? 450 : windowWidth;
	int     offset = ((Menu::windowWidth / 2) - windowWidth) / 2;
	int     posX = (Menu::windowWidth / 2) - (windowWidth + offset);
	int     buttonHeight = (Menu::windowWidth > 800) ? 50 : 30;
	int     buttonWidth = (Menu::windowWidth > 800) ? Menu::windowWidth / 6 : Menu::windowWidth / 8;
	buttonWidth = (buttonWidth > 130) ? 130 : buttonWidth;
	int     buttonPosX = ((Menu::windowWidth / 2) - (buttonWidth * 2)) / 3;
	int     tabOffsetX = (posX / 3) + 20;
	int     windowOffsetX = 2 * tabOffsetX + buttonWidth;
	int     windowHeight = Menu::windowHeight / 3;
	int     sideButtonFontSize = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 20) / 900 : 20;

	int     tmp;
	Menu::optionMenu.screenButton = new nanogui::Button(_screen, "GRAPHICS");
	Menu::optionMenu.screenButton->setSize({buttonWidth, buttonHeight});
	Menu::optionMenu.screenButton->setPosition({tabOffsetX, posY});
	Menu::optionMenu.screenButton->setTheme(Menu::optionMenu.focusTabTheme);
	Menu::optionMenu.screenButton->setFontSize(sideButtonFontSize);
	Menu::optionMenu.screenButton->setCursor(nanogui::Cursor::Hand);
	Menu::optionMenu.screenButton->setCallback([]{
		Menu::optionMenu.showScreen();
		Menu::optionMenu.screenButton->setTheme(Menu::optionMenu.focusTabTheme);
		Menu::optionMenu.audioButton->setTheme(Menu::optionMenu.unfocusTabTheme);
		Menu::optionMenu.keyButton->setTheme(Menu::optionMenu.unfocusTabTheme);
	});

	tmp = tabOffsetX + buttonWidth + 30;
	Menu::optionMenu.audioButton = new nanogui::Button(_screen, "AUDIO");
	Menu::optionMenu.audioButton->setSize({buttonWidth, buttonHeight});
	Menu::optionMenu.audioButton->setPosition({tmp, posY});
	Menu::optionMenu.audioButton->setTheme(Menu::optionMenu.unfocusTabTheme);
	Menu::optionMenu.audioButton->setFontSize(sideButtonFontSize);
	Menu::optionMenu.audioButton->setCursor(nanogui::Cursor::Hand);
	Menu::optionMenu.audioButton->setCallback([]{
		Menu::optionMenu.showAudio();
		Menu::optionMenu.audioButton->setTheme(Menu::optionMenu.focusTabTheme);
		Menu::optionMenu.screenButton->setTheme(Menu::optionMenu.unfocusTabTheme);
		Menu::optionMenu.keyButton->setTheme(Menu::optionMenu.unfocusTabTheme);
	});

	tmp = tmp + buttonWidth + 30;
	Menu::optionMenu.keyButton = new nanogui::Button(_screen, "CONTROLS");
	Menu::optionMenu.keyButton->setSize({buttonWidth, buttonHeight});
	Menu::optionMenu.keyButton->setPosition({tmp, posY});
	Menu::optionMenu.keyButton->setTheme(Menu::optionMenu.unfocusTabTheme);
	Menu::optionMenu.keyButton->setFontSize(sideButtonFontSize);
	Menu::optionMenu.keyButton->setCursor(nanogui::Cursor::Hand);
	Menu::optionMenu.keyButton->setCallback([]{
		Menu::optionMenu.showKeyBindings();
		Menu::optionMenu.keyButton->setTheme(Menu::optionMenu.focusTabTheme);
		Menu::optionMenu.screenButton->setTheme(Menu::optionMenu.unfocusTabTheme);
		Menu::optionMenu.audioButton->setTheme(Menu::optionMenu.unfocusTabTheme);
	});

	nanogui::Theme *windowTheme = new nanogui::Theme(_screen->nvgContext());
	windowTheme->mWindowFillFocused = {237, 237, 237, 0};
	windowTheme->mWindowFillUnfocused = {237, 237, 237, 0};
	windowTheme->mBorderLight = {237, 237, 237, 0};
	windowTheme->mBorderMedium = {237, 237, 237, 0};
	windowTheme->mBorderDark = {237, 237, 237, 0};
	windowTheme->mTransparent = {237, 237, 237, 0};
	windowTheme->mDropShadow = {237, 237, 237, 0};
	windowTheme->mWindowDropShadowSize = 0;
	windowTheme->mTextColor = {20, 20, 20, 255};
	windowTheme->mButtonCornerRadius = 0;

	posY = posY + buttonHeight + 20;
	int winHeight = 30;
	int halfWinX =  windowWidth / 3;
	int panelArrowSizeX = halfWinX / 4;
	int panelFontSize = 25;

	/// create screen window
	Menu::optionMenu.screenWindow = new nanogui::Window(_screen, "");
	Menu::optionMenu.screenWindow->setSize({windowWidth, windowHeight});
	Menu::optionMenu.screenWindow->setPosition({tabOffsetX, posY});
	Menu::optionMenu.screenWindow->setTheme(windowTheme);

	/// resolution
	nanogui::Label  *resLabel = new nanogui::Label(Menu::optionMenu.screenWindow, "Resolution", "sans", panelFontSize);
	resLabel->setSize({halfWinX, winHeight});
	resLabel->setPosition({2, 0});

	std::vector<int>    resolutionData = Menu::tmpOptions.resolutionList[Menu::tmpOptions.resolutionIndex];
	std::string resStr = std::to_string(resolutionData[0]) + " x " + std::to_string(resolutionData[1]) + " " + std::to_string(resolutionData[2]) + "Hz";
	nanogui::TextBox *resolutionBox = new nanogui::TextBox(Menu::optionMenu.screenWindow, resStr);
	resolutionBox->setSize({panelArrowSizeX * 5, winHeight});
	resolutionBox->setPosition({halfWinX + panelArrowSizeX, 0});
	resolutionBox->setFontSize(panelFontSize - 5);
	resolutionBox->setEditable(false);

	nanogui::Button *leftChange = new nanogui::Button(Menu::optionMenu.screenWindow, "<");
	leftChange->setSize({panelArrowSizeX, winHeight});
	leftChange->setPosition({halfWinX, 0});
	leftChange->setFontSize(panelFontSize);
	leftChange->setTextColor({230, 230, 230, 255});
	leftChange->setCursor(nanogui::Cursor::Hand);
	leftChange->setCallback([resolutionBox]{
		if (Menu::tmpOptions.resolutionIndex == 0)
			return;
		Menu::tmpOptions.resolutionIndex -= 1;
		std::vector<int>    resolutionData1 = Menu::tmpOptions.resolutionList[Menu::tmpOptions.resolutionIndex];
		std::string resStr1 = std::to_string(resolutionData1[0]) + " x " + std::to_string(resolutionData1[1]) + " " + std::to_string(resolutionData1[2]) + "Hz";
		resolutionBox->setValue(resStr1);
	});

	nanogui::Button *rightChange = new nanogui::Button(Menu::optionMenu.screenWindow, ">");
	rightChange->setSize({panelArrowSizeX, winHeight});
	rightChange->setPosition({halfWinX + panelArrowSizeX * 6, 0});
	rightChange->setFontSize(panelFontSize);
	rightChange->setTextColor({230, 230, 230, 255});
	rightChange->setCursor(nanogui::Cursor::Hand);
	rightChange->setCallback([resolutionBox]{
		if (Menu::tmpOptions.resolutionIndex + 1 == Menu::tmpOptions.resolutionList.size())
			return;
		Menu::tmpOptions.resolutionIndex += 1;
		std::vector<int>    resolutionData1 = Menu::tmpOptions.resolutionList[Menu::tmpOptions.resolutionIndex];
		std::string resStr1 = std::to_string(resolutionData1[0]) + " x " + std::to_string(resolutionData1[1]) + " " + std::to_string(resolutionData1[2]) + "Hz";
		resolutionBox->setValue(resStr1);
	});

	/// fullscreen
	int winPosY = winHeight + 10;
	nanogui::Label  *fullScreenLabel = new nanogui::Label(Menu::optionMenu.screenWindow, "Full Screen", "sans", panelFontSize);
	fullScreenLabel->setSize({halfWinX, winPosY});
	fullScreenLabel->setPosition({2, winPosY});

	nanogui::TextBox *fullScreenBox = new nanogui::TextBox(Menu::optionMenu.screenWindow, (Menu::tmpOptions.fullScreen) ? "ENABLED" : "DISABLED");
	fullScreenBox->setSize({panelArrowSizeX * 5, winHeight});
	fullScreenBox->setPosition({halfWinX + panelArrowSizeX, winPosY});
	fullScreenBox->setFontSize(panelFontSize - 5);
	fullScreenBox->setEditable(false);

	nanogui::Button *leftChange1 = new nanogui::Button(Menu::optionMenu.screenWindow, "<");
	leftChange1->setSize({panelArrowSizeX, winHeight});
	leftChange1->setPosition({halfWinX, winPosY});
	leftChange1->setFontSize(panelFontSize);
	leftChange1->setTextColor({230, 230, 230, 255});
	leftChange1->setCursor(nanogui::Cursor::Hand);
	leftChange1->setCallback([fullScreenBox]{
		if (!Menu::tmpOptions.fullScreen)
			return;
		Menu::tmpOptions.fullScreen = false;
		fullScreenBox->setValue("DISABLED");
	});

	nanogui::Button *rightChange1 = new nanogui::Button(Menu::optionMenu.screenWindow, ">");
	rightChange1->setSize({panelArrowSizeX, winHeight});
	rightChange1->setPosition({halfWinX + panelArrowSizeX * 6, winPosY});
	rightChange1->setFontSize(panelFontSize);
	rightChange1->setTextColor({230, 230, 230, 255});
	rightChange1->setCursor(nanogui::Cursor::Hand);
	rightChange1->setCallback([fullScreenBox]{
		if (Menu::tmpOptions.fullScreen)
			return;
		Menu::tmpOptions.fullScreen = true;
		fullScreenBox->setValue("ENABLED");
	});

	/// create audio window
	Menu::optionMenu.audioWindow = new nanogui::Window(_screen, "");
	Menu::optionMenu.audioWindow->setSize({windowWidth, windowHeight});
	Menu::optionMenu.audioWindow->setPosition({tabOffsetX, posY});
	Menu::optionMenu.audioWindow->setTheme(windowTheme);

	/// music volume
	nanogui::Label  *musicLabel = new nanogui::Label(Menu::optionMenu.audioWindow, "Music Volume", "sans", panelFontSize);
	musicLabel->setSize({halfWinX, winHeight});
	musicLabel->setPosition({2, 0});

	nanogui::Slider *musicSlider = new nanogui::Slider(Menu::optionMenu.audioWindow);
	musicSlider->setSize({halfWinX * 2, winHeight});
	musicSlider->setPosition({halfWinX, 0});
	musicSlider->setHighlightColor({174, 17, 2, 255});
	musicSlider->setHighlightedRange({0.0f, Menu::tmpOptions.musicVolume});
	musicSlider->setRange({0.0f, 1.0f});
	musicSlider->setValue(Menu::tmpOptions.musicVolume);
	musicSlider->setCursor(nanogui::Cursor::Hand);
	musicSlider->setCallback([musicSlider](float value){
		musicSlider->setHighlightedRange({0.0f, value});
		Menu::tmpOptions.musicVolume = value;
	});

	/// sound volume
	nanogui::Label  *soundLabel = new nanogui::Label(Menu::optionMenu.audioWindow, "Sound Volume", "sans", panelFontSize);
	soundLabel->setSize({halfWinX, winHeight});
	soundLabel->setPosition({2, winPosY});

	nanogui::Slider *soundSlider = new nanogui::Slider(Menu::optionMenu.audioWindow);
	soundSlider->setSize({halfWinX * 2, winHeight});
	soundSlider->setPosition({halfWinX, winPosY});
	soundSlider->setHighlightColor({174, 17, 2, 255});
	soundSlider->setHighlightedRange({0.0f, Menu::tmpOptions.soundVolume});
	soundSlider->setRange({0.0f, 1.0f});
	soundSlider->setValue(Menu::tmpOptions.soundVolume);
	soundSlider->setCursor(nanogui::Cursor::Hand);
	soundSlider->setCallback([soundSlider](float value){
		soundSlider->setHighlightedRange({0.0f, value});
		Menu::tmpOptions.soundVolume = value;
	});


	/// mute
	nanogui::Label  *muteLabel = new nanogui::Label(Menu::optionMenu.audioWindow, "Mute", "sans", panelFontSize);
	muteLabel->setSize({halfWinX, winPosY});
	muteLabel->setPosition({2, winPosY + winHeight + 10});

	nanogui::CheckBox *muteBox = new nanogui::CheckBox(Menu::optionMenu.audioWindow, "");
	muteBox->setSize({panelArrowSizeX, winHeight});
	muteBox->setPosition({halfWinX + panelArrowSizeX, winPosY + winHeight + 10});
	muteBox->setChecked(Menu::tmpOptions.mute);
	muteBox->setCursor(nanogui::Cursor::Hand);
	muteBox->setCallback([](bool checked){
		Menu::tmpOptions.mute = checked;
	});

	/// apply button
	Menu::optionMenu.applyButton = new nanogui::Button(_screen, "Apply");
	Menu::optionMenu.applyButton->setSize({halfWinX, winHeight + 10});
	Menu::optionMenu.applyButton->setPosition({halfWinX, posY + (winHeight + 10) * 5});
	Menu::optionMenu.applyButton->setTheme(Menu::mainMenu.buttonTheme);
	Menu::optionMenu.applyButton->setCursor(nanogui::Cursor::Hand);
	Menu::optionMenu.applyButton->setCallback([]{
		copyOptions(Menu::options, Menu::tmpOptions);
		Menu::optionMenu.deleteMenu(Menu::activeMenu->_screen);
		Menu::activeMenu->createOptionsMenu();
		Menu::mainMenu.changeView(true);
		Menu::title->setCaption("MAIN MENU");
	});

	/// cancel button
	Menu::optionMenu.cancelButton = new nanogui::Button(_screen, "Cancel");
	Menu::optionMenu.cancelButton->setSize({halfWinX, winHeight + 10});
	Menu::optionMenu.cancelButton->setPosition({halfWinX * 2 + (halfWinX / 2), posY + (winHeight + 10) * 5});
	Menu::optionMenu.cancelButton->setTheme(Menu::mainMenu.buttonTheme);
	Menu::optionMenu.cancelButton->setCursor(nanogui::Cursor::Hand);
	Menu::optionMenu.cancelButton->setCallback([]{
		copyOptions(Menu::tmpOptions, Menu::options);
		Menu::optionMenu.deleteMenu(Menu::activeMenu->_screen);
		Menu::activeMenu->createOptionsMenu();
		Menu::mainMenu.changeView(true);
		Menu::title->setCaption("MAIN MENU");
	});

	Menu::optionMenu.changeView(false);
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