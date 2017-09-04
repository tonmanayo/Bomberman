#pragma once

#include <nonStatic.hpp>

class   Block : public NonStatic
{
private:
	bool    _breakable;
    int     _id;
public:
	Block(int id, std::string type, bool breakable);
	~Block() = default;

    int         getId();
	void        setPosition(float x, float y, float z);
	bool        isBreakable();
};