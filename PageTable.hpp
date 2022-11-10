#ifndef PAGETABLE_HPP
#define PAGETABLE_HPP


#pragma once

#include <string>
#include <vector>
#include "PageTableEntry.hpp"
/// <summary>
/// This class represents a page table
/// The constructor will take 1 arguments, this argument is the number of entries that will be present in the table
/// </summary>
class PageTable
{
	int index;						//The index of this Table in the parent page directory
	int numEntries;					//Number of entries in the table
	bool validBit;					//Tells if there is anything present within the Table
	std::vector<PageTableEntry> table;
public:
	PageTable(int newIndex, int entries);
	~PageTable();

	bool getValid();
	bool checkForEntry(int x);						//Checks if the entry at index x is valid
	bool seeIfAnythingIsValid();					//Checks the table to see if there is anything that is valid
	PageTableEntry getEntry(int x);					//Retrieves the entry from the specified index and returns it
	PageTableEntry swapIn(PageTableEntry x, int y);			//Swaps the given entry into the given index
	void removeEntry(int x);						//NULLs the entry at the given index - Unsure if needed but I'll make it anyways
	void updateTimers();							//Increments the timer variable for all valid entries
	int checkForEmpty();							//Returns the first empty entry in the table
	int placeInTable(PageTableEntry x);				//Places the given entry into the first available slot in the table
	bool placeInTable(PageTableEntry x, int y);		//Places the given entry into the specified index within the table
	int findLRUEntry();								//returns the index of the entry that was used least recently
	int getTimer(int index);						//returns the timer of the PTE at the given index
	std::string toString();								//Returns string representation of the table
};

#endif