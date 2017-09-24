#include <menu.hpp>


bool Scene::worldEndLevel(glm::vec3 pos, Scene *scene)
{
	int x = scene->getWorldx(pos.x);
	int y = scene->getWorldy(pos.z);

	return (scene->_blocks[y][x] != nullptr && scene->_blocks[y][x]->getEndMap());

}

void Scene::worldGetPower(glm::vec3 pos, Scene *scene)
{
	int x = scene->getWorldx(pos.x);
	int y = scene->getWorldy(pos.z);

	if (scene->_blocks[y][x] != nullptr && scene->_blocks[y][x]->getPowerUp()) {
		Menu::playPowerUp();
		if (scene->_blocks[y][x]->getPowerName() == "PowerBombNbrInc") {
            scene->_player->incBombNbr();
            std::cout << "Got: " << scene->_player->getPowerBombNbr() << " POWER Bomb inc!\n";
            MainGame::renderer.removeFromRender("present", scene->_blocks[y][x]->getId());
            delete scene->_blocks[y][x];
            scene->_blocks[y][x] = nullptr;
        } else if (scene->_blocks[y][x]->getPowerName() == "PowerBombExplosionInc") {
            scene->_player->incBombExplosion();
            std::cout << "GotPOWER Bomb explosion inc!\n";
            MainGame::renderer.removeFromRender("incExplosionPower", scene->_blocks[y][x]->getId());
            delete scene->_blocks[y][x];
            scene->_blocks[y][x] = nullptr;
        } else if (scene->_blocks[y][x]->getPowerName() == "PowerSpeed") {
            scene->_player->incPowerSpeed();
            std::cout << "GotPOWER Speed!\n";
            MainGame::renderer.removeFromRender("star", scene->_blocks[y][x]->getId());
            delete scene->_blocks[y][x];
            scene->_blocks[y][x] = nullptr;
        } else if (scene->_blocks[y][x]->getPowerName() == "PowerHeart") {
            scene->_player->incHP(scene->getDifficulty());
            std::cout << "GotPOWER Heart!\n";
            MainGame::renderer.removeFromRender("heart", scene->_blocks[y][x]->getId());
            delete scene->_blocks[y][x];
            scene->_blocks[y][x] = nullptr;
        }
    }
}

bool Scene::worldCollisionDown(Player *enemy, glm::vec3 offset, Scene *scene)
{
    glm::vec3 newPos = enemy->getPosition() + offset;

    int x = scene->getWorldx(enemy->getPosition().x);
    int y = scene->getWorldy(enemy->getPosition().z);

    if (scene->_blocks[y -  1][x - 1] != nullptr && scene->_blocks[y -  1][x - 1]->getCollision() &&
        checkBlockCollision(scene->_blocks[y - 1][x - 1]->getPosition(), newPos))
    {
        enemy->changePosZ(2.0f * Zion::Renderable::deltaTime);
        enemy->changePosX(2.0f * Zion::Renderable::deltaTime);
        return true;
    }
    if (scene->_blocks[y -  1][x + 1] != nullptr && scene->_blocks[y -  1][x + 1]->getCollision() &&
        checkBlockCollision(scene->_blocks[y - 1][x + 1]->getPosition(), newPos))
    {
        enemy->changePosZ(2.0f * Zion::Renderable::deltaTime);
        enemy->changePosX(-2.0f * Zion::Renderable::deltaTime);
        return true;
    }

    return (scene->_blocks[y - 1][x] != nullptr  && scene->_blocks[y - 1][x]->getCollision()
            && checkBlockCollision(scene->_blocks[y - 1][x]->getPosition(), newPos));
}

bool Scene::worldCollisionUp(Player *enemy, glm::vec3 offset, Scene *scene)
{
    glm::vec3 newPos = enemy->getPosition() + offset;

    int x = scene->getWorldx(enemy->getPosition().x);
    int y = scene->getWorldy(enemy->getPosition().z);

    if (scene->_blocks[y + 1][x - 1] != nullptr && scene->_blocks[y + 1][x - 1]->getCollision() &&
        checkBlockCollision(scene->_blocks[y + 1][x - 1]->getPosition(), newPos))
    {
        enemy->changePosZ(-2.0f * Zion::Renderable::deltaTime);
        enemy->changePosX(2.0f * Zion::Renderable::deltaTime);
        return true;
    }                                            //"collide up right"
    if (scene->_blocks[y + 1][x + 1] != nullptr && scene->_blocks[y + 1][x + 1]->getCollision() &&
        checkBlockCollision(scene->_blocks[y + 1][x + 1]->getPosition(), newPos))
    {
        enemy->changePosZ(-2.0f * Zion::Renderable::deltaTime);
        enemy->changePosX(-2.0f * Zion::Renderable::deltaTime);
        return true;
    }
    return (scene->_blocks[y + 1][x] != nullptr  && scene->_blocks[y + 1][x]->getCollision()
            && checkBlockCollision(scene->_blocks[y + 1][x]->getPosition(), newPos));
}

