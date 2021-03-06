#pragma once

#include <zion.h>
#include <fstream>
#include <unistd.h>
#include <irrKlang.h>

class   MainGame;
class   Menu;

struct  Func
{
	void    (*func)(MainGame*, std::vector<void *>);
	std::vector<void *> params;
};

class   MainGame
{
private:
	float           _width;
	float           _height;
	float           _fov;
	float           _fps;
	float           _maxFps = 60.0f;
	float           _frameTime;
	Zion::Window    _window;
	Zion::Camera    *_camera;
	GAMESTATE       _state = GAMESTATE::MENU;
	std::map<std::string, Zion::Shader *>      _shaders;
	std::map<std::string, Zion::Renderable *>  _models;
	std::map<std::string, Zion::Material *>    _materials;
	std::map<std::string, std::vector<std::string> *> _levelMaps;
public:
	static MainGame                 *game;
	static Zion::ParticleSystem     *explosionLeft;
	static Zion::ParticleSystem     *explosionRight;
	static Zion::ParticleSystem     *explosionUp;
	static Zion::ParticleSystem     *explosionDown;
	static Zion::ParticleSystem     *explosionY;
	static Zion::ParticleSystem     *bombSparks;
	static Zion::ParticleSystem     *smokeParticles;
	static irrklang::ISoundEngine   *soundEngine;
	static Zion::TextRenderer       *fontRenderer1;
	static Zion::TextRenderer       *fontRenderer2;
	static Zion::TextRenderer       *fontRenderer3;
	static Zion::TextRenderer       *fontRenderer4;
	static Zion::TextRenderer       *fontRenderer5;
	static int                      stage;
public:
	static Zion::Renderer                   renderer;
	static std::map<std::string, Func>      functions;
	static std::string                      getNameFromPath(const char *path);
	static std::vector<std::string>         stringSplit(const std::string& s, const char c);
public:
	MainGame() = default;
	MainGame(float width, float height, float fov);
	MainGame(const MainGame & rhs);
	MainGame&  operator=(const MainGame & rhs);
	~MainGame() = default;

	bool    initGame(float width, float height, float fov);
	bool    initGame(GLFWwindow *window, float width, float height, float fov);
	bool    initGame2(float width, float height, float fov);
	void    setupGameCamera();
	void    setupGameCamera(glm::vec3 pos, float pitch, float yaw);
	bool    addShader(const char *name, const char *vertPath, const char *fragPath);
	bool    addModel(const char *name, Zion::Shader& shader, const char *path);
	bool    addMaterial(const char *name, const char *path);
	bool    addMap(const char *name, const char *path);
	void    loadResources();
	void    loadParticles();
	void    gameLoop();
	void	gameLoopMenu();
	void    calculateFPS();

	Zion::Window&               getGameWindow();
	Zion::Camera&               getGameCamera();
	Zion::Shader                *getShader(const std::string shaderName) const;
	Zion::Renderable            *getModel(const std::string modelName) const;
	Zion::Material              *getMaterial(const std::string materialName);
	std::vector<std::string>    *getMap(const std::string mapName);
	GAMESTATE                   getGameState();
	void                        setGameState(GAMESTATE state);
};

