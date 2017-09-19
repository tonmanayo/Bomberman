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
	static irrklang::ISoundSource   			*_menuMusic;
	static irrklang::ISoundSource    			*_bombExplosionSound;
	static irrklang::ISoundSource    			*_bombPlacementSound;
	static irrklang::ISoundSource    			*_playerHurtSound;
	static irrklang::ISoundSource    			*_enemyHurtSound;
	static irrklang::ISoundSource    			*_playerWalkingSound;
	static irrklang::ISoundSource    			*_gameMusic;
	static irrklang::ISoundSource    			*_mapLevelUp;
	static irrklang::ISoundSource    			*_playPowerUp;
    static irrklang::ISoundSource               *_playGameReady;

public:
	Scene       *scene = nullptr;
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
	static PauseMenu        pauseMenu;
	static EndGameMenu      endGameMenu;
	static LoadGameMenu     loadGameMenu;
	static OptionMenu       optionMenu;
	static Options          options;
	static Options          tmpOptions;
	static bool             isKeyBind;
	static std::string      keyBind;

public:
	Menu() = default;
	Menu(MainGame *mainGame);
	Menu(const Menu & rhs);
	Menu&   operator=(const Menu & rhs);
	~Menu() = default;

	bool        initMenu(MainGame *mainGame);
    bool        buildMenuWindows();
    void        createMainMenu();
    void        createStoryMenu();
    void        createExitWindow();
	void        createNewGameMenu();
	void        createPauseGameMenu();
	void        createEndGameMenu();
	void        createLoadGameMenu();
	void	    createOptionsMenu();
	void        createBackground();
    GLFWwindow  *getGlfwWindow();
	void        renderGui();

	static void     updateMenu(MainGame *game, std::vector<void *> params);
	static void     updateGameStateStart(MainGame *game, Menu *menu, GAMESTATE state);
	static void     updateGameStateEnd(MainGame *game, Menu *menu, GAMESTATE state);
	static void     createNewGame(int level, int difficulty, std::string saveName);
	static void     destroyGame();
	static void     loadSaveDirectory();
	static void     loadOptions();
	static void     copyOptions(Options & dest, Options & src);
	static void     updateGraphicOptions();
	/// creating callbacks for menu
	static bool     mouseCallback(int button, int action, int mod);
	static bool     cursorPositionCallback(int xpos, int ypos);
	static bool     keyCallback(int key, int scancode, int action, int mods);
	static void     playMenuMusic();
	static void     stopMenuMusic();
	static bool     isKeyAvailable(std::string &name, int keyValue);
	static void     keyPressKeyBindings(int key);
	static void     myGlfwGetKeyName(int key, std::string &dest);

	static void 	playBombExplosion();
	static void 	playBombPlacement();
	static void 	playPlayerHurt();
	static void 	playEnemyHurt();
	static void 	playPlayerWalking();
	static void		playGameSong();
	static void		playLevelUp();
	static void		playPowerUp();
	static void		playGameReady();

};