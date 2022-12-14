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
#include "Memory.hpp"

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
    unsigned int address;
    int virtPageNum;
    int pageOffset;
    int TLBTag;
    int TLBIndex;
    string TLBResult;
    string PTResult;
    int physPageNum;
    string L1Tag;
    int L1Index;
    string L1Result;
    string L2Tag;
    int L2Index;
    string L2Result;
    string L3Tag;
    int L3Index;
    string L3Result;
    string stringAddress; //String version of address (in hex form)
};


//Input/Output Method Declarations
vector<MemRefDec> insertTrace(vector<MemRefDec> memRefDecVector);
vector<MemRefInfo> initMemRefInfo(vector<MemRefDec> memRefDecVector);
void outputDecAndHex(vector<MemRefDec> memRefDecVector);
void outputEachMemRefInfo(TraceConfig insertedConfig, vector<MemRefInfo> memInfoVector);

//Cache & Page Table Method Declarations
vector<string> generateCache(int &sets, int &setSize);
void ReplacePage(vector<string> L1, vector<string> L2 , int p1, vector<int> pgs, int i);
void FindItem(vector<string> &cache, int pid);
vector<string> PageAlloc(int numpgs, int pgsize);
int LRU(vector<string> pages);
//int HitMiss(string virtAddress, vector<string> cache, vector<string> pageTable, string baseAddress, string bounds);
int HitMiss(string virtAddress, vector<string> cache);
int getFrameNumber(string virtualAddress, int blocks);
void DirectAssociative(vector<string>* L1, vector<string>* L2, string virtualAddress, string baseAddress, string bounds, int blocks);

//Address Manipulation Method Declarations
string VirtualToPhysical(string virtAddress);
string toHex(int i);
int toInt(string i);
bitset<32> HextoBinary(string x);
string BinarytoHex(bitset<32> x);
int getVirtPageNum(int virtAddress, TraceConfig insertedConfig);
int getOffset(int virtAddress, TraceConfig insertedConfig);
int getTLBTag(int virtAddress, TraceConfig insertedConfig);
int getTLBIndex(int virtAddress, TraceConfig insertedConfig);

//Other Methods
SimStats runSimulation(TraceConfig insertedConfig, SimStats simStats, vector<MemRefInfo> &memRefs);
SimStats checkReadOrWrite(int i, SimStats simStats, vector<MemRefInfo> memRefs);
SimStats calcMemPageDiskRefs(SimStats simStats, vector<MemRefInfo> memRefs);
SimStats calcHitMissCounters(SimStats simStats, vector<MemRefInfo> memRefs);
void Evict(string cache, string address, string addReplace, vector<string> L1, vector<string> L2);
void writeAlloc(vector<string> L1, vector<string> L2, string address, int blocks);
int calcPDSize(int virtPages);
int calcPTSize(int virtPages);
int calcNumLevels(int virtPages);
std::vector<int> generatePDTableAddress(int numberOfBits, string virtInHex);
int bin32ToInt(bitset<32> x);

