//
// Created by angus on 2017/09/02.
//

#include <nanogui/nanogui.h>
#include <window.hpp>
#include <yaml-cpp/yaml.h>
#include "Menu.h"

Menu::Global menuGlobal;

Menu::Menu() {

}

Menu::~Menu() {

}

void Menu::init(GLFWwindow *glfwWindow) {
    this->_glfwWindow = glfwWindow;

	menuGlobal.config = YAML::LoadFile("config.yaml");

    menuGlobal.screenPtr = this->_screen;
    this->_screen = new nanogui::Screen();
    this->_screen->initialize(glfwWindow, true);
}

nanogui::Screen *Menu::get_screen() const {
    return _screen;
}

void Menu::set_screen(nanogui::Screen *_screen) {
    Menu::_screen = _screen;
}

GLFWwindow *Menu::get_glfwWindow() const {
    return _glfwWindow;
}

void Menu::set_glfwWindow(GLFWwindow *_glfwWindow) {
    Menu::_glfwWindow = _glfwWindow;
}

void Menu::Loop(Zion::Window *window) {
    while (menuGlobal.menuState != MenuState::EXIT) {

        if (menuGlobal.menuState == MenuState::MAIN) {
            this->MainMenu();
            menuGlobal.loop = true;
        }
        else if (menuGlobal.menuState == MenuState::OPTIONS) {
            this->OptionsMenu();
            menuGlobal.loop = true;
        }
        else if (menuGlobal.menuState == MenuState::SOUND) {
            this->SoundMenu();
            menuGlobal.loop = true;
        }
        else if (menuGlobal.menuState == MenuState::GRAPHICS) {
            this->GraphicsMenu();
            menuGlobal.loop = true;
        }
        else if (menuGlobal.menuState == MenuState::CONTROLS) {
            this->ControlsMenu();
            menuGlobal.loop = true;
        }


        while (menuGlobal.loop)
        {
            window->clearWindow(0.0f, 0.0f, 0.0f, 1.0f);
    //      Draw nanogui
            menuGlobal.screenPtr->drawContents();
            menuGlobal.screenPtr->drawWidgets();


            window->updateWindow();
        }
    }
}

void Menu::MainMenu() {
    menuGlobal.menuState = MenuState::MAIN;

    this->_gui = new nanogui::FormHelper(this->_screen);
    this->_nanoguiWindow = this->_gui->addWindow(Eigen::Vector2i(10, 10), "BomberMan");

    this->_gui->addButton("New Game", []() {
        std::cout << "New Game pressed." << std::endl;
        menuGlobal.loop = false;
        menuGlobal.menuState = MenuState::NEW_GAME;
    });
    this->_gui->addButton("Load Game", []() {
        std::cout << "Load Game pressed." << std::endl;
        menuGlobal.loop = false;
        menuGlobal.menuState = MenuState::LOAD_GAME;
    });
    this->_gui->addButton("Options", []() {
        std::cout << "Options pressed." << std::endl;
        menuGlobal.loop = false;
        menuGlobal.menuState = MenuState::OPTIONS;
    });
    this->_gui->addButton("Quit", []() {
        std::cout << "Quit pressed." << std::endl;
        menuGlobal.loop = false;
        menuGlobal.menuState = MenuState::EXIT;
    });

    menuGlobal.screenPtr->setVisible(true);
    menuGlobal.screenPtr->performLayout();
    this->_nanoguiWindow->center();

    glfwSetWindowUserPointer(this->_glfwWindow, this);


    glfwSetCursorPosCallback(this->_glfwWindow,
                             [](GLFWwindow *, double x, double y) {
                                 menuGlobal.screenPtr->cursorPosCallbackEvent(x, y);
                             }
    );

    glfwSetMouseButtonCallback(this->_glfwWindow,
                               [](GLFWwindow *, int button, int action, int modifiers) {
                                   menuGlobal.screenPtr->mouseButtonCallbackEvent(button, action, modifiers);
                               }
    );

    glfwSetKeyCallback(this->_glfwWindow,
                       [](GLFWwindow *, int key, int scancode, int action, int mods) {
                           menuGlobal.screenPtr->keyCallbackEvent(key, scancode, action, mods);
                       }
    );

    glfwSetCharCallback(this->_glfwWindow,
                        [](GLFWwindow *, unsigned int codepoint) {
                            menuGlobal.screenPtr->charCallbackEvent(codepoint);
                        }
    );

    glfwSetDropCallback(this->_glfwWindow,
                        [](GLFWwindow *, int count, const char **filenames) {
                            menuGlobal.screenPtr->dropCallbackEvent(count, filenames);
                        }
    );

    glfwSetScrollCallback(this->_glfwWindow,
                          [](GLFWwindow *, double x, double y) {
                              menuGlobal.screenPtr->scrollCallbackEvent(x, y);
                          }
    );

    glfwSetFramebufferSizeCallback(this->_glfwWindow,
                                   [](GLFWwindow *, int width, int height) {
                                       menuGlobal.screenPtr->resizeCallbackEvent(width, height);
                                   }
    );
}

