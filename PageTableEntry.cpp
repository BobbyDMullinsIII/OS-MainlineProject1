#include <iostream>
#include <fstream>
#include <string>
#include "PageTableEntry.hpp"
using namespace std;

PageTableEntry::PageTableEntry()
{
	this->validBit = false;
	this->read = false;
	this->write = false;
	this->execute = false;
	this->dirtyBit = false;
	this->timer = -1;
	this->physicalAddress = -1;
}

PageTableEntry::PageTableEntry(bool r, bool w, bool x, int address)
{
	this->validBit = true;
	this->read = r;
	this->write = w;
	this->execute = x;
	this->dirtyBit = false;
	this->timer = 0;
	this->physicalAddress = address;
}

PageTableEntry::~PageTableEntry() {}

void PageTableEntry::changeValid(bool valid)
{
	this->validBit = valid;
}

void PageTableEntry::changeRead(bool r)
{
	this->read = r;
}

void PageTableEntry::changeWrite(bool w)
{
	this->write = w;
}

void PageTableEntry::changeExecute(bool x)
{
	this->execute = x;
}

void PageTableEntry::changeDirty(bool dirty)
{
	this->dirtyBit = dirty;
}

void PageTableEntry::incrementTimer()
{
	this->timer++;
}

void PageTableEntry::resetTimer()
{
	this->timer = 0;
}

void PageTableEntry::changeAddress(int address)
{
	this->physicalAddress = address;
}

bool PageTableEntry::getValid()
{
	return this->validBit;
}

bool PageTableEntry::getRead()
{
	return this->read;
}

bool PageTableEntry::getWrite()
{
	return this->write;
}

bool PageTableEntry::getExecute()
{
	return this->execute;
}

int PageTableEntry::getTimer()
{
	return this->timer;
}

int PageTableEntry::getAddress()
{
	return this->physicalAddress;
}

std::string PageTableEntry::toString()
{
	if (this->validBit)
	{
		string ts = "| ";
		if (this->read)
		{
			ts += "r";
		}
		else
		{
			ts += " ";
		}
		if (this->write)
		{
			ts += "w";
		}
		else
		{
			ts += " ";
		}
		if (this->execute)
		{
			ts += "x";
		}
		else
		{
			ts += " ";
		}
		ts += " | ";
		if (this->dirtyBit)
		{
			ts += "dirty";
		}
		else
		{
			ts += "clean";
		}
		ts += " | ";
		ts += std::to_string(this->getTimer());
		ts += " | ";
		ts += this->physicalAddress;
		ts += " |";
		return ts;
	}
	else
	{
		return "| EMPTY |";
	}
}