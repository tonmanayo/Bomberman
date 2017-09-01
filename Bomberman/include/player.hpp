#pragma once

#include <nonStatic.hpp>

class   Player : public NonStatic
{
private:
	glm::vec3   _pos;
public:
    glm::vec3   playerStart;
    glm::vec3  playerScale;
public:
	Player(int id, std::string type);
	~Player() = default;

	void        setPosition(float x, float y, float z);
	void        changePosX(float val);
	void        changePosY(float val);
	void        changePosZ(float val);
    void        reset();
	glm::vec3   getPosition();
};