//
// Created by angus on 2017/09/02.
//

#ifndef GAME_MENU_H
#define GAME_MENU_H


#include <GLFW/glfw3.h>
#include <nanogui/screen.h>

enum MenuState {
    MAIN,
    NEW_GAME,
    LOAD_GAME,
    OPTIONS,
    SOUND,
    GRAPHICS,
    CONTROLS,
    PAUSE,
    EXIT,
    PLAY
};

class Menu {

public:

    struct Global{
        nanogui::Screen *screenPtr;
        bool loop;
        MenuState menuState;
    };

    Menu();
    ~Menu();
    void initScreen(GLFWwindow *glfwWindow);

    nanogui::Screen *get_screen() const;

    void set_screen(nanogui::Screen *_screen);

    GLFWwindow *get_glfwWindow() const;

    void set_glfwWindow(GLFWwindow *_glfwWindow);

    void Loop(Zion::Window *window);

    void MainMenu();
    void GraphicsMenu();
    void OptionsMenu();
    void SoundMenu();
    void ControlsMenu();
    void NewGameMenu();
    void LoadGameMenu();
private:
    nanogui::Screen *_screen;
    GLFWwindow *_glfwWindow;
    nanogui::FormHelper *_gui;
    nanogui::Window *_nanoguiWindow;

};


#endif //GAME_MENU_H
