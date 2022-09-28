#include <iostream>
#include <string>
#include "TraceConfig.hpp"
using namespace std;

//Constructor
TraceConfig::TraceConfig()
{
    //Sets all values to 0 and false intially before Trace.Config insertion
    numTLBSets = 0;
    TLBSetSize = 0;

    numVirtPages = 0;
    numPhysPages = 0;
    pageSize = 0;

    L1NumSets = 0;
    L1SetSize = 0;
    L1LineSize = 0;
    L1WriteThrough = false;

    L2NumSets = 0;
    L2SetSize = 0;
    L2LineSize = 0;
    L2WriteThrough = false;

    VirtAddressActive = false;
    TLBActive = false;
    L2Active = false;
}

//Deconstructor
TraceConfig::~TraceConfig(){}

//Method for outputting all the raw variable values within the TraceConfig object
void TraceConfig::outputRawConfigValues()
{
    cout << "Current Config Values:\n";
    cout << "numTLBSets: " << numTLBSets << "\n";
    cout << "TLBSetSize: " << TLBSetSize << "\n";
    cout << "\n";
    cout << "numVirtPages: " << numVirtPages << "\n";
    cout << "numPhysPages: " << numPhysPages << "\n";
    cout << "pageSize: " << pageSize << "\n";
    cout << "\n";
    cout << "L1NumSets: " << L1NumSets << "\n";
    cout << "L1SetSize: " << L1SetSize << "\n";
    cout << "L1LineSize: " << L1LineSize << "\n";
    cout << "L1WriteThrough: " << boolalpha << L1WriteThrough << "\n";
    cout << "\n";
    cout << "L2NumSets: " << L2NumSets << "\n";
    cout << "L2SetSize: " << L2SetSize << "\n";
    cout << "L2LineSize: " << L2LineSize << "\n";
    cout << "L2WriteThrough: " << boolalpha << L2WriteThrough << "\n";
    cout << "\n";
    cout << "VirtAddressActive: " << boolalpha << VirtAddressActive << "\n";
    cout << "TLBActive: " << boolalpha << TLBActive << "\n";
    cout << "L2Active: " << boolalpha << L2Active << "\n";
    
}//end outputConfigValues()