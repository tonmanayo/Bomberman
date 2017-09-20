//
// Created by Tony MACK on 2017/09/04.
//

#include <scene.hpp>


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
    bool collision = false;

    char dir[3] = {'L', 'L', 'L'};

    for (int i = 0; i < scene->_enemies.size(); i++) {

        int y = scene->getWorldy(scene->_enemies[i]->getPosition().z);
        int x = scene->getWorldx(scene->_enemies[i]->getPosition().x);


        if (scene->enemyPlayerCollision(scene->_enemies[i], scene)) {
            MainGame::soundEngine->play2D("resource/sounds/playerInjured.wav");
            scene->_player->decHP(scene->getDifficulty());
            break;
        }
        if (worldCollisionDown(scene->_enemies[i], {0.0f, 0.0f, 2.0f * Zion::Renderable::deltaTime}, scene)) {
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
        else if (worldCollisionUp(scene->_enemies[i], {0.0f, 0.0f, -2.0f * Zion::Renderable::deltaTime}, scene)) {
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
        } else if (worldCollisionRight(scene->_enemies[i], {2.0f * Zion::Renderable::deltaTime, 0.0f, 0.0f}, scene)) {
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
        } else if (worldCollisionLeft(scene->_enemies[i], {-2.0f * Zion::Renderable::deltaTime, 0.0f, 0.0f}, scene)) {
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

        if (!collision && scene->_blocks[y][x + 1] == nullptr && (      // making them follow the player
                (int)round(scene->_enemies[i]->getPosition().z) == (int)round(scene->_player->getPosition().z) &&
                        abs(scene->_enemies[i]->getPosition().x) - abs(scene->_player->getPosition().x) <= 0.00f))
        {

            scene->_enemies[i]->setDirection('R');
        }

        if (!collision && scene->_blocks[y][x - 1] == nullptr && (
                (int)round(scene->_enemies[i]->getPosition().z) == (int)round(scene->_player->getPosition().z) &&
                abs(scene->_player->getPosition().x) - abs(scene->_enemies[i]->getPosition().x)  <= 0.00f))
        {

            scene->_enemies[i]->setDirection('L');
        }
        if (!collision && scene->_blocks[y + 1][x] == nullptr && (
                (int)round(scene->_enemies[i]->getPosition().x) == (int)round(scene->_player->getPosition().x) &&
                abs(scene->_enemies[i]->getPosition().z) - abs(scene->_player->getPosition().z) <= 0.00f))
        {

            scene->_enemies[i]->setDirection('U');
        }

        if (!collision && scene->_blocks[y - 1][x] == nullptr && (
                (int)round(scene->_enemies[i]->getPosition().x) == (int)round(scene->_player->getPosition().x) &&
                abs(scene->_player->getPosition().z) - abs(scene->_enemies[i]->getPosition().z)  <= 0.00f))
        {
            scene->_enemies[i]->setDirection('D');
        }
        // collision enemy with enemy
        for (int j = 0; j < scene->_enemies.size(); j++) {
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
                scene->_enemies[i]->changePosZ(2.0f * Zion::Renderable::deltaTime);
            scene->_enemies[i]->setPosition(glm::vec3(std::round(scene->_enemies[i]->getPosition().x), 0,
                                                          scene->_enemies[i]->getPosition().z));
            scene->_enemies[i]->rotate(glm::radians(0.0f), {0, 1, 0});
        } else if (scene->_enemies[i]->getDirection() == 'U') {
            if (!collision)
                scene->_enemies[i]->changePosZ(-2.0f * Zion::Renderable::deltaTime);
            scene->_enemies[i]->setPosition(glm::vec3(std::round(scene->_enemies[i]->getPosition().x), 0,
                                                          scene->_enemies[i]->getPosition().z));
            scene->_enemies[i]->rotate(glm::radians(180.0f), {0, 1, 0});
        } else if (scene->_enemies[i]->getDirection() == 'L') {
            if (!collision)
                scene->_enemies[i]->changePosX(-2.0f * Zion::Renderable::deltaTime);
            scene->_enemies[i]->setPosition(glm::vec3(scene->_enemies[i]->getPosition().x, 0, std::round(scene->_enemies[i]->getPosition().z)));
            scene->_enemies[i]->rotate(glm::radians(-90.0f), {0, 1, 0});
        } else if (scene->_enemies[i]->getDirection() == 'R') {
            if (!collision)
                scene->_enemies[i]->changePosX(2.0f * Zion::Renderable::deltaTime);
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
