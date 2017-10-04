#include <menu.hpp>

void Menu::createMainMenu()
{
	Menu::mainMenu.buttonTheme = new nanogui::Theme(_screen->nvgContext());
	Menu::mainMenu.buttonTheme->mButtonFontSize = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 25) / 1100 : 25;
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

	/// Enter profile name label
	int profileFontSize = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 30) / 1000 : 30;
	Menu::newGameMenu.textLabel = new nanogui::Label(_screen, "Enter Profile Name", "sans", profileFontSize);
	Menu::newGameMenu.textLabel->setSize({textBoxWidth, textBoxHeight});
	int tempOffset = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 80) / 1280 : 80;
	Menu::newGameMenu.textLabel->setPosition({posX + tempOffset, posY});
	Menu::newGameMenu.textLabel->setColor({50, 50, 50, 255});

	/// profile name text input box
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

	auto  *tmp = new nanogui::Theme(_screen->nvgContext());
	tmp->mFontBold = 1;
	tmp->mFontNormal = 0;
	tmp->mStandardFontSize = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 25) / 1200 : 25;
	tmp->mTextColor = {40, 40, 40, 255};

	/// easy difficulty checkbox
	Menu::newGameMenu.easy = new nanogui::CheckBox(_screen, "    Easy");
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

	/// normal difficulty checkbox
	otherPosX += otherButtonWidth;
	Menu::newGameMenu.normal = new nanogui::CheckBox(_screen, "    Normal");
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

	/// hard difficulty checkbox
	otherPosX += otherButtonWidth + 10;
	Menu::newGameMenu.hard = new nanogui::CheckBox(_screen, "    Hard");
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
	/// create new game button
	Menu::newGameMenu.createGame = new nanogui::Button(_screen, "Create Game");
	Menu::newGameMenu.createGame->setSize({buttonWidth, buttonHeight});
	Menu::newGameMenu.createGame->setPosition({buttonPosX, posY});
	Menu::newGameMenu.createGame->setTheme(Menu::newGameMenu.buttonTheme);
	Menu::newGameMenu.createGame->setCallback([]{
		if (Menu::newGameMenu.profileNameBox->value().empty())
			return;
		for (std::string& fileName : Menu::loadGameMenu.fileNames)
		{
			if (fileName == Menu::newGameMenu.profileNameBox->value())
			{

			}
		}
		MainGame::soundEngine->stopAllSounds();
		Menu::newGameMenu.changeView(false);
		Menu::title->setVisible(false);
		Menu::pauseMenu.changeView(true);
		Menu::createNewGame(1, Menu::difficulty, Menu::newGameMenu.profileNameBox->value(), 0, 0, 0, 0);
		Menu::newGameMenu.profileNameBox->setValue("");
		Menu::loadSaveDirectory();
	});
	/// cancel new game button
	Menu::newGameMenu.cancel = new nanogui::Button(_screen, "Cancel");
	Menu::newGameMenu.cancel->setSize({buttonWidth, buttonHeight});
	Menu::newGameMenu.cancel->setPosition({buttonPosX + buttonWidth + buttonPosX, posY});
	Menu::newGameMenu.cancel->setTheme(Menu::newGameMenu.buttonTheme);
	Menu::newGameMenu.cancel->setCallback([]{
		Menu::storyModeMenu.changeView(true);
		Menu::newGameMenu.changeView(false);
		Menu::title->setCaption("CAMPAIGN");
	});

	/// name already used
	Menu::newGameMenu.nameUsed = new nanogui::Label(_screen, "Profile name already used", "sans", profileFontSize);
	Menu::newGameMenu.nameUsed->setSize({textBoxWidth + 15, textBoxHeight});
	Menu::newGameMenu.nameUsed->setPosition({buttonPosX + (buttonWidth / 2), posY + buttonHeight + 10});
	Menu::newGameMenu.nameUsed->setColor({230, 0, 0, 255});
	//Menu::newGameMenu.nameUsed->setVisible(false);

	Menu::newGameMenu.changeView(false);
}

