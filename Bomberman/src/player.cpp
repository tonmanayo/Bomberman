#include <player.hpp>

Player::Player(int id, std::string type) : NonStatic(id, type){
    _directiony = 0.02f;
    _directionx = 0.0f;
}

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

void Player::setDirectionx(float i) {
    _directionx = i;
}

void Player::setDirectiony(float i){
    _directiony = i;
}

float Player::getDirectionx() {
    return  _directionx;
}

float Player::getDirectiony() {
    return  _directiony;
}

glm::vec3 Player::getPosition() { return _pos; }

