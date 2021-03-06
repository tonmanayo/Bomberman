#include <random>
#include <menu.hpp>

Scene::Scene(const Scene &rhs)
{
	(void)rhs;
}

Scene& Scene::operator=(const Scene &rhs) {
	(void)rhs;
	return *this;
}

Scene::~Scene()
{
//	for (int i = 0; i < _blocks.size(); i++)
//	{
//		for (int j = 0; j < _blocks[i].size(); j++)
//			if (_blocks[i][j] != nullptr)
//				delete _blocks[i][j];
//	}
}

int		Scene::getDifficulty() {
	if (_difficulty == 1)
		return 20;
	if (_difficulty == 2)
		return 25;
	return 50;
}
void	Scene::setDifficulty(int value) {
	_difficulty = value;
}

void Scene::_addBackground()
{
	glm::mat4 tmp = glm::translate(glm::mat4(), {0, -1.3, -5});
	tmp = glm::scale(tmp, {70, 70, 70});
	MainGame::renderer.addToRender("background", 0, _game->getModel(_backgroundType), tmp);
}

void Scene::_addWall(float x, float z, int xx, int yy)
{
	static int i = 0;

	glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(x, 0, z));
	Zion::Renderable *model = _game->getModel(_wallType);
	if (model != nullptr)
	{
		Block *block = new Block(i, "wall", false);
		_blocks[yy][xx] = block;
		_blocks[yy][xx]->setPosition(x, 0, z);
		MainGame::renderer.addToRender("wall", i, model, mat);
		i++;
	}
}

void Scene::_addPowerUps(float x, float z, int xx, int yy) {
    char powerUp[25] = {'F', 'G', 'B', 'H', 'E', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'E', 'E', '0', '0', '0',
						'0', '0', '0', '0', 'E', 'E'};              // F - fire range B - Multiple bombs S - player speed increase
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(0, 25);
    int randNbr = uniform_dist(e1);
    glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(x, -0.3, z));

    if (powerUp[randNbr] == 'E' && !_endLevel) {
        Zion::Renderable *endLevel = _game->getModel("floor2");
        _endLevel = true;
        _blocks[yy][xx]->setEndMap(true);
        if (endLevel != nullptr)
        {
            glm::mat4 mat1 = glm::translate(glm::mat4(), glm::vec3(x, -0.9, z));
            MainGame::renderer.addToRender("endLevel", 0, endLevel, mat1);
        }
    }

    if (powerUp[randNbr] == 'F' && !_blocks[yy][xx]->getEndMap()) {
        _blocks[yy][xx]->setPowerName("PowerBombNbrInc");
        _blocks[yy][xx]->setPowerUp(true);
        Zion::Renderable *present = _game->getModel("present");
        if (present != nullptr)
        {
            MainGame::renderer.addToRender("present", _blocks[yy][xx]->getId() , present, mat);
        }
    }

    if (powerUp[randNbr] == 'H' && !_blocks[yy][xx]->getEndMap()) {
        _blocks[yy][xx]->setPowerName("PowerHeart");
        _blocks[yy][xx]->setPowerUp(true);
        glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(0.3f,0.3f,0.3f));
        mat = mat * scale;
        Zion::Renderable *present = _game->getModel("heart");
        if (present != nullptr)
        {
            MainGame::renderer.addToRender("heart", _blocks[yy][xx]->getId() , present, mat);
        }
    }

    if (powerUp[randNbr] == 'G' && !_blocks[yy][xx]->getEndMap()) {
        _blocks[yy][xx]->setPowerName("PowerBombExplosionInc");
        _blocks[yy][xx]->setPowerUp(true);
        Zion::Renderable *present = _game->getModel("incExplosionPower");
        if (present != nullptr)
        {
            MainGame::renderer.addToRender("incExplosionPower", _blocks[yy][xx]->getId() , present, mat);
        }
    }
	if (powerUp[randNbr] == 'B' && !_blocks[yy][xx]->getEndMap()) {
		_blocks[yy][xx]->setPowerName("PowerSpeed");
		_blocks[yy][xx]->setPowerUp(true);
		Zion::Renderable *present = _game->getModel("star");
		if (present != nullptr)
		{
			MainGame::renderer.addToRender("star", _blocks[yy][xx]->getId() , present, mat);
		}
	}
}

