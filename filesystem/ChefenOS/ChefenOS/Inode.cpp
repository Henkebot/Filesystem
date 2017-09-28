#include "Inode.h"

Inode::Inode(char *&type, char*& name, int id, int up) : type(type), name(name), id(id), up(up)
{
	
	timestamp = time(0);
	for (int i = 0; i < nrOfBlocks; i++)
	{
		blockIndexes[i] = NOT_USED;
	}
}

Inode::~Inode()
{
	cleanup();
}

void Inode::cleanup()
{
	delete type;
	delete name;
}

void Inode::copy(const Inode & other)
{
	// Yttlig kopiering
	name = other.name;
	type = other.type;
	id = other.id;
	up = other.up;

	timestamp = other.timestamp;
	usedBlocks = other.usedBlocks;
	nrOfBlocks = other.nrOfBlocks;
	for (int i = 0; i < nrOfBlocks; i++)
	{
		blockIndexes[i] = other.blockIndexes[i];
	}
	
}

Inode & Inode::operator=(const Inode & other)
{
	if (this != &other)
	{
		cleanup();
		copy(other);
	}

	return *this;
}

void Inode::setName(char *& name)
{
	this->name = name;
}
