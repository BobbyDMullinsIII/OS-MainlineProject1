/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Solution/Project:   Project 1 (Mainline Team 1)
//  File Name:          project1.cc
//  Description:        Main file for Operating Systems Mainline Project 1
//  Course:             CSCI-4727-001 - Operating Systems
//  Authors:            Bobby Mullins, Isaiah Jayne, Jacob Manis, Zachary Lykins       
//  Created:            Wednesday, September  21, 2022
//  Copyright:          Mainline Team 1 - Operating Systems
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
#include <map>
#include "TraceConfig.hpp"
using namespace std;

//Access states for memory
#define NOOP 0 //no action 
#define RDH 1 //hit for read
#define RDM 2 //miss for read
#define WTH 3 //hit for write
#define WTM 4 //miss for write

//Struct for storing Memory References in string hex format
struct MemRefHex
{
    string type;    //Memory Access Type (Can be 'R' or 'W')
    string address; //Hex Address (Ranges from 000 (8-bit) to FFFFFFFF (32-bit unsigned))
};

//Struct for storing Memory References in integer decimal format
struct MemRefDec
{
    string type;    //Memory Access Type (Can be 'R' or 'W')
    long address;   //Long Decimal Address (Ranges from 0 (8-bit) to 4294967295 (32-bit unsigned))
};


//Input/Output Method Declarations
vector<MemRefHex> insertTrace(vector<MemRefHex> memRefHexVector);
vector<MemRefDec> convertToMemRefDec(vector<MemRefHex> memRefHexVector);
void outputHexVector(vector<MemRefHex> memRefHexVector);
void outputDecVector(vector<MemRefDec> memRefDecVector);
void outputDecAndHex(vector<MemRefDec> memRefDecVector);

//Cache & Page Table Method Declarations
vector<vector<long>> generateCache(long &sets, long &setSize);
void ReplacePage(vector<vector<long>> L1, vector<vector<long>> L2 , int p1, vector<int> pgs, int i);
void FindItem(vector<vector<long>> &cache, int pid);
vector<vector<int>> PageAlloc(int numpgs, int pgsize);
int LRU(vector<vector<int>> pages);

//Address Manipulation Method Declarations
string VirtualToPhysical(string virtAddress, string baseAddress, string bounds);
string toHex(int i);
int toInt(string i);

//Other Methods
TraceConfig runSimulation(TraceConfig insertedConfig);

int main()
{
    vector<MemRefHex> MemReferencesHex; //Vector of MemRefHex's to work from with addresses in hex (string) form
    vector<MemRefDec> MemReferencesDec; //Vector of MemRefDec's to work from with addresses in decimal (long) form
    TraceConfig config;                 //Class of config values and counters taken from trace.config file

    MemReferencesHex = insertTrace(MemReferencesHex);       //Insert memory references from stdin into MemRefHex vector
    MemReferencesDec = convertToMemRefDec(MemReferencesHex);//Make a copy of MemReferencesHex, but with addresses in easier-to-use long decimal form
    config.insertConfig();    //Insert config values from trace.config into config class object
    config.prepareCounters(); //Set all hit/miss/read/write/reference counters to 0 for counting

    //====NEEDS ACTUAL CODE - NOT FINISHED AT ALL====//
    config = runSimulation(config); //Branches to each possible combination of the 4 conditionals (Virtual Addresses, TLB, L2, L3)

    config.outputRawConfigValues();    //Output final config values and simulation statistics
    outputDecAndHex(MemReferencesDec); //Output memory traces in both hex and decimal form

    return 0; //Exit program
}


//Method for inserting Memory References from stdin into the given MemRefHex vector
//Returns a vector containing MemRefHex variables
vector<MemRefHex> insertTrace(vector<MemRefHex> memRefHexVector)
{
    string RawMemRef;   //Raw memory reference in '<accesstype>:<hexaddress>' format
    MemRefHex tempRef;  //Temporary MemRefHex for inserting each line of memory references into vector
    int stringIndex;    //Variable for keeping track of index of colon in string

    //Goes through each line in standard input and returns a single Raw Memory Reference string
    while (getline(cin, RawMemRef))
    {   
        //Get index of colon character
        stringIndex = RawMemRef.find_first_of(':');

        //Split RawMemRef into two strings for tempRef variable
        tempRef.type = RawMemRef.substr(0, stringIndex);
        tempRef.address = RawMemRef.substr(stringIndex + 1);

        //Insert tempRef variable into memRefHexVector
        memRefHexVector.push_back(tempRef);
    }

    return memRefHexVector;

}//end insertTrace()

