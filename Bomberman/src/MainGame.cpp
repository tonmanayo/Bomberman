#include <MainGame.hpp>
#include <menu.hpp>

Zion::Renderer                  MainGame::renderer;
std::map<std::string, Func>     MainGame::functions;
MainGame*                       MainGame::game;
Zion::ParticleSystem*           MainGame::explosionLeft;
Zion::ParticleSystem*           MainGame::explosionRight;
Zion::ParticleSystem*           MainGame::explosionUp;
Zion::ParticleSystem*           MainGame::explosionDown;
Zion::ParticleSystem*           MainGame::explosionY;
Zion::ParticleSystem*           MainGame::bombSparks;
Zion::ParticleSystem*           MainGame::smokeParticles;
irrklang::ISoundEngine*         MainGame::soundEngine;
Zion::TextRenderer              *MainGame::fontRenderer1;
Zion::TextRenderer              *MainGame::fontRenderer2;
Zion::TextRenderer              *MainGame::fontRenderer3;
Zion::TextRenderer              *MainGame::fontRenderer4;
Zion::TextRenderer              *MainGame::fontRenderer5;
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
	(void)rhs;
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

	srand((unsigned int)time(nullptr));
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
	if (addShader("instance", "shaders/instance.vert", "shaders/basic.frag"))
		getShader("instance")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	if (addShader("particle", "shaders/particle.vert", "shaders/particle.frag"))
		getShader("particle")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	if (addShader("fire", "shaders/basic.vert", "shaders/fire.frag"))
		getShader("fire")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	if (addShader("gui", "shaders/gui.vert", "shaders/basic.frag"))
		getShader("gui")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
	if (addShader("guiGame", "shaders/gui1.vert", "shaders/basic.frag"))
		getShader("guiGame")->setUniformMat4((GLchar *)"proj_matrix", projectionMatrix);
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
	MainGame::fontRenderer3 = new Zion::TextRenderer(getShader("text"), (GLuint)Menu::windowWidth, (GLuint)Menu::windowHeight);
	MainGame::fontRenderer3->loadFont("resource/fonts/entsans.ttf", 48);
	MainGame::fontRenderer4 = new Zion::TextRenderer(getShader("text"), (GLuint)Menu::windowWidth, (GLuint)Menu::windowHeight);
	MainGame::fontRenderer4->loadFont("resource/fonts/BaldurRegular.ttf", 48);
	MainGame::game = this;
	return true;
}

