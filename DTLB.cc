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
    //NOT DONE
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