void Scene::_addBreakableBlock(float x, float z, int xx, int yy)
{
	static int i = 0;
    Block *block = new Block(i, "breakBlock", true);
    _blocks[yy][xx] = block;
    _blocks[yy][xx]->setPosition(x, 0, z);
    _addPowerUps(x, z, xx, yy);
    glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(x, -1.15f, z));
    Zion::Renderable *breakableBlock = _game->getModel(_breakableBlockType);
	if (breakableBlock != nullptr)
	{
		MainGame::renderer.addToRender("breakBlock", i, breakableBlock, mat);
		i++;
	}
}

void Scene::_addUnbreakableBlock(float x, float z, int xx, int yy)
{
	static int i = 0;

	glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(x, 0, z));
	Zion::Renderable *model = _game->getModel(_unbreakableBlockType);
	if (model != nullptr)
	{
		Block *block = new Block(i, "unbreakBlock", false);
		_blocks[yy][xx] = block;
		_blocks[yy][xx]->setPosition(x, 0, z);
		MainGame::renderer.addToRender("unbreakBlock", i, model, mat);
		i++;
	}
}

void Scene::_addFloor(float x, float z)
{
	static int i = 0;
	Zion::Renderable *model;

	glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(x, -1, z));
	model = _game->getModel(_floorType);
	if (model != nullptr)
	{
		MainGame::renderer.addToRender("floors", i, model, mat);
		i++;
	}
}

void Scene::_addPlayer(float x, float z)
{
	Zion::Renderable *model;

	model = _game->getModel("bomberman");
	if (model != nullptr)
	{
		_player = new Player(0, "player");
		_player->setPosition(glm::vec3{x, 0, z});
		_player->scale(glm::vec3(0.7, 0.7, 0.7));
		_player->playerStart = glm::vec3(x, 0, z);
		MainGame::renderer.addToRender(_player->getType(), _player->getId(), model, _player->getTransformation());
	}

	glm::vec3 pos = _player->getPosition();
	_game->getGameCamera().setCameraPosition(glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
	_game->getGameCamera().setCameraTarget(_player->getPosition());
	_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
}

void Scene::_addEnemy(float x, float z, std::string &type)
{
	Zion::Renderable *model;
	static int i = 0;
	model = _game->getModel(type);

	if (model != nullptr)
	{
		Player *enemy = new Player(i, type);
		enemy->modelType = type;
		_enemies.push_back(enemy);
		_enemies.back()->setHp(1);
		_enemies.back()->setSpeed(2.0f);
		if (type == "illy") {
			_enemies.back()->setSpeed(6.0f);
			_enemies.back()->scale(glm::vec3{2.0f, 2.0f, 2.0f});
			_enemies.back()->setHp(5);
		}
        if (type == "moos") {
			_enemies.back()->setSpeed(1.0f);
		}
        if (type == "mag") {
			_enemies.back()->setSpeed(2.0f);
		}
        if (type == "wooki") {
			_enemies.back()->setSpeed(3.0f);
		}
        if (type == "wooki") {
			_enemies.back()->setSpeed(4.0f);
		}
        if (type == "dino") {
			_enemies.back()->setSpeed(5.0f);
		}
        if (type == "cubex") {
			_enemies.back()->setSpeed(6.0f);
		}
		_enemies.back()->setPosition(glm::vec3{getGridx(x), 0, getGridy(z)});
		_enemies.back()->playerStart = glm::vec3(getGridx(x), 0, getGridy(z));
		MainGame::renderer.addToRender(_enemies.back()->getType(), _enemies.back()->getId(), model, _enemies.back()->getTransformation());
		i++;
	}
}

void Scene::_addBomb(float x, float z)
{
	static int i = 0;

	int newx = getWorldx(x);
	int newy = getWorldy(z);

	x = getGridx(x);
	z = getGridy(z);

	if (_blocks[newy][newx] != nullptr && _blocks[newy][newx]->getType() == "bomb") {
		return;
	}
	if (_blocks[newy][newx] != nullptr && _blocks[newy][newx]->getEndMap())
		return;
	glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(x, 0, z));
	mat = glm::scale(mat, {0.6, 0.6, 0.6});
	Zion::Renderable *model = _game->getModel("bomb1");

	if (model != nullptr)
	{
		Block *block = new Block(i, "bomb", false);
		_blocks[newy][newx] = block;
		_blocks[newy][newx]->setPosition(x, 0, z);
		_bomb.emplace_back(_player->getPosition(), i);
		MainGame::renderer.addToRender("bomb", i, model, mat);
		i++;
	}
}

