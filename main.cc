/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Solution/Project:   Project 1 (Mainline Team 1)
//  File Name:          main.cc
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
#include <math.h>
using namespace std;

//Included header files
#include "PageDirectory.hpp"
#include "PageTable.hpp"
#include "PageTableEntry.hpp"
#include "DTLB.hpp"
#include "DTLBEntry.hpp"
#include "TraceConfig.hpp"
#include "SimStats.hpp"

//Access states for memory
#define NOOP 0 //no action 
#define RDH 1  //hit for read
#define RDM 2  //miss for read
#define WTH 3  //hit for write
#define WTM 4  //miss for write

//Struct for storing Memory References in integer decimal format
struct MemRefDec
{
    string type;          //Memory Access Type (Can be 'R' or 'W')
    unsigned int address; //Unsigned int Decimal Address (Ranges from 0 (8-bit) to 4294967295 (32-bit unsigned))
    string stringAddress; //String version of address (in hex form)
};

//Struct for keeping info about every memory reference that traverses the hierarchy
struct MemRefInfo 
{
    //Exactly matches project specifications
    string type;
    unsigned int virtAddress;
    int virtPageNum;
    int pageOffset;
    int TLBTag;
    int TLBIndex;
    string TLBResult;
    string PTResult;
    int physPageNum;
    int L1Tag;
    int L1Index;
    string L1Result;
    int L2Tag;
    int L2Index;
    string L2Result;
    int L3Tag;
    int L3Index;
    string L3Result;
    string stringAddress; //String version of address (in hex form)
};


//Input/Output Method Declarations
vector<MemRefDec> insertTrace(vector<MemRefDec> memRefDecVector);
vector<MemRefInfo> initMemRefInfo(vector<MemRefDec> memRefDecVector);
void outputDecAndHex(vector<MemRefDec> memRefDecVector);
void outputEachMemRefInfo(vector<MemRefInfo> memInfo);

//Cache & Page Table Method Declarations
vector<vector<string>> generateCache(int &sets, int &setSize);
void ReplacePage(vector<vector<string>> L1, vector<vector<string>> L2 , int p1, vector<int> pgs, int i);
void FindItem(vector<vector<string>> &cache, int pid);
vector<vector<string>> PageAlloc(int numpgs, int pgsize);
int LRU(vector<vector<string>> pages);
int HitMiss(string virtAddress, vector<vector<string>> cache, vector<vector<string>> pageTable, string baseAddress, string bounds);
int getFrameNumber(string virtualAddress, int blocks);
void DirectAssociative(vector<vector<string>>* L1, vector<vector<string>>* L2, string virtualAddress, string baseAddress, string bounds, int blocks);

//Address Manipulation Method Declarations
string VirtualToPhysical(string virtAddress, string baseAddress, string bounds);
string toHex(int i);
int toInt(string i);

//Other Methods
SimStats runSimulation(TraceConfig insertedConfig, SimStats simStats, vector<MemRefInfo> memRefs);


int main()
{
    //==Declaration Section==//
    vector<MemRefDec> MemRefsDec;   //Vector of MemRefDec's to work from with addresses in decimal (unsigned int) form
    vector<MemRefInfo> MemRefsInfo; //Vector of information about each reference that traverses the hierarchy
    TraceConfig config;             //Class of config values taken from trace.config file
    SimStats stats;                 //Class of simulation counters


    //==Insertion/Initialization Section==//
    MemRefsDec = insertTrace(MemRefsDec);     //Insert memory references from stdin into MemRefDec vector
    MemRefsInfo = initMemRefInfo(MemRefsDec); //Initialize MemRefsInfo vector with each memory reference's address
    config.insertConfig();                    //Insert config values from trace.config into config class object
    stats.prepareCounters();                  //Set all hit/miss/read/write/reference counters to 0 for counting

    
    //==Run Simulation Section==//
    stats = runSimulation(config, stats, MemRefsInfo); //Branches to each possible combination of the 4 conditionals (Virtual Addresses, TLB, L2, L3)


    //==Final Output Section==//
    config.outputConfigValues();         //Output config values
    outputEachMemRefInfo(MemRefsInfo);   //Output information about each memory reference in the simulation
    stats.outputSimulationStatistics(config.TLBActive, config.L2Active, config.L3Active);  //Output final simulation statistics

    return 0;
}


