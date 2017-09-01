#pragma once

#include <nonStatic.hpp>

class   Player : public NonStatic
{
private:
	glm::vec3       _pos;
    float         	_directionx;
    float        	_directiony;
public:
    glm::vec3   playerStart;
    glm::vec3  playerScale;

    void    setDirectionx(float i);
    void    setDirectiony(float i);
    float   getDirectionx();
    float   getDirectiony();

public:
	Player(int id, std::string type);
	~Player() = default;

	void        setPosition(float x, float y, float z);
	void        changePosX(float val);
	void        changePosY(float val);
	void        changePosZ(float val);
	void 		mv();
    void        reset();
	glm::vec3   getPosition();
};