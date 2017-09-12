#pragma once

#include <nonStatic.hpp>
#include <MainGame.hpp>

class   Player : public NonStatic
{
private:
	glm::vec3       _pos;
    char        	_direction;
	int 			_powerBombExplosion;
	int 			_powerBombNumber;
	float 			_powerSpeed;
	int 			_hp;
public:
    glm::vec3   playerStart;
    glm::vec3  	playerScale;

    char   setDirection(char c);

    char   getDirection();

	int getHP();
	void incHP();
	void decHP(int );

public:
	Player(int id, std::string type);
	~Player() = default;

	void        setPosition(glm::vec3 pos);
	void        changePosX(float val);
	void        changePosY(float val);
	void        changePosZ(float val);
	void 		mv();
    void        reset();
	void		incBombExplosion();
	void		incBombNbr();
	void		incBombSpeed();
	int			getPowerExplosion();
	int			getPowerBombNbr();
	float		getPowerSpeed();

};