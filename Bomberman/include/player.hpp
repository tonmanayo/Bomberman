#pragma once

#include <nonStatic.hpp>

class   Player : public NonStatic
{
private:
	glm::vec3       _pos;
    char        	_direction;
public:
    glm::vec3   playerStart;
    glm::vec3  playerScale;

    char   setDirection(char c);

    char   getDirection();

public:
	Player(int id, std::string type);
	~Player() = default;

	void        setPosition(glm::vec3 pos);
	void        changePosX(float val);
	void        changePosY(float val);
	void        changePosZ(float val);
	void 		mv();
    void        reset();
	glm::vec3   getPosition();
};