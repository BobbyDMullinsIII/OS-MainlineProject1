/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Solution/Project:   Project 1 (Mainline Team 1)
//	File Name:          project1.cc
//	Description:        Main file for Operating Systems Mainline Project 1
//	Course:             CSCI-4727-001 - Operating Systems
//	Authors:            Bobby Mullins, Isaiah Jayne, Jacob Manis, Zachary Lykins       
//	Created:            Wednesday, September  21, 2022
//	Copyright:          Mainline Team 1 - Operating Systems
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

//Struct for storing Memory References in an easier to use format
struct MemRef 
{
  string type;    //Memory Access Type (Can be 'R' or 'W')
  string address; //Hex Address (Ranges from 000 (8-bit) to FFFFFFFF (32-bit))
};

//Struct for storing trace.config information within single structure
struct TraceConfig 
{
  //Data TLB Configuration
  int numTLBSets;                 //Number of sets within TLB (Max: 8 sets,  1 set = direct mapped, Must be power of 2)
  int TLBSetSize;                 //Number of entries in each set (Max: 64 entries, Must be power of 2)

  //Page Table Configuration
  int numVirtPages;               //Number of virtual pages in page table (Max: 8192 pages, Must be power of 2)
  int numPhysPages;               //Number of physical pages in page table (Max: 2048 pages, Must be power of 2)
  int pageSize;                   //Size (in bytes) of each page (Max: 4096 bytes (4kb), Must be power of 2)

  //L1 Cache Configuration
  int L1NumSets;                  //Number of sets in L1 (Max: 8 sets, 1 set = direct mapped, Must be power of 2)
  int L1SetSize;                  //Number of entries in each set in L1 (Max: 128 entries, Must be power of 2)
  int L1LineSize;                 //Size (in bytes) of each line in L1 (Min: 8 bytes, Must be power of 2)
  bool L1WriteThrough;            //Switch for L1 to be write-through or not (y/n = true/false)

  //L2 Cache Configuration
  int L2NumSets;                  //Number of sets in L2 (Max: 8 sets, 1 set = direct mapped, Must be power of 2)
  int L2SetSize;                  //Number of entries in each set in L2 (Max: 512 entries, Must be power of 2)
  int L2LineSize;                 //Size (in bytes) of each line in L2 (Min: 8 bytes, Must be power of 2)
  bool L2WriteThrough;            //Switch for L2 to be write-through or not (y/n = true/false)

  //Active Modules Configuration
  bool VirtAddressActive;         //Determines if addresses read in are virtual or physical (y/n = true/false)
  bool TLBActive;                 //Switch for TLB to be active or not (y/n = true/false)
  bool L2Active;                  //Switch for L2 Cache to be active or not (y/n = true/false)
};


//Initial Method Declarations
vector<MemRef> insertTrace(vector<MemRef> testVector);
TraceConfig insertConfig(TraceConfig traceConfig);
void testVectorOutput(vector<MemRef> testVector);
void testConfigOutput(TraceConfig testConfig);


int main()
{
  vector<MemRef> MemReferences; //Vector of MemRef's to work from
  TraceConfig config;           //Data struture of values taken from trace.config file

  //Insert memory references from stdin into MemRef vector
  MemReferences = insertTrace(MemReferences);

  //Insert config values into TraceConfig variable
  config = insertConfig(config);


  //Memory hierarchy code should go here or in another class object file (.hpp and/or .cpp)


  return 0;
}


//Method for inserting Memory References from stdin into the given MemRef vector
//Returns a vector containing MemRef variables
vector<MemRef> insertTrace(vector<MemRef> memRefVector)
{
  string RawMemRef;   //Raw memory reference in '<accesstype>:<hexaddress>' format
  MemRef tempRef;     //Temporary MemRef for inserting each line of memory references into vector
  int stringPosition; //Position variable for splitting memory references by colon

  //Goes through each line in standard input and returns a single Raw Memory Reference string
  while (getline(cin, RawMemRef))
  {   
    //Get position of colon character
    stringPosition = RawMemRef.find_first_of(':');

    //Split RawMemRef into two strings for tempRef variable
    tempRef.type = RawMemRef.substr(0, stringPosition);
    tempRef.address = RawMemRef.substr(stringPosition + 1);

    //Insert tempRef variable into memRefVector
    memRefVector.push_back(tempRef);
  }

  return memRefVector;

}//end insertTrace()

//Method for inserting all data within trace.config into a TraceConfig data structure
//Also checks if any of the config values are not valid and stops program to state what was incorrect
//Returns TraceConfig struct contaning all config data present in trace.config file
TraceConfig insertConfig(TraceConfig traceConfig)
{
  ifstream in("trace.config", ios_base::in);

  //Method not finished yet - Bobby

}//end insertConfig

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

//Method for testing if inserting the config into the TraceConfig variable was successful
void testConfigOutput(TraceConfig testConfig)
{
  cout << "Config Values (Should match trace.config file):\n";
  cout << "numTLBSets: " << testConfig.numTLBSets << "\n";
  cout << "TLBSetSize: " << testConfig.TLBSetSize << "\n";
  cout << "\n";
  cout << "numVirtPages: " << testConfig.numVirtPages << "\n";
  cout << "numPhysPages: " << testConfig.numPhysPages << "\n";
  cout << "pageSize: " << testConfig.pageSize << "\n";
  cout << "\n";
  cout << "L1NumSets: " << testConfig.L1NumSets << "\n";
  cout << "L1SetSize: " << testConfig.L1SetSize << "\n";
  cout << "L1LineSize: " << testConfig.L1LineSize << "\n";
  cout << "L1WriteThrough: " << boolalpha << testConfig.L1WriteThrough << "\n";
  cout << "\n";
  cout << "L2NumSets: " << testConfig.L2NumSets << "\n";
  cout << "L2SetSize: " << testConfig.L2SetSize << "\n";
  cout << "L2LineSize: " << testConfig.L2LineSize << "\n";
  cout << "L2WriteThrough: " << boolalpha << testConfig.L2WriteThrough << "\n";
  cout << "\n";
  cout << "VirtAddressActive: " << boolalpha << testConfig.VirtAddressActive << "\n";
  cout << "TLBActive: " << boolalpha << testConfig.TLBActive << "\n";
  cout << "L2Active: " << boolalpha << testConfig.L2Active << "\n";

}//end testConfigOutput()