//Method for inserting Memory References from stdin into the given MemRefDec vector
//Returns a vector containing MemRefDec variables
vector<MemRefDec> insertTrace(vector<MemRefDec> memRefDecVector)
{
    string RawMemRef;   //Raw memory reference in '<accesstype>:<hexaddress>' format
    MemRefDec tempRef;  //Temporary MemRefDec for inserting each line of memory references into vector
    int stringIndex;    //Variable for keeping track of index of colon in string

    //Goes through each line in standard input and returns a single Raw Memory Reference string
    while (getline(cin, RawMemRef))
    {   
        //Get index of colon character
        stringIndex = RawMemRef.find_first_of(':');

        //Split RawMemRef into two strings for tempRef variable
        tempRef.type = RawMemRef.substr(0, stringIndex);
        tempRef.address = (unsigned int)strtoll(RawMemRef.substr(stringIndex + 1).c_str(), NULL, 16);
        tempRef.stringAddress = RawMemRef.substr(stringIndex + 1);

        //Insert tempRef variable into memRefDecVector
        memRefDecVector.push_back(tempRef);
    }

    return memRefDecVector;

}//end insertTrace()

//Method for inserting every variable in memRefDecVector into a new MemRefInfo vector
vector<MemRefInfo> initMemRefInfo(vector<MemRefDec> memRefDecVector)
{
    vector<MemRefInfo> memInfoVector; //MemRefInfo vector to return
    MemRefInfo tempInfo;              //Temporary variable for insertion into each index
    
    //Copy each MemRefDec variable to a MemRefInfo and insert it into the new vector
    for (int i = 0; i != memRefDecVector.size(); i++)
    {
        tempInfo.type = memRefDecVector[i].type;
        tempInfo.virtAddress = memRefDecVector[i].address;
        tempInfo.virtPageNum = 0;
        tempInfo.pageOffset = 0;
        tempInfo.TLBTag = 0;
        tempInfo.TLBIndex = 0;
        tempInfo.TLBResult = "null";
        tempInfo.PTResult = "null";
        tempInfo.physPageNum = 0;
        tempInfo.L1Tag = 0;
        tempInfo.L1Index = 0;
        tempInfo.L1Result = "null";
        tempInfo.L2Tag = 0;
        tempInfo.L2Index = 0;
        tempInfo.L2Result = "null";
        tempInfo.L3Tag = 0;
        tempInfo.L3Index = 0;
        tempInfo.L3Result = "null";
        tempInfo.stringAddress = memRefDecVector[i].stringAddress;
        
        //Add tempInfo to memInfoVector
        memInfoVector.push_back(tempInfo);
    }

    return memInfoVector;

}//end initializeMemRefInfo()

//Method for outputing memory addresses in both Hex and Decimal form (and a copy in string form)
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

