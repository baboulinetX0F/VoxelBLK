#include "Block.h"



Block::Block()
{
	_active = false;
	_type = BlockType::BLOCK_DEFAULT;
}


Block::~Block()
{
}

bool Block::isActive()
{
	return _active;
}

BlockType Block::getType()
{
	return _type;
}

void Block::setActive(bool state)
{
	_active = state;
}
