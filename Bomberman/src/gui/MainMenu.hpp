//
// Created by Angus BURROUGHS on 2017/08/28.
//

#ifndef GAME_MAINMENU_HPP
#define GAME_MAINMENU_HPP


#include <nanogui/screen.h>
#include <nanogui/nanogui.h>

class OptionsMenu {
public:
	OptionsMenu(GLFWwindow *glfwWindow);
	~OptionsMenu();

private:
	void _init(GLFWwindow *glfwWindow);

	nanogui::Screen *_screen;
	nanogui::FormHelper *_gui;
	nanogui::Window *_nanoguiWindow;

	GLFWwindow *_glfwWindow;
};


#endif //GAME_MAINMENU_HPP
