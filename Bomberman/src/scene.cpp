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
			_addFloor(x, z);
			x += GRID_BLOCK_SIZE;
			xx++;
		}
		z -= GRID_BLOCK_SIZE;
		yy++;
	}
	std::vector<void *> params;
	params.push_back(this);
	MainGame::functions.insert(std::pair<const char *, Func>("updateBomb", {Scene::updateBomb, params}));
	MainGame::functions.insert(std::pair<const char *, Func>("updatePlayer", {Scene::updatePlayer, params}));
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
	Zion::Renderable *model = _game->getModel("block1");
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
		MainGame::renderer.addToRender(_player->getType(), _player->getId(), model,
				_player->getTransformation());
	}

	glm::vec3 pos = _player->getPosition();
	_game->getGameCamera().setCameraPosition(
			glm::vec3(pos.x + 0, pos.y + 10, pos.z + 6));
	_game->getGameCamera().setCameraTarget(_player->getPosition());
	_game->getGameCamera().setCameraUp(glm::vec3(0, 1, 0));
}

bool Scene::worldCollisionDown(glm::vec3 pos, glm::vec3 offset, Scene *scene)
{
	glm::vec3 newPos = pos + offset;

    int x = scene->getWorldx(pos.x);
    int y = scene->getWorldy(pos.z);

	if (scene->_blocks[y -  1][x] != nullptr &&
            checkBlockCollision(scene->_blocks[y - 1][x]->getPosition(), newPos))
	{
			return true;
	}
	if (scene->_blocks[y -  1][x - 1] != nullptr &&
            checkBlockCollision(scene->_blocks[y - 1][x - 1]->getPosition(), newPos))
    {
			scene->_player->changePosZ(0.01f);
			scene->_player->changePosX(0.01f);
			return true;
	}
	if (scene->_blocks[y -  1][x + 1] != nullptr &&
            checkBlockCollision(scene->_blocks[y - 1][x + 1]->getPosition(), newPos))
	{
			scene->_player->changePosZ(0.01f);
			scene->_player->changePosX(-0.01f);
			return true;
	}
    return false;
}

bool Scene::worldCollisionUp(glm::vec3 pos, glm::vec3 offset, Scene *scene)
{
	glm::vec3 newPos = pos + offset;

	int x = scene->getWorldx(pos.x);
	int y = scene->getWorldy(pos.z);
                                                //"collide up"
	if (scene->_blocks[y + 1][x] != nullptr &&
            checkBlockCollision1(scene->_blocks[y + 1][x]->getPosition(), newPos))
	{
			return true;
	}                                           //"collide up left"
	if (scene->_blocks[y + 1][x - 1] != nullptr &&
            checkBlockCollision(scene->_blocks[y + 1][x - 1]->getPosition(), newPos))
	{
			scene->_player->changePosZ(-0.005f);
			scene->_player->changePosX(0.005f);
			return true;
	}                                            //"collide up right"
	if (scene->_blocks[y + 1][x + 1] != nullptr &&
            checkBlockCollision(scene->_blocks[y + 1][x + 1]->getPosition(), newPos))
	{
			scene->_player->changePosZ(-0.005f);
			scene->_player->changePosX(-0.005f);
			return true;
	}
	return false;
}

bool Scene::worldCollisionLeft(glm::vec3 pos, glm::vec3 offset, Scene *scene)
{
	glm::vec3 newPos = pos + offset;

	int x = scene->getWorldx(pos.x);
	int y = scene->getWorldy(pos.z);

	if (scene->_blocks[y - 1][x - 1] != nullptr &&
            checkBlockCollision1(scene->_blocks[y - 1][x - 1]->getPosition(), newPos))
	{
			scene->_player->changePosZ(-0.005f);
			scene->_player->changePosX(0.005f);
			return true;
	}
	if (scene->_blocks[y][x - 1] != nullptr &&
            checkBlockCollision(scene->_blocks[y][x - 1]->getPosition(), newPos))
	{
			return true;
	}
	if (scene->_blocks[y + 1][x - 1] != nullptr &&
            checkBlockCollision1(scene->_blocks[y + 1][x - 1]->getPosition(), newPos))
	{
			scene->_player->changePosZ(0.005f);
			scene->_player->changePosX(0.005f);
			return true;
	}
	return false;
}

bool Scene::worldCollisionRight(glm::vec3 pos, glm::vec3 offset, Scene *scene)
{
	glm::vec3 newPos = pos + offset;

	int x = scene->getWorldx(pos.x);
	int y = scene->getWorldy(pos.z);

	if (scene->_blocks[y - 1][x + 1] != nullptr &&
            checkBlockCollision1(scene->_blocks[y - 1][x + 1]->getPosition(), newPos))
	{
			scene->_player->changePosZ(-0.005f);
			scene->_player->changePosX(0.005f);
			return true;
	}
	if (scene->_blocks[y][x + 1] != nullptr &&
            checkBlockCollision(scene->_blocks[y][x + 1]->getPosition(), newPos))
	{
			return true;
	}
	if (scene->_blocks[y + 1][x + 1] != nullptr &&
            checkBlockCollision1(scene->_blocks[y + 1][x + 1]->getPosition(), newPos))
	{
			scene->_player->changePosZ(0.005f);
			scene->_player->changePosX(0.005f);
			return true;
	}
	return false;
}

