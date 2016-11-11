#pragma once

enum BlockType
{
	BLOCK_DEFAULT,
	BLOCK_DIRT,
	BLOCK_STONE
};

class Block
{
public:
	Block();
	~Block();

	bool isActive();	
	BlockType getType();

	void setActive(bool state);

private:
	bool _active;	
	BlockType _type;
};

