#include <MainGame.hpp>

Zion::Renderer  MainGame::renderer;
std::map<const char *, Func>    MainGame::functions;

std::string MainGame::getNameFromPath(const char *path)
{
	std::string str = std::string(path);
	std::size_t found = str.find_last_of((char *)"/");
	if (found == std::string::npos)
		return str;
	return str.substr(found+1);
}

MainGame::MainGame(float width, float height, float fov)
{
	initGame(width, height, fov);
}

MainGame::MainGame(const MainGame &rhs) { *this = rhs; }

MainGame& MainGame::operator=(const MainGame &rhs)
{
	return *this;
}

bool MainGame::initGame(float width, float height, float fov)
{
	glm::mat4       projectionMatrix;

	/// Creating glfw window
	_width = width;
	_height = height;
	_fov = fov;
	_window.initWindow("Bomberman", (int)width, (int)height);
	/// Calculating perspective
	projectionMatrix = glm::perspective(glm::radians(70.0f), width / height, 0.1f, 1000.0f);
	/// Loading Shaders
	if (addShader("basic", "shaders/basic.vert", "shaders/basic.frag"))
		getShader("basic")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	/// setup default camera
	setupGameCamera();
	/// load resources
	loadResources();
	return true;
}

void MainGame::setupGameCamera()
{
	_camera = new Zion::Camera(glm::vec3(0, 0, 3));
}

void MainGame::setupGameCamera(glm::vec3 pos, float pitch, float yaw)
{
	_camera = new Zion::Camera(pos, pitch, yaw);
}

bool MainGame::addShader(const char *name, const char *vertPath, const char *fragPath)
{
	auto *shader = new Zion::Shader(vertPath, fragPath);
	_shaders.insert(std::pair<const char *, Zion::Shader *>(name, shader));
	return true;
}

bool MainGame::addModel(const char *name, Zion::Shader& shader, const char *path)
{
	auto *model = new Zion::Gltf(shader, path);
	_models.insert(std::pair<const char *, Zion::Renderable *>(name, model));
	return true;
}

bool MainGame::addMaterial(const char *name, const char *path)
{
	auto *material = new Zion::Material();
	material->texure.loadTextureFromPath(path);
	_materials.insert(std::pair<const char *, Zion::Material *>(name, material));
	return true;
}

bool MainGame::addMap(const char *name, const char *path)
{
	std::ifstream file;
	auto *map = new std::vector<std::string>();

	file.open(path);
	if (file.fail())
	{
		std::cout << "Failed to load map : " << name << std::endl;
		return false;
	}
	std::string line;
	while (std::getline(file, line))
		map->push_back(line);
	_levelMaps.insert(std::pair<const char *, std::vector<std::string> *>(name, map));
	return true;
}

void MainGame::loadResources()
{
	addModel("block1", *getShader("basic"), "resource/models/blocks/block1.gltf");
	addModel("block2", *getShader("basic"), "resource/models/blocks/block2.gltf");
	addModel("block3", *getShader("basic"), "resource/models/blocks/block3.gltf");
	addModel("floor1", *getShader("basic"), "resource/models/blocks/floor1.gltf");
	addModel("floor2", *getShader("basic"), "resource/models/blocks/floor2.gltf");
	addModel("bomberman", *getShader("basic"), "resource/models/bomberman/bomberman.gltf");
	addMap("map1", "resource/maps/map1");
}

void MainGame::gameLoop()
{
	glm::mat4 viewMatrix;

	Zion::Renderable::startTime = (float)glfwGetTime();
	Zion::Renderable::runTime = Zion::Renderable::startTime;
	while (!_window.shouldClose() && !_window.isKeyPressed(GLFW_KEY_ESCAPE))
	{
		auto currentTime = (float)glfwGetTime();
		Zion::Renderable::deltaTime = currentTime - Zion::Renderable::runTime;
		Zion::Renderable::runTime = currentTime;

		_window.clearWindow(0.0f, 0.0f, 0.0f, 1.0f);
		/// calling all functions for loop
		for (std::pair<const char *, Func> func : functions)
			func.second.func(this, func.second.params);
		viewMatrix = _camera->getViewMatrix();
		for (std::pair<const char *, Zion::Shader *> shader : _shaders)
			shader.second->setUniformMat4((GLchar *)"view_matrix", viewMatrix);
		MainGame::renderer.render();
		_window.updateWindow();
	}
}

Zion::Window& MainGame::getGameWindow() { return _window; }

Zion::Camera& MainGame::getGameCamera() { return *_camera; }

Zion::Shader* MainGame::getShader(const char *shaderName) const
{
	Zion::Shader *shader;
	try {
		shader = _shaders.at(shaderName);
	}catch (const std::out_of_range& oor){
		return nullptr;
	}
	return shader;
}

Zion::Renderable* MainGame::getModel(const char *modelName) const
{
	Zion::Renderable *model;
	try {
		model = _models.at(modelName);
	}catch (const std::out_of_range& oor){
		return nullptr;
	}
	return model;
}

Zion::Material* MainGame::getMaterial(const char *materialName)
{
	Zion::Material *material;
	try {
		material = _materials.at(materialName);
	}catch (const std::out_of_range& oor){
		return nullptr;
	}
	return material;
}

std::vector<std::string>* MainGame::getMap(const char *mapName)
{
	std::vector<std::string>    *map;
	try {
		map = _levelMaps.at(mapName);
	}catch (const std::out_of_range& oor){
		return nullptr;
	}
	return map;

}