#include <block.hpp>

Block::Block(int id, std::string type, bool breakable) : NonStatic(id, type), _breakable(breakable)
{}

void Block::setPosition(float x, float y, float z)
{
	translate(glm::vec3(x, y, z));
}