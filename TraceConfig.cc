#include <iostream>
#include <fstream>
#include <string>
#include "TraceConfig.hpp"
using namespace std;

//Constructor
TraceConfig::TraceConfig()
{
    //Sets all values to -1 and false initially before Trace.Config insertion
    numTLBSets = -1;
    TLBSetSize = -1;

    numVirtPages = -1;
    numPhysPages = -1;
    pageSize = -1;
    pageTableIndexBits = -1;
    pageOffsetBits = -1;

    L1NumSets = -1;
    L1SetSize = -1;
    L1LineSize = -1;
    L1WriteThrough = false;
    L1IndexBits = -1;
    L1OffsetBits = -1;

    L2NumSets = -1;
    L2SetSize = -1;
    L2LineSize = -1;
    L2WriteThrough = false;
    L2IndexBits = -1;
    L2OffsetBits = -1;

    L3NumSets = -1;
    L3SetSize = -1;
    L3LineSize = -1;
    L3WriteThrough = false;
    L3IndexBits = -1;
    L3OffsetBits = -1;

    VirtAddressActive = false;
    TLBActive = false;
    L2Active = false;
    L3Active = false;

    dtlbHitCount = -1;
    dtlbMissCount = -1;
    ptHitCount = -1;
    ptFaultCount = -1;
    dcHitCount = -1;
    dcMissCount = -1;
    l2HitCount = -1;
    l2MissCount = -1;
    l3HitCount = -1;
    l3MissCount = -1;
    readsCount = -1;
    writesCount = -1;
    mainMemRefsCount = -1;
    pageTableRefsCount = -1;
    diskRefsCount = -1;
}

//Deconstructor
TraceConfig::~TraceConfig(){}

