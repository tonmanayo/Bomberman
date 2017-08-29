#include <scene.hpp>

Scene::Scene(MainGame *game, std::vector<std::string> *map, int enemyCount)
{
	_enemyCount = enemyCount;
	_map = map;
	_game = game;
	buildMap();
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
			_addFloor(x, z);
			x += GRID_BLOCK_SIZE;
			xx++;
		}
		z -= GRID_BLOCK_SIZE;
		yy++;
	}
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
		MainGame::renderer.addToRender(_player->getType(), _player->getId(), model,
				_player->getTransformation());
	}

	glm::vec3 pos = _player->getPosition();
	_game->getGameCamera().setCameraPosition(
			glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
	_game->getGameCamera().setCameraTarget(_player->getPosition());
	_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));

	std::vector<void *> params;
	params.push_back(this);
	MainGame::functions.insert(std::pair<const char *, Func>("updatePlayer",
			{Scene::updatePlayer, params}));
}