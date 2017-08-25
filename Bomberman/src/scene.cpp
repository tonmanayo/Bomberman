#include <scene.hpp>
#include <time.h>

Scene::Scene(MainGame *game, std::vector<std::string> *map, int enemyCount)
{
	_enemyCount = enemyCount;
	_map = map;
	_game = game;
	buildMap();
}

Scene::Scene(const Scene &rhs)
{}

Scene& Scene::operator=(const Scene &rhs)
{}

Scene::~Scene()
{}

bool Scene::buildMap()
{
	_mapWidth = _map->size();
	_mapLength = _map[0].size();

	srand (time(NULL));
	float  z = -1;
	for (std::string line : *_map)
	{
		float x = -4;
		for (char c : line)
		{
			if (c == 'R')
				_addWall(x, z);
			else if (c == 'G')
				_addBreakableBlock(x, z);
			else if (c == 'L')
				_addUnbreakableBlock(x, z);
			_addFloor(x, z);
			x += 2;
		}
		z -= 2;
	}
	return true;
}

void Scene::_addWall(float x, float z)
{
	static int i = 0;

	glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(x, 0, z));
	Zion::Renderable *model = _game->getModel("block1");
	if (model != nullptr)
	{
		MainGame::renderer.addToRender("wall", i, model, mat);
		i++;
	}
}

void Scene::_addBreakableBlock(float x, float z)
{
	static int i = 0;

	glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(x, 0, z));
	Zion::Renderable *model = _game->getModel("block2");
	if (model != nullptr)
	{
		MainGame::renderer.addToRender("breakBlock", i, model, mat);
		i++;
	}
}

void Scene::_addUnbreakableBlock(float x, float z)
{
	static int i = 0;

	glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(x, 0, z));
	Zion::Renderable *model = _game->getModel("block3");
	if (model != nullptr)
	{
		MainGame::renderer.addToRender("unbreakBlock", i, model, mat);
		i++;
	}
}

void Scene::_addFloor(float x, float z)
{
	static int i = 0;
	Zion::Renderable *model;

	int tmp = rand() % 2 + 1;

	glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(x, -1, z));
	//if (tmp == 1)
		model = _game->getModel("floor1");
	//else
	//	model = _game->getModel("floor2");
	if (model != nullptr)
	{
		MainGame::renderer.addToRender("floors", i, model, mat);
		i++;
	}
}