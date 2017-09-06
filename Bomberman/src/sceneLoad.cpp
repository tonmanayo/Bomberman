#include <scene.hpp>

bool Scene::newGame(MainGame *game, std::string mapName)
{
	std::vector<std::string>    *map = game->getMap(mapName);
	if (map == nullptr)
		return false;
	_mapName = mapName;
	_game = game;
	for (std:: string& line : *map)
	{
		std::vector<std::string> strSplits = MainGame::stringSplit(line, ' ');
		if (strSplits[0] == std::string("FloorType"))
			_floorType = strSplits[1];
		else if (strSplits[0] == std::string("WallType"))
			_wallType = strSplits[1];
		else if (strSplits[0] == std::string("BreakType"))
			_breakableBlockType = strSplits[1];
		else if (strSplits[0] == std::string("UnbreakType"))
			_unbreakableBlockType = strSplits[1];
		else if (strSplits[0] == std::string("BgType"))
			_backgroundType = strSplits[1];
		else if (line[0] == 'R')
			_loadNewGameLine(line);
	}
	_addBackground();
	/// adding scene update to render loop functions
	std::vector<void *> params;
	params.push_back(this);
	MainGame::functions.insert(std::pair<const char *, Func>("sceneUpdate", {Scene::sceneUpdate, params}));
	return true;
}

void Scene::_loadNewGameLine(std::string &line)
{
	static float  z = GRID_START_Z;
	static int gridY = 0;
	float x = GRID_START_X;
	int   gridX = 0;
	for (char c : line)
	{
		if (c == 'R')
			_addWall(x, z, gridX, gridY);
		else if (c == 'G')
			_addBreakableBlock(x, z, gridX, gridY);
		else if (c == 'L')
			_addUnbreakableBlock(x, z, gridX, gridY);
		else if (c == '@')
			_addPlayer(x, z);
		else if (c == 'E')
			_addEnemy(x, z);
		_addFloor(x, z);
		x += GRID_BLOCK_SIZE;
		gridX++;
	}
	z -= GRID_BLOCK_SIZE;
	gridY++;
}

bool Scene::saveGame(std::string fileName)
{
	std::ofstream   save;
	glm::vec3       pos;

	save.open(std::string("save/") + fileName, std::ios::out);
	if (!save.is_open())
		return false;
	save << "FloorType " << _floorType << std::endl;
	save << "WallType " << _wallType << std::endl;
	save << "MapWidth " << _mapWidth << std::endl;
	save << "MapHeight " << _mapLength << std::endl;
	save << "BreakableBlockType " << _breakableBlockType << std::endl;
	save << "UnbreakableBlockType " << _unbreakableBlockType << std::endl;
	save << "BackgroundType " << _backgroundType << std::endl;
	pos = _player->getPosition();
	save << "@ " << pos.x << " " << pos.y << " " << pos.z << std::endl;
	for (std::pair<int, std::map<int, Block *>> pair : _blocks)
	{
		for (std::pair<int, Block *> block : pair.second)
		{
			if (block.second == nullptr)
				continue;
			if (!block.second->isBreakable())
				continue;
			pos = block.second->getPosition();
			save << "G " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		}
	}
	for (Player *enemy : _enemies)
	{
		pos = enemy->getPosition();
		save << "E " << enemy->modelType << " " << pos.x << " " << pos.y << " " << pos.z << std::endl;
	}
	save.close();
	return true;
}