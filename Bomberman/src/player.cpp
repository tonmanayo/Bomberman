#include <player.hpp>

Player::Player(int id, std::string type) : NonStatic(id, type){
  	_direction = 'U';
	_powerBombExplosion = 0;
	_powerBombNumber = 0;
	_powerSpeed = 0;
	_hp = 100;
}

int Player::getHP() {
	return _hp;
}

void Player::incHP() {
	if (_hp + 25 < 100)
		_hp += 25;
	else if (_hp + 25 > 100)
		_hp = 100;
}

void Player::decHP(int x) {
	if (_hp - x > 0)
		_hp -= x;
	else if (_hp - x <= 0)
		_hp = 0;
}

void Player::incBombExplosion() {
	_powerBombExplosion++;
}

void Player::incBombNbr(){
	_powerBombNumber++;
}

void Player::incPowerSpeed(){
	_powerSpeed += 1.5f;
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
	return c;
}

char Player::getDirection() {
    return  _direction;
}

int			Player::getPowerExplosion() {
	return _powerBombExplosion;
}
int			Player::getPowerBombNbr() {
	return _powerBombNumber;
}
float			Player::getPowerSpeed() {
	return _powerSpeed;
}

void Player::setPowerExplosion(int val)
{
	_powerBombExplosion = val;
}

void Player::setPowerBombNbr(int val)
{
	_powerBombNumber = val;
}

void Player::setPowerSpeed(float val)
{
	_powerSpeed = val;
}

void Player::setHp(int val)
{
	_hp = val;
}

float		Player::getSpeed() {
	return _speed;
}
void		Player::setSpeed(float val) {
	_speed = val;
}