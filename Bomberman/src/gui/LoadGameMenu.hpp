//
// Created by Angus BURROUGHS on 2017/08/28.
//

#ifndef GAME_LOADGAMEMENU_HPP
#define GAME_LOADGAMEMENU_HPP


#include <nanogui/screen.h>
#include <nanogui/nanogui.h>

class LoadGameMenu {
public:
	LoadGameMenu(GLFWwindow *glfwWindow);
	~LoadGameMenu();

private:
	void _init(GLFWwindow *glfwWindow);

	nanogui::Screen *_screen;
	nanogui::FormHelper *_gui;
	nanogui::Window *_nanoguiWindow;

	GLFWwindow *_glfwWindow;
};


#endif //GAME_LOADGAMEMENU_HPP
