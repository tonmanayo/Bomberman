#include <scene.hpp>

Scene::Scene(MainGame *game, std::vector<std::string> *map, int enemyCount)
{
	_enemyCount = enemyCount;
	_map = map;
	_game = game;
	buildMap();
    _nbBombs = 0;
}

Scene::Scene(const Scene &rhs)
{}

Scene& Scene::operator=(const Scene &rhs) { return *this; }

Scene::~Scene()
{
	for (int i = 0; i < _blocks.size(); i++)
	{
		for (int j = 0; j < _blocks[i].size(); j++)
			if (_blocks[i][j] != nullptr)
				delete _blocks[i][j];
	}
}

bool Scene::buildMap()
{
	_mapWidth = _map->size();
	_mapLength = _map[0].size();

	float  z = GRID_START_Z;
	int yy = 0;
	for (std::string line : *_map)
	{
		float x = GRID_START_X;
		int xx = 0;
		for (char c : line)
		{
			if (c == 'R')
				_addWall(x, z, xx, yy);
			else if (c == 'G')
				_addBreakableBlock(x, z, xx, yy);
			else if (c == 'L')
				_addUnbreakableBlock(x, z, xx, yy);
			else if (c == '@')
				_addPlayer(x, z);
            else if (c == 'E')
                _addEnemy(x, z);
			_addFloor(x, z);
			x += GRID_BLOCK_SIZE;
			xx++;
		}
		z -= GRID_BLOCK_SIZE;
		yy++;
	}
	std::vector<void *> params;
	params.push_back(this);
	MainGame::functions.insert(std::pair<const char *, Func>("sceneUpdate", {Scene::sceneUpdate, params}));
    glm::mat4 tmp = glm::translate(glm::mat4(), {0, -1.3, -5});
    tmp = glm::scale(tmp, {50, 50, 50});
	MainGame::renderer.addToRender("background", 0, _game->getModel("lavaBackground"), tmp);
    return true;
}

void Scene::_addWall(float x, float z, int xx, int yy)
{
	static int i = 0;

	glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(x, 0, z));
	Zion::Renderable *model = _game->getModel("block1");
	if (model != nullptr)
	{
		Block *block = new Block(i, "wall", false);
		_blocks[yy][xx] = block;
		_blocks[yy][xx]->setPosition(x, 0, z);
		MainGame::renderer.addToRender("wall", i, model, mat);
		i++;
	}
}

void Scene::_addBreakableBlock(float x, float z, int xx, int yy)
{
	static int i = 0;

	glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(x, 0, z));
	Zion::Renderable *model = _game->getModel("block2");
	if (model != nullptr)
	{
		Block *block = new Block(i, "breakBlock", true);
		_blocks[yy][xx] = block;
		_blocks[yy][xx]->setPosition(x, 0, z);
		MainGame::renderer.addToRender("breakBlock", i, model, mat);
		i++;
	}
}

void Scene::_addUnbreakableBlock(float x, float z, int xx, int yy)
{
	static int i = 0;

	glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(x, 0, z));
	Zion::Renderable *model = _game->getModel("block3");
	if (model != nullptr)
	{
		Block *block = new Block(i, "breakBlock", false);
		_blocks[yy][xx] = block;
		_blocks[yy][xx]->setPosition(x, 0, z);
		MainGame::renderer.addToRender("unbreakBlock", i, model, mat);
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

void Scene::_addBomb(float x, float z)
{
	static int i = 0;

	int newx = getWorldx(x);
	int newy = getWorldy(z);

	x = getGridx(x);
	z = getGridy(z);

	glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(x, 0, z));
	Zion::Renderable *model = _game->getModel("bomb");
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



void Scene::_addFloor(float x, float z)
{
	static int i = 0;
	Zion::Renderable *model;

	glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(x, -1, z));
	model = _game->getModel("floor1");
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
	glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(x, 0, z));
	if (model != nullptr)
	{
		_player = new Player(0, "player");
		_player->setPosition(x, 0, z);
		_player->scale(glm::vec3(0.3, 0.3, 0.3));
        _player->playerStart = glm::vec3(x, 0, z);
		MainGame::renderer.addToRender(_player->getType(), _player->getId(), model, _player->getTransformation());
	}

	glm::vec3 pos = _player->getPosition();
	_game->getGameCamera().setCameraPosition(glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
	_game->getGameCamera().setCameraTarget(_player->getPosition());
	_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
}

void Scene::_addEnemy(float x, float z)
{
    Zion::Renderable *model;
    static int i = 0;

    model = _game->getModel("enemy1");
    glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(x, 0, z));
    if (model != nullptr)
    {
        _enemies.emplace_back(i, "enemy1");
        _enemies[i].setPosition(x, 0, z);
        _enemies[i].playerStart = glm::vec3(x, 0, z);
        MainGame::renderer.addToRender("enemy1", i, model, mat);
    }
}

void Scene::sceneUpdate(MainGame *game, std::vector<void *> params)
{
	auto *scene = (Scene *)params[0];

	if (game->getGameState() == GAMESTATE::GAME)
	{
		if (game->getGameWindow().isKeyPressed(GLFW_KEY_ESCAPE))
		{
			std::cout << "esc pressed" << std::endl;
			//game->setGameState(GAMESTATE::PAUSE);
			return;
		}
		updateBomb(game, scene);
		updateEnemy(game, scene);
		updatePlayer(game, scene);
	}
}