#pragma once

#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <nanogui/label.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/stackedwidget.h>
#include <nanogui/checkbox.h>
#include <nanogui/combobox.h>
#include <nanogui/common.h>
#include <nanogui/theme.h>
#include <nanogui/vscrollpanel.h>
#include <MainGame.hpp>
#include <scene.hpp>
#include <MainGame.hpp>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

struct  MainMenu
{
    nanogui::Button     *start = nullptr;
    nanogui::Button     *options = nullptr;
    nanogui::Button     *exit = nullptr;
    nanogui::Theme      *buttonTheme;

    MainMenu() = default;

    void    changeView(bool value){
        if (start != nullptr)
            start->setVisible(value);
        if (options != nullptr)
            options->setVisible(value);
        if (exit != nullptr)
            exit->setVisible(value);
    }
};

struct  StoryModeMenu
{
    nanogui::Button     *newGame = nullptr;
    nanogui::Button     *loadGame = nullptr;
    nanogui::Button     *back = nullptr;
    nanogui::Theme      *buttonTheme;

    StoryModeMenu() = default;

    void    changeView(bool value){
        if (newGame != nullptr)
            newGame->setVisible(value);
        if (loadGame != nullptr)
            loadGame->setVisible(value);
        if (back != nullptr)
            back->setVisible(value);
    }
};

struct  NewGameMenu
{
	nanogui::Theme      *buttonTheme;
	nanogui::Button     *createGame = nullptr;
	nanogui::Button     *cancel = nullptr;
	nanogui::Label      *textLabel = nullptr;
	nanogui::TextBox    *profileNameBox = nullptr;
	nanogui::CheckBox   *easy = nullptr;
	nanogui::CheckBox   *normal = nullptr;
	nanogui::CheckBox   *hard = nullptr;

	NewGameMenu() = default;

	void    changeView(bool value){
		if (createGame != nullptr)
			createGame->setVisible(value);
		if (cancel != nullptr)
			cancel->setVisible(value);
		if (profileNameBox != nullptr)
			profileNameBox->setVisible(value);
		if (textLabel != nullptr)
			textLabel->setVisible(value);
		if (easy != nullptr)
			easy->setVisible(value);
		if (normal != nullptr)
			normal->setVisible(value);
		if (hard != nullptr)
			hard->setVisible(value);
	}
};

struct  LoadGameMenu
{
	nanogui::Theme      *buttonTheme;
	nanogui::Button     *loadGame = nullptr;
	nanogui::Button     *cancel = nullptr;
	nanogui::VScrollPanel   *panel = nullptr;
	std::vector<std::string>    fileNames;

	LoadGameMenu() = default;

	void    changeView(bool value) {
		if (loadGame != nullptr)
			loadGame->setVisible(value);
		if (cancel != nullptr)
			cancel->setVisible(value);
		if (panel != nullptr)
			panel->setVisible(value);
	}

	void    destroy(){
		if (loadGame != nullptr)
			delete loadGame;
		if (cancel != nullptr)
			delete cancel;
		if (panel != nullptr)
			delete panel;
		fileNames.clear();
	}
};

struct  PauseMenu
{
	nanogui::Theme      *buttonTheme;
	nanogui::Window     *window = nullptr;
	nanogui::Button     *resume = nullptr;
	nanogui::Button     *save = nullptr;
	nanogui::Button     *load = nullptr;
	nanogui::Button     *quit = nullptr;
	nanogui::Label      *saveLabel = nullptr;
	nanogui::Window     *quitWindow = nullptr;
	nanogui::Button     *yes = nullptr;
	nanogui::Button     *no = nullptr;

	PauseMenu() = default;

	void    changeView(bool value){
		if (window != nullptr)
			window->setVisible(value);
	}
};

struct  EndGameMenu
{
	nanogui::Window     *window = nullptr;
	nanogui::Button     *yes = nullptr;
	nanogui::Button     *no = nullptr;

	EndGameMenu() = default;

	void    changeView(bool value){
		if (window != nullptr)
			window->setVisible(value);
	}
};

struct  Gui
{
	Zion::SquareSprite  *bombMan;
	Zion::SquareSprite  *enemy1;
	Zion::SquareSprite  *enemy2;
	Zion::SquareSprite  *timeBack;
	Zion::SquareSprite  *heart;
	Zion::SquareSprite  *speed;
	Zion::SquareSprite  *bomb;
	Zion::SquareSprite  *explode;
	Zion::SquareSprite  *heartCase;
	Zion::SquareSprite  *whiteBanner;
};

struct  OptionMenu
{
	nanogui::Theme      *buttonTheme;
	nanogui::Theme      *focusTabTheme;
	nanogui::Theme      *unfocusTabTheme;
	nanogui::Theme      *buttonPressedTheme;
	nanogui::Button     *screenButton = nullptr;
	nanogui::Button     *audioButton = nullptr;
	nanogui::Button     *keyButton = nullptr;
	nanogui::Window     *screenWindow = nullptr;
	nanogui::Window     *audioWindow = nullptr;
	nanogui::Window     *keyWindow = nullptr;
	nanogui::Window     *panelDown = nullptr;

	OptionMenu() = default;


	void    deleteMenu(nanogui::Screen *screen){
		if (screenButton != nullptr)
			screen->removeChild(screenButton);
		if (audioButton != nullptr)
			screen->removeChild(audioButton);
		if (keyButton != nullptr)
			screen->removeChild(keyButton);
		if (panelDown != nullptr)
			screen->removeChild(panelDown);
		if (screenWindow != nullptr)
			screen->removeChild(screenWindow);
		if (audioWindow != nullptr)
			screen->removeChild(audioWindow);
		if (keyWindow != nullptr)
			screen->removeChild(keyWindow);
	}

	void    changeView(bool value)
	{
		if (screenButton != nullptr)
			screenButton->setVisible(value);
		if (audioButton != nullptr)
			audioButton->setVisible(value);
		if (keyButton != nullptr)
			keyButton->setVisible(value);
		if (screenWindow != nullptr)
			screenWindow->setVisible(value);
		if (audioWindow != nullptr)
			audioWindow->setVisible(value);
		if (keyWindow != nullptr)
			keyWindow->setVisible(value);
		if (panelDown != nullptr)
			panelDown->setVisible(value);
	}

	void    showScreen()
	{
		if (screenWindow != nullptr)
			screenWindow->setVisible(true);
		if (panelDown != nullptr)
			panelDown->setVisible(true);
		if (audioWindow != nullptr)
			audioWindow->setVisible(false);
		if (keyWindow != nullptr)
			keyWindow->setVisible(false);
	}

	void    showAudio()
	{
		if (audioWindow != nullptr)
			audioWindow->setVisible(true);
		if (panelDown != nullptr)
			panelDown->setVisible(true);
		if (screenWindow != nullptr)
			screenWindow->setVisible(false);
		if (keyWindow != nullptr)
			keyWindow->setVisible(false);
	}

	void    showKeyBindings()
	{
		if (keyWindow != nullptr)
			keyWindow->setVisible(true);
		if (panelDown != nullptr)
			panelDown->setVisible(true);
		if (screenWindow != nullptr)
			screenWindow->setVisible(false);
		if (audioWindow != nullptr)
			audioWindow->setVisible(false);
	}
};

struct  Options
{
	float   musicVolume;
	float   soundVolume;
	bool    mute;
	int     resolutionIndex;
	bool    fullScreen;
	std::map<int, std::vector<int>> resolutionList;
};