void Menu::createPauseGameMenu()
{
	/// window theme
	auto  *winTheme = new nanogui::Theme(_screen->nvgContext());
	winTheme->mWindowCornerRadius = 0;
	winTheme->mWindowFillFocused = {30, 30, 30, 255};
	winTheme->mWindowFillUnfocused = {30, 30, 30, 255};
	winTheme->mStandardFontSize = (Menu::windowWidth / 1600) * 30;
	winTheme->mButtonFontSize = (Menu::windowWidth <= 800) ? 15 : 30;

	/// dimensions and positions for buttons
	int     posY = Menu::windowHeight / 3;
	int     winSizeX = Menu::windowWidth / 5;
	winSizeX = (winSizeX > 350) ? 350 : winSizeX;
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
		MainGame::soundEngine->setAllSoundsPaused(false);
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
		Menu::playMenuMusic();
		Zion::ParticleMaster::clearAll();
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
	auto  *winTheme = new nanogui::Theme(_screen->nvgContext());
	winTheme->mWindowCornerRadius = 0;
	winTheme->mWindowFillFocused = {30, 30, 30, 255};
	winTheme->mWindowFillUnfocused = {30, 30, 30, 255};

	/// dimensions and positions for buttons
	int     winSizeX = Menu::windowWidth / 5;
	winSizeX = (winSizeX < 350) ? 350 : winSizeX;

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
		Zion::ParticleMaster::clearAll();
		destroyGame();
		Menu::endGameMenu.changeView(false);
		Menu::mainMenu.changeView(false);
		Menu::pauseMenu.changeView(true);
		createNewGame(level, difficulty, activeMenu->_saveFileName, 0, 0, 0, 0);
		Menu::textStartTime = 0;
	});

	/// no button
	Menu::endGameMenu.no = new nanogui::Button(Menu::endGameMenu.window, "No");
	Menu::endGameMenu.no->setSize({quitWinOffset, 30});
	Menu::endGameMenu.no->setPosition({2 * padding + quitWinOffset, 35});
	Menu::endGameMenu.no->setCallback([]{
		Zion::ParticleMaster::clearAll();
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

	/// fileNames panel
	auto  *panelTheme = new nanogui::Theme(_screen->nvgContext());
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
			MainGame::soundEngine->stopAllSounds();
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
	int     windowWidth = Menu::windowWidth / 3;
	windowWidth = (windowWidth > 450) ? 450 : windowWidth;
	int     offset = ((Menu::windowWidth / 2) - windowWidth) / 2;
	int     posX = (Menu::windowWidth / 2) - (windowWidth + offset);
	int     buttonHeight = (Menu::windowHeight > 900) ? 50 : 30;
	int     buttonWidth = (Menu::windowWidth > 800) ? Menu::windowWidth / 6 : Menu::windowWidth / 8;
	buttonWidth = (buttonWidth > 130) ? 130 : buttonWidth;
	int     tabOffsetX = (posX / 3);
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

	tmp = tabOffsetX + buttonWidth + ((Menu::windowWidth < 1280) ? (Menu::windowWidth * 30) / 1280 : 30);
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

	tmp = tmp + buttonWidth + ((Menu::windowWidth < 1280) ? (Menu::windowWidth * 30) / 1280 : 30);
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

	auto *windowTheme = new nanogui::Theme(_screen->nvgContext());
	windowTheme->mWindowFillFocused = {20, 20, 20, 0};
	windowTheme->mWindowFillUnfocused = {20, 20, 20, 0};
	windowTheme->mBorderLight = {237, 237, 237, 0};
	windowTheme->mBorderMedium = {237, 237, 237, 0};
	windowTheme->mBorderDark = {237, 237, 237, 0};
	windowTheme->mTransparent = {237, 237, 237, 0};
	windowTheme->mDropShadow = {237, 237, 237, 0};
	windowTheme->mWindowDropShadowSize = 0;
	windowTheme->mTextColor = {20, 20, 20, 255};
	windowTheme->mButtonCornerRadius = 0;

	posY = posY + buttonHeight + 20;
	int winHeight = (Menu::windowHeight < 900) ? (Menu::windowHeight * 30) / 900 : 30;
	int panelFontSize = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 30) / 1100 : 30;

	int newWinSizeX = (tmp - tabOffsetX) + buttonWidth;
	int midOffset = newWinSizeX / 2;
	int halfWinX =  newWinSizeX / 4;
	int panelArrowSizeX = halfWinX / 3;

	/// create screen window
	Menu::optionMenu.screenWindow = new nanogui::Window(_screen, "");
	Menu::optionMenu.screenWindow->setSize({newWinSizeX + panelArrowSizeX, (winHeight + 10) * 5});
	Menu::optionMenu.screenWindow->setPosition({tabOffsetX, posY});
	Menu::optionMenu.screenWindow->setTheme(windowTheme);

	/// resolution
	nanogui::Label  *resLabel = new nanogui::Label(Menu::optionMenu.screenWindow, "Resolution", "sans", panelFontSize);
	resLabel->setSize({midOffset, winHeight});
	resLabel->setPosition({0, 0});

	std::vector<int>    resolutionData = Menu::tmpOptions.resolutionList[Menu::tmpOptions.resolutionIndex];
	std::string resStr = std::to_string(resolutionData[0]) + " x " + std::to_string(resolutionData[1]) + " " + std::to_string(resolutionData[2]) + "Hz";
	auto *resolutionBox = new nanogui::TextBox(Menu::optionMenu.screenWindow, resStr);
	resolutionBox->setSize({panelArrowSizeX * 5, winHeight});
	resolutionBox->setPosition({midOffset + panelArrowSizeX, 0});
	resolutionBox->setFontSize(panelFontSize - ((Menu::windowWidth < 1280) ? (Menu::windowWidth * 5) / 1100 : 5));
	resolutionBox->setEditable(false);

	nanogui::Button *leftChange = new nanogui::Button(Menu::optionMenu.screenWindow, "<");
	leftChange->setSize({panelArrowSizeX, winHeight});
	leftChange->setPosition({midOffset, 0});
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
	rightChange->setPosition({midOffset + panelArrowSizeX * 6, 0});
	rightChange->setFontSize(panelFontSize);
	rightChange->setTextColor({230, 230, 230, 255});
	rightChange->setCursor(nanogui::Cursor::Hand);
	rightChange->setCallback([resolutionBox]{
		if (Menu::tmpOptions.resolutionIndex + 1 == (int)Menu::tmpOptions.resolutionList.size())
			return;
		Menu::tmpOptions.resolutionIndex += 1;
		std::vector<int>    resolutionData1 = Menu::tmpOptions.resolutionList[Menu::tmpOptions.resolutionIndex];
		std::string resStr1 = std::to_string(resolutionData1[0]) + " x " + std::to_string(resolutionData1[1]) + " " + std::to_string(resolutionData1[2]) + "Hz";
		resolutionBox->setValue(resStr1);
	});

	/// fullScreen
	int winPosY = winHeight + 10;
	nanogui::Label  *fullScreenLabel = new nanogui::Label(Menu::optionMenu.screenWindow, "Full Screen", "sans", panelFontSize);
	fullScreenLabel->setSize({midOffset, winPosY});
	fullScreenLabel->setPosition({0, winPosY});

	nanogui::TextBox *fullScreenBox = new nanogui::TextBox(Menu::optionMenu.screenWindow, (Menu::tmpOptions.fullScreen) ? "ENABLED" : "DISABLED");
	fullScreenBox->setSize({panelArrowSizeX * 5, winHeight});
	fullScreenBox->setPosition({midOffset + panelArrowSizeX, winPosY});
	fullScreenBox->setFontSize(panelFontSize - 5);
	fullScreenBox->setEditable(false);

	nanogui::Button *leftChange1 = new nanogui::Button(Menu::optionMenu.screenWindow, "<");
	leftChange1->setSize({panelArrowSizeX, winHeight});
	leftChange1->setPosition({midOffset, winPosY});
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
	rightChange1->setPosition({midOffset + panelArrowSizeX * 6, winPosY});
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
	Menu::optionMenu.audioWindow->setSize({newWinSizeX + panelArrowSizeX, (winHeight + 10) * 5});
	Menu::optionMenu.audioWindow->setPosition({tabOffsetX, posY});
	Menu::optionMenu.audioWindow->setTheme(windowTheme);

	/// music volume
	nanogui::Label  *musicLabel = new nanogui::Label(Menu::optionMenu.audioWindow, "Music Volume", "sans", panelFontSize);
	musicLabel->setSize({midOffset, winHeight});
	musicLabel->setPosition({2, 0});

	auto *musicSlider = new nanogui::Slider(Menu::optionMenu.audioWindow);
	musicSlider->setSize({halfWinX * 2, winHeight});
	musicSlider->setPosition({midOffset, 0});
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
	soundLabel->setSize({midOffset, winHeight});
	soundLabel->setPosition({2, winPosY});

	auto *soundSlider = new nanogui::Slider(Menu::optionMenu.audioWindow);
	soundSlider->setSize({halfWinX * 2, winHeight});
	soundSlider->setPosition({midOffset, winPosY});
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
	muteLabel->setSize({midOffset, winPosY});
	muteLabel->setPosition({2, winPosY + winHeight + 10});

	nanogui::CheckBox *muteBox = new nanogui::CheckBox(Menu::optionMenu.audioWindow, "");
	muteBox->setSize({panelArrowSizeX, winHeight});
	muteBox->setPosition({midOffset + panelArrowSizeX, winPosY + winHeight + 10});
	muteBox->setChecked(Menu::tmpOptions.mute);
	muteBox->setCursor(nanogui::Cursor::Hand);
	muteBox->setCallback([](bool checked){
		Menu::tmpOptions.mute = checked;
	});

	/// create panel window
	Menu::optionMenu.panelDown = new nanogui::Window(_screen, "");
	Menu::optionMenu.panelDown->setSize({windowWidth, windowHeight});
	Menu::optionMenu.panelDown->setPosition({tabOffsetX, posY + (winHeight + 10) * 6});
	Menu::optionMenu.panelDown->setTheme(windowTheme);

	/// apply button
	nanogui::Button *applyButton = new nanogui::Button(Menu::optionMenu.panelDown, "Apply");
	applyButton->setSize({halfWinX, winHeight + 10});
	applyButton->setPosition({0, 0});
	applyButton->setTheme(Menu::mainMenu.buttonTheme);
	applyButton->setCursor(nanogui::Cursor::Hand);
	applyButton->setCallback([]{
		copyOptions(Menu::options, Menu::tmpOptions);
		Menu::optionMenu.changeView(false);
		Menu::title->setVisible(false);
		Menu::updateGraphicOptions();
		Menu::updateSoundOptions();
		Menu::activeMenu->createMainMenu();
		Menu::activeMenu->createOptionsMenu();
		Menu::activeMenu->createStoryMenu();
		Menu::activeMenu->createExitWindow();
		Menu::activeMenu->createNewGameMenu();
		Menu::activeMenu->createLoadGameMenu();
		Menu::activeMenu->createPauseGameMenu();
		Menu::activeMenu->createEndGameMenu();
		Menu::mainMenu.changeView(false);
		Menu::optionMenu.changeView(true);
		Menu::optionMenu.showScreen();
		Menu::title->setCaption("OPTIONS");
		Menu::saveOptionsConfig();
		Menu::playMenuMusic();
	});

	/// cancel button
	nanogui::Button *cancelButton = new nanogui::Button(Menu::optionMenu.panelDown, "Back");
	cancelButton->setSize({halfWinX, winHeight + 10});
	cancelButton->setPosition({windowWidth / 2, 0});
	cancelButton->setTheme(Menu::mainMenu.buttonTheme);
	cancelButton->setCursor(nanogui::Cursor::Hand);
	cancelButton->setCallback([]{
		copyOptions(Menu::tmpOptions, Menu::options);
		Menu::optionMenu.changeView(false);
		Menu::title->setVisible(false);
		Menu::activeMenu->createMainMenu();
		Menu::activeMenu->createOptionsMenu();
		Menu::activeMenu->createStoryMenu();
		Menu::activeMenu->createExitWindow();
		Menu::activeMenu->createNewGameMenu();
		Menu::activeMenu->createLoadGameMenu();
		Menu::activeMenu->createPauseGameMenu();
		Menu::activeMenu->createEndGameMenu();
	});

	/// create press key window
	auto *pressKey = new nanogui::Theme(_screen->nvgContext());
	pressKey->mWindowFillFocused = {20, 20, 20, 200};
	pressKey->mWindowFillUnfocused = {20, 20, 20, 200};
	pressKey->mBorderLight = {20, 20, 20, 200};
	pressKey->mBorderMedium = {20, 20, 20, 200};
	pressKey->mBorderDark = {20, 20, 20, 200};
	pressKey->mTransparent = {20, 20, 20, 200};
	pressKey->mDropShadow = {20, 20, 20, 200};

	int labelFontSize = (Menu::windowWidth < 1280) ? (Menu::windowWidth * 30) / 1000 : 30;
	int labelPositionX = Menu::windowWidth / 3;
	int labelPositionY = (Menu::windowHeight / 2) - 20;

	Menu::optionMenu.keyBindWindow = new nanogui::Window(_screen, "");
	Menu::optionMenu.keyBindWindow->setSize({Menu::windowWidth, Menu::windowHeight});
	Menu::optionMenu.keyBindWindow->setPosition({0, 0});
	Menu::optionMenu.keyBindWindow->setVisible(false);
	Menu::optionMenu.keyBindWindow->setTheme(pressKey);

	nanogui::Label  *keyBindLabel = new nanogui::Label(Menu::optionMenu.keyBindWindow, "Press an unused key to map to this action", "sans-bold", labelFontSize);
	keyBindLabel->setSize({450, 40});
	keyBindLabel->setPosition({labelPositionX, labelPositionY});

	/// create key binding windows
	Menu::optionMenu.keyWindow = new nanogui::Window(_screen, "");
	Menu::optionMenu.keyWindow->setSize({newWinSizeX + panelArrowSizeX, (winHeight + 10) * 6});
	Menu::optionMenu.keyWindow->setPosition({tabOffsetX, posY});
	Menu::optionMenu.keyWindow->setTheme(windowTheme);

	/// move up key
	nanogui::Label  *upLabel = new nanogui::Label(Menu::optionMenu.keyWindow, "Move Up", "sans", panelFontSize);
	upLabel->setSize({midOffset, winHeight});
	upLabel->setPosition({2, 0});

	auto *upKey = new nanogui::Button(Menu::optionMenu.keyWindow, Menu::tmpOptions.moveUp.name);
	upKey->setSize({panelArrowSizeX * 5, winHeight});
	upKey->setPosition({midOffset, 0});
	upKey->setFontSize(panelFontSize);
	upKey->setTextColor({230, 230, 230, 255});
	upKey->setCursor(nanogui::Cursor::Hand);
	upKey->setCallback([]{
		Menu::optionMenu.keyBindWindow->setVisible(true);
		Menu::isKeyBind = true;
		Menu::keyBind = "moveUp";
	});

	/// move down key
	nanogui::Label  *downLabel = new nanogui::Label(Menu::optionMenu.keyWindow, "Move Down", "sans", panelFontSize);
	downLabel->setSize({midOffset, winHeight});
	downLabel->setPosition({2, winPosY});

	auto *downKey = new nanogui::Button(Menu::optionMenu.keyWindow, Menu::tmpOptions.moveDown.name);
	downKey->setSize({panelArrowSizeX * 5, winHeight});
	downKey->setPosition({midOffset, winPosY});
	downKey->setFontSize(panelFontSize);
	downKey->setTextColor({230, 230, 230, 255});
	downKey->setCursor(nanogui::Cursor::Hand);
	downKey->setCallback([]{
		Menu::optionMenu.keyBindWindow->setVisible(true);
		Menu::isKeyBind = true;
		Menu::keyBind = "moveDown";
	});

	winPosY = winPosY + winHeight + 10;
	/// move left key
	nanogui::Label  *leftLabel = new nanogui::Label(Menu::optionMenu.keyWindow, "Move Left", "sans", panelFontSize);
	leftLabel->setSize({midOffset, winHeight});
	leftLabel->setPosition({2, winPosY});

	auto *leftKey = new nanogui::Button(Menu::optionMenu.keyWindow, Menu::tmpOptions.moveLeft.name);
	leftKey->setSize({panelArrowSizeX * 5, winHeight});
	leftKey->setPosition({midOffset, winPosY});
	leftKey->setFontSize(panelFontSize);
	leftKey->setTextColor({230, 230, 230, 255});
	leftKey->setCursor(nanogui::Cursor::Hand);
	leftKey->setCallback([]{
		Menu::optionMenu.keyBindWindow->setVisible(true);
		Menu::isKeyBind = true;
		Menu::keyBind = "moveLeft";
	});

	winPosY = winPosY + winHeight + 10;
	/// move right key
	nanogui::Label  *rightLabel = new nanogui::Label(Menu::optionMenu.keyWindow, "Move Right", "sans", panelFontSize);
	rightLabel->setSize({midOffset, winHeight});
	rightLabel->setPosition({2, winPosY});

	auto *rightKey = new nanogui::Button(Menu::optionMenu.keyWindow, Menu::tmpOptions.moveRight.name);
	rightKey->setSize({panelArrowSizeX * 5, winHeight});
	rightKey->setPosition({midOffset, winPosY});
	rightKey->setFontSize(panelFontSize);
	rightKey->setTextColor({230, 230, 230, 255});
	rightKey->setCursor(nanogui::Cursor::Hand);
	rightKey->setCallback([]{
		Menu::optionMenu.keyBindWindow->setVisible(true);
		Menu::isKeyBind = true;
		Menu::keyBind = "moveRight";
	});

	winPosY = winPosY + winHeight + 10;
	/// move place bomb key
	nanogui::Label  *placeBombLabel = new nanogui::Label(Menu::optionMenu.keyWindow, "Place Bomb", "sans", panelFontSize);
	placeBombLabel->setSize({midOffset, winHeight});
	placeBombLabel->setPosition({2, winPosY});

	auto *placeBombKey = new nanogui::Button(Menu::optionMenu.keyWindow, Menu::tmpOptions.placeBomb.name);
	placeBombKey->setSize({panelArrowSizeX * 5, winHeight});
	placeBombKey->setPosition({midOffset, winPosY});
	placeBombKey->setFontSize(panelFontSize);
	placeBombKey->setTextColor({230, 230, 230, 255});
	placeBombKey->setCursor(nanogui::Cursor::Hand);
	placeBombKey->setCallback([]{
		Menu::optionMenu.keyBindWindow->setVisible(true);
		Menu::isKeyBind = true;
		Menu::keyBind = "placeBomb";
	});

	winPosY = winPosY + winHeight + 10;
	/// move pause key
	nanogui::Label  *pauseLabel = new nanogui::Label(Menu::optionMenu.keyWindow, "Pause", "sans", panelFontSize);
	pauseLabel->setSize({midOffset, winHeight});
	pauseLabel->setPosition({2, winPosY});

	auto *pauseKey = new nanogui::Button(Menu::optionMenu.keyWindow, Menu::tmpOptions.pause.name);
	pauseKey->setSize({panelArrowSizeX * 5, winHeight});
	pauseKey->setPosition({midOffset, winPosY});
	pauseKey->setFontSize(panelFontSize);
	pauseKey->setTextColor({230, 230, 230, 255});
	pauseKey->setCursor(nanogui::Cursor::Hand);
	pauseKey->setCallback([]{
		Menu::optionMenu.keyBindWindow->setVisible(true);
		Menu::isKeyBind = true;
		Menu::keyBind = "pause";
	});


	Menu::optionMenu.upKey = upKey;
	Menu::optionMenu.downKey = downKey;
	Menu::optionMenu.leftKey = leftKey;
	Menu::optionMenu.rightKey = rightKey;
	Menu::optionMenu.placeBombKey = placeBombKey;
	Menu::optionMenu.pauseKey = pauseKey;

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
	Menu::gui.heart = new Zion::SquareSprite(*_mainGame->getShader("guiGame"), 0, 0, 0.05, 0.08);
	Menu::gui.heart->addTextureFromFile("resource/images/heart.png");
	/// adding speed
	Menu::gui.speed = new Zion::SquareSprite(*_mainGame->getShader("guiGame"), 0, 0, 0.2, 0.2);
	Menu::gui.speed->addTextureFromFile("resource/images/bolt.png");
	/// adding bomb logo
	Menu::gui.bomb = new Zion::SquareSprite(*_mainGame->getShader("guiGame"), 0, 0, 0.08, 0.13);
	Menu::gui.bomb->addTextureFromFile("resource/images/bomb.png");
	/// adding explode
	Menu::gui.explode = new Zion::SquareSprite(*_mainGame->getShader("guiGame"), 0, 0, 0.08, 0.12);
	Menu::gui.explode->addTextureFromFile("resource/images/explode.png");
	/// adding heart case
	Menu::gui.heartCase = new Zion::SquareSprite(*_mainGame->getShader("guiGame"), 0, 0, 0.5, 0.7);
	Menu::gui.heartCase->addTextureFromFile("resource/images/hudBg.png");
	/// add enemy2
	Menu::gui.enemy2 = new Zion::SquareSprite(*_mainGame->getShader("guiGame"), 0, 0, 0.08, 0.12);
	Menu::gui.enemy2->addTextureFromFile("resource/images/enemy2.png");
	/// add banner
	Menu::gui.whiteBanner = new Zion::SquareSprite(*_mainGame->getShader("guiGame"), 0, 0, 7.5, 0.5);
	Menu::gui.whiteBanner->addBaseColor({0.2, 0.2, 0.2, 0.5});
	/// add timeBanner
	Menu::gui.timeBack = new Zion::SquareSprite(*_mainGame->getShader("guiGame"), 0, 0, 0.4, 0.3);
	Menu::gui.timeBack->addTextureFromFile("resource/images/backTime.png");
}

