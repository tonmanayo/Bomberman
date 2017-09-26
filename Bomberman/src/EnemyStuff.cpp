#include <menu.hpp>

void Scene::enemiesExplosionCollision(glm::vec3 pos, Scene *scene)
{
    int 	bombx = scene->getWorldx(pos.x);
    int 	bomby = scene->getWorldy(pos.z);
    bool 	del = false;
    bool 	r = true, l = true, u = true, d = true;


    for (int i = 0; i < (int)scene->_enemies.size(); ++i) {

        int playerx = scene->getWorldx(scene->_enemies[i]->getPosition().x);
        int playery = scene->getWorldy(scene->_enemies[i]->getPosition().z);

        for (int j = 0; j < scene->_player->getPowerExplosion() + 1; ++j) {

            if (playerx == bombx - 1 - j && playery == bomby && l) {
                del = true;
            }
            if (playerx == bombx + 1 + j && playery == bomby && r) {
                del = true;
            }
            if (playery == bomby - 1 - j && playerx == bombx && d) {
                del = true;
            }
            if (playery == bomby + 1 + j && playerx == bombx && u) {
                del = true;
            }
            if (playery == bomby && playerx == bombx) {
                del = true;
            }
            if (del) {
                Menu::playEnemyHurt();
                scene->_enemies[i]->setHp(scene->_enemies[i]->getHP() - 1);
                if (scene->_enemies[i]->getHP() <= 0) {
                    MainGame::renderer.removeFromRender(scene->_enemies[i]->getType(), scene->_enemies[i]->getId());
                    delete scene->_enemies[i];
                    scene->_enemies[i] = scene->_enemies.back();
                    scene->_enemies.pop_back();
                }
                del = false;
            }
            if (scene->_blocks[bomby - 1 - i][bombx] != nullptr &&
                scene->_blocks[bomby - 1 - i][bombx]->getCollision() && d) {
                d = false;
            }
            if (scene->_blocks[bomby + 1 + i][bombx] != nullptr &&
                scene->_blocks[bomby + 1 + i][bombx]->getCollision() && u) {
                u = false;
            }
            if (scene->_blocks[bomby][bombx - 1 - i] != nullptr &&
                scene->_blocks[bomby][bombx - 1 - i]->getCollision() && l) {
                l = false;
            }
            if (scene->_blocks[bomby][bombx + 1 + i] != nullptr &&
                scene->_blocks[bomby][bombx  + 1 + i]->getCollision() && r) {
                r = false;
            }
        }
    }
}

char Scene::oppDir(char dir) {
    if (dir == 'L')
        dir = 'R';
    if (dir == 'R')
        dir = 'L';
    if (dir == 'U')
        dir = 'D';
    if (dir == 'D')
        dir = 'U';
    return dir;
}