//Method for outputting all the raw variable values within the TraceConfig object
void TraceConfig::outputRawConfigValues()
{
    cout << "Current Config Values:\n";
    cout << "Data TLB contains " << numTLBSets << " sets.\n";
    cout << "Each set contains " << TLBSetSize << " entries.\n";
    cout << "\n";
    cout << "Number of virtual pages is " << numVirtPages << ".\n";
    cout << "Number of physical pages is " << numPhysPages << ".\n";
    cout << "Each page contains " << pageSize << " bytes.\n";
    cout << "Number of bits used for page table index is " << pageTableIndexBits << ". THIS IS NOT CORRECT, NOT IMPLEMENTED YET\n";
    cout << "Number of bits used for page offset is " << pageOffsetBits << ". THIS IS NOT CORRECT, NOT IMPLEMENTED YET\n";
    cout << "\n";
    cout << "L1NumSets: " << L1NumSets << "\n";
    cout << "L1SetSize: " << L1SetSize << "\n";
    cout << "L1LineSize: " << L1LineSize << "\n";
    cout << "L1WriteThrough: " << boolalpha << L1WriteThrough << "\n";
    cout << "Number of bits used for L1 index is " << L1IndexBits << ". THIS IS NOT CORRECT, NOT IMPLEMENTED YET\n";
    cout << "Number of bits used for L1 offset is " << L1OffsetBits << ". THIS IS NOT CORRECT, NOT IMPLEMENTED YET\n";
    cout << "\n";
    cout << "L2NumSets: " << L2NumSets << "\n";
    cout << "L2SetSize: " << L2SetSize << "\n";
    cout << "L2LineSize: " << L2LineSize << "\n";
    cout << "L2WriteThrough: " << boolalpha << L2WriteThrough << "\n";
    cout << "Number of bits used for L2 index is " << L2IndexBits << ". THIS IS NOT CORRECT, NOT IMPLEMENTED YET\n";
    cout << "Number of bits used for L2 offset is " << L2OffsetBits << ". THIS IS NOT CORRECT, NOT IMPLEMENTED YET\n";
    cout << "\n";
    cout << "L3NumSets: " << L3NumSets << "\n";
    cout << "L3SetSize: " << L3SetSize << "\n";
    cout << "L3LineSize: " << L3LineSize << "\n";
    cout << "L3WriteThrough: " << boolalpha << L3WriteThrough << "\n";
    cout << "Number of bits used for L3 index is " << L3IndexBits << ". THIS IS NOT CORRECT, NOT IMPLEMENTED YET\n";
    cout << "Number of bits used for L3 offset is " << L3OffsetBits << ". THIS IS NOT CORRECT, NOT IMPLEMENTED YET\n";
    cout << "\n";
    cout << "VirtAddressActive: " << boolalpha << VirtAddressActive << "\n";
    cout << "TLBActive: " << boolalpha << TLBActive << "\n";
    cout << "L2Active: " << boolalpha << L2Active << "\n";
    cout << "L3Active: " << boolalpha << L3Active << "\n";
    cout << "\n";
    cout << "\n";
    cout << "Virtual  Virt.\tPage\tTLB\tTLB\tTLB\tPT\tPhys\t\tDC\tDC\t\tL2\tL2\n";
    cout << "Address  Page #\tOff\tTag\tInd\tRes.\tRes.\tPg #\tDC Tag\tInd\tRes.\tL2 Tag\tInd\tRes.\n";
    cout << "-------- ------\t----\t------\t---\t----\t----\t----\t------\t---\t---\t------\t---\t----\n";
    // here we should be able to plug in values for each item here as we find them. 
    // I believe format should follow something very close to: (number of spaces between each being important)
    //cout << "00000c84      c\t  84\t     6\t  0\tmiss\tmiss\t   0\t     2\t  0\tmiss\t     0\t  8\tmiss\n";
    //this should put values at back of column like in spec example output, but may look incorrect with more or less characters in the values.
    cout << "\n";
    cout << "Simulation Statistics\n";
    cout << "\n";
    cout << "dtlb hits: " << dtlbHitCount << "\n";
    cout << "dtlb misses: " << dtlbMissCount << "\n";
    if((dtlbHitCount+dtlbMissCount) == 0) //Catches divide by zero error
    {
        cout << "dtlb hit ratio: 0" << "\n";
    }
    else
    {
        cout << "dtlb hit ratio: " << (double)(dtlbHitCount/(dtlbHitCount+dtlbMissCount)) << "\n";
    }
    cout << "\n";
    cout << "pt hits: " << ptHitCount << "\n";
    cout << "pt faults: " << ptFaultCount << "\n";
    if((ptHitCount+ptFaultCount) == 0) //Catches divide by zero error
    {
        cout << "pt hit ratio: 0" << "\n";
    }
    else
    {
        cout << "pt hit ratio: " << (double)(ptHitCount/(ptHitCount+ptFaultCount)) << "\n";
    }
    cout << "\n";
    cout << "dc hits: " << dcHitCount << "\n";
    cout << "dc misses: " << dcMissCount << "\n";
    if((dcHitCount+dcMissCount) == 0) //Catches divide by zero error
    {
        cout << "dc hit ratio: 0" << "\n";
    }
    else
    {
        cout << "dc hit ratio: " << (double)(dcHitCount/(dcHitCount+dcMissCount)) << "\n";
    }
    cout << "\n";
    cout << "L2 hits: " << l2HitCount << "\n";
    cout << "L2 misses: " << l2MissCount << "\n";
    if((l2HitCount+l2MissCount) == 0) //Catches divide by zero error
    {
        cout << "L2 hit ratio: 0" << "\n";
    }
    else
    {
        cout << "L2 hit ratio: " << (double)(l2HitCount/(l2HitCount+l2MissCount)) << "\n";
    }
    cout << "\n";
    cout << "L3 hits: " << l3HitCount << "\n";
    cout << "L3 misses: " << l3MissCount << "\n";
    if((l3HitCount+l3MissCount) == 0) //Catches divide by zero error
    {
        cout << "L3 hit ratio: 0" << "\n";
    }
    else
    {
        cout << "L3 hit ratio: " << (double)(l3HitCount/(l3HitCount+l3MissCount)) << "\n";
    }
    cout << "\n";
    cout << "Total Reads: " << readsCount << "\n";
    cout << "Total Writes: " << writesCount << "\n";
    if((readsCount+writesCount) == 0) //Catches divide by zero error
    {
        cout << "Ratio of Reads: 0" << "\n";
    }
    else
    {
        cout << "Ratio of Reads: " << (double)(readsCount/(readsCount+writesCount)) << "\n";
    }
    cout << "\n";
    cout << "main memory refs: " << mainMemRefsCount << "\n";
    cout << "page table refs: " << pageTableRefsCount << "\n";
    cout << "disk refs: " << diskRefsCount << "\n";
    
}//end outputConfigValues()

