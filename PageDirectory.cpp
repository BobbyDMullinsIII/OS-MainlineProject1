#include <iostream>
#include <fstream>
#include <string>
#include "PageDirectory.hpp"
#include "PageTable.hpp"
#include "PageTableEntry.hpp"
using namespace std;
/*
You access individual PTE's in this system by using an address given in a vector<int>
the form of this address is similar to that of what is used in an actual multi-level page table system, but converted to a vector
Essentially the system will read the address from left to right, using each element as the index it needs to access within each table/directory
Say you have a 4 level system
This means that are 4 Page Directories you must navigate through before encountering an actual page table
The vector address will be of size 5
The first 4 elements will be used to navigate through the page directories (the first element tells which index in the top level directory, second element tells which index in the next directory, etc.)
The 4th element is the index of an actual page table
The final (5th) element gives the index of the PTE within the page table given at level 4
*/

/// <summary>
/// Constructor
/// </summary>
/// <param name="newName"></param>
/// <param name="numEntries"></param>
/// <param name="numLevel"></param>
/// <param name="ptSize"></param>
PageDirectory::PageDirectory(int newName, int numEntries, int numLevel, int ptSize)
{
	this->name = newName;
	this->entries = numEntries;
	this->level = numLevel;
	this->childTableSize = ptSize;
	this->validBit = false;
	this->full = false;

	if (level > 1)
	{
		for (int i = 0; i < this->entries; i++)
		{
			this->PDTable.push_back(PageDirectory(i, this->entries, (this->level - 1), this->childTableSize));
		}
	}
	else
	{
		for (int i = 0; i < this->entries; i++)
		{
			this->PTTable.push_back(PageTable(i, this->childTableSize));
		}
	}
}

/// <summary>
/// Deconstructor
/// </summary>
PageDirectory::~PageDirectory() {};