void MainGame::loadResources()
{
	/// loading level 1 block models
	addModel("stage1_Wall", *getShader("instance"), "resource/models/blocks/stage1/stage1_Wall.gltf");
	addModel("stage1_Breakable", *getShader("fire"), "resource/models/blocks/stage1/stage1_Breakable.gltf");
	addModel("stage1_Unbreakable", *getShader("instance"), "resource/models/blocks/stage1/stage1_Unbreakable.gltf");
	addModel("stage1_Floor", *getShader("instance"), "resource/models/blocks/stage1/stage1_Floor.gltf");
	addModel("stage1_Background", *getShader("instance"), "resource/models/blocks/stage1/stage1_Background.gltf");

	/// loading level 2 block models
	addModel("stage2_Wall", *getShader("instance"), "resource/models/blocks/stage2/stage2_Wall.gltf");
	addModel("stage2_Breakable", *getShader("fire"), "resource/models/blocks/stage2/stage2_Breakable.gltf");
	addModel("stage2_Unbreakable", *getShader("instance"), "resource/models/blocks/stage2/stage2_Unbreakable.gltf");
	addModel("stage2_Floor", *getShader("instance"), "resource/models/blocks/stage2/stage2_Floor.gltf");
	addModel("stage2_Background", *getShader("instance"), "resource/models/blocks/stage2/stage2_Background.gltf");

	/// loading level 3 block models
	addModel("stage3_Wall", *getShader("instance"), "resource/models/blocks/stage3/stage3_Wall.gltf");
	addModel("stage3_Breakable", *getShader("fire"), "resource/models/blocks/stage3/stage3_Breakable.gltf");
	addModel("stage3_Unbreakable", *getShader("instance"), "resource/models/blocks/stage3/stage3_Unbreakable.gltf");
	addModel("stage3_Floor", *getShader("instance"), "resource/models/blocks/stage3/stage3_Floor.gltf");
	addModel("stage3_Background", *getShader("instance"), "resource/models/blocks/stage3/stage3_Background.gltf");

	/// loading level 4 block models
	addModel("stage4_Wall", *getShader("instance"), "resource/models/blocks/stage4/stage4_Wall.gltf");
	addModel("stage4_Breakable", *getShader("fire"), "resource/models/blocks/stage4/stage4_Breakable.gltf");
	addModel("stage4_Unbreakable", *getShader("instance"), "resource/models/blocks/stage4/stage4_Unbreakable.gltf");
	addModel("stage4_Floor", *getShader("instance"), "resource/models/blocks/stage4/stage4_Floor.gltf");
	addModel("stage4_Background", *getShader("instance"), "resource/models/blocks/stage4/stage4_Background.gltf");

	/// loading level 5 block models
	addModel("stage5_Wall", *getShader("instance"), "resource/models/blocks/stage5/stage5_Wall.gltf");
	addModel("stage5_Breakable", *getShader("fire"), "resource/models/blocks/stage5/stage5_Breakable.gltf");
	addModel("stage5_Unbreakable", *getShader("instance"), "resource/models/blocks/stage5/stage5_Unbreakable.gltf");
	addModel("stage5_Floor", *getShader("instance"), "resource/models/blocks/stage5/stage5_Floor.gltf");
	addModel("stage5_Background", *getShader("instance"), "resource/models/blocks/stage5/stage5_Background.gltf");

	/// loading level 6 block models
	addModel("stage6_Wall", *getShader("instance"), "resource/models/blocks/stage6/stage6_Wall.gltf");
	addModel("stage6_Breakable", *getShader("fire"), "resource/models/blocks/stage6/stage6_Breakable.gltf");
	addModel("stage6_Unbreakable", *getShader("instance"), "resource/models/blocks/stage6/stage6_Unbreakable.gltf");
	addModel("stage6_Floor", *getShader("instance"), "resource/models/blocks/stage6/stage6_Floor.gltf");
	addModel("stage6_Background", *getShader("instance"), "resource/models/blocks/stage6/stage6_Background.gltf");

	/// loading floor models
	addModel("floor2", *getShader("instance"), "resource/models/others/endGame.gltf");
	addModel("explosion", *getShader("basic"), "resource/models/blocks/fireBlock.gltf");

	/// loading bomberman model
	addModel("bomberman", *getShader("anime"), "resource/models/bomberman/bomberman1.glb");

	/// loading other models
	addModel("bomb1", *getShader("animeNoJoint"), "resource/models/others/bomb1.gltf");
	addModel("ring", *getShader("animeNoJoint"), "resource/models/others/ring.gltf");

	/// loading enemies
	addModel("dino", *getShader("anime"), "resource/models/enemies/dino1.glb");
	addModel("moos", *getShader("anime"), "resource/models/enemies/moos.gltf");
	addModel("cubex", *getShader("anime"), "resource/models/enemies/cubex.glb");
	addModel("wooki", *getShader("anime"), "resource/models/enemies/wooki.gltf");
	addModel("enemyBallon2", *getShader("anime"), "resource/models/enemies/enemyBallon2.gltf");
	addModel("mag", *getShader("anime"), "resource/models/enemies/MagmaEnemy.glb");
	addModel("enemyBallon", *getShader("animeNoJoint"), "resource/models/enemies/enemyBallon.gltf");
	addModel("illy", *getShader("anime"), "resource/models/enemies/illy.gltf");


	/// loading powerUps
	addModel("heart", *getShader("instance"), "resource/models/powerUps/heart.glb");
	addModel("present", *getShader("instance"), "resource/models/powerUps/blue_bomb.gltf");
	addModel("incExplosionPower", *getShader("instance"), "resource/models/powerUps/red_bomb.gltf");
	addModel("star", *getShader("instance"), "resource/models/powerUps/battery.gltf");

	/// loading maps
	addMap("stage1", "resource/maps/stage1");
	addMap("stage2", "resource/maps/stage2");
	addMap("stage3", "resource/maps/stage3");
	addMap("stage4", "resource/maps/stage4");
	addMap("stage5", "resource/maps/stage5");
	addMap("stage6", "resource/maps/stage6");

	/// loading materials
	auto *mat = new Zion::Material();
	mat->texure.loadTextureFromPath("resource/models/enemies/cubex_Diffuse.psd");
	auto *cubex = (Zion::Model *)getModel("cubex");
	cubex->addMaterial(0, *mat);

	mat->texure.loadTextureFromPath("resource/models/enemies/OrdileDh.png");
	auto *dino = (Zion::Model *)getModel("dino");
	dino->addMaterial(0, *mat);

	mat->texure.loadTextureFromPath("resource/models/enemies/mag_Diffuse.tga");
	auto *mag = (Zion::Model *)getModel("mag");
	mag->addMaterial(0, *mat);

	addMaterial("fireBlock", "resource/images/fireTex.png");
	addMaterial("flame1", "resource/images/flame1.png");
	addMaterial("particleAtlas", "resource/images/particleAtlas.png");
	getMaterial("particleAtlas")->numRows =  4;
	addMaterial("explosion1", "resource/images/explosion2.jpg");
	getMaterial("explosion1")->numRows =  4;
	addMaterial("explosion2", "resource/images/explode1.png");
	getMaterial("explosion2")->numRows =  3;
	addMaterial("explosion3", "resource/images/explosion3.jpg");
	getMaterial("explosion3")->numRows =  4;
	addMaterial("explosion4", "resource/images/explosion4.jpg");
	getMaterial("explosion4")->numRows =  4;
	addMaterial("smoke1", "resource/images/smoke2.png");
	getMaterial("smoke1")->numRows =  7;
	loadParticles();
	_window.enableVsync();
}

