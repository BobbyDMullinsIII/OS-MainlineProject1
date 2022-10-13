#ifndef PAGEDIRECTORY_HPP
#define PAGEDIRECTORY_HPP

#pragma once

#include "PageTable.hpp"
#include "PageTableEntry.hpp"
#include <string>
#include <vector>
class PageDirectory
{
	int name;							//Name index of array in the parent directory) if this is the top level directory then name is -1
	int entries;						//Number of entries in the Page Directory
	int level;							//The level of the PageDirectory level one directory would contain array of pageTables, anything above contains array of pageDirectories
	bool validBit;						//Tells if the directory has anything initialized within it (One of the pageDirectory/PageTables within contains a valid entry
	bool full;							//Tells if the directory is full
	int childTableSize;					//The size of any pageTables a level 1 directory would contain
	std::vector<PageTable> PTTable;			//For Level one Directories, array of pagetables
	std::vector<PageDirectory> PDTable;		//For level 2+ directories

public:
	PageDirectory(int newLevel, int newEntries, int numLevels, int ptSize);
	~PageDirectory();

	bool checkIfIndexIsValid(int x);								//Checks the given index to see if it is valid (contains anything)
	bool seeIfAnythingIsValid();									//Checks to see if anything within PT/PD is valid
	bool checkForEmpty();											//Sees if there is any available space within PTTable/PDTable
	void updateTimers();											//Update timers in every entry in the directory
	PageTableEntry grabEntry(std::vector<int> address);						//Returns the entry at the given address, there should be level+1 entries in the array
	PageTableEntry swapEntry(PageTableEntry x, std::vector<int> address);		//Swaps the given entry into the given address and returns the entry that was swapped out
	void removeEntry(std::vector<int> address);							//Removes the entry at the given address and returns it
	std::vector<int> findLRUEntry();										//Iterates entire array and finds the LRU entry
	std::vector<int> placeEntry(PageTableEntry x);						//Sticks the entry into the first available position and returns the address in some form
	std::string toString();
	std::string toString(std::vector<PageTable> x);
	std::string toString(std::vector<PageDirectory> x);

};


#endif