//Method for converting a vector of MemRefHex's to a vector of MemRefDec's
//Returns a vector converted to MemRefDec's
vector<MemRefDec> convertToMemRefDec(vector<MemRefHex> memRefHexVector)
{
    vector<MemRefDec> memRefDecVector; //Vector to return
    MemRefDec tempRef;                 //Temporary MemRefDec for inserting each index of memory references into vector

    //Converts each MemRefHex into a MemRefDec for insertion into the return vector
    for (int i = 0; i != memRefHexVector.size(); i++)
    {
        tempRef.type = memRefHexVector[i].type;
        tempRef.address = (long)strtoll(memRefHexVector[i].address.c_str(), NULL, 16);
        memRefDecVector.push_back(tempRef);
    }

    return memRefDecVector;

}//end insertTrace()

//Method for outputing memory addresses in Hex (MemRefHex) form
void outputHexVector(vector<MemRefHex> memRefHexVector)
{
    //Test to see if input to vector code worked correctly
    cout << "Memory References from inserted trace file (Hex Form):\n";
    for (int i = 0; i != memRefHexVector.size(); i++)
    {
        cout << memRefHexVector[i].type << ":" << memRefHexVector[i].address <<"\n";
    }

}//end outputHexVector()

//Method for outputing memory addresses in Decimal (MemRefDec) form
void outputDecVector(vector<MemRefDec> memRefDecVector)
{
    //Test to see if input to vector code worked correctly
    cout << "Memory References from inserted trace file (Decimal Form):\n";
    for (int i = 0; i != memRefDecVector.size(); i++)
    {
        cout << memRefDecVector[i].type << ":" << memRefDecVector[i].address <<"\n";
    }

}//end outputDecVector()

//Method for outputing memory addresses in both Hex and Decimal form
void outputDecAndHex(vector<MemRefDec> memRefDecVector)
{
        //Show every memory trace in both Decimal and Hex form and exit loop
    cout << "\n\nMemory References (Hex|Decimal):\n";
    for (int i = 0; i != memRefDecVector.size(); i++)
    {
        cout << memRefDecVector[i].type << ":" << std::hex << memRefDecVector[i].address << "   |   ";
        cout << memRefDecVector[i].type << ":" << std::dec << memRefDecVector[i].address << "\n";
    }

}//end outputDecAndHex()

//*should*
//generate a cache and resize to the number of sets in config and set each set size from config
vector<vector<long>> generateCache(long &sets, long &setSize)
{
    vector<vector<long>> cache;

    cache.resize((long)sets);

    for(int i = 0; i < sets; i++)
    {
        cache[i].resize(setSize);
    }

    return cache;
}

//replaces a page in a page list
//L1,L2 are cahces
//p1 is new page
//pgs is existing pages
//i is location in pgs of page being replaced
void ReplacePage(vector<vector<long>> L1, vector<vector<long>> L2 , int p1, vector<int> pgs, int i)
{
   FindItem(L1, pgs[i]);//finds if there is existing pid of that page in L1 and replaces with null
   FindItem(L2, pgs[i]);//finds if there is existing pid of that page in L2 and replaces with null
   pgs[i] = p1;//replaces page
}

//finds pid of replaced page and places null where pid is found
void FindItem(vector<vector<long>> &cache, int pid)
{
    //searches for a pid in the cache, and if exists replace with null
    for (vector<long> &v : cache)
    {
        replace(v.begin(), v.end(), pid, -1);
    }
}

//allocates a chuck in memory for physical pages
//use as pages[pagenum][pageloc]
vector<vector<int>> PageAlloc(int numpgs, int pgsize)
{
    //2D vector to return from page allocation
    vector<vector<int>> pgs;

    //Sets the rows to the number of pages
    //Sets the columns to the page size
    //Sets all values to the initial value of '-1'
    pgs.resize(numpgs, vector<int>(pgsize, -1));

    return pgs;
}

//gets least recently used page
//returns index of page
//assuming least recently used is being tracked
//by very last int on the page
int LRU(vector<vector<int>> pages)
{
    int LRU = 0;
    for(int i = 0; i < pages.size(); i++)
    {
        if(pages[i][pages[i].size()-1] < pages[LRU][pages[i].size()-1])
        {
            LRU = i;
        }
    }

    return LRU;
}

