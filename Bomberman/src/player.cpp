#include <player.hpp>

Player::Player(int id, std::string type) : NonStatic(id, type){
  _direction = 'U';
}

void Player::setPosition(glm::vec3 pos)
{
	_pos = pos;
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

char Player::setDirection(char c) {
    _direction = c;
}

char Player::getDirection() {
    return  _direction;
}

glm::vec3 Player::getPosition() { return _pos; }

