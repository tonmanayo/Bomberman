#include <scene.hpp>

bool Scene::buildMap()
{
	/// setting params for save game state
	_mapWidth = _map->size();
	_mapLength = _map[0].size();
	_floorType = "floor1";
	_wallType = "block1";
	_breakableBlockType = "block2";
	_unbreakableBlockType = "block3";
	_backgroundType = "lavaBackground";

	float  z = GRID_START_Z;
	int yy = 0;
	_addBackground();
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
	/// adding scene update to render loop functions
	std::vector<void *> params;
	params.push_back(this);
	MainGame::functions.insert(std::pair<const char *, Func>("sceneUpdate", {Scene::sceneUpdate, params}));
	return true;
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