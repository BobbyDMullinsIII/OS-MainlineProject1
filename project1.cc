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

//Struct for storing Memory References in a simple and easier to use format
struct MemRef 
{
  string type;    //Memory access type as string (Can only be 'R' or 'W')
  string address; //Hex address as string (Ranges from 000 (8-bit) to FFFFFFFF (32-bit))
};


//Method declarations
vector<MemRef> insertTrace(vector<MemRef> testVector);
void testVectorOutput(vector<MemRef> testVector);


int main()
{
  //Vector of Memory References to work from (Similar to array, but does not have definite size)
  vector<MemRef> MemReferences;

  //Insert memory trace references from stdin into vector
  MemReferences = insertTrace(MemReferences);

  //Test if vector fill from stdin actually worked (It does work!)
  //testVectorOutput(MemReferences);

  return 0;
}


//Method for inserting Memory References from a string into the given MemRef vector
vector<MemRef> insertTrace(vector<MemRef> memRefVector)
{
  string RawMemRef;   //Raw full memory reference in '<accesstype>:<hexaddress>' format
  MemRef tempRef;     //Temporary MemRef for each line of memory reference into vector
  int stringPosition; //Int variable for splittinf memory reference by colon

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

//Method for testing if inserting the trace input into the vector with a custom struct worked
void testVectorOutput(vector<MemRef> memRefVector)
{
  //Test to see if input to vector code worked correctly
  cout << "Memory References from inserted trace file:\n";
  for (int i = 0; i != memRefVector.size(); i++)
  {
    cout << memRefVector[i].type << ":" << memRefVector[i].address <<"\n";
  }

}//end testVectorOutput()
