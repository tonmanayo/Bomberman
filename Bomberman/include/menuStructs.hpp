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
	/// todo: delete previous loadGameMenu state to free memory
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
	Zion::SquareSprite  *heart;
	Zion::SquareSprite  *speed;
	Zion::SquareSprite  *bomb;
	Zion::SquareSprite  *explode;
	Zion::SquareSprite  *heartCase;
	Zion::SquareSprite  *whiteBanner;
};

struct  Options
{
	int     volume;
	int     resolutionIndex;
	bool    mute;
	bool    fullScreen;
	std::vector<std::string>    resolutionList;
};