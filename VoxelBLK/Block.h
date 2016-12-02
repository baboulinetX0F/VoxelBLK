#pragma once

enum BlockType
{
	BLOCK_DEFAULT,
	BLOCK_DIRT,
	BLOCK_STONE
};

struct BlockFaces
{
	bool top;
	bool down;
	bool left;
	bool right;
	bool front;
	bool back;
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
	bool _active = false;
	BlockType _type = BlockType::BLOCK_DEFAULT;
};