void Menu::updateGraphicOptions()
{
	Menu::isFullScreen = Menu::options.fullScreen;
	Menu::windowWidth = Menu::options.resolutionList[Menu::options.resolutionIndex][0];
	Menu::windowHeight = Menu::options.resolutionList[Menu::options.resolutionIndex][1];

	if (Menu::isFullScreen){
		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		glfwSetWindowMonitor(Menu::activeMenu->_screen->glfwWindow(), monitor, 0, 0,
		Menu::windowWidth, Menu::windowHeight, Menu::options.resolutionList[Menu::options.resolutionIndex][2]);
	}else{
		glfwSetWindowMonitor(Menu::activeMenu->_screen->glfwWindow(), nullptr, 0, 0,
		Menu::windowWidth, Menu::windowHeight, Menu::options.resolutionList[Menu::options.resolutionIndex][2]);
	}
	/// fixing projection matrix
	delete MainGame::fontRenderer1;
	MainGame::fontRenderer1 = new Zion::TextRenderer(MainGame::game->getShader("text"), (GLuint)Menu::windowWidth, (GLuint)Menu::windowHeight);
	MainGame::fontRenderer1->loadFont("resource/fonts/angryBirds.ttf", 48);
	delete MainGame::fontRenderer2;
	MainGame::fontRenderer2 = new Zion::TextRenderer(MainGame::game->getShader("text"), (GLuint)Menu::windowWidth, (GLuint)Menu::windowHeight);
	MainGame::fontRenderer2->loadFont("resource/fonts/sansSerious.ttf", 48);
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(70.0f), (float)Menu::windowWidth / (float)Menu::windowHeight, 0.1f, 1000.0f);
	if (MainGame::game->getShader("basic") != nullptr)
		MainGame::game->getShader("basic")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	if (MainGame::game->getShader("particle") != nullptr)
		MainGame::game->getShader("particle")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	if (MainGame::game->getShader("fire") != nullptr)
		MainGame::game->getShader("fire")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	if (MainGame::game->getShader("gui") != nullptr)
		MainGame::game->getShader("gui")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	if (MainGame::game->getShader("guiGame") != nullptr)
		MainGame::game->getShader("guiGame")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	if (MainGame::game->getShader("anime") != nullptr)
		MainGame::game->getShader("anime")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	if (MainGame::game->getShader("animeNoJoint") != nullptr)
		MainGame::game->getShader("animeNoJoint")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
}

