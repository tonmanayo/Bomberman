#include <MainGame.hpp>
#include <menu.hpp>

Zion::Renderer                  MainGame::renderer;
std::map<const char *, Func>    MainGame::functions;
MainGame*                       MainGame::game;
Zion::ParticleSystem*           MainGame::explosionLeft;
Zion::ParticleSystem*           MainGame::explosionRight;
Zion::ParticleSystem*           MainGame::explosionUp;
Zion::ParticleSystem*           MainGame::explosionDown;
Zion::ParticleSystem*           MainGame::bombSparks;
Zion::ParticleSystem*           MainGame::smokeParticles;
irrklang::ISoundEngine*         MainGame::soundEngine;
Zion::TextRenderer              *MainGame::fontRenderer1;
Zion::TextRenderer              *MainGame::fontRenderer2;
int                             MainGame::stage = 1;

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

bool MainGame::initGame(GLFWwindow *window, float width, float height, float fov)
{
	glm::mat4       projectionMatrix;

	_window.initWindow(window, "Bomberman", (int)width, (int)height);
	return initGame2(width, height, fov);
}

bool MainGame::initGame(float width, float height, float fov)
{
	glm::mat4       projectionMatrix;

	srand(time(0));
	/// Creating glfw window
	_width = width;
	_height = height;
	_fov = fov;
	_window.initWindow("Bomberman", (int)width, (int)height);
	return initGame2(width, height, fov);
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
	_shaders.insert(std::pair<std::string, Zion::Shader *>(std::string(name), shader));
	return true;
}

bool MainGame::addModel(const char *name, Zion::Shader& shader, const char *path)
{
	auto *model = new Zion::Gltf(shader, path);
	_models.insert(std::pair<std::string, Zion::Renderable *>(std::string(name), model));
	return true;
}

bool MainGame::addMaterial(const char *name, const char *path)
{
	auto *material = new Zion::Material();
	material->texure.loadTextureFromPath(path);
	_materials.insert(std::pair<std::string, Zion::Material *>(std::string(name), material));
	return true;
}

bool MainGame::addMap(const char *name, const char *path)
{
	std::ifstream   file;
	std::string     line;

	file.open(path);
	if (file.fail())
	{
		std::cout << "Failed to load map : " << name << std::endl;
		return false;
	}
	auto *map = new std::vector<std::string>();
	while (std::getline(file, line))
		map->push_back(line);
	_levelMaps.insert(std::pair<std::string, std::vector<std::string> *>(std::string(name), map));
	return true;
}


bool MainGame::initGame2(float width, float height, float fov)
{
	glm::mat4       projectionMatrix;
	/// Calculating perspective
	projectionMatrix = glm::perspective(glm::radians(fov), width / height, 0.1f, 1000.0f);
	/// Loading Shaders
	if (addShader("basic", "shaders/basic.vert", "shaders/basic.frag"))
		getShader("basic")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	if (addShader("particle", "shaders/particle.vert", "shaders/particle.frag"))
		getShader("particle")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	if (addShader("fire", "shaders/basic.vert", "shaders/fire.frag"))
		getShader("fire")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	if (addShader("gui", "shaders/gui.vert", "shaders/basic.frag"))
		getShader("gui")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	if (addShader("anime", "shaders/anime.vert", "shaders/basic.frag"))
		getShader("anime")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	if (addShader("animeNoJoint", "shaders/animeNoJoint.vert", "shaders/basic.frag"))
		getShader("animeNoJoint")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	if (addShader("text", "shaders/text.vert", "shaders/text.frag"))
		getShader("text")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	/// setup default camera
	setupGameCamera();
	/// load resources
	loadResources();
	/// load fonts
	MainGame::fontRenderer1 = new Zion::TextRenderer(getShader("text"), (GLuint)Menu::windowWidth, (GLuint)Menu::windowHeight);
	MainGame::fontRenderer1->loadFont("resource/fonts/angryBirds.ttf", 48);
	MainGame::fontRenderer2 = new Zion::TextRenderer(getShader("text"), (GLuint)Menu::windowWidth, (GLuint)Menu::windowHeight);
	MainGame::fontRenderer2->loadFont("resource/fonts/sansSerious.ttf", 48);
	MainGame::game = this;
	return true;
}