//takes a virtual address and return the physical address of the accessed area in memory
//virtAddress = virtual address to convert to physical address
//baseAddress = base address of the process
//bounds = size of address space allocated  
string VirtualToPhysical(string virtAddress, string baseAddress, string bounds)
{
    int virtInt = toInt(virtAddress);
    int baseInt = toInt(baseAddress);
    int boundInt = toInt(bounds);

    int physicalAddress = baseInt + virtInt;
    string physicalHex;

    if(baseInt <= physicalAddress)
    {
        if((baseInt+boundInt) > physicalAddress)
        {
            physicalHex = toHex(physicalAddress);
            return physicalHex;
        }
        else
        {
            return "Out of Bounds: past max bounds of process";
        }
    }
    else
    {
        return "Out of Bounds: Less than base address of process";
    }
}

//converts an int to a hex string
string toHex(int i)
{
    ostringstream stream;
    stream << std::hex << i;
    return stream.str();
}

//converts a string to an int
int toInt(string i)
{
    stringstream stream;
    int x;
    stream << hex << i;
    stream >> x;

    return x;
}

//Method for branching to each possible combination of the 4 conditionals (Virtual Addresses, TLB, L2, L3)
//Returns final config values for final output
TraceConfig runSimulation(TraceConfig insertedConfig)
{
    if(insertedConfig.VirtAddressActive == true)
    {
        if(insertedConfig.TLBActive == true)
        {
            if(insertedConfig.L2Active == true)
            {
                if(insertedConfig.L3Active == true)
                {
                    //Virtual Addresses == true
                    //TLB == true
                    //L2 == true
                    //L3 == true

                    //=============================//
                    //(Code and/or methods go here)//
                    //=============================//
                }
                else if (insertedConfig.L3Active == false)
                {
                    //Virtual Addresses == true
                    //TLB == true
                    //L2 == true
                    //L3 == false

                    //=============================//
                    //(Code and/or methods go here)//
                    //=============================//
                }
            }
            else 
            {   
                if (insertedConfig.L2Active == false && insertedConfig.L3Active == false)
                {
                    //Virtual Addresses == true
                    //TLB == true
                    //L2 == false
                    //L3 == false

                    //=============================//
                    //(Code and/or methods go here)//
                    //=============================//
                }
            }
        }
        else if (insertedConfig.TLBActive == false)
        {
            if(insertedConfig.L2Active == true)
            {
                if(insertedConfig.L3Active == true)
                {
                    //Virtual Addresses == true
                    //TLB == false
                    //L2 == true
                    //L3 == true

                    //=============================//
                    //(Code and/or methods go here)//
                    //=============================//
                }
                else if (insertedConfig.L3Active == false)
                {
                    //Virtual Addresses == true
                    //TLB == false
                    //L2 == true
                    //L3 == false

                    //=============================//
                    //(Code and/or methods go here)//
                    //=============================//
                }
            }
            else 
            {   
                if (insertedConfig.L2Active == false && insertedConfig.L3Active == false)
                {
                    //Virtual Addresses == true
                    //TLB == false
                    //L2 == false
                    //L3 == false

                    //=============================//
                    //(Code and/or methods go here)//
                    //=============================//
                }
            }
        }
    }
    else if (insertedConfig.VirtAddressActive == false && insertedConfig.TLBActive == false)
    {
        if(insertedConfig.L2Active == true)
        {
            if(insertedConfig.L3Active == true)
            {
                //Virtual Addresses == false
                //TLB == false
                //L2 == true
                //L3 == true

                //=============================//
                //(Code and/or methods go here)//
                //=============================//
            }
            else if (insertedConfig.L3Active == false)
            {
                //Virtual Addresses == false
                //TLB == false
                //L2 == true
                //L3 == false

                //=============================//
                //(Code and/or methods go here)//
                //=============================//
            }
        }
        else 
        {   
            if (insertedConfig.L2Active == false && insertedConfig.L3Active == false)
            {
                //Virtual Addresses == false
                //TLB == false
                //L2 == false
                //L3 == false

                //=============================//
                //(Code and/or methods go here)//
                //=============================//
            }
        }
    }

    return insertedConfig;
}