bool Scene::checkBlockCollision(glm::vec3 blockPos, glm::vec3 entityPos)
{
	float sqDist = 0.0f;
	float minX, minZ, maxX, maxZ;

	entityPos.x += HALF_PLAYER_SIZE;
	entityPos.z -= HALF_PLAYER_SIZE;
	blockPos.x += HALF_GRID_BLOCK_SIZE;
	blockPos.z -= HALF_GRID_BLOCK_SIZE;

	minX = blockPos.x - (float)HALF_GRID_BLOCK_SIZE;
	maxX = blockPos.x + (float)HALF_GRID_BLOCK_SIZE;
	minZ = blockPos.z - (float)HALF_GRID_BLOCK_SIZE;
	maxZ = blockPos.z + (float)HALF_GRID_BLOCK_SIZE;

	if (entityPos.x < minX)
		sqDist += (minX - entityPos.x) * (minX - entityPos.x);
	if (entityPos.x > maxX)
		sqDist += (entityPos.x - maxX) * (entityPos.x - maxX);

	if (entityPos.z < minZ)
		sqDist += (minZ - entityPos.z) * (minZ - entityPos.z);
	if (entityPos.z > maxZ)
		sqDist += (entityPos.z - maxZ) * (entityPos.z - maxZ);
	return sqDist + 0.3f <= (float)(HALF_PLAYER_SIZE * HALF_PLAYER_SIZE);
}

bool Scene::checkBlockCollision1(glm::vec3 blockPos, glm::vec3 entityPos)
{
	float sqDist = 0.0f;
	float minX, minZ, maxX, maxZ;

	entityPos.x += HALF_PLAYER_SIZE;
	entityPos.z -= HALF_PLAYER_SIZE;
	blockPos.x += HALF_GRID_BLOCK_SIZE;
	blockPos.z -= HALF_GRID_BLOCK_SIZE;

	minX = blockPos.x - (float)HALF_GRID_BLOCK_SIZE;
	maxX = blockPos.x + (float)HALF_GRID_BLOCK_SIZE;
	minZ = blockPos.z - (float)HALF_GRID_BLOCK_SIZE;
	maxZ = blockPos.z + (float)HALF_GRID_BLOCK_SIZE;

	if (entityPos.x < minX)
		sqDist += (minX - entityPos.x) * (minX - entityPos.x);
	if (entityPos.x > maxX)
		sqDist += (entityPos.x - maxX) * (entityPos.x - maxX);

	if (entityPos.z < minZ)
		sqDist += (minZ - entityPos.z) * (minZ - entityPos.z);
	if (entityPos.z > maxZ)
		sqDist += (entityPos.z - maxZ) * (entityPos.z - maxZ);
	return sqDist + 0.3f <= (float)(HALF_PLAYER_SIZE * HALF_PLAYER_SIZE);
}

bool Scene::breakableBrickCollisionDown(glm::vec3 pos, Scene *scene)
{
    int x = scene->getWorldx(pos.x);
    int y = scene->getWorldy(pos.z);

    if (scene->_blocks[y -  1][x] != nullptr && scene->_blocks[y - 1][x]->isBreakable())
    {
        return true;
    }
    return false;
}

bool Scene::breakableBrickCollisionUp(glm::vec3 pos, Scene *scene)
{
    int x = scene->getWorldx(pos.x);
    int y = scene->getWorldy(pos.z);
    //"collide up"
    if (scene->_blocks[y + 1][x] != nullptr && scene->_blocks[y + 1][x]->isBreakable())
    {
        return true;
    }                                           //"collide up left"
    return false;
}

bool Scene::breakableBrickCollisionLeft(glm::vec3 pos, Scene *scene)
{
    int x = scene->getWorldx(pos.x);
    int y = scene->getWorldy(pos.z);

    if (scene->_blocks[y][x - 1] != nullptr && scene->_blocks[y][x - 1]->isBreakable())
    {
        return true;
    }

    return false;
}

bool Scene::breakableBrickCollisionRight(glm::vec3 pos, Scene *scene)
{
    int x = scene->getWorldx(pos.x);
    int y = scene->getWorldy(pos.z);

    if (scene->_blocks[y][x + 1] != nullptr && scene->_blocks[y][x + 1]->isBreakable())
    {
        return true;
    }

    return false;
}

bool Scene::PlayerExplosionCollision(glm::vec3 pos, Scene *scene)
{
    int bombx = scene->getWorldx(pos.x);
    int bomby = scene->getWorldy(pos.z);

    int playerx = scene->getWorldx(scene->_player->getPosition().x);
    int playery = scene->getWorldy(scene->_player->getPosition().z);


    if (playerx == bombx - 1 ||
            playerx == bombx + 1 ||
            playery == bomby - 1 ||
            playery == bomby + 1)
    {
        return true;
    }
    return false;
}