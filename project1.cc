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
#include "TraceConfig.hpp"
using namespace std;

//Access states for memory
#define NOOP 0 //no action 
#define RDH 1 //hit for read
#define RDM 2 //miss for read
#define WTH 3 //hit for write
#define WTM 4 //miss for write

//Struct for storing Memory References in an easier to use format
struct MemRef 
{
    string type;    //Memory Access Type (Can be 'R' or 'W')
    string address; //Hex Address (Ranges from 000 (8-bit) to FFFFFFFF (32-bit))
};


//Initial Method Declarations
vector<MemRef> insertTrace(vector<MemRef> testVector);
void testVectorOutput(vector<MemRef> testVector);
vector<vector<long>> generateCache(long &sets, long &setSize);
vector<vector<long>> getL1();
vector<vector<long>> getL2();
vector<vector<long>> getL3();
void FindItem(vector<vector<long>> &cache, int pid);
void ReplacePage(vector<vector<long>> L1, vector<vector<long>> L2 , int p1, vector<int> pgs, int i);
int RLU(int[][] pages);
int[][] PageAlloc(int numpgs, int pgsize);

int main()
{
    vector<MemRef> MemReferences; //Vector of MemRef's to work from
    TraceConfig config;           //Class of config values taken from trace.config file

    MemReferences = insertTrace(MemReferences); //Insert memory references from stdin into MemRef vector
    config.insertConfig();                      //Insert trace.config file values into TraceConfig variable

    //Test if it worked
    config.outputRawConfigValues();


    //Memory hierarchy code should go here or in another class object file (.cc object file and .hpp header file)


    return 0;
}


//Method for inserting Memory References from stdin into the given MemRef vector
//Returns a vector containing MemRef variables
vector<MemRef> insertTrace(vector<MemRef> memRefVector)
{
    string RawMemRef;   //Raw memory reference in '<accesstype>:<hexaddress>' format
    MemRef tempRef;     //Temporary MemRef for inserting each line of memory references into vector
    int stringIndex;    //Variable for keeping track of index of colon in string

    //Goes through each line in standard input and returns a single Raw Memory Reference string
    while (getline(cin, RawMemRef))
    {   
        //Get index of colon character
        stringIndex = RawMemRef.find_first_of(':');

        //Split RawMemRef into two strings for tempRef variable
        tempRef.type = RawMemRef.substr(0, stringIndex);
        tempRef.address = RawMemRef.substr(stringIndex + 1);

        //Insert tempRef variable into memRefVector
        memRefVector.push_back(tempRef);
    }

    return memRefVector;

}//end insertTrace()

//Method for testing if inserting the stdin into the vector with MemRef's was successful
void testVectorOutput(vector<MemRef> memRefVector)
{
    //Test to see if input to vector code worked correctly
    cout << "Memory References from inserted trace file:\n";
    for (int i = 0; i != memRefVector.size(); i++)
    {
        cout << memRefVector[i].type << ":" << memRefVector[i].address <<"\n";
    }

}//end testVectorOutput()

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


//return L1
vector<vector<long>> getL1()
{
    return this->L1;
}

//return L2
vector<vector<long>> getL2()
{
    return this->L2;
}

//return L3
vector<vector<long>> getL3()
{
    return this->L3;
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
        replace(v.begin(), v.end(), pid, NULL);
    }
}

//allocates a chuck in memory for physical pages
//use as pages[pagenum][pageloc]
int** PageAlloc(int numpgs, int pgsize)
{
    int** pgs = 0;
    pages = new int*[numpgs];

    for(int i = 0; i < numpgs; i++)
    {
        pgs[i] = new int[pgsize];
        
        for(int j = 0; j < pgsize; j++)
        {
            pgs[i][j] = 0;
        }
    }

    return pgs;
}

//gets least recently used page
//returns index of page
//assuming least recently used is being tracked
//by very last int on the page
int RLU(int** pages)
{
    int RLU = 0;
    for(int i = 0; i < pages.size; i++)
    {
        if(pages[i][pages[i].size-1] < pages[RLU][pages[i].size-1])
        {
            RLU = i;
        }
    }

    return RLU;
}