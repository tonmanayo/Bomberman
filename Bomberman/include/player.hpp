#pragma once

#include <nonStatic.hpp>

class   Player : public NonStatic
{
private:
	float   _posX;
	float   _posY;
	float   _posZ;

public:
	Player(int id, std::string type);
	~Player() = default;

	void    setPosition(float x, float y, float z);
	void    changePosX(float val);
	void    changePosY(float val);
	void    changePosZ(float val);
};