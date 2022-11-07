#include <iostream>
#include <fstream>
#include <string>
#include "DTLBEntry.hpp"
using namespace std;

//Constructor
DTLBEntry::DTLBEntry()
{
    //Sets initial values to -1 or false
	this->validBit = false;
    this->index = -1;
    this->tag = -1;
    this->virtAddr = -1;
    this->frameNum = -1;
    this->LRUNum = -1000;
}


//Paramaterized Constructor
DTLBEntry::DTLBEntry(int newBit, int newIndex, int newTag, int virtAddr, int physAddr, int lru)
{
    this->validBit = newBit;
    this->index = newIndex;
    this->tag = newTag;
    this->virtAddr = virtAddr;
    this->frameNum = physAddr;
    this->LRUNum = lru;
}


//Deconstructor
DTLBEntry::~DTLBEntry(){}