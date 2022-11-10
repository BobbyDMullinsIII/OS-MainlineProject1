#ifndef PAGETABLEENTRY_HPP
#define PAGETABLEENTRY_HPP
#pragma once
#include <string>
class PageTableEntry
{
	bool validBit;							//Tells if there's anything actually in this entry
	bool read;
	bool write;
	bool execute;
	bool dirtyBit;							//Tells if Page has been written to
	int timer;								//Tracks how long page has been in table
	int physicalAddress;			//The physical address of the data
public:
	PageTableEntry();
	PageTableEntry(bool r, bool w, bool x, int address);
	~PageTableEntry();
	void changeValid(bool valid);
	void changeRead(bool r);
	void changeWrite(bool w);
	void changeExecute(bool x);
	void changeDirty(bool dirty);
	void incrementTimer();
	void resetTimer();
	void changeAddress(int address);
	bool getValid();
	bool getRead();
	bool getWrite();
	bool getExecute();
	int getTimer();
	int getAddress();
	std::string toString();
};



#endif
