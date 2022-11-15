#ifndef MEMORY_HPP
#define MEMORY_HPP

#pragma once
#include "Memory.hpp"
#include <vector>

using namespace std;
/// <summary>
/// This class is going to represent our memory for this simulation and will be what the physical pages map too
/// As memory references come in, this will slowly fill up because virtual addresses in our page table are going to be assigned to 'physical locations' in memory
/// Thus this vector of bools is going to keep track of which spots in memory are currently being used and that's it. 
/// It's not going to keep track of anything besides whether something is used or not used.
/// From what i understand, there's no real point in this besides letting us know if memory is full/not full
/// Once it is full it's going to stay full. After the point where it becomes full we will simply be 'swapping' data in and out of memory, we will not be freeing any memory at any point
/// Thus, all of the 'swapping' is going to be done in the page table. If memory is full and the virtual address that just came in does not have a place in memory
/// we will find the LRU entry in the page table, null that out, and assign that physical location to the virtual address that just came in
/// 
/// </summary>
class Memory
{
	int size;
	bool full;									//false if there are empty memory locations, true if there are none
	std::vector<bool> memory;
public:
	Memory();
	Memory(int x);
	~Memory();
	void assignLocation(int x);					//Simply flips the bit at location x to true
	int findFree();								//finds the first free memory location within the vector, if none are free, return -1
};

#endif