//Method for outputting information about each memory reference that has traversed the memory hierarchy
void outputEachMemRefInfo(vector<MemRefInfo> memInfoVector)
{
    cout << "\n";
    cout << "Addr          Virt.  Page TLB    TLB TLB  PT   Phys        DC  DC          L2  L2          L3  L3  \n";
    cout << "Type Address  Page # Off  Tag    Ind Res. Res. Pg # DC Tag Ind Res. L2 Tag Ind Res. L3 Tag Ind Res.\n";
    cout << "---- -------- ------ ---- ------ --- ---- ---- ---- ------ --- ---- ------ --- ---- ------ --- ----\n";
    for (int i = 0; i != memInfoVector.size(); i++)
    {
        printf("%4s %08x %6x %4x %6x %3x %4s %4s %4x %6x %3x %4s %6x %3x %4s %6x %3x %4s\n", 
        memInfoVector[i].type.c_str(),
        memInfoVector[i].virtAddress,
        memInfoVector[i].virtPageNum,
        memInfoVector[i].pageOffset,
        memInfoVector[i].TLBTag,
        memInfoVector[i].TLBIndex,
        memInfoVector[i].TLBResult.c_str(),
        memInfoVector[i].PTResult.c_str(),
        memInfoVector[i].physPageNum,
        memInfoVector[i].L1Tag,
        memInfoVector[i].L1Index,
        memInfoVector[i].L1Result.c_str(),
        memInfoVector[i].L2Tag,
        memInfoVector[i].L2Index,
        memInfoVector[i].L2Result.c_str(),
        memInfoVector[i].L3Tag,
        memInfoVector[i].L3Index,
        memInfoVector[i].L3Result.c_str());
    }
    cout << "\n";

}//end outputEachMemRefInfo()

//generate a cache and resize to the number of sets in config and set each set size from config
vector<vector<string>> generateCache(int &sets, int &setSize)
{
    vector<vector<string>> cache;

    cache.resize((int)sets);

    for(int i = 0; i < sets; i++)
    {
        cache[i].resize(setSize);
    }

    return cache;
}

/*//replaces a page in a page list
//L1, L2, L3 are caches
//p1 is new page
//pgs is existing pages
//i is location in pgs of page being replaced
void ReplacePage(vector<vector<string>> L1, vector<vector<string>> L2, vector<vector<string>> L3, int p1, vector<int> pgs, int i)
{
    //finds if there is existing pid of that page in each cache, and replaces with null (-1)
    FindItem(L1, pgs[i]);
    FindItem(L2, pgs[i]);
    FindItem(L3, pgs[i]);

    pgs[i] = p1;//replaces page
}

//finds pid of replaced page and places null where pid is found
void FindItem(vector<vector<string>> &cache, string pid)
{
    //searches for a pid in the cache, and if exists replace with null (-1)
    for (vector<string> &v : cache)
    {
        replace(v.begin(), v.end(), pid, -1);
    }
}*/

/*//allocates a chuck in memory for physical pages
//use as pages[pagenum][pageloc]
vector<vector<string>> PageAlloc(int numpgs, int pgsize)
{
    //2D vector to return from page allocation
    vector<vector<string>> pgs;

    //Sets the rows to the number of pages
    //Sets the columns to the page size
    //Sets all values to the initial value of '-1'
    pgs.resize(numpgs, vector<int>(pgsize, -1));

    return pgs;
}*/

//gets least recently used page
//returns index of page
//assuming least recently used is being tracked
//by very last int on the page
int LRU(vector<vector<string>> pages)
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

//searches the cahce for the process, if there return -1
//otherwise add the process to the page table and return false
int HitMiss(string virtAddress, vector<vector<string>> cache, vector<vector<string>> pageTable, string baseAddress, string bounds)
{

    string physicalAddress, frameNumber;

    string offset, index, tag;

    //return value
    //1 = hit
    //2 = conflict miss
    //3 - compulsory miss
    int retnum = 0;

    offset = virtAddress.substr(1,10);
    index = virtAddress.substr(2,8);
    tag = virtAddress.substr(5,0);

    bool hit = false;

    for(int i = 0; i < cache.size(); i++)
    {
        if(retnum > 0)
        {
            break;
        }

        if(tag.compare(cache[i][1].substr(5,0)) == 0)
        {
            if(index.compare(cache[i][1].substr(2,8)))
            {
                if(offset.compare(cache[i][1].substr(1,10)) != 0)
                {
                    retnum = 1;
                }
            }
        }
        else if(index.compare(cache[i][1].substr(2,8)))
        {
            retnum = 2;
        }
        else
        {
            retnum = 3;
        }
    }

    return retnum;
}