int Scene::getWorldx(float x) {
	return 	 std::abs(static_cast<int>(std::round((x - GRID_START_X) / (float)GRID_BLOCK_SIZE)));

}

int Scene::getWorldy(float y) {
	return  std::abs((int)std::round((y - GRID_START_Z) / (float)GRID_BLOCK_SIZE));
}

float Scene::getGridx(float x) {
	x = static_cast<float>(std::round(x / GRID_BLOCK_SIZE));
	return (x * (float)GRID_BLOCK_SIZE);
}
float Scene::getGridy(float z) {
	z = static_cast<float>(std::round(z / GRID_BLOCK_SIZE));
	return (z * (float)GRID_BLOCK_SIZE);
}

void Scene::sceneUpdate(MainGame *game, std::vector<void *> params)
{
	auto *scene = (Scene *)params[0];

	if (game->getGameState() == GAMESTATE::GAME)
	{
		if (Zion::Input::getKeyPressOnce(Menu::options.pause.glfwValue))
		{
			std::cout << "paused"<< std::endl;
			game->setGameState(GAMESTATE::PAUSE);
			MainGame::soundEngine->setAllSoundsPaused(true);
			return;
		}
		updateBomb(game, scene);
		if (scene->_player->getHP() > 0){
			updatePlayer(game, scene);
			updateEnemy(game, scene);
		}
		else{
			float   positionChange = 5.5f * Zion::Renderable::deltaTime;
			scene->_player->scale({0.3f, 0.3f, 0.3f});
			scene->_player->changePosY(positionChange);
			if (scene->_player->getDirection() == 'U')
				scene->_player->changePosZ(-positionChange);
			else if (scene->_player->getDirection() == 'D')
				scene->_player->changePosZ(positionChange);
			if (scene->_player->getDirection() == 'L')
				scene->_player->changePosX(-positionChange);
			else if (scene->_player->getDirection() == 'R')
				scene->_player->changePosX(positionChange);
			MainGame::renderer.applyTransformationToRenderable(scene->_player->getType(), scene->_player->getId(),
					scene->_player->getTransformation());
			MainGame::smokeParticles->generateParticles(scene->_player->getPosition(), true);
			if (scene->_player->getPosition().y > 10.0f){
				game->setGameState(GAMESTATE::END);
				MainGame::soundEngine->stopAllSounds();
			}
		}
	}
}

bool Scene::enemyPlayerCollision(Player *enemy, Scene *scene){
	int enemyX = scene->getWorldx(enemy->getPosition().x);
	int enemyY = scene->getWorldy(enemy->getPosition().z);

	int playerx = scene->getWorldx(scene->_player->getPosition().x);
	int playery = scene->getWorldy(scene->_player->getPosition().z);

	if (playery == enemyY && playerx == enemyX)
	{
			const float MIN_DISTANCE = HALF_PLAYER_SIZE * 2.0f;
			glm::vec3 centrePos1 = enemy->getPosition() + glm::vec3(HALF_PLAYER_SIZE);
			glm::vec3 centrePos2 = scene->_player->getPosition() + glm::vec3(HALF_PLAYER_SIZE);
			glm::vec3 vecDist = centrePos1 - centrePos2;
			float dist = glm::length(vecDist);
			float collisionDepth = MIN_DISTANCE - dist;

			if (collisionDepth > 0) {
				glm::vec3 collisionDepthVec = glm::normalize(vecDist) * collisionDepth;
				enemy->setPosition(enemy->getPosition() += collisionDepthVec / 2.0f);                                      // Push them in opposite directions
				scene->_player->setPosition(scene->_player->getPosition() -= collisionDepthVec / 2.0f);
				enemy->setDirection(scene->oppDir(enemy->getDirection()));
			}
		return true;
	}
	return false;
}