#include <player.hpp>

Player::Player(int id, std::string type) : NonStatic(id, type) {}

void Player::setPosition(float x, float y, float z)
{
	_posX = x;
	_posY = y;
	_posZ = z;
	translate(glm::vec3(x, y, z));
}

void Player::changePosX(float val)
{
	_posX += val;
	translate(glm::vec3(_posX, _posY, _posZ));
}
void Player::changePosY(float val)
{
	_posY += val;
	translate(glm::vec3(_posX, _posY, _posZ));
}
void Player::changePosZ(float val)
{
	_posZ += val;
	translate(glm::vec3(_posX, _posY, _posZ));
}