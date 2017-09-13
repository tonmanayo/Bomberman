#pragma once

#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <nanogui/label.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/checkbox.h>
#include <nanogui/combobox.h>
#include <nanogui/common.h>
#include <nanogui/theme.h>
#include <MainGame.hpp>
#include <scene.hpp>
#include <yaml-cpp/yaml.h>
#include <nanogui/vscrollpanel.h>
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