void Menu::OptionsMenu() {
    menuGlobal.menuState = MenuState::OPTIONS;

    this->_gui = new nanogui::FormHelper(this->_screen);
    this->_nanoguiWindow = this->_gui->addWindow(Eigen::Vector2i(10, 10), "Options");

    this->_gui->addButton("Graphics", []() {
        std::cout << "Graphics pressed." << std::endl;
        menuGlobal.loop = false;
        menuGlobal.menuState = MenuState::GRAPHICS;
    });
    this->_gui->addButton("Sound", []() {
        std::cout << "Sound pressed." << std::endl;
        menuGlobal.loop = false;
        menuGlobal.menuState = MenuState::SOUND;
    });
    this->_gui->addButton("Controls", []() {
        std::cout << "Controls pressed." << std::endl;
        menuGlobal.loop = false;
        menuGlobal.menuState = MenuState::SOUND;
    });
    this->_gui->addButton("Back", []() {
        std::cout << "Back pressed." << std::endl;
        menuGlobal.loop = false;
        menuGlobal.menuState = MenuState::MAIN;
    });

    menuGlobal.screenPtr->setVisible(true);
    menuGlobal.screenPtr->performLayout();
    this->_nanoguiWindow->center();

    glfwSetWindowUserPointer(this->_glfwWindow, this);


    glfwSetCursorPosCallback(this->_glfwWindow,
                             [](GLFWwindow *, double x, double y) {
                                 menuGlobal.screenPtr->cursorPosCallbackEvent(x, y);
                             }
    );

    glfwSetMouseButtonCallback(this->_glfwWindow,
                               [](GLFWwindow *, int button, int action, int modifiers) {
                                   menuGlobal.screenPtr->mouseButtonCallbackEvent(button, action, modifiers);
                               }
    );

    glfwSetKeyCallback(this->_glfwWindow,
                       [](GLFWwindow *, int key, int scancode, int action, int mods) {
                           menuGlobal.screenPtr->keyCallbackEvent(key, scancode, action, mods);
                       }
    );

    glfwSetCharCallback(this->_glfwWindow,
                        [](GLFWwindow *, unsigned int codepoint) {
                            menuGlobal.screenPtr->charCallbackEvent(codepoint);
                        }
    );

    glfwSetDropCallback(this->_glfwWindow,
                        [](GLFWwindow *, int count, const char **filenames) {
                            menuGlobal.screenPtr->dropCallbackEvent(count, filenames);
                        }
    );

    glfwSetScrollCallback(this->_glfwWindow,
                          [](GLFWwindow *, double x, double y) {
                              menuGlobal.screenPtr->scrollCallbackEvent(x, y);
                          }
    );

    glfwSetFramebufferSizeCallback(this->_glfwWindow,
                                   [](GLFWwindow *, int width, int height) {
                                       menuGlobal.screenPtr->resizeCallbackEvent(width, height);
                                   }
    );
}

