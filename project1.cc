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
bool IsPowerOfTwo(int x);


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
    int tempInt;        //Variable for storing current int value for power of two and min/max checks

    ifstream filein("trace.config"); //Input trace.config file

    getline(filein, currentLine); //"Data TLB configuration" info line
    
    //NumTLBSets
    getline(filein, currentLine); //"Number of sets:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempInt = stoi(tempString); //Convert string number at end of line into int
    if(tempInt < 1 || tempInt > 16 || IsPowerOfTwo(tempInt) != true ) //(Max: 16 sets, 1 set = direct mapped, 16 sets = fully associative, Must be power of 2)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The number of TLB sets should be at least 1, at most 16, and a power of 2.\n";
        cout << "Invalid value read: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Insert config value into appropriate parameter
        traceConfig.numTLBSets = tempInt;
    }

    //TLBSetSize
    getline(filein, currentLine);   //"Set size:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempInt = stoi(tempString); //Convert string number at end of line into int
    if(tempInt < 1 || tempInt > 64 || IsPowerOfTwo(tempInt) != true ) //(Max: 64 entries, Must be power of 2)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The number of entries within each TLB set should be at least 1, at most 64, and a power of 2.\n";
        cout << "Invalid value read: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Insert config value into appropriate parameter
        traceConfig.TLBSetSize = tempInt;
    }

    getline(filein, currentLine); //blank line
    getline(filein, currentLine); //"Page Table configuration" info line

    //numVirtPages
    getline(filein, currentLine);   //"Number of virtual pages:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempInt = stoi(tempString); //Convert string number at end of line into int
    if(tempInt < 1 || tempInt > 8192 || IsPowerOfTwo(tempInt) != true ) //(Max: 8192 pages, Must be power of 2)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The number of virtual pages within the page table should be at least 1, at most 8192, and a power of 2.\n";
        cout << "Invalid value read: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Insert config value into appropriate parameter
        traceConfig.numVirtPages = tempInt;
    }

    //numPhysPages
    getline(filein, currentLine);   //"Number of physical pages:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempInt = stoi(tempString); //Convert string number at end of line into int
    if(tempInt < 1 || tempInt > 2048 || IsPowerOfTwo(tempInt) != true ) //(Max: 2048 pages, Must be power of 2)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The number of physical pages within the page table should be at least 1, at most 2048, and a power of 2.\n";
        cout << "Invalid value read: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Insert config value into appropriate parameter
        traceConfig.numPhysPages = tempInt;
    }

    //pageSize
    getline(filein, currentLine);   //"Page size:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempInt = stoi(tempString); //Convert string number at end of line into int
    if(tempInt < 1 || tempInt > 4096 || IsPowerOfTwo(tempInt) != true ) //(Max: 4096 bytes (4kb), Must be power of 2)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The page size of each page within the page table should be at least 1 byte, at most 4096 bytes (4kb), and a power of 2.\n";
        cout << "Invalid value read: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Insert config value into appropriate parameter
        traceConfig.pageSize = tempInt;
    }

    getline(filein, currentLine); //blank line
    getline(filein, currentLine); //"L1 Cache configuration" info line

    //L1NumSets
    getline(filein, currentLine);   //"Number of sets:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempInt = stoi(tempString); //Convert string number at end of line into int
    if(tempInt < 1 || tempInt > 8 || IsPowerOfTwo(tempInt) != true ) //(Max: 8 sets, 1 set = direct mapped, Must be power of 2)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The number of sets within the L1 cache should be at least 1, at most 8, and a power of 2.\n";
        cout << "Invalid value read: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Insert config value into appropriate parameter
        traceConfig.L1NumSets = tempInt;
    }

    //L1SetSize
    getline(filein, currentLine);   //"Set size:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempInt = stoi(tempString); //Convert string number at end of line into int
    if(tempInt < 1 || tempInt > 128 || IsPowerOfTwo(tempInt) != true ) //(Max: 128 entries, Must be power of 2)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The number of entries within each L1 cache set should be at least 1, at most 128, and a power of 2.\n";
        cout << "Invalid value read: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Insert config value into appropriate parameter
        traceConfig.L1SetSize = tempInt;
    }

    //L1LineSize
    getline(filein, currentLine);   //"Set size:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempInt = stoi(tempString); //Convert string number at end of line into int
    if(tempInt < 8 || tempInt > 64 || IsPowerOfTwo(tempInt) != true ) //(Min: 8 bytes, Max: 64 bytes, Must be power of 2)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The line size within the L1 cache should be at least 8 bytes, at most 64 bytes, and a power of 2.\n";
        cout << "Invalid value read: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Insert config value into appropriate parameter
        traceConfig.L1LineSize = tempInt;
    }

    //L1WriteThrough
    getline(filein, currentLine);   //"Write through/no write allocate:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    if(tempString != "y" && tempString != "n") //(y/n = true/false)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The L1 write-through toggle must be 'y' or 'n'.\n";
        cout << "Invalid value read: " << tempString <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {   
        //Insert config value into appropriate parameter
        if(tempString == "y")
        { traceConfig.L1WriteThrough = true; }
        if(tempString == "n")
        { traceConfig.L1WriteThrough = false; }
    }

    getline(filein, currentLine); //blank line
    getline(filein, currentLine); //"L2 Cache configuration" info line

    //L2NumSets
    getline(filein, currentLine);   //"Number of sets:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempInt = stoi(tempString); //Convert string number at end of line into int
    if(tempInt < 1 || tempInt > 816 || IsPowerOfTwo(tempInt) != true ) //(Max: 16 sets, 1 set = direct mapped, Must be power of 2)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The number of sets within the L2 cache should be at least 1, at most 16, and a power of 2.\n";
        cout << "Invalid value read: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Insert config value into appropriate parameter
        traceConfig.L2NumSets = tempInt;
    }

    //L2SetSize
    getline(filein, currentLine);   //"Set size:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempInt = stoi(tempString); //Convert string number at end of line into int
    if(tempInt < 1 || tempInt > 512 || IsPowerOfTwo(tempInt) != true ) //(Max: 512 entries, Must be power of 2)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The number of entries within each L2 cache set should be at least 1, at most 512, and a power of 2.\n";
        cout << "Invalid value read: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Insert config value into appropriate parameter
        traceConfig.L2SetSize = tempInt;
    }

    //L2LineSize
    getline(filein, currentLine);   //"Set size:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempInt = stoi(tempString); //Convert string number at end of line into int
    if(tempInt < 8 || tempInt > 64 || IsPowerOfTwo(tempInt) != true ) //(Min: 8 bytes, Max: 64 bytes, Must be power of 2)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The line size within the L2 cache should be at least 8 bytes, at most 64 bytes, and a power of 2.\n";
        cout << "Invalid value read: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Insert config value into appropriate parameter
        traceConfig.L2LineSize = tempInt;
    }

    //L2WriteThrough
    getline(filein, currentLine);   //"Write through/no write allocate:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    if(tempString != "y" && tempString != "n") //(y/n = true/false)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The L2 write-through toggle must be 'y' or 'n'.\n";
        cout << "Invalid value read: " << tempString <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {   
        //Insert config value into appropriate parameter
        if(tempString == "y")
        { traceConfig.L2WriteThrough = true; }
        if(tempString == "n")
        { traceConfig.L2WriteThrough = false; }
    }

    getline(filein, currentLine); //blank line

    //VirtAddressActive
    getline(filein, currentLine);   //"Virtual addresses:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    if(tempString != "y" && tempString != "n") //(y/n = true/false)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The virtual addresses active toggle must be 'y' or 'n'.\n";
        cout << "Invalid value read: " << tempString <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {   
        //Insert config value into appropriate parameter
        if(tempString == "y")
        { traceConfig.VirtAddressActive = true; }
        if(tempString == "n")
        { traceConfig.VirtAddressActive = false; }
    }

    //TLBActive
    getline(filein, currentLine);   //"TLB:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    if(tempString != "y" && tempString != "n") //(y/n = true/false)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The TLB active toggle must be 'y' or 'n'.\n";
        cout << "Invalid value read: " << tempString <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {   
        //Insert config value into appropriate parameter
        if(tempString == "y")
        { traceConfig.TLBActive = true; }
        if(tempString == "n")
        { traceConfig.TLBActive = false; }
    }

    //L2Active
    getline(filein, currentLine);   //"L2 cache:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    if(tempString != "y" && tempString != "n") //(y/n = true/false)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The L2 cache active toggle must be 'y' or 'n'.\n";
        cout << "Invalid value read: " << tempString <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {   
        //Insert config value into appropriate parameter
        if(tempString == "y")
        { traceConfig.L2Active = true; }
        if(tempString == "n")
        { traceConfig.L2Active = false; }
    }

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