void ReorderCache (vector<MemRefInfo> &LCache, int loc, vector<MemRefInfo> &MemRefs);


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
    config.outputConfigValues(); //Output config values
    outputEachMemRefInfo(config, MemRefsInfo); //Output information about each memory reference in the simulation
    stats.outputSimulationStatistics(config.VirtAddressActive, config.TLBActive, config.L2Active, config.L3Active);  //Output final simulation statistics

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
        tempInfo.address = memRefDecVector[i].address;
        tempInfo.virtPageNum = 0;
        tempInfo.pageOffset = 0;
        tempInfo.TLBTag = 0;
        tempInfo.TLBIndex = 0;
        tempInfo.TLBResult = "null";
        tempInfo.PTResult = "null";
        tempInfo.physPageNum = 0;
        tempInfo.L1Tag = "0";
        tempInfo.L1Index = 0;
        tempInfo.L1Result = "null";
        tempInfo.L2Tag = "0";
        tempInfo.L2Index = 0;
        tempInfo.L2Result = "null";
        tempInfo.L3Tag = "0";
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
void outputEachMemRefInfo(TraceConfig insertedConfig, vector<MemRefInfo> memInfoVector)
{
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

                    cout << "\n";
                    cout << "Addr          Virt.  Page TLB    TLB TLB  PT   Phys        DC  DC          L2  L2          L3  L3  \n";
                    cout << "Type Address  Page # Off  Tag    Ind Res. Res. Pg # DC Tag Ind Res. L2 Tag Ind Res. L3 Tag Ind Res.\n";
                    cout << "---- -------- ------ ---- ------ --- ---- ---- ---- ------ --- ---- ------ --- ---- ------ --- ----\n";
                    for (int i = 0; i != memInfoVector.size(); i++)
                    {
                        printf("%4s %08x %6x %4x %6x %3x %4s %4s %4x %6s %3x %4s %6s %3x %4s %6s %3x %4s\n", 
                        memInfoVector[i].type.c_str(),
                        memInfoVector[i].address,
                        memInfoVector[i].virtPageNum,
                        memInfoVector[i].pageOffset,
                        memInfoVector[i].TLBTag,
                        memInfoVector[i].TLBIndex,
                        memInfoVector[i].TLBResult.c_str(),
                        memInfoVector[i].PTResult.c_str(),
                        memInfoVector[i].physPageNum,
                        memInfoVector[i].L1Tag.c_str(),
                        memInfoVector[i].L1Index,
                        memInfoVector[i].L1Result.c_str(),
                        memInfoVector[i].L2Tag.c_str(),
                        memInfoVector[i].L2Index,
                        memInfoVector[i].L2Result.c_str(),
                        memInfoVector[i].L3Tag.c_str(),
                        memInfoVector[i].L3Index,
                        memInfoVector[i].L3Result.c_str());
                    }
                    cout << "\n";
                }
                else if (insertedConfig.L3Active == false)
                {
                    //Virtual Addresses == true
                    //TLB == true
                    //L2 == true
                    //L3 == false

                    cout << "\n";
                    cout << "Addr          Virt.  Page TLB    TLB TLB  PT   Phys        DC  DC          L2  L2  \n";
                    cout << "Type Address  Page # Off  Tag    Ind Res. Res. Pg # DC Tag Ind Res. L2 Tag Ind Res.\n";
                    cout << "---- -------- ------ ---- ------ --- ---- ---- ---- ------ --- ---- ------ --- ----\n";
                    for (int i = 0; i != memInfoVector.size(); i++)
                    {
                        printf("%4s %08x %6x %4x %6x %3x %4s %4s %4x %6s %3x %4s %6s %3x %4s\n", 
                        memInfoVector[i].type.c_str(),
                        memInfoVector[i].address,
                        memInfoVector[i].virtPageNum,
                        memInfoVector[i].pageOffset,
                        memInfoVector[i].TLBTag,
                        memInfoVector[i].TLBIndex,
                        memInfoVector[i].TLBResult.c_str(),
                        memInfoVector[i].PTResult.c_str(),
                        memInfoVector[i].physPageNum,
                        memInfoVector[i].L1Tag.c_str(),
                        memInfoVector[i].L1Index,
                        memInfoVector[i].L1Result.c_str(),
                        memInfoVector[i].L2Tag.c_str(),
                        memInfoVector[i].L2Index,
                        memInfoVector[i].L2Result.c_str());
                    }
                    cout << "\n";
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

                    cout << "\n";
                    cout << "Addr          Virt.  Page TLB    TLB TLB  PT   Phys        DC  DC  \n";
                    cout << "Type Address  Page # Off  Tag    Ind Res. Res. Pg # DC Tag Ind Res.\n";
                    cout << "---- -------- ------ ---- ------ --- ---- ---- ---- ------ --- ----\n";
                    for (int i = 0; i != memInfoVector.size(); i++)
                    {
                        printf("%4s %08x %6x %4x %6x %3x %4s %4s %4x %6s %3x %4s\n", 
                        memInfoVector[i].type.c_str(),
                        memInfoVector[i].address,
                        memInfoVector[i].virtPageNum,
                        memInfoVector[i].pageOffset,
                        memInfoVector[i].TLBTag,
                        memInfoVector[i].TLBIndex,
                        memInfoVector[i].TLBResult.c_str(),
                        memInfoVector[i].PTResult.c_str(),
                        memInfoVector[i].physPageNum,
                        memInfoVector[i].L1Tag.c_str(),
                        memInfoVector[i].L1Index,
                        memInfoVector[i].L1Result.c_str());
                    }
                    cout << "\n";
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

                    cout << "\n";
                    cout << "Addr          Virt.  Page PT   Phys        DC  DC          L2  L2          L3  L3  \n";
                    cout << "Type Address  Page # Off  Res. Pg # DC Tag Ind Res. L2 Tag Ind Res. L3 Tag Ind Res.\n";
                    cout << "---- -------- ------ ---- ---- ---- ------ --- ---- ------ --- ---- ------ --- ----\n";
                    for (int i = 0; i != memInfoVector.size(); i++)
                    {
                        printf("%4s %08x %6x %4x %4s %4x %6s %3x %4s %6s %3x %4s %6s %3x %4s\n", 
                        memInfoVector[i].type.c_str(),
                        memInfoVector[i].address,
                        memInfoVector[i].virtPageNum,
                        memInfoVector[i].pageOffset,
                        memInfoVector[i].PTResult.c_str(),
                        memInfoVector[i].physPageNum,
                        memInfoVector[i].L1Tag.c_str(),
                        memInfoVector[i].L1Index,
                        memInfoVector[i].L1Result.c_str(),
                        memInfoVector[i].L2Tag.c_str(),
                        memInfoVector[i].L2Index,
                        memInfoVector[i].L2Result.c_str(),
                        memInfoVector[i].L3Tag.c_str(),
                        memInfoVector[i].L3Index,
                        memInfoVector[i].L3Result.c_str());
                    }
                    cout << "\n";
                }
                else if (insertedConfig.L3Active == false)
                {
                    //Virtual Addresses == true
                    //TLB == false
                    //L2 == true
                    //L3 == false

                    cout << "\n";
                    cout << "Addr          Virt.  Page PT   Phys        DC  DC          L2  L2  \n";
                    cout << "Type Address  Page # Off  Res. Pg # DC Tag Ind Res. L2 Tag Ind Res.\n";
                    cout << "---- -------- ------ ---- ---- ---- ------ --- ---- ------ --- ----\n";
                    for (int i = 0; i != memInfoVector.size(); i++)
                    {
                        printf("%4s %08x %6x %4x %4s %4x %6s %3x %4s %6s %3x %4s\n", 
                        memInfoVector[i].type.c_str(),
                        memInfoVector[i].address,
                        memInfoVector[i].virtPageNum,
                        memInfoVector[i].pageOffset,
                        memInfoVector[i].PTResult.c_str(),
                        memInfoVector[i].physPageNum,
                        memInfoVector[i].L1Tag.c_str(),
                        memInfoVector[i].L1Index,
                        memInfoVector[i].L1Result.c_str(),
                        memInfoVector[i].L2Tag.c_str(),
                        memInfoVector[i].L2Index,
                        memInfoVector[i].L2Result.c_str());
                    }
                    cout << "\n";
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

                    cout << "\n";
                    cout << "Addr          Virt.  Page PT   Phys        DC  DC  \n";
                    cout << "Type Address  Page # Off  Res. Pg # DC Tag Ind Res.\n";
                    cout << "---- -------- ------ ---- ---- ---- ------ --- ----\n";
                    for (int i = 0; i != memInfoVector.size(); i++)
                    {
                        printf("%4s %08x %6x %4x %4s %4x %6s %3x %4s\n", 
                        memInfoVector[i].type.c_str(),
                        memInfoVector[i].address,
                        memInfoVector[i].virtPageNum,
                        memInfoVector[i].pageOffset,
                        memInfoVector[i].PTResult.c_str(),
                        memInfoVector[i].physPageNum,
                        memInfoVector[i].L1Tag.c_str(),
                        memInfoVector[i].L1Index,
                        memInfoVector[i].L1Result.c_str());
                    }
                    cout << "\n";
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

                cout << "\n";
                cout << "Addr                 DC  DC          L2  L2          L3  L3  \n";
                cout << "Type Address  DC Tag Ind Res. L2 Tag Ind Res. L3 Tag Ind Res.\n";
                cout << "---- -------- ------ --- ---- ------ --- ---- ------ --- ----\n";
                for (int i = 0; i != memInfoVector.size(); i++)
                {
                    printf("%4s %08x %6s %3x %4s %6s %3x %4s %6s %3x %4s\n", 
                    memInfoVector[i].type.c_str(),
                    memInfoVector[i].address,
                    memInfoVector[i].L1Tag.c_str(),
                    memInfoVector[i].L1Index,
                    memInfoVector[i].L1Result.c_str(),
                    memInfoVector[i].L2Tag.c_str(),
                    memInfoVector[i].L2Index,
                    memInfoVector[i].L2Result.c_str(),
                    memInfoVector[i].L3Tag.c_str(),
                    memInfoVector[i].L3Index,
                    memInfoVector[i].L3Result.c_str());
                }
                cout << "\n";
            }
            else if (insertedConfig.L3Active == false)
            {
                //Virtual Addresses == false
                //TLB == false
                //L2 == true
                //L3 == false

                cout << "\n";
                cout << "Addr                 DC  DC          L2  L2  \n";
                cout << "Type Address  DC Tag Ind Res. L2 Tag Ind Res.\n";
                cout << "---- -------- ------ --- ---- ------ --- ----\n";
                for (int i = 0; i != memInfoVector.size(); i++)
                {
                    printf("%4s %08x %6s %3x %4s %6s %3x %4s\n", 
                    memInfoVector[i].type.c_str(),
                    memInfoVector[i].address,
                    memInfoVector[i].L1Tag.c_str(),
                    memInfoVector[i].L1Index,
                    memInfoVector[i].L1Result.c_str(),
                    memInfoVector[i].L2Tag.c_str(),
                    memInfoVector[i].L2Index,
                    memInfoVector[i].L2Result.c_str());
                }
                cout << "\n";
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

                cout << "\n";
                cout << "Addr                 DC  DC  \n";
                cout << "Type Address  DC Tag Ind Res.\n";
                cout << "---- -------- ------ --- ----\n";
                for (int i = 0; i != memInfoVector.size(); i++)
                {
                    printf("%4s %08x %6s %3x %4s\n", 
                    memInfoVector[i].type.c_str(),
                    memInfoVector[i].address,
                    memInfoVector[i].L1Tag.c_str(),
                    memInfoVector[i].L1Index,
                    memInfoVector[i].L1Result.c_str());
                }
                cout << "\n";
            }
        }
    }
}//end outputEachMemRefInfo()

