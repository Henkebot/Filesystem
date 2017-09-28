#ifndef Inode_H
#define Inode_H
#include <ctime>
#include "block.h"
#define NOT_USED -1

class Inode
{
private:
	//Metadata
	Inode(char *&type,  char *&name, int id, int up);
	Inode(const Inode &other);

	~Inode();

	char* type;
	char* name;
	int id;
	int up;

	time_t timestamp;
	
	int usedBlocks;

	int nrOfBlocks;
	int blockIndexes[12];

	Inode& operator=(const Inode &other);
public:
	bool setName(const char *&name);
	int getID() const;
	time_t getTimeStamp() const;
	char* getName() const;
	int getUsedBlocks() const;
	int getNrOfBlocks() const;
	int* getBlockIndexes() const;

	bool setBlock(int adress);

private:
	void cleanup();
	void copy(const Inode &other);


};

#endif