#include "Memory.hpp"
#include <vector>
using namespace std;

Memory::Memory()
{
	this->size = -1;
	this->full = false;
}

Memory::Memory(int x)
{
	this->full = false;
	this->size = x;
	for (int i = 0; i < x; i++)
	{
		this->memory.push_back(false);
	}
}

void Memory::assignLocation(int x)
{
	this->memory[x] = true;
}

int Memory::findFree()
{
	for (int i = 0; i < this->size; i++)
	{
		if (!memory[i])
		{
			return i;
		}
	}
	this->full = true;
	return -1;
}