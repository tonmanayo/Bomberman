#pragma once

#include <zion.h>
#include <fstream>

class   MainGame;

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
	Zion::Window    _window;
	Zion::Camera    *_camera;
	std::map<const char *, Zion::Shader *>      _shaders;
	std::map<const char *, Zion::Renderable *>  _models;
	std::map<const char *, Zion::Material *>    _materials;
	std::map<const char *, std::vector<std::string> *> _levelMaps;
public:
	static Zion::Renderer  renderer;
	static std::map<const char *, Func>   functions;
	static std::string     getNameFromPath(const char *path);
public:
	MainGame() = default;
	MainGame(float width, float height, float fov);
	MainGame(const MainGame & rhs);
	MainGame&  operator=(const MainGame & rhs);
	~MainGame() = default;

	bool    initGame(float width, float height, float fov);
	void    setupGameCamera();
	void    setupGameCamera(glm::vec3 pos, float pitch, float yaw);
	bool    addShader(const char *name, const char *vertPath, const char *fragPath);
	bool    addModel(const char *name, Zion::Shader& shader, const char *path);
	bool    addMaterial(const char *name, const char *path);
	bool    addMap(const char *name, const char *path);
	void    loadResources();
	void    gameLoop();

	Zion::Window&               getGameWindow();
	Zion::Camera&               getGameCamera();
	Zion::Shader                *getShader(const char *shaderName) const;
	Zion::Renderable            *getModel(const char *modelName) const;
	Zion::Material              *getMaterial(const char *materialName);
	std::vector<std::string>    *getMap(const char *mapName);
};

