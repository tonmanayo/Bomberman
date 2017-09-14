#pragma once

#include <menuStructs.hpp>

class   Menu
{
private:
    MainGame            *_mainGame;
    Zion::SquareSprite  *_menuBg;
    Zion::SquareSprite  *_menuTitle;
    Zion::SquareSprite  *_heart;
    std::string         _saveFileName;
	nanogui::Screen     *_screen;
    nanogui::Window     *_exitWindow;

	nanogui::Window     *_startMenu;
	nanogui::Window		*_optionsMenu;
	nanogui::Window     *_newGameMenu;
	nanogui::Window     *_loadGameMenu;
	nanogui::Window     *_pauseGameMenu;
	nanogui::TextBox    *_newGameTextBox;
	nanogui::Window     *_gameSaved;
private:
	static irrklang::ISoundSource    *_menuMusic;
public:
	Scene       *scene = nullptr;
    YAML::Node	config;

    static Menu             *activeMenu;
    static bool             isFullScreen;
    static int              windowWidth;
    static int              windowHeight;
	static int              difficulty;
	static float            textStartTime;
	static Gui              gui;
    static nanogui::Label   *title;
    static MainMenu         mainMenu;
    static StoryModeMenu    storyModeMenu;
	static NewGameMenu      newGameMenu;

public:
	Menu() = default;
	Menu(float width, float height, MainGame *mainGame, bool fullScreen = false, bool resizable = false);
	Menu(const Menu & rhs);
	Menu&   operator=(const Menu & rhs);
	~Menu() = default;

	bool        initMenu(float width, float height, MainGame *mainGame, bool fullScreen = false, bool resizable = false);
    bool        buildMenuWindows();
    void        createMainMenu();
    void        createStoryMenu();
    void        createExitWindow();
	void        createNewGameMenu();
    GLFWwindow  *getGlfwWindow();

    bool        startMenu();
	bool        startGameMenu();
    void	    createOptionsMenu();
    void        createLoadGameMenu();
    void        createPauseGameMenu();
    void        createBackground();

	static void    updateMenu(MainGame *game, std::vector<void *> params);
	/// creating callbacks for menu
	static bool     mouseCallback(int button, int action, int mod);
	static bool     cursorPositionCallback(int xpos, int ypos);
	static bool     keyCallback(int key, int scancode, int action, int mods);
	static void     playMenuMusic();
	static void     stopMenuMusic();
};