void MainGame::loadResources()
{
	/// loading block models
	addModel("block1", *getShader("basic"), "resource/models/blocks/block1.gltf");
	addModel("block2", *getShader("fire"), "resource/models/blocks/block2.gltf");
	addModel("explosion", *getShader("basic"), "resource/models/blocks/fireBlock.gltf");
	addModel("block3", *getShader("basic"), "resource/models/blocks/block3.gltf");

	/// loading floor models
	addModel("floor1", *getShader("basic"), "resource/models/blocks/floor1.gltf");
	addModel("floor2", *getShader("basic"), "resource/models/blocks/ManHole.gltf");

	/// loading bomberman model
	addModel("bomberman", *getShader("anime"), "resource/models/bomberman/bomberman1.glb");

	/// loading other models
	addModel("bomb", *getShader("basic"), "resource/models/others/bomb.gltf");
	addModel("bomb1", *getShader("animeNoJoint"), "resource/models/others/bomb1.gltf");
	addModel("lavaBackground", *getShader("basic"), "resource/models/bomberman/lavaBackground.gltf");
	addModel("bg", *getShader("gui"), "resource/models/others/bg.gltf");

	/// loading enemies
	addModel("onile", *getShader("anime"), "resource/models/enemies/Cubex.glb");
	addModel("enemy1", *getShader("basic"), "resource/models/enemies/enemy2.gltf");
	addModel("enemyBallon", *getShader("animeNoJoint"), "resource/models/enemies/enemyBallon.gltf");

	/// loading powerUps
	addModel("heart", *getShader("basic"), "resource/models/powerUps/heart.glb");
	addModel("present", *getShader("basic"), "resource/models/powerUps/present.gltf");
	addModel("lemon", *getShader("basic"), "resource/models/powerUps/lemon.gltf");
	addModel("star", *getShader("basic"), "resource/models/powerUps/star.gltf");

	/// loading maps
	addMap("map2", "resource/maps/map2");

	/// loading materials
	auto *mat = new Zion::Material();
	mat->texure.loadTextureFromPath("resource/models/enemies/cubex_Diffuse.psd");
	auto *onileModel = (Zion::Model *)getModel("onile");
	onileModel->addMaterial(0, *mat);

	addMaterial("fireBlock", "resource/images/fireTex.png");
	addMaterial("flame1", "resource/images/flame1.png");
	addMaterial("particleAtlas", "resource/images/particleAtlas.png");
	getMaterial("particleAtlas")->numRows =  4;
	addMaterial("explosion1", "resource/images/explosion1.png");
	getMaterial("explosion1")->numRows =  4;
	addMaterial("explosion2", "resource/images/explode1.png");
	getMaterial("explosion2")->numRows =  3;
	addMaterial("explosion3", "resource/images/explosion3.jpg");
	getMaterial("explosion3")->numRows =  4;
	addMaterial("explosion4", "resource/images/explosion4.jpg");
	getMaterial("explosion4")->numRows =  4;
	loadParticles();
	_window.enableVsync();
}

void MainGame::loadParticles()
{
	Zion::ParticleMaster::init(*getShader("particle"));
	/// explosion to the left
	explosionLeft = new Zion::ParticleSystem(getMaterial("explosion2"), 15, 3, 0.0f, 1.6, 0.5f);
	explosionLeft->randomizeRotation();
	explosionLeft->setDirection({-1, 0, 0}, 0.1f);
	explosionLeft->setLifeError(0.3f);
	explosionLeft->setSpeedError(0.25f);
	explosionLeft->setScaleError(0.5f);
	explosionLeft->setPositionError(0.7f);
	/// explosion to the right
	explosionRight = new Zion::ParticleSystem(getMaterial("explosion2"), 15, 3, 0.0f, 1.6, 0.5f);
	explosionRight->randomizeRotation();
	explosionRight->setDirection({1, 0, 0}, 0.1f);
	explosionRight->setLifeError(0.3f);
	explosionRight->setSpeedError(0.25f);
	explosionRight->setScaleError(0.5f);
	explosionRight->setPositionError(0.7f);
	/// explosion up
	explosionUp = new Zion::ParticleSystem(getMaterial("explosion2"), 15, 3, 0.0f, 1.6, 0.5f);
	explosionUp->randomizeRotation();
	explosionUp->setDirection({0, 0, -1}, 0.1f);
	explosionUp->setLifeError(0.3f);
	explosionUp->setSpeedError(0.25f);
	explosionUp->setScaleError(0.5f);
	explosionUp->setPositionError(0.7f);
	/// explosion down
	explosionDown = new Zion::ParticleSystem(getMaterial("explosion2"), 15, 3, 0.0f, 1.6, 0.5f);
	explosionDown->randomizeRotation();
	explosionDown->setDirection({0, 0, 1}, 0.1f);
	explosionDown->setLifeError(0.3f);
	explosionDown->setSpeedError(0.25f);
	explosionDown->setScaleError(0.5f);
	explosionDown->setPositionError(0.7f);

}