void Menu::GraphicsMenu() {
    menuGlobal.menuState = MenuState::GRAPHICS;

    this->_gui = new nanogui::FormHelper(this->_screen);
    this->_nanoguiWindow = this->_gui->addWindow(Eigen::Vector2i(10, 10), "Graphics");

    std::vector<std::string> resList = menuGlobal.config["graphics"]["resolutionList"].as<std::vector<std::string>>();
    nanogui::ComboBox *resCoBo = new nanogui::ComboBox(this->_nanoguiWindow, resList);

	this->_gui->addButton("Apply", []() {
		std::cout << "Settings Applied" << std::endl;
		menuGlobal.config["graphics"]["resolution"] = menuGlobal.config["graphics"]["resolutionList"][resCoBo->selectedIndex()];
	});

    this->_gui->addButton("Back", []() {
        std::cout << "Back pressed." << std::endl;
        menuGlobal.loop = false;
        menuGlobal.menuState = MenuState::OPTIONS;
    });

    menuGlobal.screenPtr->setVisible(true);
    menuGlobal.screenPtr->performLayout();
    this->_nanoguiWindow->center();

    glfwSetWindowUserPointer(this->_glfwWindow, this);


    glfwSetCursorPosCallback(this->_glfwWindow,
                             [](GLFWwindow *, double x, double y) {
                                 menuGlobal.screenPtr->cursorPosCallbackEvent(x, y);
                             }
    );

    glfwSetMouseButtonCallback(this->_glfwWindow,
                               [](GLFWwindow *, int button, int action, int modifiers) {
                                   menuGlobal.screenPtr->mouseButtonCallbackEvent(button, action, modifiers);
                               }
    );

    glfwSetKeyCallback(this->_glfwWindow,
                       [](GLFWwindow *, int key, int scancode, int action, int mods) {
                           menuGlobal.screenPtr->keyCallbackEvent(key, scancode, action, mods);
                       }
    );

    glfwSetCharCallback(this->_glfwWindow,
                        [](GLFWwindow *, unsigned int codepoint) {
                            menuGlobal.screenPtr->charCallbackEvent(codepoint);
                        }
    );

    glfwSetDropCallback(this->_glfwWindow,
                        [](GLFWwindow *, int count, const char **filenames) {
                            menuGlobal.screenPtr->dropCallbackEvent(count, filenames);
                        }
    );

    glfwSetScrollCallback(this->_glfwWindow,
                          [](GLFWwindow *, double x, double y) {
                              menuGlobal.screenPtr->scrollCallbackEvent(x, y);
                          }
    );

    glfwSetFramebufferSizeCallback(this->_glfwWindow,
                                   [](GLFWwindow *, int width, int height) {
                                       menuGlobal.screenPtr->resizeCallbackEvent(width, height);
                                   }
    );
}

void Menu::SoundMenu() {
    menuGlobal.menuState = MenuState::SOUND;

    this->_gui = new nanogui::FormHelper(this->_screen);
    this->_nanoguiWindow = this->_gui->addWindow(Eigen::Vector2i(10, 10), "Sound");

    nanogui::Slider *volumeSlider = new nanogui::Slider(this->_nanoguiWindow);
    volumeSlider->setValue(menuGlobal.config["sound"]["volume"].as<float>());
    nanogui::TextBox *volumeTxtBox = new nanogui::TextBox(this->_nanoguiWindow);
    volumeTxtBox->setValue(std::to_string(volumeSlider->value()));
    volumeTxtBox->setUnits("%");
    volumeSlider->setCallback([volumeTxtBox](float value) {
        volumeTxtBox->setValue(std::to_string(static_cast<int>(value)));
    });
    volumeSlider->setFinalCallback([&](float value) {
        std::cout << "Final volumeSlider value: " << static_cast<int>(value) << std::endl;
    });

    nanogui::CheckBox *muteCb = new nanogui::CheckBox(this->_nanoguiWindow, "Mute",
		[](bool state) { std::cout << "Mute: " << state << std::endl; }
	);

	this->_gui->addButton("Apply", []() {
		std::cout << "Settings Applied" << std::endl;
		menuGlobal.config["sound"]["volume"] = std::to_string(volumeSlider->value());
		menuGlobal.config["sound"]["mute"] = muteCb->checked();
	});

    this->_gui->addButton("Back", []() {
        std::cout << "Back pressed." << std::endl;
        menuGlobal.loop = false;
        menuGlobal.menuState = MenuState::OPTIONS;
    });

    menuGlobal.screenPtr->setVisible(true);
    menuGlobal.screenPtr->performLayout();
    this->_nanoguiWindow->center();

    glfwSetWindowUserPointer(this->_glfwWindow, this);


    glfwSetCursorPosCallback(this->_glfwWindow,
                             [](GLFWwindow *, double x, double y) {
                                 menuGlobal.screenPtr->cursorPosCallbackEvent(x, y);
                             }
    );

    glfwSetMouseButtonCallback(this->_glfwWindow,
                               [](GLFWwindow *, int button, int action, int modifiers) {
                                   menuGlobal.screenPtr->mouseButtonCallbackEvent(button, action, modifiers);
                               }
    );

    glfwSetKeyCallback(this->_glfwWindow,
                       [](GLFWwindow *, int key, int scancode, int action, int mods) {
                           menuGlobal.screenPtr->keyCallbackEvent(key, scancode, action, mods);
                       }
    );

    glfwSetCharCallback(this->_glfwWindow,
                        [](GLFWwindow *, unsigned int codepoint) {
                            menuGlobal.screenPtr->charCallbackEvent(codepoint);
                        }
    );

    glfwSetDropCallback(this->_glfwWindow,
                        [](GLFWwindow *, int count, const char **filenames) {
                            menuGlobal.screenPtr->dropCallbackEvent(count, filenames);
                        }
    );

    glfwSetScrollCallback(this->_glfwWindow,
                          [](GLFWwindow *, double x, double y) {
                              menuGlobal.screenPtr->scrollCallbackEvent(x, y);
                          }
    );

    glfwSetFramebufferSizeCallback(this->_glfwWindow,
                                   [](GLFWwindow *, int width, int height) {
                                       menuGlobal.screenPtr->resizeCallbackEvent(width, height);
                                   }
    );
}