void MainGame::loadParticles()
{
	Zion::ParticleMaster::init(*getShader("particle"));
	/// smoke effects
	smokeParticles = new Zion::ParticleSystem(getMaterial("smoke1"), 40, 1.0f, 0.0f, 2.5, 0.5f);
	smokeParticles->randomizeRotation();
	smokeParticles->setDirection({-1, -1, 0}, 0.1f);
	smokeParticles->setLifeError(0.4f);
	smokeParticles->setSpeedError(0.1f);
	smokeParticles->setScaleError(0.3f);
	smokeParticles->setPositionError(0.5f);

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

	explosionY = new Zion::ParticleSystem(getMaterial("explosion2"), 15, 3, 0.0f, 1.6, 0.5f);
	explosionY->randomizeRotation();
	explosionY->setDirection({0, 1, 0}, 0.1f);
	explosionY->setLifeError(0.3f);
	explosionY->setSpeedError(0.25f);
	explosionY->setScaleError(0.5f);
	explosionY->setPositionError(0.7f);

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
	Menu::playMenuMusic();
	while (!_window.shouldClose())
	{
		auto currentTime = (float)glfwGetTime();
		float startTicks = currentTime * 1000.0f;
		Zion::Renderable::deltaTime = currentTime - Zion::Renderable::runTime;
		Zion::Renderable::runTime = currentTime;

		_window.clearWindow(0.93f, 0.93f, 0.93f, 1.0f);
		/// calling all functions for loop
		for (std::pair<std::string, Func> func : functions)
		{
			if (!(bool)std::strcmp(func.first.c_str(), "sceneUpdate"))
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
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			MainGame::renderer.render();
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
		calculateFPS();
		/*static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10){
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}*/
		float frameTicks = ((float)glfwGetTime() * 1000.0f) - startTicks;
		/// fps limiter
		if (1000.0f / _maxFps > frameTicks){
			glfwSwapInterval((int)(1000.0f / _maxFps - frameTicks));
		}
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

void MainGame::calculateFPS()
{
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;
	static int frameCount = 0;

	if (frameCount < NUM_SAMPLES)
		frameCount++;

	static float prevTicks = (float)glfwGetTime() * 1000;

	float currentTicks;
	currentTicks = (float)glfwGetTime() * 1000;

	_frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame++] = _frameTime;

	if (currentFrame >= NUM_SAMPLES)
		currentFrame = 0;

	float frameTimeAverage = 0;
	for (int i = 0; i < frameCount; i++){
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= frameCount;
	if (frameTimeAverage > 0){
		_fps = 1000.0f / frameTimeAverage;
	}else{
		_fps = 0.0f;
	}
	prevTicks = currentTicks;
}