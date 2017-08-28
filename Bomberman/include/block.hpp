#pragma once

#include <nonStatic.hpp>

class   Block : public NonStatic
{
private:
	bool    _breakable;
public:
	Block(int id, std::string type, bool breakable);
	~Block() = default;

	void        setPosition(float x, float y, float z);
	bool        isBreakable(){ return _breakable; }
};