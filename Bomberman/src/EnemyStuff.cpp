//
// Created by Tony MACK on 2017/09/04.
//

#include <scene.hpp>

bool Scene::enemyWorldCollisionDown(glm::vec3 pos, glm::vec3 offset, Scene *scene)
{
    glm::vec3 newPos = pos + offset;

    int x = scene->getWorldx(pos.x);
    int y = scene->getWorldy(pos.z);

    return (scene->_blocks[y -  1][x] != nullptr && checkBlockCollision(scene->_blocks[y - 1][x]->getPosition(), newPos));
}

bool Scene::enemyWorldCollisionUp(glm::vec3 pos, glm::vec3 offset, Scene *scene)
{
    glm::vec3 newPos = pos + offset;

    int x = scene->getWorldx(pos.x);
    int y = scene->getWorldy(pos.z);
    //"collide up"
    return (scene->_blocks[y + 1][x] != nullptr && checkBlockCollision(scene->_blocks[y + 1][x]->getPosition(), newPos));
}

bool Scene::enemyWorldCollisionLeft(glm::vec3 pos, glm::vec3 offset, Scene *scene)
{
    glm::vec3 newPos = pos + offset;

    int x = scene->getWorldx(pos.x);
    int y = scene->getWorldy(pos.z);

    return (scene->_blocks[y][x - 1] != nullptr && checkBlockCollision(scene->_blocks[y][x - 1]->getPosition(), newPos));
}

bool Scene::enemyWorldCollisionRight(glm::vec3 pos, glm::vec3 offset, Scene *scene)
{
    glm::vec3 newPos = pos + offset;

    int x = scene->getWorldx(pos.x);
    int y = scene->getWorldy(pos.z);

    return (scene->_blocks[y][x + 1] != nullptr && checkBlockCollision(scene->_blocks[y][x + 1]->getPosition(), newPos));
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

void Scene::collisionPush(Scene *scene, int x, int y, int i, int dirx, int diry) {
    const float MIN_DISTANCE = static_cast<float>(HALF_PLAYER_SIZE) * 2.0f;
    glm::vec3 centrePos1 = scene->_enemies[i]->getPosition() + glm::vec3(static_cast<float>(HALF_PLAYER_SIZE));
    glm::vec3 centrePos2 = scene->_blocks[y + diry][x + dirx]->getPosition() + glm::vec3(static_cast<float>(HALF_GRID_BLOCK_SIZE));
    glm::vec3 vecDist = centrePos1 - centrePos2;
    float dist = glm::length(vecDist);
    float collisionDepth = MIN_DISTANCE - dist;

    if (collisionDepth > 0) {
        glm::vec3 collisionDepthVec =
                glm::normalize(vecDist) * collisionDepth;     // push them away from each other
        scene->_enemies[i]->setPosition(scene->_enemies[i]->getPosition() += collisionDepthVec /
                                                                             2.0f);                                      // Push them in opposite directions
    }
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
        if (enemyWorldCollisionDown(scene->_enemies[i]->getPosition(), {0.0f, 0.0f, 2.0f * Zion::Renderable::deltaTime}, scene)) {
            collision = true;
            dir[0] = 'U';
            dir[1] = 'R';
            dir[2] = 'L';

            collisionPush(scene, x, y, i, 0, -1);
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
        if (enemyWorldCollisionUp(scene->_enemies[i]->getPosition(), {0.0f, 0.0f, -2.0f * Zion::Renderable::deltaTime}, scene)) {
            collision = true;

            dir[0] = 'D';
            dir[1] = 'R';
            dir[2] = 'L';

            collisionPush(scene, x, y, i, 0, 1);
            if (scene->_blocks[y - 1][x] != nullptr) {
                dir[0] = 'R';
            }
            if (scene->_blocks[y][x + 1] != nullptr) {
                dir[1] = 'L';
            }
            if (scene->_blocks[y][x - 1] != nullptr) {
                dir[2] = 'D';
            }
        } else if (enemyWorldCollisionRight(scene->_enemies[i]->getPosition(), {2.0f * Zion::Renderable::deltaTime, 0.0f, 0.0f}, scene)) {
            collision = true;

            dir[0] = 'L';
            dir[1] = 'U';
            dir[2] = 'D';

            collisionPush(scene, x, y, i, 1, 0);
            if (scene->_blocks[y][x - 1] != nullptr) {
                dir[0] = 'D';
            }
            if (scene->_blocks[y + 1][x] != nullptr) {
                dir[1] = 'L';
            }
            if (scene->_blocks[y - 1][x] != nullptr) {
                dir[2] = 'U';
            }
        } else if (enemyWorldCollisionLeft(scene->_enemies[i]->getPosition(), {-2.0f * Zion::Renderable::deltaTime, 0.0f, 0.0f}, scene)) {
            collision = true;

            collisionPush(scene, x, y, i, -1, 0);
            dir[0] = 'R';
            dir[1] = 'D';
            dir[2] = 'U';

            scene->_enemies[i]->setPosition(glm::vec3(std::round(scene->_enemies[i]->getPosition().x), 0,
                                                      scene->_enemies[i]->getPosition().z));

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

        if (collision) {
            int randNbr = rand() % 3;
            scene->_enemies[i]->setDirection(dir[randNbr]);
        }
//        if (!collision && (fmod(scene->_enemies[i]->getPosition().x, 2.0f) == 0) && (
//                scene->_enemies[i]->getDirection() == 'L' ||
//                scene->_enemies[i]->getDirection() == 'R')) {
//                scene->_enemies[i]->setDirection('D');
//            }
//
//        if(!collision && (int)std::round(scene->_enemies[i]->getPosition().x) % 2 == 0 &&
//                scene->_blocks[y - 1][x] == nullptr) {
//            scene->_enemies[i]->setDirection('D');
//        }
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

        if (scene->_enemies[i]->getDirection() == 'D') {
                scene->_enemies[i]->changePosZ(2.0f * Zion::Renderable::deltaTime);
                scene->_enemies[i]->setPosition(glm::vec3(std::round(scene->_enemies[i]->getPosition().x), 0,
                                                          scene->_enemies[i]->getPosition().z));
            scene->_enemies[i]->rotate(glm::radians(0.0f), {0, 1, 0});
        } else if (scene->_enemies[i]->getDirection() == 'U') {
                scene->_enemies[i]->changePosZ(-2.0f * Zion::Renderable::deltaTime);
                scene->_enemies[i]->setPosition(glm::vec3(std::round(scene->_enemies[i]->getPosition().x), 0,
                                                          scene->_enemies[i]->getPosition().z));
            scene->_enemies[i]->rotate(glm::radians(180.0f), {0, 1, 0});
        } else if (scene->_enemies[i]->getDirection() == 'L') {
                scene->_enemies[i]->changePosX(-2.0f * Zion::Renderable::deltaTime);
                scene->_enemies[i]->setPosition(glm::vec3(scene->_enemies[i]->getPosition().x, 0, std::round(scene->_enemies[i]->getPosition().z)));
            scene->_enemies[i]->rotate(glm::radians(-90.0f), {0, 1, 0});
        } else if (scene->_enemies[i]->getDirection() == 'R') {
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