bool PageDirectory::checkIfIndexIsValid(int x)
{
	if (x >= this->entries)
	{
		throw new exception("Seg fault");
	}
	if (this->level > 1)
	{
		if (this->PDTable[x].validBit == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (this->PTTable[x].getValid() == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

/// <summary>
/// Iterates through the array and checks to see if there is anything valid within the Directory
/// </summary>
/// <returns>
/// True if there is anything valid
/// False if not
/// </returns>
bool PageDirectory::seeIfAnythingIsValid()
{
	for (int i = 0; i < this->entries; i++)
	{
		if (this->checkIfIndexIsValid(i))
		{
			this->validBit = true;
			return true;
		}
	}
	this->validBit = false;
	return false;
}

/// <summary>
/// Checks if there is an empty PTE position within the directory
/// </summary>
/// <returns>
/// True if there is an empty position
/// False if there isn't
/// </returns>
bool PageDirectory::checkForEmpty()
{
	if (this->level > 1)
	{
		for (int i = 0; i < this->PDTable.size(); i++)
		{
			if (this->PDTable[i].checkForEmpty() == true)
			{
				return true;
			}
			else
			{
				this->validBit = true;
			}
		}
		this->full = true;
		return false;
	}
	else
	{
		for (int i = 0; i < this->PTTable.size(); i++)
		{
			if (this->PTTable[i].checkForEmpty() != -1)
			{
				return true;
			}
			else
			{
				this->validBit = true;
			}
		}
		this->full = true;
		return false;
	}
}

/// <summary>
/// Increments every PTE timer within the directory by 1
/// </summary>
void PageDirectory::updateTimers()
{
	if (level > 1)
	{
		for (int i = 0; i < this->entries; i++)
		{
			this->PDTable[i].updateTimers();
		}
	}
	else
	{
		for (int i = 0; i < this->entries; i++)
		{
			this->PTTable[i].updateTimers();
		}
	}
}

/// <summary>
/// Retrieves the PTE at the given address
/// </summary>
/// <param name="address"></param>
/// <returns>
/// PTE at the given address within the Page table system
/// </returns>
PageTableEntry PageDirectory::grabEntry(vector<int> address)
{
	int tempInt = this->level;
	tempInt++;
	if (address.size() != tempInt)
	{
		throw new exception("invalid address");
	}
	if (this->level > 1)
	{
		if (this->checkIfIndexIsValid(address[0]))
		{
			vector<int> temp;
			for (int i = 1; i < address.size(); i++)
			{
				temp.push_back(address[i]);
			}
			return this->PDTable[address[0]].grabEntry(temp);
		}
		else
		{
			this->seeIfAnythingIsValid();
			return PageTableEntry();
		}
	}
	else
	{
		if (this->checkIfIndexIsValid(address[0]))
		{
			return this->PTTable[address[0]].getEntry(address[1]);
		}
		else
		{
			this->seeIfAnythingIsValid();
			return PageTableEntry();
		}
	}
}

/// <summary>
/// Swaps the given entry into the given address and returns the Entry previously at that address
/// </summary>
/// <param name="x"></param>
/// <param name="address"></param>
/// <returns></returns>
PageTableEntry PageDirectory::swapEntry(PageTableEntry x, vector<int> address)
{
	int tempInt = this->level;
	tempInt++;
	if (address.size() != tempInt)
	{
		throw new exception("invalid address");
	}
	if (this->level > 1)
	{
		if (this->checkIfIndexIsValid(address[0]))
		{
			vector<int> temp;
			for (int i = 1; i < address.size(); i++)
			{
				temp.push_back(address[i]);
			}
			return this->PDTable[address[0]].swapEntry(x, temp);
		}
		else
		{
			this->seeIfAnythingIsValid();
			return PageTableEntry();
		}
	}
	else
	{
		if (this->checkIfIndexIsValid(address[0]))
		{
			return this->PTTable[address[0]].swapIn(x, address[1]);
		}
		else
		{
			this->seeIfAnythingIsValid();
			return PageTableEntry();
		}
	}


}

/// <summary>
/// Removes the entry at the given address
/// </summary>
/// <param name="address"></param>
void PageDirectory::removeEntry(vector<int> address)
{
	int tempInt = this->level;
	tempInt++;
	if (address.size() != tempInt)
	{
		throw new exception("invalid address");
	}
	if (this->level > 1)
	{
		if (this->checkIfIndexIsValid(address[0]))
		{
			vector<int> temp;
			for (int i = 1; i < address.size(); i++)
			{
				temp.push_back(address[i]);
			}
			this->PDTable[address[0]].removeEntry(temp);
			this->seeIfAnythingIsValid();
			this->full = false;
		}
		else
		{
			this->seeIfAnythingIsValid();
		}
	}
	else
	{
		if (this->checkIfIndexIsValid(address[0]))
		{
			this->PTTable[address[0]].removeEntry(address[1]);
			this->seeIfAnythingIsValid();
			this->full = false;
		}
		else
		{
			this->seeIfAnythingIsValid();
		}
	}
}

/// <summary>
/// Finds the address of the entry that was used least recently
/// </summary>
/// <returns></returns>
vector<int> PageDirectory::findLRUEntry()
{

	if (this->level > 1)
	{
		int position = -1;
		vector<int> address;
		vector<int> temp;
		for (int i = 0; i < this->PDTable.size(); i++)
		{
			if (this->PDTable[i].validBit)
			{
				temp = this->PDTable[i].findLRUEntry();
				if (temp[temp.size() - 1] != -1)
				{
					if (position == -1)
					{
						address = temp;
						position = i;
					}
					else
					{
						if (temp[temp.size() - 1] > address[address.size() - 1])
						{
							address = temp;
							position = i;
						}
					}
				}
			}
		}

		vector<int> x;
		if (position == -1)
		{
			for (int i = 0; i < (this->level + 1); i++)
			{
				x.push_back(0);
			}
			x.push_back(-1);
		}
		else
		{
			x.push_back(position);
			for (int i = 0; i < address.size(); i++)
			{
				x.push_back(address[i]);
			}
		}

		if (this->name == -1)
		{
			x.pop_back();
		}
		return x;
	}
	else
	{
		int directoryIndex = -1;
		int tableIndex = NULL;
		int value = NULL;
		int tempDI;
		int tempTI;
		int tempVal;
		for (int i = 0; i < this->PTTable.size(); i++)
		{
			if (this->PTTable[i].getValid())
			{
				tempDI = i;
				tempTI = this->PTTable[i].findLRUEntry();
				tempVal = this->PTTable[i].getTimer(tempTI);
				if (directoryIndex == -1)
				{
					directoryIndex = tempDI;
					tableIndex = tempTI;
					value = tempVal;
				}
				else
				{
					if (tempVal > value)
					{
						directoryIndex = tempDI;
						tableIndex = tempTI;
						value = tempVal;
					}
				}

			}
		}
		vector<int> x;
		if (directoryIndex == -1)
		{
			x.push_back(0);
			x.push_back(0);
			x.push_back(-1);
		}
		else
		{
			x.push_back(directoryIndex);
			x.push_back(tableIndex);
			x.push_back(value);
		}
		return x;
	}
}

/// <summary>
/// Places the given PTE at the first available position
/// </summary>
/// <param name="x"></param>
/// <returns></returns>
vector<int> PageDirectory::placeEntry(PageTableEntry x)
{
	if (this->full)
	{
		vector<int> returnVec = { -1 };
		return returnVec;
	}
	if (level > 1)
	{
		for (int i = 0; i < this->PDTable.size(); i++)
		{
			if (this->PDTable[i].full == false)
			{
				vector<int> vecX = this->PDTable[i].placeEntry(x);
				this->validBit = true;
				vector<int> y;
				this->checkForEmpty();
				y.push_back(i);
				for (int i = 0; i < vecX.size(); i++)
				{
					y.push_back(vecX[i]);
				}
				return y;
			}
		}
		this->full = true;
		vector<int> returnVec = { -1 };
		return returnVec;
	}
	else
	{
		for (int i = 0; i < this->PTTable.size(); i++)
		{
			if (this->PTTable[i].checkForEmpty() != -1)
			{
				vector<int> vecZ;
				vecZ.push_back(i);
				vecZ.push_back(PTTable[i].checkForEmpty());
				PTTable[i].placeInTable(x);
				this->checkForEmpty();
				return vecZ;
			}
		}
		this->full = true;
		vector<int> returnVec = { -1 };
		return returnVec;
	}
}
/*
int main()
{
	PageDirectory x(-1, 4, 3, 8);
	PageTableEntry a1(true, true, true, "thisIsA1");

	for (int i = 0; i < 512; i++)
	{
		x.placeEntry(PageTableEntry(true, true, true, "thisIsXX"));
		x.updateTimers();
	}

	
	vector<int> vecTemp = x.placeEntry(a1);
	for (int i = 0; i < vecTemp.size(); i++)
	{
		cout << std::to_string(vecTemp[i]);
		cout << " ";
	}
	cout << "\n\n";

	cout << x.grabEntry(vecTemp).toString();
	cout << "\n\n";

	vector<int> vec1 = { 0,0,0,0 };
	x.grabEntry(vec1);
	vector<int> vec2{ 0,0,0,1 };
	x.grabEntry(vec2);

	vecTemp = x.findLRUEntry();
	for (int i = 0; i < vecTemp.size(); i++)
	{
		cout << std::to_string(vecTemp[i]);
		cout << " ";
	}

	cout << "\n\n";

	cout << x.swapEntry(a1, vecTemp).toString();
	
	vector<int> vec1 = { 2,1,3,6 };
	x.removeEntry(vec1);
	cout << "\n\n";
	cout << "helloworld";
}
*/