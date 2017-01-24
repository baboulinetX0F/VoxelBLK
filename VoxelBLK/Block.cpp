#include "Block.h"



Block::Block()
{	
	
}


Block::~Block()
{
}

bool Block::isActive() const
{
	return _active;
}

BlockType Block::getType() const
{
	return _type;
}

void Block::setActive(bool state)
{
	_active = state;
}
