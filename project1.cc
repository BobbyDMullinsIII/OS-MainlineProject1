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

int main()
{
    vector<MemRefHex> MemReferencesHex; //Vector of MemRefHex's to work from with addresses in hex (string) form
    vector<MemRefDec> MemReferencesDec; //Vector of MemRefDec's to work from with addresses in decimal (long) form
    TraceConfig config;                 //Class of config values and counters taken from trace.config file

    MemReferencesHex = insertTrace(MemReferencesHex);       //Insert memory references from stdin into MemRefHex vector
    MemReferencesDec = convertToMemRefDec(MemReferencesHex);//Make a copy of MemReferencesHex, but with addresses in easier-to-use long decimal form
    //config.insertConfig(); //ISSUE: Will not let program continue because of weird issue where it will not read 'y' or 'n' correctly in the trace.config file
    config.prepareCounters();

    //=========================================================================================================//
    //Memory hierarchy code should go here or in another class object file (.cc object file and .hpp header file)
    //=========================================================================================================//

    //Output final config values and simulation statistics
    config.outputRawConfigValues();

    //Output memory traces in both decimal and hex form if user wants to
    outputDecAndHex(MemReferencesDec);

    //Exit program
    return 0;
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
        cout << memRefDecVector[i].type << ":" << memRefDecVector[i].address << "   |   ";
        cout << memRefDecVector[i].type << ":" << std::hex << memRefDecVector[i].address <<"\n" << std::dec;
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