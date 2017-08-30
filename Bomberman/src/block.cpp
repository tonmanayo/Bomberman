#include <block.hpp>

Block::Block(int id, std::string type, bool breakable) : NonStatic(id, type), _breakable(breakable)
{
    _id = id;
}

void Block::setPosition(float x, float y, float z)
{
	translate(glm::vec3(x, y, z));
}

bool        Block::isBreakable()
{
    return _breakable;
}

int         Block::getId()
{
    return  _id;
}

