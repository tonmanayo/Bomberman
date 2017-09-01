#include <scene.hpp>

bool Scene::worldCollisionDown(glm::vec3 pos, glm::vec3 offset, Scene *scene)
{
	glm::vec3 newPos = pos + offset;

	int x = abs((int)round((pos.x) / (float)GRID_BLOCK_SIZE));
	int y = abs((int)round((pos.z) / (float)GRID_BLOCK_SIZE));

	if (scene->_blocks[y -  1][x] != nullptr)
	{
		if (checkBlockCollision(scene->_blocks[y - 1][x]->getPosition(), newPos))
		{
			//std::cout << "collide down" << std::endl;
			return true;
		}
	}
	if (scene->_blocks[y -  1][x - 1] != nullptr)
	{
		if (checkBlockCollision(scene->_blocks[y - 1][x - 1]->getPosition(), newPos))
		{
			//std::cout << "collide down left" << std::endl;
			scene->_player->changePosZ(0.01f);
			scene->_player->changePosX(0.01f);
			return true;
		}
	}
	if (scene->_blocks[y -  1][x + 1] != nullptr)
	{
		if (checkBlockCollision(scene->_blocks[y - 1][x + 1]->getPosition(), newPos))
		{
			scene->_player->changePosZ(0.01f);
			scene->_player->changePosX(-0.01f);
			//std::cout << "collide down right" << std::endl;
			return true;
		}
	}
	return false;
}

bool Scene::worldCollisionUp(glm::vec3 pos, glm::vec3 offset, Scene *scene)
{
	glm::vec3 newPos = pos + offset;

	int x = scene->getWorldx(pos.x);
	int y = scene->getWorldy(pos.z);

	if (scene->_blocks[y + 1][x] != nullptr)
	{
		if (checkBlockCollision1(scene->_blocks[y + 1][x]->getPosition(), newPos))
		{
			//std::cout << "collide up" << std::endl;
			return true;
		}
	}
	if (scene->_blocks[y + 1][x - 1] != nullptr)
	{
		if (checkBlockCollision(scene->_blocks[y + 1][x - 1]->getPosition(), newPos))
		{
			//std::cout << "collide up left" << std::endl;
			scene->_player->changePosZ(-0.005f);
			scene->_player->changePosX(0.005f);
			return true;
		}
	}
	if (scene->_blocks[y + 1][x + 1] != nullptr)
	{
		if (checkBlockCollision(scene->_blocks[y + 1][x + 1]->getPosition(), newPos))
		{
			scene->_player->changePosZ(-0.005f);
			scene->_player->changePosX(-0.005f);
			//std::cout << "collide up right" << std::endl;
			return true;
		}
	}
	return false;
}

bool Scene::worldCollisionLeft(glm::vec3 pos, glm::vec3 offset, Scene *scene)
{
	glm::vec3 newPos = pos + offset;

	int x = scene->getWorldx(pos.x);
	int y = scene->getWorldy(pos.z);

	if (scene->_blocks[y - 1][x - 1] != nullptr)
	{
		if (checkBlockCollision1(scene->_blocks[y - 1][x - 1]->getPosition(), newPos)) {
			scene->_player->changePosZ(-0.005f);
			scene->_player->changePosX(0.005f);
			return true;
		}
	}
	if (scene->_blocks[y][x - 1] != nullptr)
	{
		if (checkBlockCollision(scene->_blocks[y][x - 1]->getPosition(), newPos)) {
			return true;
		}
	}
	if (scene->_blocks[y + 1][x - 1] != nullptr)
	{
		if (checkBlockCollision1(scene->_blocks[y + 1][x - 1]->getPosition(), newPos)) {
			scene->_player->changePosZ(0.005f);
			scene->_player->changePosX(0.005f);
			return true;
		}
	}
	return false;
}

bool Scene::worldCollisionRight(glm::vec3 pos, glm::vec3 offset, Scene *scene)
{
	glm::vec3 newPos = pos + offset;

	int x = scene->getWorldx(pos.x);
	int y = scene->getWorldy(pos.z);

	if (scene->_blocks[y - 1][x + 1] != nullptr)
	{
		if (checkBlockCollision1(scene->_blocks[y - 1][x + 1]->getPosition(), newPos)) {
			scene->_player->changePosZ(-0.005f);
			scene->_player->changePosX(0.005f);
			return true;
		}
	}
	if (scene->_blocks[y][x + 1] != nullptr)
	{
		if (checkBlockCollision(scene->_blocks[y][x + 1]->getPosition(), newPos))
			return true;
	}
	if (scene->_blocks[y + 1][x + 1] != nullptr)
	{
		if (checkBlockCollision1(scene->_blocks[y + 1][x + 1]->getPosition(), newPos)) {
			scene->_player->changePosZ(0.005f);
			scene->_player->changePosX(0.005f);
			return true;
		}
	}
	return false;
}

bool Scene::checkBlockCollision(glm::vec3 blockPos, glm::vec3 entityPos)
{
	float sqDist = 0.0f;
	float v;
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
	float v;
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