void Scene::updateEnemy(MainGame *game, Scene *scene) {
	(void)game;
    bool collision = false;

    char dir[3] = {'L', 'L', 'L'};

    for (int i = 0; i < (int)scene->_enemies.size(); i++) {

        int y = scene->getWorldy(scene->_enemies[i]->getPosition().z);
        int x = scene->getWorldx(scene->_enemies[i]->getPosition().x);


        if (scene->enemyPlayerCollision(scene->_enemies[i], scene)) {
            MainGame::soundEngine->play2D("resource/sounds/playerInjured.wav");
            scene->_player->decHP(scene->getDifficulty());
            break;
        }
        if (worldCollisionDown(scene->_enemies[i], {0.0f, 0.0f, scene->_enemies[i]->getSpeed() * Zion::Renderable::deltaTime}, scene)) {
            collision = true;
            dir[0] = 'U';
            dir[1] = 'R';
            dir[2] = 'L';

            if (scene->_blocks[y + 1][x] != nullptr) {
                dir[0] = 'L';
            }
            if (scene->_blocks[y][x + 1] != nullptr) {
                dir[1] = 'U';
            }
            if (scene->_blocks[y][x - 1] != nullptr) {
                dir[2] = 'R';
            }
        }
        else if (worldCollisionUp(scene->_enemies[i], {0.0f, 0.0f, -scene->_enemies[i]->getSpeed() * Zion::Renderable::deltaTime}, scene)) {
            collision = true;

            dir[0] = 'D';
            dir[1] = 'R';
            dir[2] = 'L';

            if (scene->_blocks[y - 1][x] != nullptr) {
                dir[0] = 'R';
            }
            if (scene->_blocks[y][x + 1] != nullptr) {
                dir[1] = 'L';
            }
            if (scene->_blocks[y][x - 1] != nullptr) {
                dir[2] = 'D';
            }
        } else if (worldCollisionRight(scene->_enemies[i], {scene->_enemies[i]->getSpeed() * Zion::Renderable::deltaTime, 0.0f, 0.0f}, scene)) {
            collision = true;

            dir[0] = 'L';
            dir[1] = 'U';
            dir[2] = 'D';

            if (scene->_blocks[y][x - 1] != nullptr) {
                dir[0] = 'D';
            }
            if (scene->_blocks[y + 1][x] != nullptr) {
                dir[1] = 'L';
            }
            if (scene->_blocks[y - 1][x] != nullptr) {
                dir[2] = 'U';
            }
        } else if (worldCollisionLeft(scene->_enemies[i], {-scene->_enemies[i]->getSpeed() * Zion::Renderable::deltaTime, 0.0f, 0.0f}, scene)) {
            collision = true;

            dir[0] = 'R';
            dir[1] = 'D';
            dir[2] = 'U';

            if (scene->_blocks[y][x + 1] != nullptr) {
                dir[0] = 'D';
            }
            if (scene->_blocks[y - 1][x] != nullptr) {
                dir[1] = 'U';
            }
            if (scene->_blocks[y + 1][x] != nullptr) {
                dir[2] = 'R';
            }
        }

        if (collision) {                // if there was a collision give them a new direction
            int randNbr = rand() % 3;
            scene->_enemies[i]->setDirection(dir[randNbr]);
        }
        enemyFollowPlayer(collision, scene->_blocks, scene->_enemies[i], scene->_player, x, y);
        // collision enemy with enemy
        for (int j = 0; j < (int)scene->_enemies.size(); j++) {
            if (j == i)
                continue;
            const float MIN_DISTANCE = PLAYER_SIZE;
            glm::vec3 centrePos1 = scene->_enemies[i]->getPosition() + glm::vec3(static_cast<float>(HALF_PLAYER_SIZE));
            glm::vec3 centrePos2 = scene->_enemies[j]->getPosition() + glm::vec3(static_cast<float>(HALF_PLAYER_SIZE));
            glm::vec3 vecDist = centrePos1 - centrePos2;
            float dist = glm::length(vecDist);
            float collisionDepth = MIN_DISTANCE - dist;

            if (collisionDepth > 0) {
                glm::vec3 collisionDepthVec =
                        glm::normalize(vecDist) * collisionDepth;     // push them away from each other
                scene->_enemies[j]->setPosition(scene->_enemies[j]->getPosition() -= collisionDepthVec / 2.0f);
                scene->_enemies[j]->setDirection(scene->_enemies[i]->getDirection());
                if (!collision) {
                    scene->_enemies[i]->setPosition(scene->_enemies[i]->getPosition() += collisionDepthVec / 2.0f);
                }
            }
        }
        // moving the enemy in the right direction
        if (scene->_enemies[i]->getDirection() == 'D') {
            if (!collision)
                scene->_enemies[i]->changePosZ(scene->_enemies[i]->getSpeed() * Zion::Renderable::deltaTime);
            scene->_enemies[i]->setPosition(glm::vec3(std::round(scene->_enemies[i]->getPosition().x), 0,
                                                          scene->_enemies[i]->getPosition().z));
            scene->_enemies[i]->rotate(glm::radians(0.0f), {0, 1, 0});
        } else if (scene->_enemies[i]->getDirection() == 'U') {
            if (!collision)
                scene->_enemies[i]->changePosZ(-(scene->_enemies[i]->getSpeed()) * Zion::Renderable::deltaTime);
            scene->_enemies[i]->setPosition(glm::vec3(std::round(scene->_enemies[i]->getPosition().x), 0,
                                                          scene->_enemies[i]->getPosition().z));
            scene->_enemies[i]->rotate(glm::radians(180.0f), {0, 1, 0});
        } else if (scene->_enemies[i]->getDirection() == 'L') {
            if (!collision)
                scene->_enemies[i]->changePosX(-(scene->_enemies[i]->getSpeed()) * Zion::Renderable::deltaTime);
            scene->_enemies[i]->setPosition(glm::vec3(scene->_enemies[i]->getPosition().x, 0, std::round(scene->_enemies[i]->getPosition().z)));
            scene->_enemies[i]->rotate(glm::radians(-90.0f), {0, 1, 0});
        } else if (scene->_enemies[i]->getDirection() == 'R') {
            if (!collision)
                scene->_enemies[i]->changePosX(scene->_enemies[i]->getSpeed() * Zion::Renderable::deltaTime);
            scene->_enemies[i]->setPosition(glm::vec3(scene->_enemies[i]->getPosition().x, 0,
                                                          std::round(scene->_enemies[i]->getPosition().z)));
            scene->_enemies[i]->rotate(glm::radians(90.0f), {0, 1, 0});
        }
        MainGame::renderer.applyTransformationToRenderable(scene->_enemies[i]->getType(),
                                                               scene->_enemies[i]->getId(),
                                                               scene->_enemies[i]->getTransformation());
        collision = false;
    }
}
