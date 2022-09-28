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

//Struct for storing Memory References in an easier to use format
struct MemRef 
{
    string type;    //Memory Access Type (Can be 'R' or 'W')
    string address; //Hex Address (Ranges from 000 (8-bit) to FFFFFFFF (32-bit))
};


//Initial Method Declarations
vector<MemRef> insertTrace(vector<MemRef> testVector);
TraceConfig insertConfig(TraceConfig traceConfig);
void testVectorOutput(vector<MemRef> testVector);
void testConfigOutput(TraceConfig testConfig);


int main()
{
    vector<MemRef> MemReferences; //Vector of MemRef's to work from
    TraceConfig config;           //Class of config values taken from trace.config file

    MemReferences = insertTrace(MemReferences); //Insert memory references from stdin into MemRef vector
    config = insertConfig(config);              //Insert trace.config file values into TraceConfig variable

    //Test if it worked
    config.outputRawConfigValues();

    //Memory hierarchy code should go here or in another class object file (.hpp and/or .cpp/.cc)


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

//Method for inserting all data within trace.config into a TraceConfig data structure
//Also checks if any of the config values are not valid and stops program to state what was not valid
//Returns TraceConfig struct contaning all config data present in trace.config file
TraceConfig insertConfig(TraceConfig traceConfig)
{
    string currentLine; //Variable for storing entire current line
    string tempString;  //Variable for storing current int or bool value before conversion
    int stringIndex;    //Variable for keeping track of index of colon in string
    int tempInt;        //Variable for storing current int value for power of two and min/max checks

    ifstream filein("trace.config");

    getline(filein, currentLine);
    
    getline(filein, currentLine);
    stringIndex = currentLine.find_first_of(':');
    tempString = currentLine.substr(stringIndex + 2);
    tempInt = stoi(tempString);
    if(tempInt < 1 || tempInt > 16 || IsPowerOfTwo(tempInt) != true )
    {
        cout << "Number of TLB sets should be at least 1, at most 16, and a power of 2.\n";
        cout << "Value read was: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        traceConfig.numTLBSets = tempInt;
    }

    //NOT DONE YET!

    return traceConfig;

}//end insertConfig()

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

//Method for checking if number is a power of two
//Stolen from https://stackoverflow.com/questions/600293/how-to-check-if-a-number-is-a-power-of-2
//Returns if the input number (x) is a power of 2
bool IsPowerOfTwo(int x)
{
    return (x != 0) && ((x & (x - 1)) == 0);

}//end IsPowerOfTwo()