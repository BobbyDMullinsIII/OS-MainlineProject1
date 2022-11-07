#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits.h>
#include "DTLB.hpp"
#include "DTLBEntry.hpp"
using namespace std;


//Paramaterized Constructor
DTLB::DTLB(int sets, int size)
{
    this->numSets = sets;
    this->setSize = size;
    this->LRUCounter = 0;

    //Sets 2d vector to have specified number of sets and set size
    dtlbSets = vector<vector<DTLBEntry>>(sets, vector<DTLBEntry>(size));
}


//Deconstructor
DTLB::~DTLB(){}


//Method for inserting most recent virtual address translation into DTLB
//Uses LRU replacement policy
//Will not insert if it is already within DTLB in order to avoid duplicate translations taking multiple slots
void DTLB::insertRecentAddress(int index, int tag, int address, int frame)
{
    int lowBlock;            //Keeps track of block index with lowest LRU number seen so far
    int lowNum = INT_MAX;    //Number for comparison of LRUNums (Starts at very high number for first comparison)

    //Iterate through DTLB vector to determine which one with the lowest LRUNum to insert the address and frame into
    for(int i = 0; i < dtlbSets[index].size(); i++)
    {
        if(dtlbSets[index][i].LRUNum < lowNum)
        {
            //Sets lowNum to currently lowest found LRUNum in DTLB and saves location
            lowNum = dtlbSets[index][i].LRUNum;

            //Saves lowest found LRUNum location so far
            lowBlock = i;
        }
    }


    //Create new DTLBEntry with inserted address, frame and proper valid bit and current LRUCounter value
    DTLBEntry newEntry;
    newEntry.validBit = true; //Always set to true (1) when new entry is entered
    newEntry.index = index; 
    newEntry.tag = tag;
    newEntry.virtAddr = address;
    newEntry.frameNum = frame;
    newEntry.LRUNum = LRUCounter;

    //Insert new DTLBEntry into DTLB
    dtlbSets[index][lowBlock] = newEntry;

    //Increment LRUCounter by one for proper LRU ordering
    this->LRUCounter++;
}


//Method for checking if given virtual address is within DTLB
//Goes through each of the sets if set-associative
//Returns whether inserted address is within the DTLB
bool DTLB::checkDTLB(int checkTag, int checkIndex)
{
    //Iterate through specified set (by the index) to determine if virtual address translation is already within DTLB in that set
    for(int i = 0; i < dtlbSets.size(); i++)
    {
        if(checkTag == dtlbSets[checkIndex][i].tag && checkIndex == dtlbSets[checkIndex][i].index)
        {
            return true; //Returns true if any of the addresses match in the set
        }
    }

    return false; //Will only get here and return false if none of the virtual addresses match in the set
}