bool Scene::worldCollisionLeft(Player *enemy, glm::vec3 offset, Scene *scene)
{
    glm::vec3 newPos = enemy->getPosition() + offset;

    int x = scene->getWorldx(enemy->getPosition().x);
    int y = scene->getWorldy(enemy->getPosition().z);

    if (scene->_blocks[y - 1][x - 1] != nullptr && scene->_blocks[y - 1][x - 1]->getCollision() &&
        checkBlockCollision(scene->_blocks[y - 1][x - 1]->getPosition(), newPos))
    {
        enemy->changePosZ(-2.0f * Zion::Renderable::deltaTime);
        enemy->changePosX(2.0f * Zion::Renderable::deltaTime);
        return true;
    }
    if (scene->_blocks[y + 1][x - 1] != nullptr && scene->_blocks[y + 1][x - 1]->getCollision() &&
        checkBlockCollision(scene->_blocks[y + 1][x - 1]->getPosition(), newPos))
    {
        enemy->changePosZ(2.0f * Zion::Renderable::deltaTime);
        enemy->changePosX(2.0f * Zion::Renderable::deltaTime);
        return true;
    }

    return (scene->_blocks[y][x - 1] != nullptr  && scene->_blocks[y][x - 1]->getCollision()
            && checkBlockCollision(scene->_blocks[y][x - 1]->getPosition(), newPos));
}

bool Scene::worldCollisionRight(Player *enemy, glm::vec3 offset, Scene *scene)
{
    glm::vec3 newPos = enemy->getPosition() + offset;

    int x = scene->getWorldx(enemy->getPosition().x);
    int y = scene->getWorldy(enemy->getPosition().z);

    if (scene->_blocks[y - 1][x + 1] != nullptr && scene->_blocks[y - 1][x + 1]->getCollision() &&
        checkBlockCollision(scene->_blocks[y - 1][x + 1]->getPosition(), newPos))
    {
        enemy->changePosZ(-2.0f * Zion::Renderable::deltaTime);
        enemy->changePosX(2.0f * Zion::Renderable::deltaTime);
        return true;
    }

    if (scene->_blocks[y + 1][x + 1] != nullptr && scene->_blocks[y + 1][x + 1]->getCollision() &&
        checkBlockCollision(scene->_blocks[y + 1][x + 1]->getPosition(), newPos))
    {
        enemy->changePosZ(2.0f * Zion::Renderable::deltaTime);
        enemy->changePosX(2.0f * Zion::Renderable::deltaTime );
        return true;
    }

    return (scene->_blocks[y][x + 1] != nullptr  && scene->_blocks[y][x + 1]->getCollision()
            && checkBlockCollision(scene->_blocks[y][x + 1]->getPosition(), newPos));
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

bool Scene::PlayerExplosionCollision(glm::vec3 pos, Scene *scene)
{
	int bombx = scene->getWorldx(pos.x);
	int bomby = scene->getWorldy(pos.z);

	int playerx = scene->getWorldx(scene->_player->getPosition().x);
	int playery = scene->getWorldy(scene->_player->getPosition().z);
	bool r = true, l = true, u = true, d = true;


	for (int i = 0; i < scene->_player->getPowerExplosion() + 1; ++i) {						// powerup loop

		if (playerx == bombx - 1 - i && playery == bomby && l) {
			return true;
		}
		if (playerx == bombx + 1 + i && playery == bomby && r) {
			return true;
		}
		if (playery == bomby - 1 - i && playerx == bombx && d) {
			return true;
		}
		if (playery == bomby + 1 + i && playerx == bombx && u) {
			return true;
		}
		if (playery == bomby && playerx == bombx) {
			return true;
		}
		if (scene->_blocks[bomby - 1 - i][bombx] != nullptr && d) {
			d = false;
		}
		if (scene->_blocks[bomby + 1 + i][bombx] != nullptr && u) {
			u = false;
		}
		if (scene->_blocks[bomby][bombx - 1 - i] != nullptr && l) {
			l = false;
		}
		if (scene->_blocks[bomby][bombx + 1 + i] != nullptr && r) {
			r = false;
		}
	}
	return false;
}