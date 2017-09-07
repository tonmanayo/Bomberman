#include <scene.hpp>



bool Scene::worldEndLevel(glm::vec3 pos, Scene *scene)
{
	int x = scene->getWorldx(pos.x);
	int y = scene->getWorldy(pos.z);

	if (scene->_blocks[y][x] != nullptr && scene->_blocks[y][x]->getEndMap())
	{
		return true;
	}

	return false;
}

void Scene::worldGetPower(glm::vec3 pos, Scene *scene)
{
	int x = scene->getWorldx(pos.x);
	int y = scene->getWorldy(pos.z);

	if (scene->_blocks[y][x] != nullptr && scene->_blocks[y][x]->getPowerUp())
	{
		if (scene->_blocks[y][x]->getPowerName() == "RangeUp") {
			scene->_player->incBombNbr();
			std::cout << "GotPOWER!\n";
			delete scene->_blocks[y][x];
			scene->_blocks[y][x] = nullptr;
		}
	}
}

bool Scene::worldCollisionDown(glm::vec3 pos, glm::vec3 offset, Scene *scene)
{
	glm::vec3 newPos = pos + offset;

	int x = scene->getWorldx(pos.x);
	int y = scene->getWorldy(pos.z);

	if (scene->_blocks[y -  1][x] != nullptr && scene->_blocks[y -  1][x]->getCollision() &&
	    checkBlockCollision(scene->_blocks[y - 1][x]->getPosition(), newPos))
	{
		return true;
	}
	if (scene->_blocks[y -  1][x - 1] != nullptr && scene->_blocks[y -  1][x - 1]->getCollision() &&
	    checkBlockCollision(scene->_blocks[y - 1][x - 1]->getPosition(), newPos))
	{
		scene->_player->changePosZ(0.01f);
		scene->_player->changePosX(0.01f);
		return true;
	}
	if (scene->_blocks[y -  1][x + 1] != nullptr && scene->_blocks[y -  1][x + 1]->getCollision() &&
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
	if (scene->_blocks[y + 1][x] != nullptr && scene->_blocks[y + 1][x]->getCollision() &&
	    checkBlockCollision1(scene->_blocks[y + 1][x]->getPosition(), newPos))
	{
		return true;
	}                                           //"collide up left"
	if (scene->_blocks[y + 1][x - 1] != nullptr && scene->_blocks[y + 1][x - 1]->getCollision() &&
	    checkBlockCollision(scene->_blocks[y + 1][x - 1]->getPosition(), newPos))
	{
		scene->_player->changePosZ(-0.01f);
		scene->_player->changePosX(0.01f);
		return true;
	}                                            //"collide up right"
	if (scene->_blocks[y + 1][x + 1] != nullptr && scene->_blocks[y + 1][x + 1]->getCollision() &&
	    checkBlockCollision(scene->_blocks[y + 1][x + 1]->getPosition(), newPos))
	{
		scene->_player->changePosZ(-0.01f);
		scene->_player->changePosX(-0.01f);
		return true;
	}
	return false;
}

bool Scene::worldCollisionLeft(glm::vec3 pos, glm::vec3 offset, Scene *scene)
{
	glm::vec3 newPos = pos + offset;

	int x = scene->getWorldx(pos.x);
	int y = scene->getWorldy(pos.z);

	if (scene->_blocks[y - 1][x - 1] != nullptr && scene->_blocks[y - 1][x - 1]->getCollision() &&
	    checkBlockCollision1(scene->_blocks[y - 1][x - 1]->getPosition(), newPos))
	{
		scene->_player->changePosZ(-0.01f);
		scene->_player->changePosX(0.01f);
		return true;
	}
	if (scene->_blocks[y][x - 1] != nullptr && scene->_blocks[y][x - 1]->getCollision() &&
	    checkBlockCollision(scene->_blocks[y][x - 1]->getPosition(), newPos))
	{
		return true;
	}
	if (scene->_blocks[y + 1][x - 1] != nullptr && scene->_blocks[y + 1][x - 1]->getCollision() &&
	    checkBlockCollision1(scene->_blocks[y + 1][x - 1]->getPosition(), newPos))
	{
		scene->_player->changePosZ(0.01f);
		scene->_player->changePosX(0.01f);
		return true;
	}
	return false;
}

bool Scene::worldCollisionRight(glm::vec3 pos, glm::vec3 offset, Scene *scene)
{
	glm::vec3 newPos = pos + offset;

	int x = scene->getWorldx(pos.x);
	int y = scene->getWorldy(pos.z);

	if (scene->_blocks[y - 1][x + 1] != nullptr && scene->_blocks[y - 1][x + 1]->getCollision() &&
	    checkBlockCollision1(scene->_blocks[y - 1][x + 1]->getPosition(), newPos))
	{
		scene->_player->changePosZ(-0.01f);
		scene->_player->changePosX(0.01f);
		return true;
	}
	if (scene->_blocks[y][x + 1] != nullptr && scene->_blocks[y][x + 1]->getCollision() &&
	    checkBlockCollision(scene->_blocks[y][x + 1]->getPosition(), newPos))
	{
		return true;
	}
	if (scene->_blocks[y + 1][x + 1] != nullptr && scene->_blocks[y + 1][x + 1]->getCollision() &&
	    checkBlockCollision1(scene->_blocks[y + 1][x + 1]->getPosition(), newPos))
	{
		scene->_player->changePosZ(0.01f);
		scene->_player->changePosX(0.01f);
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
//todo add powerup
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

	//todo increase distance by power up
	if (playerx == bombx - 1 && playery == bomby)
	{
		return true;
	}
	if (playerx == bombx + 1 && playery == bomby)
	{
		return true;
	}
	if (playery == bomby - 1 && playerx == bombx)
	{
		return true;
	}
	if (playery == bomby + 1 && playerx == bombx)
	{
		return true;
	}
	if (playery == bomby && playerx == bombx)
	{
		return true;
	}
	return false;
}

void Scene::enemiesExplosionCollision(glm::vec3 pos, Scene *scene)
{
	int 	bombx = scene->getWorldx(pos.x);
	int 	bomby = scene->getWorldy(pos.z);
	bool 	del = false;

	for (int i = 0; i < scene->_enemies.size(); ++i) {

		int playerx = scene->getWorldx(scene->_enemies[i]->getPosition().x);
		int playery = scene->getWorldy(scene->_enemies[i]->getPosition().z);

		//todo increase distance by power up
		if (playerx == bombx - 1 && playery == bomby) {
			del = true;
		}
		if (playerx == bombx + 1 && playery == bomby) {
			del = true;
		}
		if (playery == bomby - 1 && playerx == bombx) {
			del = true;
		}
		if (playery == bomby + 1 && playerx == bombx) {
			del = true;
		}
		if (playery == bomby && playerx == bombx) {
			del = true;
		}
		if (del) {
			MainGame::renderer.removeFromRender("enemy1", scene->_enemies[i]->getId());
			delete scene->_enemies[i];
			scene->_enemies[i] = scene->_enemies.back();
			scene->_enemies.pop_back();
			del = false;
		}
	}
}