//Method for inserting all data within trace.config into a TraceConfig data structure
//Also checks if any of the config values are not valid and stops program to state what was not valid
void TraceConfig::insertConfig()
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
        numTLBSets = tempInt;
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
        TLBSetSize = tempInt;
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
        numVirtPages = tempInt;
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
        numPhysPages = tempInt;
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
        pageSize = tempInt;
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
        L1NumSets = tempInt;
    }

    //L1SetSize
    getline(filein, currentLine);   //"Set size:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempInt = stoi(tempString); //Convert string number at end of line into int
    if(tempInt < 1 || tempInt > 128 || IsPowerOfTwo(tempInt) != true ) //(Min: 1 entry, Max: 128 entries, Must be power of 2)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The number of entries within each L1 cache set should be at least 1, at most 128, and a power of 2.\n";
        cout << "Invalid value read: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Insert config value into appropriate parameter
        L1SetSize = tempInt;
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
        L1LineSize = tempInt;
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
        { L1WriteThrough = true; }
        if(tempString == "n")
        { L1WriteThrough = false; }
    }

    getline(filein, currentLine); //blank line
    getline(filein, currentLine); //"L2 Cache configuration" info line

    //L2NumSets
    getline(filein, currentLine);   //"Number of sets:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempInt = stoi(tempString); //Convert string number at end of line into int
    if(tempInt < 1 || tempInt > 16 || IsPowerOfTwo(tempInt) != true ) //(Max: 16 sets, 1 set = direct mapped, Must be power of 2)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The number of sets within the L2 cache should be at least 1, at most 16, and a power of 2.\n";
        cout << "Invalid value read: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Insert config value into appropriate parameter
        L2NumSets = tempInt;
    }

    //L2SetSize
    getline(filein, currentLine);   //"Set size:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempInt = stoi(tempString); //Convert string number at end of line into int
    if(tempInt < 4 || tempInt > 512 || IsPowerOfTwo(tempInt) != true ) //(Min: 4 entries, Max: 512 entries, Must be power of 2)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The number of entries within each L2 cache set should be at least 4, at most 512, and a power of 2.\n";
        cout << "Invalid value read: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Insert config value into appropriate parameter
        L2SetSize = tempInt;
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
        L2LineSize = tempInt;
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
        { L2WriteThrough = true; }
        if(tempString == "n")
        { L2WriteThrough = false; }
    }

    getline(filein, currentLine); //blank line
    getline(filein, currentLine); //"L3 Cache configuration" info line

    //L3NumSets
    getline(filein, currentLine);   //"Number of sets:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempInt = stoi(tempString); //Convert string number at end of line into int
    if(tempInt < 1 || tempInt > 16 || IsPowerOfTwo(tempInt) != true ) //(Max: 16 sets, 1 set = direct mapped, Must be power of 2)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The number of sets within the L3 cache should be at least 1, at most 16, and a power of 2.\n";
        cout << "Invalid value read: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Insert config value into appropriate parameter
        L3NumSets = tempInt;
    }

    //L3SetSize
    getline(filein, currentLine);   //"Set size:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempInt = stoi(tempString); //Convert string number at end of line into int
    if(tempInt < 32 || tempInt > 4096 || IsPowerOfTwo(tempInt) != true ) //(Min: 32 entries, Max: 4096 entries, Must be power of 2)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The number of entries within each L3 cache set should be at least 32, at most 4096, and a power of 2.\n";
        cout << "Invalid value read: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Insert config value into appropriate parameter
        L3SetSize = tempInt;
    }

    //L3LineSize
    getline(filein, currentLine);   //"Set size:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempInt = stoi(tempString); //Convert string number at end of line into int
    if(tempInt < 8 || tempInt > 64 || IsPowerOfTwo(tempInt) != true ) //(Min: 8 bytes, Max: 64 bytes, Must be power of 2)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The line size within the L3 cache should be at least 8 bytes, at most 64 bytes, and a power of 2.\n";
        cout << "Invalid value read: " << tempInt <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Insert config value into appropriate parameter
        L3LineSize = tempInt;
    }

    //L3WriteThrough
    getline(filein, currentLine);   //"Write through/no write allocate:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    if(tempString != "y" && tempString != "n") //(y/n = true/false)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The L3 write-through toggle must be 'y' or 'n'.\n";
        cout << "Invalid value read: " << tempString <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {   
        //Insert config value into appropriate parameter
        if(tempString == "y")
        { L3WriteThrough = true; }
        if(tempString == "n")
        { L3WriteThrough = false; }
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
        { VirtAddressActive = true; }
        if(tempString == "n")
        { VirtAddressActive = false; }
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
        //Will automatically set TLBActive to false if virtual addresses are disabled, regardless of its config setting
        //There is no point to the TLB if virtual addresses are not used
        if(VirtAddressActive == false)
        {
            TLBActive = false;
        }
        else
        {
            //Insert config value into appropriate parameter
            if(tempString == "y")
            { TLBActive = true; }
            if(tempString == "n")
            { TLBActive = false; }
        }
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
        { L2Active = true; }
        if(tempString == "n")
        { L2Active = false; }
    }

    //L3Active
    getline(filein, currentLine);   //"L2 cache:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    if(tempString != "y" && tempString != "n") //(y/n = true/false)
    {
        //Output error message and exit program if any of the fail conditions met
        cout << "The L3 cache active toggle must be 'y' or 'n'.\n";
        cout << "Invalid value read: " << tempString <<"\n";
        exit(EXIT_FAILURE);
    }
    else
    {   
        //Will automatically set L3Active to false if L2 cache is disabled, regardless of its config setting
        //There is no point to the L3 cache if the L2 cache is disabled
        if(L2Active == false)
        {
            L3Active = false;
        }
        else
        {
            //Insert config value into appropriate parameter
            if(tempString == "y")
            { L3Active = true; }
            if(tempString == "n")
            { L3Active = false; }
        }
    }

}//end insertConfig()

//Method for setting all counters to 0 to prepare for hit/miss/read/write/reference counting
void TraceConfig::prepareCounters()
{
    //Sets all counters to 0 to prepare for hit/miss/read/write/reference counting
    dtlbHitCount = 0;
    dtlbMissCount = 0;
    ptHitCount = 0;
    ptFaultCount = 0;
    dcHitCount = 0;
    dcMissCount = 0;
    l2HitCount = 0;
    l2MissCount = 0;
    l3HitCount = 0;
    l3MissCount = 0;
    readsCount = 0;
    writesCount = 0;
    mainMemRefsCount = 0;
    pageTableRefsCount = 0;
    diskRefsCount = 0;
}

//Method for checking if number is a power of two
//Stolen from https://stackoverflow.com/questions/600293/how-to-check-if-a-number-is-a-power-of-2
//Returns if the input number (x) is a power of 2
bool TraceConfig::IsPowerOfTwo(int x)
{
    return (x != 0) && ((x & (x - 1)) == 0);

}//end IsPowerOfTwo()
