#include <iostream>
#include <fstream>
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
        L2NumSets = tempInt;
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
        //Insert config value into appropriate parameter
        if(tempString == "y")
        { TLBActive = true; }
        if(tempString == "n")
        { TLBActive = false; }
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

}//end insertConfig()

//Method for checking if number is a power of two
//Stolen from https://stackoverflow.com/questions/600293/how-to-check-if-a-number-is-a-power-of-2
//Returns if the input number (x) is a power of 2
bool TraceConfig::IsPowerOfTwo(int x)
{
    return (x != 0) && ((x & (x - 1)) == 0);

}//end IsPowerOfTwo()