#include <iostream>
#include <fstream>
#include <string>
#include "PageTable.hpp"
#include "PageTableEntry.hpp"
using namespace std;



/// <summary>
/// Constructor for a PageTable
/// </summary>
/// <param name="newName">The name of the table - the index of the table in the page directory it is located at</param>
/// <param name="entries"></param>
PageTable::PageTable(int newIndex, int entries)
{
	this->index = newIndex;
	this->numEntries = entries;
	this->validBit = false;
	for (int i = 0; i < numEntries; i++)
	{
		this->table.push_back(PageTableEntry());
	}
}

/// <summary>
/// Deconstructor
/// </summary>
PageTable::~PageTable() {}

/// <summary>
/// Returns the value validBit
/// </summary>
/// <returns></returns>
bool PageTable::getValid()
{
	return this->validBit;
}

/// <summary>
/// Checks to see if there is an empty entry in the table
/// </summary>
/// <returns>
/// index if there is an empty position
///	-1 if full	
/// </returns>
int PageTable::checkForEmpty()
{
	for (int i = 0; i < this->numEntries; i++)
	{
		if (this->table[i].getValid() == false)
		{
			return i;
		}
	}
	return -1;
}

/// <summary>
/// Checks if the specified entry in the page table is valid or not
/// </summary>
/// <param name="x">index</param>
/// <returns>
/// true if entry is valid
/// false if not
/// </returns>
bool PageTable::checkForEntry(int x)
{
	if (x >= this->numEntries)
	{
		//throw exception("Seg Fault");
		cout << "Seg Fault\n";
	}
	if (this->table[x].getValid() == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/// <summary>
/// Iterates through entire tablet to see if there are any valid entries
/// </summary>
/// <returns>
/// True if there is at least one valid entry in the table
/// False if there are no valid entries in the table
/// </returns>
bool PageTable::seeIfAnythingIsValid()
{
	for (int i = 0; i < this->numEntries; i++)
	{
		if (checkForEntry(i))
		{
			this->validBit = true;
			return true;
		}
	}
	this->validBit = false;
	return false;
}

/// <summary>
/// Grabs the entry from the specified index
/// </summary>
/// <param name="x">index</param>
/// <returns>
/// Returns entry if entry is valid
/// Returns null if entry is empty/invalid
/// </returns>
PageTableEntry PageTable::getEntry(int x)
{
	if (checkForEntry(x))
	{
		this->table[x].resetTimer();
		return this->table[x];
	}
	else
	{
		return PageTableEntry();
	}
}

/// <summary>
/// Places the given entry into the first available slot in the table
/// </summary>
/// <param name="x">Entry to put in table</param>
/// <returns>
/// Returns the index of the position the entry was placed in if there was space
/// Returns -1 if there was no space
/// </returns>
int PageTable::placeInTable(PageTableEntry x)
{
	int num = this->checkForEmpty();
	if (num == -1)
	{
		return -1;
	}
	else
	{
		this->table[num] = x;
		this->validBit = true;
		return num;
	}
}

bool PageTable::placeInTable(PageTableEntry x, int y)
{
	if (this->table[y].getValid() == true)
	{
		return false;
	}
	else
	{
		this->table[y] = x;
		this->validBit = true;
		return true;
	}
}

/// <summary>
/// Finds the entry that was accessed least recently
/// </summary>
/// <returns>
/// Returns the index of the entry that was used least recently
/// If the entire table is empty returns -1
/// </returns>
int PageTable::findLRUEntry()
{
	int time = -1;
	int position = -1;
	for (int i = 0; i < this->numEntries; i++)
	{
		if (checkForEntry(i))
		{
			if (this->table[i].getTimer() > time)
			{
				time = this->table[i].getTimer();
				position = i;
			}
		}
	}
	if (position == -1)
	{
		this->validBit = true;
	}
	return position;
}

/// <summary>
/// Places the given entry into the specified index of the table
/// </summary>
/// <param name="x">Entry to swap in</param>
/// <param name="y">Index entry was placed at</param>
/// <returns>
/// Returns the entry that was replaced if the entry was valid
/// Returns null if there was no valid entry at index x
/// </returns>
PageTableEntry PageTable::swapIn(PageTableEntry x, int y)
{
	if (checkForEntry(y))
	{
		PageTableEntry temp = table[y];
		this->table[y] = x;
		return temp;
	}
	else
	{
		this->table[y] = x;
		return PageTableEntry();
	}
}

/// <summary>
/// Nulls out the entry at index x
/// </summary>
/// <param name="x">index</param>
void PageTable::removeEntry(int x)
{
	if (checkForEntry(x))
	{
		this->table[x] = PageTableEntry();
	}
	this->seeIfAnythingIsValid();
}

/// <summary>
/// Incrememnts the timers of all valid entries by 1
/// </summary>
void PageTable::updateTimers()
{
	for (int i = 0; i < this->numEntries; i++)
	{
		if (this->table[i].getValid() == true)
		{
			this->table[i].incrementTimer();
		}
	}
}


/// <summary>
/// To string representation of the entire table
/// </summary>
/// <returns></returns>
string PageTable::toString()
{
	string ts = "";
	for (int i = 0; i < this->numEntries; i++)
	{
		ts += "| ";
		ts += std::to_string(i);
		ts += " ";
		ts += this->table[i].toString();
		ts += "\n";
	}
	return ts;
}

int PageTable::getTimer(int index)
{
	if (this->checkForEntry(index))
	{
		return this->table[index].getTimer();
	}
	else
	{
		return -1;
	}
}

/*
int main()
{

	PageTable x(1, 16);
	PageTableEntry a1(true, true, true, "thisIsA1");
	PageTableEntry a2(true, false, true, "thisIsA2");
	PageTableEntry a3(true, true, false, "thisIsA3");
	PageTableEntry a4(false, true, true, "thisIsA4");
	PageTableEntry a5(false, false, false, "thisIsA5");

	cout << std::to_string(x.placeInTable(a1));
	cout << "\n";
	x.updateTimers();
	cout << std::to_string(x.placeInTable(a2));
	cout << "\n";
	x.updateTimers();
	cout << std::to_string(x.placeInTable(a3));
	cout << "\n";
	x.updateTimers();
	cout << std::to_string(x.placeInTable(a4));
	cout << "\n";
	x.updateTimers();

	cout << std::to_string(x.findLRUEntry());
	cout << "\n";
	cout << x.getEntry(x.findLRUEntry()).toString();
	cout << "\n";

	cout << x.swapIn(a5, x.findLRUEntry()).toString();
	cout << "\n\n";

	for (int i = 4; i < 16; i++)
	{
		x.placeInTable(PageTableEntry(true, true, true, "thisIsXX"));
	}

	cout << std::to_string(x.placeInTable(PageTableEntry(true, true, true, "thisIsX1")));

	cout << "\n\n";
	cout << x.toString();
	cout << "\n";
}
*/