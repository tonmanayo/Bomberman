//
// Created by Angus BURROUGHS on 2017/08/28.
//

#include "MainMenu.hpp"

nanogui::Screen *screenPtr;

OptionsMenu::OptionsMenu(GLFWwindow *glfwWindow) {
	_init(glfwWindow);
}

OptionsMenu::~OptionsMenu() {

}

void OptionsMenu::_init(GLFWwindow *glfwWindow) {
	this->_glfwWindow = glfwWindow;

	screenPtr = this->_screen;
	this->_screen = new nanogui::Screen();
	this->_screen->initialize(glfwWindow, true);

	this->_gui = new nanogui::FormHelper(this->_screen);
	this->_nanoguiWindow = this->_gui->addWindow(Eigen::Vector2i(10, 10), "BomberMan");

	this->_gui->addButton("New Game", []() { std::cout << "New Game pressed." << std::endl; });
	this->_gui->addButton("Load Game", []() { std::cout << "Load Game pressed." << std::endl; });
	this->_gui->addButton("Options", []() { std::cout << "Options pressed." << std::endl; });
	this->_gui->addButton("Extras", []() { std::cout << "Extras pressed." << std::endl; });
	this->_gui->addButton("Quit", []() { std::cout << "Quit pressed." << std::endl; });

	this->_screen->setVisible(true);
	this->_screen->performLayout();
	this->_nanoguiWindow->center();

	glfwSetWindowUserPointer(this->_glfwWindow, this);


	glfwSetCursorPosCallback(this->_glfwWindow,
							 [](GLFWwindow *, double x, double y) {
								 screenPtr->cursorPosCallbackEvent(x, y);
							 }
	);

	glfwSetMouseButtonCallback(this->_glfwWindow,
							   [](GLFWwindow *, int button, int action, int modifiers) {
								   screenPtr->mouseButtonCallbackEvent(button, action, modifiers);
							   }
	);

	glfwSetKeyCallback(this->_glfwWindow,
					   [](GLFWwindow *, int key, int scancode, int action, int mods) {
						   screenPtr->keyCallbackEvent(key, scancode, action, mods);
					   }
	);

	glfwSetCharCallback(this->_glfwWindow,
						[](GLFWwindow *, unsigned int codepoint) {
							screenPtr->charCallbackEvent(codepoint);
						}
	);

	glfwSetDropCallback(this->_glfwWindow,
						[](GLFWwindow *, int count, const char **filenames) {
							screenPtr->dropCallbackEvent(count, filenames);
						}
	);

	glfwSetScrollCallback(this->_glfwWindow,
						  [](GLFWwindow *, double x, double y) {
							  screenPtr->scrollCallbackEvent(x, y);
						  }
	);

	glfwSetFramebufferSizeCallback(this->_glfwWindow,
								   [](GLFWwindow *, int width, int height) {
									   screenPtr->resizeCallbackEvent(width, height);
								   }
	);


}
