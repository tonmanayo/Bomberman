#include <player.hpp>

Player::Player(int id, std::string type) : NonStatic(id, type){}

void Player::setPosition(float x, float y, float z)
{
	_pos = glm::vec3(x, y, z);
	translate(_pos);
}

void Player::changePosX(float val)
{
	_pos.x += val;
	translate(_pos);
}
void Player::changePosY(float val)
{
	_pos.y += val;
	translate(_pos);
}
void Player::changePosZ(float val)
{
	_pos.z += val;
	translate(_pos);
}

void Player::reset() {
    _pos = playerStart;
    translate(playerStart);
}

glm::vec3 Player::getPosition() { return _pos; }