void Menu::updateSoundOptions()
{
	if (Menu::options.mute){
		MainGame::soundEngine->setSoundVolume(0);
	}else{
		MainGame::soundEngine->stopAllSounds();
		MainGame::soundEngine->setSoundVolume(1.0f);
		/// music
		Menu::activeMenu->_menuMusic->setDefaultVolume(Menu::options.musicVolume);
		Menu::activeMenu->_level1Song->setDefaultVolume(Menu::options.musicVolume);
		Menu::activeMenu->_level2Song->setDefaultVolume(Menu::options.musicVolume);
		Menu::activeMenu->_level3Song->setDefaultVolume(Menu::options.musicVolume);
		Menu::activeMenu->_level4Song->setDefaultVolume(Menu::options.musicVolume);
		Menu::activeMenu->_level5Song->setDefaultVolume(Menu::options.musicVolume);
		Menu::activeMenu->_level6Song->setDefaultVolume(Menu::options.musicVolume);
		/// sound
		Menu::activeMenu->_bombExplosionSound->setDefaultVolume(Menu::options.soundVolume);
		Menu::activeMenu->_bombPlacementSound->setDefaultVolume(Menu::options.soundVolume);
		Menu::activeMenu->_playerHurtSound->setDefaultVolume(Menu::options.soundVolume);
		Menu::activeMenu->_enemyHurtSound->setDefaultVolume(Menu::options.soundVolume);
		Menu::activeMenu->_playerWalkingSound->setDefaultVolume(Menu::options.soundVolume);
		Menu::activeMenu->_mapLevelUp->setDefaultVolume(Menu::options.soundVolume);
		Menu::activeMenu->_playPowerUp->setDefaultVolume(Menu::options.soundVolume);
		Menu::activeMenu->_playGameReady->setDefaultVolume(Menu::options.soundVolume);
		//MainGame::soundEngine->play2D(Menu::activeMenu->_menuMusic);
	}
}