//gets the frame number from the virtual address and map type
int getFrameNumber(string virtualAddress, int blocks)
{
    return toInt(virtualAddress) % ((int)pow((double)2,(double)blocks));    
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
SimStats runSimulation(TraceConfig insertedConfig, SimStats simStats,  vector<MemRefInfo> memRefs)
{
    //Caches, DTLB, Page Table (In Progress, Needs to be integrated)
    vector<vector<string>> L1 = generateCache(insertedConfig.L1NumSets, insertedConfig.L1SetSize);
    vector<vector<string>> L2 = generateCache(insertedConfig.L2NumSets, insertedConfig.L2SetSize);
    vector<vector<string>> L3 = generateCache(insertedConfig.L3NumSets, insertedConfig.L3SetSize); //Ignore, not implementing
    DTLB tlb = DTLB(insertedConfig.numTLBSets, insertedConfig.TLBSetSize);
    //(Insert page table declaration & initialization here)


    //Large if/else tree for Virtual Addresses, TLB, L2, L3
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

                    //Sequentially conducts simulation on each memory reference line
                    for(int i = 0; i < memRefs.size(); i++)
                    {
                        //============================================//
                        //Do not code here, we are not implementing L3//
                        //============================================//
                    }
                }
                else if (insertedConfig.L3Active == false)
                {
                    //Virtual Addresses == true
                    //TLB == true
                    //L2 == true
                    //L3 == false

                    //Sequentially conducts simulation on each memory reference line
                    for(int i = 0; i < memRefs.size(); i++)
                    {
                        //===================================//
                        //Simulation execution code goes here//
                        //===================================//
                    }
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

                    //Sequentially conducts simulation on each memory reference line
                    for(int i = 0; i < memRefs.size(); i++)
                    {
                        //===================================//
                        //Simulation execution code goes here//
                        //===================================//
                    }
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

                    //Sequentially conducts simulation on each memory reference line
                    for(int i = 0; i < memRefs.size(); i++)
                    {
                        //============================================//
                        //Do not code here, we are not implementing L3//
                        //============================================//
                    }
                }
                else if (insertedConfig.L3Active == false)
                {
                    //Virtual Addresses == true
                    //TLB == false
                    //L2 == true
                    //L3 == false

                    //Sequentially conducts simulation on each memory reference line
                    for(int i = 0; i < memRefs.size(); i++)
                    {
                        //===================================//
                        //Simulation execution code goes here//
                        //===================================//
                    }
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

                    //Sequentially conducts simulation on each memory reference line
                    for(int i = 0; i < memRefs.size(); i++)
                    {
                        //===================================//
                        //Simulation execution code goes here//
                        //===================================//
                    }
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

                //Sequentially conducts simulation on each memory reference line
                for(int i = 0; i < memRefs.size(); i++)
                {
                    //============================================//
                    //Do not code here, we are not implementing L3//
                    //============================================//
                }
            }
            else if (insertedConfig.L3Active == false)
            {
                //Virtual Addresses == false
                //TLB == false
                //L2 == true
                //L3 == false

                //Sequentially conducts simulation on each memory reference line
                for(int i = 0; i < memRefs.size(); i++)
                {
                    //===================================//
                    //Simulation execution code goes here//
                    //===================================//
                }
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

                //Sequentially conducts simulation on each memory reference line
                for(int i = 0; i < memRefs.size(); i++)
                {
                    //===================================//
                    //Simulation execution code goes here//
                    //===================================//
                }
            }
        }
    }

    return simStats;
}


void DirectAssociative(vector<vector<string>> L1, vector<vector<string>> L2, string virtualAddress, string baseAddress, string bounds, int blocks)
{
    //direct associative
    string physical = VirtualToPhysical(virtualAddress, baseAddress, bounds);
    int frameNum = getFrameNumber(physical, blocks);

    L1[frameNum][1] = virtualAddress;
    L2[frameNum][1] = virtualAddress;
}