void MainGame::gameLoop()
{
	glm::mat4   viewMatrix;
	glm::vec3   viewPos;

	Zion::Renderable::startTime = (float)glfwGetTime();
	Zion::Renderable::runTime = Zion::Renderable::startTime;
	Zion::Window::frameStartTime = Zion::Window::startTime;
	Zion::Window::frameChangeTime = 0.0f;

	srand(0);
	//Menu::playMenuMusic();
	while (!_window.shouldClose())
	{
		auto currentTime = (float)glfwGetTime();
		Zion::Renderable::deltaTime = currentTime - Zion::Renderable::runTime;
		Zion::Renderable::runTime = currentTime;

		_window.clearWindow(0.93f, 0.93f, 0.93f, 1.0f);
		/// calling all functions for loop
		for (std::pair<const char *, Func> func : functions)
		{
			if (!(bool)std::strcmp(func.first, "sceneUpdate"))
				func.second.func(this, func.second.params);
		}

		viewMatrix = _camera->getViewMatrix();
		viewPos = _camera->getCameraPosition();

		for (std::pair<std::string, Zion::Shader *> shader : _shaders)
		{
			if (!shader.first.compare("gui") || !shader.first.compare("text"))
				continue;
			shader.second->setUniformMat4((GLchar *)"view_matrix", viewMatrix);
			shader.second->setUniform3f((GLchar *)"viewPos", viewPos);
		}

		/// render game scene
		if (_state == GAMESTATE::GAME || _state == GAMESTATE::START || _state == GAMESTATE::PAUSE || _state == GAMESTATE::END)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glDisable(GL_BLEND);
			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			MainGame::renderer.render();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		/// render particles
		if (_state == GAMESTATE::GAME)
			Zion::ParticleMaster::update(_camera);
		Zion::ParticleMaster::renderParticles(_camera, viewMatrix);

		/// render and update nanogui menu
		functions["menuUpdate"].func(this, functions["menuUpdate"].params);
		_window.updateWindow();
		Zion::Input::updateKeys();
		Zion::Window::frameChangeTime = (float)glfwGetTime() - Zion::Window::frameStartTime;
		Zion::Window::frameStartTime = (float)glfwGetTime();
	}
}

Zion::Window& MainGame::getGameWindow() { return _window; }

Zion::Camera& MainGame::getGameCamera() { return *_camera; }

Zion::Shader* MainGame::getShader(const std::string shaderName) const
{
	Zion::Shader *shader;
	try {
		shader = _shaders.at(shaderName);
	}catch (const std::out_of_range& oor){
		return nullptr;
	}
	return shader;
}

Zion::Renderable* MainGame::getModel(const std::string modelName) const
{
	Zion::Renderable *model;
	try {
		model = _models.at(modelName);
	}catch (const std::out_of_range& oor){
		return nullptr;
	}
	return model;
}

Zion::Material* MainGame::getMaterial(const std::string materialName)
{
	Zion::Material *material;
	try {
		material = _materials.at(materialName);
	}catch (const std::out_of_range& oor){
		return nullptr;
	}
	return material;
}

std::vector<std::string>* MainGame::getMap(const std::string mapName)
{
	std::vector<std::string>    *map;
	try {
		map = _levelMaps.at(mapName);
	}catch (const std::out_of_range& oor){
		return nullptr;
	}
	return map;

}

GAMESTATE MainGame::getGameState() {
	return _state;
}

void MainGame::setGameState(GAMESTATE state) { _state = state; }

std::vector<std::string> MainGame::stringSplit(const std::string &s, const char c)
{
	std::string buff{""};
	std::vector<std::string> v;

	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);

	return v;
}