bool Menu::isKeyAvailable(std::string &name, int keyValue)
{
	if (name != "moveUp" && keyValue == Menu::tmpOptions.moveUp.glfwValue)
		return false;
	if (name != "moveLeft" && keyValue == Menu::tmpOptions.moveLeft.glfwValue)
		return false;
	if (name != "moveRight" && keyValue == Menu::tmpOptions.moveRight.glfwValue)
		return false;
	if (name != "moveDown" && keyValue == Menu::tmpOptions.moveDown.glfwValue)
		return false;
	if (name != "placeBomb" && keyValue == Menu::tmpOptions.placeBomb.glfwValue)
		return false;
	if  (name != "pause" && keyValue == Menu::tmpOptions.pause.glfwValue)
		return false;
	return true;
}

void Menu::keyPressKeyBindings(int key)
{
	if (!Menu::isKeyAvailable(Menu::keyBind, key))
		return;
	Menu::isKeyBind = false;
	if (Menu::optionMenu.keyBindWindow != nullptr)
		Menu::optionMenu.keyBindWindow->setVisible(false);
	if (Menu::keyBind == "moveUp"){
		Menu::tmpOptions.moveUp.glfwValue = key;
		const char* key_name = glfwGetKeyName(key, 0);
		if (key_name == nullptr)
			Menu::myGlfwGetKeyName(key, Menu::tmpOptions.moveUp.name);
		else
			Menu::tmpOptions.moveUp.name = key_name;
		Menu::optionMenu.upKey->setCaption(Menu::tmpOptions.moveUp.name);
	}
	else if (Menu::keyBind == "moveDown"){
		Menu::tmpOptions.moveDown.glfwValue = key;
		const char* key_name = glfwGetKeyName(key, 0);
		if (key_name == nullptr)
			Menu::myGlfwGetKeyName(key, Menu::tmpOptions.moveDown.name);
		else
			Menu::tmpOptions.moveDown.name = key_name;
		Menu::optionMenu.downKey->setCaption(Menu::tmpOptions.moveDown.name);
	}
	else if (Menu::keyBind == "moveLeft"){
		Menu::tmpOptions.moveLeft.glfwValue = key;
		const char* key_name = glfwGetKeyName(key, 0);
		if (key_name == nullptr)
			Menu::myGlfwGetKeyName(key, Menu::tmpOptions.moveLeft.name);
		else
			Menu::tmpOptions.moveLeft.name = key_name;
		Menu::optionMenu.leftKey->setCaption(Menu::tmpOptions.moveLeft.name);
	}
	else if (Menu::keyBind == "moveRight"){
		Menu::tmpOptions.moveRight.glfwValue = key;
		const char* key_name = glfwGetKeyName(key, 0);
		if (key_name == nullptr)
			Menu::myGlfwGetKeyName(key, Menu::tmpOptions.moveRight.name);
		else
			Menu::tmpOptions.moveRight.name = key_name;
		Menu::optionMenu.rightKey->setCaption(Menu::tmpOptions.moveRight.name);
	}
	else if (Menu::keyBind == "placeBomb"){
		Menu::tmpOptions.placeBomb.glfwValue = key;
		const char* key_name = glfwGetKeyName(key, 0);
		if (key_name == nullptr)
			Menu::myGlfwGetKeyName(key, Menu::tmpOptions.placeBomb.name);
		else
			Menu::tmpOptions.placeBomb.name = key_name;
		Menu::optionMenu.placeBombKey->setCaption(Menu::tmpOptions.placeBomb.name);
	}
	else if (Menu::keyBind == "pause"){
		Menu::tmpOptions.pause.glfwValue = key;
		const char* key_name = glfwGetKeyName(key, 0);
		if (key_name == nullptr)
			Menu::myGlfwGetKeyName(key, Menu::tmpOptions.pause.name);
		else
			Menu::tmpOptions.pause.name = key_name;
		Menu::optionMenu.pauseKey->setCaption(Menu::tmpOptions.pause.name);
	}
}