void Menu::ControlsMenu() {
    menuGlobal.menuState = MenuState::CONTROLS;
	//TODO: Map to a file
	std::string up = "UpArrow";
	std::string down = "DownArrow";
	std::string left = "LeftArrow";
	std::string right = "RightArrow";
	std::string bomb = "SpaceBar";

    this->_gui = new nanogui::FormHelper(this->_screen);
    this->_nanoguiWindow = this->_gui->addWindow(Eigen::Vector2i(10, 10), "Controls");

	this->_gui->addVariable("Up", up);
	this->_gui->addVariable("Down", down);
	this->_gui->addVariable("Left", left);
	this->_gui->addVariable("Right", right);
	this->_gui->addVariable("Bomb", bomb);

	this->_gui->addButton("Apply", []() {
		std::cout << "Settings Applied" << std::endl;
		menuGlobal.config["controls"]["up"] = up;
		menuGlobal.config["controls"]["down"] = down;
		menuGlobal.config["controls"]["left"] = left;
		menuGlobal.config["controls"]["right"] = right;
		menuGlobal.config["controls"]["bomb"] = up;
	});
	this->_gui->addButton("Back", []() {
		std::cout << "Back pressed" << std::endl;
		menuGlobal.loop = false;
		menuGlobal.menuState = MenuState::OPTIONS;
	});

	menuGlobal.screenPtr->setVisible(true);
    menuGlobal.screenPtr->performLayout();
    this->_nanoguiWindow->center();

    glfwSetWindowUserPointer(this->_glfwWindow, this);


    glfwSetCursorPosCallback(this->_glfwWindow,
                             [](GLFWwindow *, double x, double y) {
                                 menuGlobal.screenPtr->cursorPosCallbackEvent(x, y);
                             }
    );

    glfwSetMouseButtonCallback(this->_glfwWindow,
                               [](GLFWwindow *, int button, int action, int modifiers) {
                                   menuGlobal.screenPtr->mouseButtonCallbackEvent(button, action, modifiers);
                               }
    );

    glfwSetKeyCallback(this->_glfwWindow,
                       [](GLFWwindow *, int key, int scancode, int action, int mods) {
                           menuGlobal.screenPtr->keyCallbackEvent(key, scancode, action, mods);
                       }
    );

    glfwSetCharCallback(this->_glfwWindow,
                        [](GLFWwindow *, unsigned int codepoint) {
                            menuGlobal.screenPtr->charCallbackEvent(codepoint);
                        }
    );

    glfwSetDropCallback(this->_glfwWindow,
                        [](GLFWwindow *, int count, const char **filenames) {
                            menuGlobal.screenPtr->dropCallbackEvent(count, filenames);
                        }
    );

    glfwSetScrollCallback(this->_glfwWindow,
                          [](GLFWwindow *, double x, double y) {
                              menuGlobal.screenPtr->scrollCallbackEvent(x, y);
                          }
    );

    glfwSetFramebufferSizeCallback(this->_glfwWindow,
                                   [](GLFWwindow *, int width, int height) {
                                       menuGlobal.screenPtr->resizeCallbackEvent(width, height);
                                   }
    );
}
