
#include <scene.hpp>

void    Scene::enemyFollowPlayer(bool collision, std::map<int, std::map<int, Block *>> _blocks, Player *_enemies, Player *_player, int x, int y) {
    if (!collision && _blocks[y][x + 1] == nullptr && (      // making them follow the player
            (int)round(_enemies->getPosition().z) == (int)round(_player->getPosition().z) &&
            std::abs(_enemies->getPosition().x) - std::abs(_player->getPosition().x) <= 0.00f))
    {

        _enemies->setDirection('R');
    }

    if (!collision && _blocks[y][x - 1] == nullptr && (
            (int)round(_enemies->getPosition().z) == (int)round(_player->getPosition().z) &&
                    std::abs(_player->getPosition().x) - std::abs(_enemies->getPosition().x)  <= 0.00f))
    {

        _enemies->setDirection('L');
    }
    if (!collision && _blocks[y + 1][x] == nullptr && (
            (int)round(_enemies->getPosition().x) == (int)round(_player->getPosition().x) &&
                    std::abs(_enemies->getPosition().z) - std::abs(_player->getPosition().z) <= 0.00f))
    {

        _enemies->setDirection('U');
    }

    if (!collision && _blocks[y - 1][x] == nullptr && (
            (int)round(_enemies->getPosition().x) == (int)round(_player->getPosition().x) &&
                    std::abs(_player->getPosition().z) - std::abs(_enemies->getPosition().z)  <= 0.00f))
    {
        _enemies->setDirection('D');
    }

}