void Menu::myGlfwGetKeyName(int key, std::string &dest)
{
	if (key == GLFW_KEY_UP)
		dest = "Up-Arrow";
	else if (key == GLFW_KEY_DOWN)
		dest = "Down-Arrow";
	else if (key == GLFW_KEY_LEFT)
		dest = "Left-Arrow";
	else if (key == GLFW_KEY_RIGHT)
		dest = "Right-Arrow";
	else if (key == GLFW_KEY_ESCAPE)
		dest = "Esc";
	else if (key == GLFW_KEY_SPACE)
		dest = "Space";
	else if (key == GLFW_KEY_KP_0)
		dest = "Num0";
	else if (key == GLFW_KEY_KP_1)
		dest = "Num1";
	else if (key == GLFW_KEY_KP_2)
		dest = "Num2";
	else if (key == GLFW_KEY_KP_3)
		dest = "Num3";
	else if (key == GLFW_KEY_KP_4)
		dest = "Num4";
	else if (key == GLFW_KEY_KP_5)
		dest = "Num5";
	else if (key == GLFW_KEY_KP_6)
		dest = "Num6";
	else if (key == GLFW_KEY_KP_7)
		dest = "Num7";
	else if (key == GLFW_KEY_KP_8)
		dest = "Num8";
	else if (key == GLFW_KEY_KP_9)
		dest = "Num9";
	else if (key == GLFW_KEY_ENTER)
		dest = "Enter";
	else if (key == GLFW_KEY_TAB)
		dest = "Tab";
}