//generate a cache and resize to the number of sets in config and set each set size from config
vector<string> generateCache(int &sets, int &setSize)
{
    vector<string> cache;

    cache.resize((int)sets);

    for(int i = 0; i < sets; i++)
    {
        cache[i].resize(setSize);
        cache[i] = "NULL";
    }

    return cache;
}

/*//replaces a page in a page list
//L1, L2, L3 are caches
//p1 is new page
//pgs is existing pages
//i is location in pgs of page being replaced
void ReplacePage(vector<string> L1, vector<string> L2, vector<string> L3, int p1, vector<int> pgs, int i)
{
    //finds if there is existing pid of that page in each cache, and replaces with null (-1)
    FindItem(L1, pgs[i]);
    FindItem(L2, pgs[i]);
    FindItem(L3, pgs[i]);

    pgs[i] = p1;//replaces page
}

//finds pid of replaced page and places null where pid is found
void FindItem(vector<string> &cache, string pid)
{
    //searches for a pid in the cache, and if exists replace with null (-1)
    for (vector<string> &v : cache)
    {
        replace(v.begin(), v.end(), pid, -1);
    }
}*/

/*//allocates a chuck in memory for physical pages
//use as pages[pagenum][pageloc]
vector<string> PageAlloc(int numpgs, int pgsize)
{
    //2D vector to return from page allocation
    vector<string> pgs;

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
int LRU(vector<string> pages)
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
int HitMiss(string virtAddress, vector<string> cache)
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

    for(int i = 0; i < cache.size(); i++)
    {
        if(retnum > 0)
        {
            break;
        }

        if(tag.compare(cache[i].substr(5,0)) == 0)
        {
            if(index.compare(cache[i].substr(2,8)))
            {
                if(offset.compare(cache[i].substr(1,10)) != 0)
                {
                    retnum = 1;
                }
            }
        }
        else if(index.compare(cache[i].substr(2,8)))
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
string VirtualToPhysical(string virtAddress)
{
    string new_str = std::string((8 - virtAddress.length()), '0') + virtAddress;
    string stringy = new_str.substr(new_str.length() - 2, 2);

    return stringy;
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

bitset<32> HextoBinary(string x)
{
    stringstream stream;

    stream << hex << x;
    unsigned v;
    stream >> v;

    bitset<32> b(v);

    return b;
}

string BinarytoHex(bitset<32> x)
{
    stringstream ss;
    unsigned n = x.to_ulong();
    ss << hex << n;
    string ret = ss.str();

    for_each(ret.begin(), ret.end(), [](char& c)
    {
        c = toupper((unsigned char) c);
    });

    return ret;
}

int getVirtPageNum(int virtAddress, TraceConfig insertedConfig)
{
    //Converting address to binary number
    bitset<32> num = HextoBinary(toHex(virtAddress));
    string binary = num.to_string();

    //Calculate virtual page number
    string virtPageNum = binary.substr(0, binary.length()-(insertedConfig.pageOffsetBits));
    bitset<32> binVirtNum(virtPageNum);
    //cout << BinarytoHex(binVirtNum) << "\n";

    return toInt(BinarytoHex(binVirtNum));
}

int getOffset(int virtAddress, TraceConfig insertedConfig)
{
    //Converting address to binary number
    bitset<32> num = HextoBinary(toHex(virtAddress));
    string binary = num.to_string();

    //Calculate offset
    string offset = binary.substr((binary.length()-(insertedConfig.pageOffsetBits)), insertedConfig.pageOffsetBits);
    bitset<32> binOffset(offset);
    //cout << BinarytoHex(binOffset) << "\n";
                        
    return toInt(BinarytoHex(binOffset));
}

int getTLBTag(int virtAddress, TraceConfig insertedConfig)
{
    //Converting address to binary number
    bitset<32> num = HextoBinary(toHex(virtAddress));
    string binary = num.to_string();

    string TLBTag = binary.substr(0, (binary.length() - insertedConfig.pageTableIndexBits)-3);
    //cout << TLBTag << "\n";
    bitset<32> binTLBTag(TLBTag);
    //cout << BinarytoHex(binTLBTag) << "\n";

    return toInt(BinarytoHex(binTLBTag));
}

int getTLBIndex(int virtAddress, TraceConfig insertedConfig)
{
    //Converting address to binary number
    bitset<32> num = HextoBinary(toHex(virtAddress));
    string binary = num.to_string();

    string TLBIndex = binary.substr((binary.length() - insertedConfig.pageTableIndexBits) - insertedConfig.TLBIndexBits, insertedConfig.TLBIndexBits);
    bitset<32> binTLBIndex(TLBIndex);

    return toInt(BinarytoHex(binTLBIndex));
}

//Method for branching to each possible combination of the 4 conditionals (Virtual Addresses, TLB, L2, L3)
//Returns final config values for final output
SimStats runSimulation(TraceConfig insertedConfig, SimStats simStats,  vector<MemRefInfo> &memRefs)
{
    //Caches, DTLB, Page Table (In Progress, Needs to be integrated)
    vector<MemRefInfo> L1;
    L1.resize(insertedConfig.L1NumSets);
    vector<MemRefInfo> L2;
    L2.resize(insertedConfig.L2NumSets);
    //vector<MefRefInfo> L3 = generateCache(insertedConfig.L3NumSets, insertedConfig.L3SetSize); //Ignore, not implementing
    DTLB tlb = DTLB(insertedConfig.numTLBSets, insertedConfig.TLBSetSize);
    //(Insert page table declaration & initialization here)
    PageDirectory PD = PageDirectory(-1, calcPDSize(insertedConfig.numVirtPages), calcNumLevels(insertedConfig.numVirtPages), calcPTSize(insertedConfig.numVirtPages));
    Memory mainMem = Memory(insertedConfig.numPhysPages);

    //bitset<32> L1bitset;


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
                        //Increases read or write counters
                        simStats = checkReadOrWrite(i, simStats, memRefs);

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
                        //Get both hex and binary representation of address in string form
                        string hex = toHex(memRefs[i].address);
                        string binary = HextoBinary(hex).to_string();

                        //Get physical address from inserted virtual address
                        string str1 = HextoBinary(VirtualToPhysical(toHex(memRefs[i].address))).to_string();
                        bitset<32> bs3(str1.substr(0, str1.length() - (insertedConfig.L1IndexBits + insertedConfig.L1OffsetBits)));
                        string physAddr = BinarytoHex(bs3); //String hex representation of physical address
                        int physAddrInt = toInt(physAddr);  //Integer representation of physical address

                        //Increases read or write counters
                        simStats = checkReadOrWrite(i, simStats, memRefs);

                        //Get virtual page number and offset
                        memRefs[i].virtPageNum = getVirtPageNum(memRefs[i].address, insertedConfig);
                        memRefs[i].pageOffset = getOffset(memRefs[i].address, insertedConfig);

                        //Get TLB tag and index
                        memRefs[i].TLBTag = getTLBTag(memRefs[i].address, insertedConfig);
                        memRefs[i].TLBIndex = getTLBIndex(memRefs[i].address, insertedConfig);

                        PD.updateTimers();
                        //This command grabs whatever entry is at the given virtual page number
                        PageTableEntry pte = PD.grabEntry(generatePDTableAddress(log2(insertedConfig.numVirtPages), toHex(memRefs[i].virtPageNum)));

                        //Check for tag and index in TLB, insert if not already within TLB
                        if(tlb.checkDTLB(memRefs[i].TLBTag, memRefs[i].TLBIndex) == false)
                        {
                            //Miss if TLB tag and index is not already within TLB
                            memRefs[i].TLBResult = "miss";
                            tlb.insertRecentAddress(memRefs[i].TLBIndex, memRefs[i].TLBTag, memRefs[i].address, physAddrInt);

                            //(Check page table here for tag and index)
                            //(If already within page table, hit and figure out physical page number)
                            
                            
                            if (!pte.getValid())                                //If the valid bit is not flipped then the PT missed
                            {
                                memRefs[i].PTResult = "miss";
                                int loc = mainMem.findFree();                   //Attempt to find an empty position in Main memory to assign the new Entry too
                                if (loc < 0)                                    //If there isn't any free space, we'll find the LRU entry, remove it, and assign the LRU entry's main memory location to the new PTE
                                {
                                    vector<int> oldLoc = PD.findLRUEntry();
                                    PageTableEntry temp = PD.grabEntry(oldLoc);
                                    PD.removeEntry(oldLoc);
                                    PD.placeEntry(temp, generatePDTableAddress(log2(insertedConfig.numVirtPages), toHex(memRefs[i].virtPageNum)));
                                    pte = temp;

                                }
                                else                                            //If there is free space in main mem then find the first open slot and assign it to the virtual address we're given
                                {
                                    PageTableEntry temp = PageTableEntry(true, true, true, loc);
                                    PD.placeEntry(temp, generatePDTableAddress(log2(insertedConfig.numVirtPages), toHex(memRefs[i].virtPageNum)));
                                    pte = temp;
                                    mainMem.assignLocation(loc);
                                }
                            }
                            else                                                //If the valid bit is flipped then the PT hit
                            {
                                memRefs[i].PTResult = "hit";
                            }

                            memRefs[i].physPageNum = pte.getAddress();
                            
                            //(Else: Miss and insert into page table and figure out physical page number)
                        }
                        else
                        {
                            //Hit if TLB tag and index is already within TLB
                            //(This will skip page table because it should also be in the page table if it is already within TLB)
                            memRefs[i].TLBResult = "hit";
                            memRefs[i].PTResult = "";
                            memRefs[i].physPageNum = pte.getAddress();
                        }
                        
                        if(i == 0)
                        {
                                memRefs[i].L1Result = "miss";
                                memRefs[i].L2Result = "miss";
                        }

                        //L1 tag (done)
                        if(hex.length() == 1)
                        {
                            hex = "00" + hex;
                        }
                        else if(hex.length() == 2)
                        {
                            hex = "0" + hex;
                        }
                        string cachePhys = to_string(memRefs[i].physPageNum) + hex.substr(1, (hex.length() - 1));
                        cachePhys = HextoBinary(cachePhys).to_string().substr(0, 32 - (insertedConfig.L1OffsetBits + insertedConfig.L1IndexBits));
                        memRefs[i].L1Tag = toHex(stoi(cachePhys, 0, 2));

                        //L1 index
                        string L1IndexStr = binary.substr(binary.length() - (insertedConfig.L1OffsetBits + insertedConfig.L1IndexBits), insertedConfig.L1IndexBits);
                        bitset<32> L1bitset(L1IndexStr);
                        memRefs[i].L1Index = toInt(BinarytoHex(L1bitset));
                        
                        //L1 hit/miss 
                        //L2 must hit then L1 can hit
                        for(int p = i; p > 0; p--)
                        {
                            if(memRefs[p].L2Result == "hit")
                            {
                                if(memRefs[p].physPageNum == memRefs[i].physPageNum && memRefs[p].L1Tag == memRefs[i].L1Tag)
                                {
                                    if(memRefs[p].TLBResult == "hit")
                                    {
                                        memRefs[i].L1Result = "hit";
                                        ReorderCache(L1, i, memRefs);
                                        break;
                                    }
                                    else
                                    {
                                        memRefs[i].L1Result = "miss";
                                        ReorderCache(L1, i, memRefs);
                                    }
                                }
                                else
                                {
                                    memRefs[i].L1Result = "miss";
                                    ReorderCache(L1, i, memRefs);
                                }
                            }
                            else
                            {
                                    memRefs[i].L1Result = "miss";
                                    ReorderCache(L1, i, memRefs);
                            }
                        }
                        

                        //If L1 hits on current memref, skip L2
                        if(memRefs[i].L1Result == "hit")
                        {
                            //Zero/null out L2 cache if L1 hit
                            memRefs[i].L2Tag = "";
                            memRefs[i].L2Index = 0;
                            memRefs[i].L2Result = "";
                        }
                        else //Else, go through L2 cache
                        {
                            //L2 tag (done)
                            memRefs[i].L2Tag = to_string(memRefs[i].physPageNum);

                            //L2 index
                            string L2IndexStr = binary.substr(binary.length() - (insertedConfig.L2OffsetBits + insertedConfig.L2IndexBits), insertedConfig.L2IndexBits);
                            bitset<32> L2bitset(L2IndexStr);
                            memRefs[i].L2Index = toInt(BinarytoHex(L2bitset));

                            //L2 hit/miss 
                            for(int p = 0; p < i; p++)
                            {
                                if(memRefs[p].L2Tag == memRefs[i].L2Tag && memRefs[p].L2Index == memRefs[i].L2Index)
                                {
                                    memRefs[i].L2Result = "hit";
                                    ReorderCache(L2, i, memRefs);
                                    break;    
                                }                                
                                else
                                {
                                    memRefs[i].L2Result = "miss";
                                    ReorderCache(L2, i, memRefs);
                                }
                            }
                        }

                        //===================================//
                        //Calc virtual page number (DONE)
                        //Calc page offset (DONE)
                        //Calc TLB tag (DONE)
                        //Calc TLB index (DONE)
                        //Calc TLB result (hit/miss) (DONE)
                        //Calc Page Table result (hit/miss) (DONE)
                        //Calc physical page number (DONE)
                        //Calc L1/DC tag (DONE)
                        //Calc L1/DC index (DONE)
                        //Calc L1/DC result (hit/miss) (In Progress)
                        //Calc L2 tag (DONE)
                        //Calc L2 index (DONE)
                        //Calc L2 result (hit/miss) (In Progress)
                        //===================================//
                    }
  
                    //Finalizes simulation statistic counters
                    simStats = calcHitMissCounters(simStats, memRefs);
                    simStats = calcMemPageDiskRefs(simStats, memRefs);
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
                        //Get both hex and binary representation of address in string form
                        string hex = toHex(memRefs[i].address);
                        string binary = HextoBinary(hex).to_string();

                        //Get physical address from inserted virtual address
                        string str1 = HextoBinary(VirtualToPhysical(toHex(memRefs[i].address))).to_string();
                        bitset<32> bs3(str1.substr(0, str1.length() - (insertedConfig.L1IndexBits + insertedConfig.L1OffsetBits)));
                        string physAddr = BinarytoHex(bs3); //String hex representation of physical address
                        int physAddrInt = toInt(physAddr);  //Integer representation of physical address

                        //Increases read or write counters
                        simStats = checkReadOrWrite(i, simStats, memRefs);

                        //Get virtual page number and offset
                        memRefs[i].virtPageNum = getVirtPageNum(memRefs[i].address, insertedConfig);
                        memRefs[i].pageOffset = getOffset(memRefs[i].address, insertedConfig);

                        //Get TLB tag and index
                        memRefs[i].TLBTag = getTLBTag(memRefs[i].address, insertedConfig);
                        memRefs[i].TLBIndex = getTLBIndex(memRefs[i].address, insertedConfig);

                        PD.updateTimers();
                        //This command grabs whatever entry is at the given virtual page number
                        PageTableEntry pte = PD.grabEntry(generatePDTableAddress(log2(insertedConfig.numVirtPages), toHex(memRefs[i].virtPageNum)));

                        //Check for tag and index in TLB, insert if not already within TLB
                        if(tlb.checkDTLB(memRefs[i].TLBTag, memRefs[i].TLBIndex) == false)
                        {
                            //Miss if TLB tag and index is not already within TLB
                            memRefs[i].TLBResult = "miss";
                            tlb.insertRecentAddress(memRefs[i].TLBIndex, memRefs[i].TLBTag, memRefs[i].address, physAddrInt);

                            //(Check page table here for tag and index)
                            //(If already within page table, hit and figure out physical page number)
                            
                            
                            if (!pte.getValid())                                //If the valid bit is not flipped then the PT missed
                            {
                                memRefs[i].PTResult = "miss";
                                int loc = mainMem.findFree();                   //Attempt to find an empty position in Main memory to assign the new Entry too
                                if (loc < 0)                                    //If there isn't any free space, we'll find the LRU entry, remove it, and assign the LRU entry's main memory location to the new PTE
                                {
                                    vector<int> oldLoc = PD.findLRUEntry();
                                    PageTableEntry temp = PD.grabEntry(oldLoc);
                                    PD.removeEntry(oldLoc);
                                    PD.placeEntry(temp, generatePDTableAddress(log2(insertedConfig.numVirtPages), toHex(memRefs[i].virtPageNum)));
                                    pte = temp;

                                }
                                else                                            //If there is free space in main mem then find the first open slot and assign it to the virtual address we're given
                                {
                                    PageTableEntry temp = PageTableEntry(true, true, true, loc);
                                    PD.placeEntry(temp, generatePDTableAddress(log2(insertedConfig.numVirtPages), toHex(memRefs[i].virtPageNum)));
                                    pte = temp;
                                    mainMem.assignLocation(loc);
                                }
                            }
                            else                                                //If the valid bit is flipped then the PT hit
                            {
                                memRefs[i].PTResult = "hit";
                            }

                            memRefs[i].physPageNum = pte.getAddress();
                            
                            //(Else: Miss and insert into page table and figure out physical page number)
                        }
                        else
                        {
                            //Hit if TLB tag and index is already within TLB
                            //(This will skip page table because it should also be in the page table if it is already within TLB)
                            memRefs[i].TLBResult = "hit";
                            memRefs[i].PTResult = "";
                            memRefs[i].physPageNum = pte.getAddress();
                        }
                        
                        if(i == 0)
                        {
                                memRefs[i].L1Result = "miss";
                                memRefs[i].L2Result = "miss";
                        }

                        //L1 tag (done)
                        if(hex.length() == 1)
                        {
                            hex = "00" + hex;
                        }
                        else if(hex.length() == 2)
                        {
                            hex = "0" + hex;
                        }
                        string cachePhys = to_string(memRefs[i].physPageNum) + hex.substr(1, (hex.length() - 1));
                        cachePhys = HextoBinary(cachePhys).to_string().substr(0, 32 - (insertedConfig.L1OffsetBits + insertedConfig.L1IndexBits));
                        memRefs[i].L1Tag = toHex(stoi(cachePhys, 0, 2));

                        //L1 index
                        string L1IndexStr = binary.substr(binary.length() - (insertedConfig.L1OffsetBits + insertedConfig.L1IndexBits), insertedConfig.L1IndexBits);
                        bitset<32> L1bitset(L1IndexStr);
                        memRefs[i].L1Index = toInt(BinarytoHex(L1bitset));
                        
                        //L1 hit/miss 
                        //L2 must hit then L1 can hit
                        for(int p = i; p > 0; p--)
                        {
                            if(memRefs[p].physPageNum == memRefs[i].physPageNum && memRefs[p].L1Tag == memRefs[i].L1Tag)
                            {
                                if(memRefs[p].TLBResult == "hit")
                                {
                                    memRefs[i].L1Result = "hit";
                                    ReorderCache(L1, i, memRefs);
                                    break;
                                }
                                else
                                {
                                    memRefs[i].L1Result = "miss";
                                    ReorderCache(L1, i, memRefs);
                                }
                            }
                            else
                            {
                                memRefs[i].L1Result = "miss";
                                ReorderCache(L1, i, memRefs);
                            }
                            
                        }
                        

                        

                        //===================================//
                        //Calc virtual page number (DONE)
                        //Calc page offset (DONE)
                        //Calc TLB tag (DONE)
                        //Calc TLB index (DONE)
                        //Calc TLB result (hit/miss) (DONE)
                        //Calc Page Table result (hit/miss) (DONE)
                        //Calc physical page number (DONE)
                        //Calc L1/DC tag (DONE)
                        //Calc L1/DC index (DONE)
                        //Calc L1/DC result (hit/miss) (In Progress)
                        //===================================//
                    }

                    //Finalizes simulation statistic counters
                    simStats = calcHitMissCounters(simStats, memRefs);
                    simStats = calcMemPageDiskRefs(simStats, memRefs);
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
                        //Increases read or write counters
                        simStats = checkReadOrWrite(i, simStats, memRefs);

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
                        //Get both hex and binary representation of address in string form
                        string hex = toHex(memRefs[i].address);
                        string binary = HextoBinary(hex).to_string();

                        //Get physical address from inserted virtual address
                        string str1 = HextoBinary(VirtualToPhysical(toHex(memRefs[i].address))).to_string();
                        bitset<32> bs3(str1.substr(0, str1.length() - (insertedConfig.L1IndexBits + insertedConfig.L1OffsetBits)));
                        string physAddr = BinarytoHex(bs3); //String hex representation of physical address
                        int physAddrInt = toInt(physAddr);  //Integer representation of physical address

                        //Increases read or write counters
                        simStats = checkReadOrWrite(i, simStats, memRefs);

                        //Get virtual page number and offset
                        memRefs[i].virtPageNum = getVirtPageNum(memRefs[i].address, insertedConfig);
                        memRefs[i].pageOffset = getOffset(memRefs[i].address, insertedConfig);

                        PD.updateTimers();
                        //This command grabs whatever entry is at the given virtual page number
                        PageTableEntry pte = PD.grabEntry(generatePDTableAddress(log2(insertedConfig.numVirtPages), toHex(memRefs[i].virtPageNum)));

                        if (!pte.getValid())                                //If the valid bit is not flipped then the PT missed
                        {
                            memRefs[i].PTResult = "miss";
                            int loc = mainMem.findFree();                   //Attempt to find an empty position in Main memory to assign the new Entry too
                            if (loc < 0)                                    //If there isn't any free space, we'll find the LRU entry, remove it, and assign the LRU entry's main memory location to the new PTE
                            {
                                vector<int> oldLoc = PD.findLRUEntry();
                                PageTableEntry temp = PD.grabEntry(oldLoc);
                                PD.removeEntry(oldLoc);
                                PD.placeEntry(temp, generatePDTableAddress(log2(insertedConfig.numVirtPages), toHex(memRefs[i].virtPageNum)));
                                pte = temp;

                            }
                            else                                            //If there is free space in main mem then find the first open slot and assign it to the virtual address we're given
                            {
                                PageTableEntry temp = PageTableEntry(true, true, true, loc);
                                PD.placeEntry(temp, generatePDTableAddress(log2(insertedConfig.numVirtPages), toHex(memRefs[i].virtPageNum)));
                                pte = temp;
                                mainMem.assignLocation(loc);
                            }
                        }
                        else                                                //If the valid bit is flipped then the PT hit
                        {
                            memRefs[i].PTResult = "hit";
                        }

                        memRefs[i].physPageNum = pte.getAddress();
         
                        if(i == 0)
                        {
                                memRefs[i].L1Result = "miss";
                                memRefs[i].L2Result = "miss";
                        }

                        //L1 tag (done)
                        if(hex.length() == 1)
                        {
                            hex = "00" + hex;
                        }
                        else if(hex.length() == 2)
                        {
                            hex = "0" + hex;
                        }
                        string cachePhys = to_string(memRefs[i].physPageNum) + hex.substr(1, (hex.length() - 1));
                        cachePhys = HextoBinary(cachePhys).to_string().substr(0, 32 - (insertedConfig.L1OffsetBits + insertedConfig.L1IndexBits));
                        memRefs[i].L1Tag = toHex(stoi(cachePhys, 0, 2));

                        //L1 index
                        string L1IndexStr = binary.substr(binary.length() - (insertedConfig.L1OffsetBits + insertedConfig.L1IndexBits), insertedConfig.L1IndexBits);
                        bitset<32> L1bitset(L1IndexStr);
                        memRefs[i].L1Index = toInt(BinarytoHex(L1bitset));
                        
                        //L1 hit/miss 
                        //L2 must hit then L1 can hit
                        for(int p = i; p > 0; p--)
                        {
                            if(memRefs[p].L2Result == "hit")
                            {
                                if(memRefs[p].physPageNum == memRefs[i].physPageNum && memRefs[p].L1Tag == memRefs[i].L1Tag)
                                {
                                    if(memRefs[p].TLBResult == "hit")
                                    {
                                        memRefs[i].L1Result = "hit";
                                        ReorderCache(L1, i, memRefs);
                                        break;
                                    }
                                    else
                                    {
                                        memRefs[i].L1Result = "miss";
                                        ReorderCache(L1, i, memRefs);
                                    }
                                }
                                else
                                {
                                    memRefs[i].L1Result = "miss";
                                    ReorderCache(L1, i, memRefs);
                                }
                            }
                            else
                            {
                                    memRefs[i].L1Result = "miss";
                                    ReorderCache(L1, i, memRefs);
                            }
                        }
                        

                        //If L1 hits on current memref, skip L2
                        if(memRefs[i].L1Result == "hit")
                        {
                            //Zero/null out L2 cache if L1 hit
                            memRefs[i].L2Tag = "";
                            memRefs[i].L2Index = 0;
                            memRefs[i].L2Result = "";
                        }
                        else //Else, go through L2 cache
                        {
                            //L2 tag (done)
                            memRefs[i].L2Tag = to_string(memRefs[i].physPageNum);

                            //L2 index
                            string L2IndexStr = binary.substr(binary.length() - (insertedConfig.L2OffsetBits + insertedConfig.L2IndexBits), insertedConfig.L2IndexBits);
                            bitset<32> L2bitset(L2IndexStr);
                            memRefs[i].L2Index = toInt(BinarytoHex(L2bitset));

                            
                            for(int p = 0; p < i; p++)
                            {
                                if(memRefs[p].L2Tag == memRefs[i].L2Tag && memRefs[p].L2Index == memRefs[i].L2Index)
                                {
                                    memRefs[i].L2Result = "hit";
                                    ReorderCache(L2, i, memRefs);
                                    break;    
                                }                                
                                else
                                {
                                    memRefs[i].L2Result = "miss";
                                    ReorderCache(L2, i, memRefs);
                                }
                            }
                        }

                        //===================================//
                        //Calc virtual page number (DONE)
                        //Calc page offset (DONE)
                        //Calc Page Table result (hit/miss) (DONE)
                        //Calc physical page number (DONE)
                        //Calc L1/DC tag (DONE)
                        //Calc L1/DC index (DONE)
                        //Calc L1/DC result (hit/miss) (In Progress)
                        //Calc L2 tag (DONE)
                        //Calc L2 index (DONE)
                        //Calc L2 result (hit/miss) (In Progress)
                        //===================================//
                    }

                    //Finalizes simulation statistic counters
                    simStats = calcHitMissCounters(simStats, memRefs);
                    simStats = calcMemPageDiskRefs(simStats, memRefs);
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
                        //Get both hex and binary representation of address in string form
                        string hex = toHex(memRefs[i].address);
                        string binary = HextoBinary(hex).to_string();

                        //Get physical address from inserted virtual address
                        string str1 = HextoBinary(VirtualToPhysical(toHex(memRefs[i].address))).to_string();
                        bitset<32> bs3(str1.substr(0, str1.length() - (insertedConfig.L1IndexBits + insertedConfig.L1OffsetBits)));
                        string physAddr = BinarytoHex(bs3); //String hex representation of physical address
                        int physAddrInt = toInt(physAddr);  //Integer representation of physical address

                        //Increases read or write counters
                        simStats = checkReadOrWrite(i, simStats, memRefs);

                        //Get virtual page number and offset
                        memRefs[i].virtPageNum = getVirtPageNum(memRefs[i].address, insertedConfig);
                        memRefs[i].pageOffset = getOffset(memRefs[i].address, insertedConfig);

                        PD.updateTimers();
                        //This command grabs whatever entry is at the given virtual page number
                        PageTableEntry pte = PD.grabEntry(generatePDTableAddress(log2(insertedConfig.numVirtPages), toHex(memRefs[i].virtPageNum)));

                        if (!pte.getValid())                                //If the valid bit is not flipped then the PT missed
                        {
                            memRefs[i].PTResult = "miss";
                            int loc = mainMem.findFree();                   //Attempt to find an empty position in Main memory to assign the new Entry too
                            if (loc < 0)                                    //If there isn't any free space, we'll find the LRU entry, remove it, and assign the LRU entry's main memory location to the new PTE
                            {
                                vector<int> oldLoc = PD.findLRUEntry();
                                PageTableEntry temp = PD.grabEntry(oldLoc);
                                PD.removeEntry(oldLoc);
                                PD.placeEntry(temp, generatePDTableAddress(log2(insertedConfig.numVirtPages), toHex(memRefs[i].virtPageNum)));
                                pte = temp;

                            }
                            else                                            //If there is free space in main mem then find the first open slot and assign it to the virtual address we're given
                            {
                                PageTableEntry temp = PageTableEntry(true, true, true, loc);
                                PD.placeEntry(temp, generatePDTableAddress(log2(insertedConfig.numVirtPages), toHex(memRefs[i].virtPageNum)));
                                pte = temp;
                                mainMem.assignLocation(loc);
                            }
                        }
                        else                                                //If the valid bit is flipped then the PT hit
                        {
                            memRefs[i].PTResult = "hit";
                        }

                        memRefs[i].physPageNum = pte.getAddress();
         
                        if(i == 0)
                        {
                                memRefs[i].L1Result = "miss";
                                memRefs[i].L2Result = "miss";
                        }

                        //L1 tag (done)
                        if(hex.length() == 1)
                        {
                            hex = "00" + hex;
                        }
                        else if(hex.length() == 2)
                        {
                            hex = "0" + hex;
                        }
                        string cachePhys = to_string(memRefs[i].physPageNum) + hex.substr(1, (hex.length() - 1));
                        cachePhys = HextoBinary(cachePhys).to_string().substr(0, 32 - (insertedConfig.L1OffsetBits + insertedConfig.L1IndexBits));
                        memRefs[i].L1Tag = toHex(stoi(cachePhys, 0, 2));

                        //L1 index
                        string L1IndexStr = binary.substr(binary.length() - (insertedConfig.L1OffsetBits + insertedConfig.L1IndexBits), insertedConfig.L1IndexBits);
                        bitset<32> L1bitset(L1IndexStr);
                        memRefs[i].L1Index = toInt(BinarytoHex(L1bitset));

                        for(int p = i; p > 0; p--)
                        {
                            if(memRefs[p].physPageNum == memRefs[i].physPageNum && memRefs[p].L1Tag == memRefs[i].L1Tag)
                            {
                                if(memRefs[p].TLBResult == "hit")
                                {
                                    memRefs[i].L1Result = "hit";
                                    ReorderCache(L1, i, memRefs);
                                    break;
                                }
                                else
                                {
                                    memRefs[i].L1Result = "miss";
                                    ReorderCache(L1, i, memRefs);
                                }
                            }
                            else
                            {
                                memRefs[i].L1Result = "miss";
                                ReorderCache(L1, i, memRefs);
                            }
                            
                        }

                        //===================================//
                        //Calc virtual page number (DONE)
                        //Calc page offset (DONE)
                        //Calc Page Table result (hit/miss) (DONE)
                        //Calc physical page number (DONE)
                        //Calc L1/DC tag (DONE)
                        //Calc L1/DC index (DONE)
                        //Calc L1/DC result (hit/miss) (In Progess)
                        //===================================//
                    }

                    //Finalizes simulation statistic counters
                    simStats = calcHitMissCounters(simStats, memRefs);
                    simStats = calcMemPageDiskRefs(simStats, memRefs);
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
                    //Increases read or write counters
                    simStats = checkReadOrWrite(i, simStats, memRefs);

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
                    //Get both hex and binary representation of address in string form
                    string hex = toHex(memRefs[i].address);
                    string binary = HextoBinary(hex).to_string();

                    //Increases read or write counters
                    simStats = checkReadOrWrite(i, simStats, memRefs);

                    if(i == 0)
                    {
                            memRefs[i].L1Result = "miss";
                            memRefs[i].L2Result = "miss";
                    }

                    //L1 tag (done)
                    if(hex.length() == 1)
                    {
                        hex = "00" + hex;
                    }
                    else if(hex.length() == 2)
                    {
                        hex = "0" + hex;
                    }
                    string cachePhys = to_string(memRefs[i].physPageNum) + hex.substr(1, (hex.length() - 1));
                    cachePhys = HextoBinary(cachePhys).to_string().substr(0, 32 - (insertedConfig.L1OffsetBits + insertedConfig.L1IndexBits));
                    memRefs[i].L1Tag = toHex(stoi(cachePhys, 0, 2));

                    //L1 index
                    string L1IndexStr = binary.substr(binary.length() - (insertedConfig.L1OffsetBits + insertedConfig.L1IndexBits), insertedConfig.L1IndexBits);
                    bitset<32> L1bitset(L1IndexStr);
                    memRefs[i].L1Index = toInt(BinarytoHex(L1bitset));



                    //===================================//
                    //Calc L1/DC tag (DONE)
                    //Calc L1/DC index (DONE)
                    //Calc L1/DC result (hit/miss) (In Progress)
                    //Calc L2 tag (DONE)
                    //Calc L2 index (DONE)
                    //Calc L2 result (hit/miss) (In Progress)
                    //===================================//
                }

                //Finalizes simulation statistic counters
                simStats = calcHitMissCounters(simStats, memRefs);
                simStats = calcMemPageDiskRefs(simStats, memRefs);
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
                    //Get both hex and binary representation of address in string form
                    string hex = toHex(memRefs[i].address);
                    string binary = HextoBinary(hex).to_string();

                    //Increases read or write counters
                    simStats = checkReadOrWrite(i, simStats, memRefs);

                    //===================================//
                    //Calc L1/DC tag (DONE)
                    //Calc L1/DC index (DONE)
                    //Calc L1/DC result (hit/miss) (In Progress)
                    //===================================//
                }

                //Finalizes simulation statistic counters
                simStats = calcHitMissCounters(simStats, memRefs);
                simStats = calcMemPageDiskRefs(simStats, memRefs);
            }
        }
    }

    return simStats;
}


void DirectAssociative(vector<string> L1, vector<string> L2, string virtualAddress, string baseAddress, string bounds, int blocks)
{
    //direct associative
    string physical = VirtualToPhysical(virtualAddress);
    int frameNum = getFrameNumber(physical, blocks);

    L1[frameNum] = virtualAddress;
    L2[frameNum] = virtualAddress;
}

//Method for returning to a SimStats object whether or not the memory reference was a read or write
//Returns modified SimStats object with updated Read/Write counters
SimStats checkReadOrWrite(int i, SimStats simStats, vector<MemRefInfo> memRefs)
{
    if(memRefs[i].type == "R")
    {
        simStats.readsCount++;
    }
    else if (memRefs[i].type == "W")
    {
        simStats.writesCount++;
    }

    return simStats;
}

//Method for calculating the number of main memory, page table, and disk references
//Returns modified SimStats object with updated main memory, page table, and disk reference counters
SimStats calcMemPageDiskRefs(SimStats simStats, vector<MemRefInfo> memRefs)
{
    //Goes through every memory reference to count main memory, page table, and disk references
    for(int i = 0; i < memRefs.size(); i++)
    {
        //If both caches miss (or just DC if L2 cache is disabled), increase main memory reference counter
        //(This might be correct???)
        if(memRefs[i].L1Result == "miss" 
        && (memRefs[i].L2Result == "miss" || memRefs[i].L2Result == "null") 
        && (memRefs[i].L3Result == "miss" || memRefs[i].L3Result == "null"))
        {
            simStats.mainMemRefsCount++;
        }

        //If memory reference accesses the page table at all, increase page table reference counter
        if(memRefs[i].PTResult == "hit" || memRefs[i].PTResult == "miss")
        {
            simStats.pageTableRefsCount++;
        }

        //If memory reference misses all of the components (including null due to toggleable TLB and L2 cache), increase disk reference counter
        if((memRefs[i].TLBResult == "miss" || memRefs[i].TLBResult == "null")
        && (memRefs[i].PTResult == "miss" || memRefs[i].PTResult == "null")
        && memRefs[i].L1Result == "miss" 
        && (memRefs[i].L2Result == "miss" || memRefs[i].L2Result == "null")
        && (memRefs[i].L3Result == "miss" || memRefs[i].L3Result == "null"))
        {
            simStats.diskRefsCount++;
        }
    }

    return simStats;
}

//Method for calculating the number of total hits and misses for all of the memory references
//Returns modified SimStats object with updated hit and miss counters
SimStats calcHitMissCounters(SimStats simStats, vector<MemRefInfo> memRefs)
{
    //Goes through every memory reference to count hits and misses
    for(int i = 0; i < memRefs.size(); i++)
    {
        //Counts TLB hits and misses
        if(memRefs[i].TLBResult == "hit")
        {
            simStats.dtlbHitCount++;
        }
        else if (memRefs[i].TLBResult == "miss")
        {
            simStats.dtlbMissCount++;
        }

        //Counts Page Table hits and misses
        if(memRefs[i].PTResult == "hit")
        {
            simStats.ptHitCount++;
        }
        else if (memRefs[i].PTResult == "miss")
        {
            simStats.ptFaultCount++;
        }

        //Counts L1/DC Cache hits and misses
        if(memRefs[i].L1Result == "hit")
        {
            simStats.dcHitCount++;
        }
        else if (memRefs[i].L1Result == "miss")
        {
            simStats.dcMissCount++;
        }
        
        //Counts L2 Cache hits and misses
        if(memRefs[i].L2Result == "hit")
        {
            simStats.l2HitCount++;
        }
        else if (memRefs[i].L2Result == "miss")
        {
            simStats.l2MissCount++;
        }

        //Counts L3 Cache hits and misses
        if(memRefs[i].L3Result == "hit")
        {
            simStats.l3HitCount++;
        }
        else if (memRefs[i].L3Result == "miss")
        {
            simStats.l3MissCount++;
        }
    }

    return simStats;
}

//finds the address to evict and replaces with new address
//cache = the cache being evicted from
//address = the address being evicted
//addReplace = the new address
//L1 = reference to L1 cache
//L2 = reference to L2 cache
void Evict(string cache, string address, string addReplace, vector<string> L1, vector<string> L2)
{
    if(cache.compare("L1") == 0)
    {
        for(int i = 0; i < L1.size(); i++)
        {
            if(L1[i].compare(address) == 0)
            {
                L1[i] = addReplace;
                break;
            }
        }
    }
    else
    {
        for(int i = 0; i < L1.size(); i++)
        {
            if(L1[i].compare(address) == 0)
            {
                L1[i] = addReplace;
                break;
            }
        }

        for(int i = 0; i < L2.size(); i++)
        {
            if(L2[i].compare(address) == 0)
            {
                L2[i] = addReplace;
                break;
            }
        }
    }

}

void writeAlloc(vector<string> L1, vector<string> L2, string address, int blocks)
{
    int frameNum = getFrameNumber(address, blocks);
    for(int i = 0; i < L1.size(); i++)
    {
        if(L1[i].compare(address) == 0)
        {
            break;
        }

        else if(L2[i].compare(address) == 0)
        {
            break;
        }

        else
        {
            L1[frameNum] = address;
            L2[frameNum] = address;
        }
    }
}

int calcPDSize(int virtPages)
{
    int x = log2(virtPages);
    if (x < 6)
    {
        return 1;
    }
    else
    {
        return 8;
    }
}

int calcPTSize(int virtPages)
{
    int x = log2(virtPages);
    int y = x % 3;
    int z = x / 3;
    if (x < 6)
    {
        return pow(2, x);
    }
    else
    {
        return pow(2, 3 + y);
    }
}

int calcNumLevels(int virtPages)
{
    int x = log2(virtPages);
    int z = x / 3;
    if (x < 6)
    {
        return 1;
    }
    else
    {
        return z - 1;
    }
}

std::vector<int> generatePDTableAddress(int numberOfBits, string virtInHex)
{
    vector<int> temp;
    bitset<32> bits = HextoBinary(virtInHex);
    bitset<32> tempBits = HextoBinary("0");
    if (numberOfBits < 6)
    {
        temp.push_back(0);
        temp.push_back(bin32ToInt(bits));
        return temp;
    }
    else 
    {
        int levels = numberOfBits / 3;
        for (int i = 0; i < levels - 1; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (bits[numberOfBits - 1 - j - (3 * i)])
                {
                    tempBits[2 - j].flip();
                }
            }
            temp.push_back(bin32ToInt(tempBits));
            tempBits.reset();
        }
        int ptBits = 3 + (numberOfBits % 3);
        for (int i = ptBits - 1; i >= 0; i--)
        {
            if (bits[i])
            {
                tempBits[i].flip();
            }
        }
        temp.push_back(bin32ToInt(tempBits));
        return temp;
    }
}

int bin32ToInt(bitset<32> x)
{
    int temp = 0;
    for (int i = 31; i >= 0; i--)
    {
        if (x[i])
        {
            temp += pow(2, i);
        }
    }
    return temp;
}

//reoders cache to put most recently used address at top of cache
void ReorderCache (vector<MemRefInfo> &LCache, int loc, vector<MemRefInfo> &MemRefs)
{
    MemRefInfo temp = MemRefs[loc];
    for(int i = 0; i < LCache.size() - 1; i++)
    {
        LCache[i + 1] = LCache[i];
    }

    LCache[0] = temp;
}