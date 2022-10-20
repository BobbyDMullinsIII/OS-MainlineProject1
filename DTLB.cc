#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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
void DTLB::insertRecentAddress(int address, int frame)
{
    int lowCol;    //Keeps track of column index with lowest LRU number seen so far
    int lowRow;    //Keeps track of row index with lowest LRU number seen so far
    int lowNum = 1000000000;    //Number for comparison of LRUNums (Starts at very high number for first comparison)

    //Iterate through 2D DTLB vector to determine which one with the lowers LRUNum to insert the address and frame into
    for(int i = 0; i < dtlbSets.size(); i++)
    {
        for(int j = 0; j < dtlbSets[i].size(); j++)
        {
            if(dtlbSets[i][j].LRUNum < lowNum)
            {
                //Sets lowNum to currently lowest found LRUNum in DTLB and saves location
                lowNum = dtlbSets[i][j].LRUNum;

                //Saves lowest found LRUNum location so far
                lowCol = i;
                lowRow = j;
            }
        }
    }


    //Create new DTLBEntry with inserted address, frame and proper valid bit and current LRUCounter value
    DTLBEntry newEntry;
    newEntry.validBit = true;
    newEntry.pageNum = address;
    newEntry.frameNum = frame;
    newEntry.LRUNum = LRUCounter;

    //Insert new DTLBEntry into DTLB
    dtlbSets[lowCol][lowRow] = newEntry;

    //Increment LRUCounter by one for proper LRU ordering
    this->LRUCounter++;
}


//Method for checking if given virtual address is within DTLB
//Goes through each of the sets if set-associative
//Returns whether inserted address is within the DTLB
bool DTLB::checkForAddress(int checkAddr)
{
    //Iterate through 2D DTLB vector to determine if virtual address translation is already within DTLB
    for(int i = 0; i < dtlbSets.size(); i++)
    {
        for(int j = 0; j < dtlbSets[i].size(); j++)
        {
            if(checkAddr == dtlbSets[i][j].pageNum)
            {
                return true; //Returns true if any of the addresses match
            }
        }
    }

    return false; //Will only get here and return false if none of the virtual addresses match
}
