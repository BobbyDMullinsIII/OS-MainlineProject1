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
    this->pageNum = -1;
    this->frameNum = -1;
}


//Paramaterized Constructor
DTLBEntry::DTLBEntry(int newBit, int virtAddr, int physAddr)
{
    this->validBit = newBit;
    this->pageNum = virtAddr;
    this->frameNum = physAddr;
}


//Deconstructor
DTLBEntry::~DTLBEntry(){}