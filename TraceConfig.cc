#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "TraceConfig.hpp"
using namespace std;

//Constructor
TraceConfig::TraceConfig()
{
    //Sets all values to -1 and false (Except where hard-coded) initially before Trace.Config insertion
    this->numTLBSets = -1;
    this->TLBSetSize = -1;
    this->TLBIndexBits = -1;

    this->numVirtPages = -1;
    this->numPhysPages = -1;
    this->pageSize = -1;
    this->pageTableIndexBits = -1;
    this->pageOffsetBits = -1;

    this->L1NumSets = -1;
    this->L1SetSize = -1;
    this->L1LineSize = -1;
    this->L1WriteThrough = false;
    this->L1IndexBits = -1;
    this->L1OffsetBits = -1;

    this->L2NumSets = -1;
    this->L2SetSize = -1;
    this->L2LineSize = -1;
    this->L2WriteThrough = false;
    this->L2IndexBits = -1;
    this->L2OffsetBits = -1;

    this->L3NumSets = -1;
    this->L3SetSize = -1;
    this->L3LineSize = -1;
    this->L3WriteThrough = false;
    this->L3IndexBits = -1;
    this->L3OffsetBits = -1;

    this->VirtAddressActive = false;
    this->TLBActive = false;
    this->L2Active = false;
    this->L3Active = false;
}

//Deconstructor
TraceConfig::~TraceConfig(){}

//Method for outputting all the initial config variable values within the TraceConfig object
void TraceConfig::outputConfigValues()
{
    cout << "\n";
    cout << "Data TLB contains " << numTLBSets << " sets.\n";
    cout << "Each set contains " << TLBSetSize << " entries.\n";
    cout << "Number of bits used for the index is " << TLBIndexBits << ".\n";
    cout << "\n";
    cout << "Number of virtual pages is " << numVirtPages << ".\n";
    cout << "Number of physical pages is " << numPhysPages << ".\n";
    cout << "Each page contains " << pageSize << " bytes.\n";
    cout << "Number of bits used for page table index is " << pageTableIndexBits << ".\n";
    cout << "Number of bits used for page offset is " << pageOffsetBits << ".\n";
    cout << "\n";
    cout << "L1NumSets: " << L1NumSets << "\n";
    cout << "L1SetSize: " << L1SetSize << "\n";
    cout << "L1LineSize: " << L1LineSize << "\n";
    if(L1WriteThrough == true)
    {
        cout << "L1 uses a no write-allocate and write-through policy.\n";
    }
    else if(L1WriteThrough == false)
    {
        cout << "L1 uses a write-allocate and write-back policy.\n";
    }
    cout << "Number of bits used for L1 index is " << L1IndexBits << ".\n";
    cout << "Number of bits used for L1 offset is " << L1OffsetBits << ".\n";
    cout << "\n";
    cout << "L2NumSets: " << L2NumSets << "\n";
    cout << "L2SetSize: " << L2SetSize << "\n";
    cout << "L2LineSize: " << L2LineSize << "\n";
    if(L2WriteThrough == true)
    {
        cout << "L2 uses a no write-allocate and write-through policy.\n";
    }
    else if(L2WriteThrough == false)
    {
        cout << "L2 uses a write-allocate and write-back policy.\n";
    }
    cout << "Number of bits used for L2 index is " << L2IndexBits << ".\n";
    cout << "Number of bits used for L2 offset is " << L2OffsetBits << ".\n";
    cout << "\n";
    cout << "L3NumSets: " << L3NumSets << "\n";
    cout << "L3SetSize: " << L3SetSize << "\n";
    cout << "L3LineSize: " << L3LineSize << "\n";
    if(L3WriteThrough == true)
    {
        cout << "L3 uses a no write-allocate and write-through policy.\n";
    }
    else if(L3WriteThrough == false)
    {
        cout << "L3 uses a write-allocate and write-back policy.\n";
    }
    cout << "Number of bits used for L3 index is " << L3IndexBits << ".\n";
    cout << "Number of bits used for L3 offset is " << L3OffsetBits << ".\n";
    cout << "\n";
    cout << "VirtAddressActive: " << boolalpha << VirtAddressActive << "\n";
    cout << "TLBActive: " << boolalpha << TLBActive << "\n";
    cout << "L2Active: " << boolalpha << L2Active << "\n";
    cout << "L3Active: " << boolalpha << L3Active << "(Not implemented)\n";
    cout << "\n";
    if(VirtAddressActive == true)
    {
        cout << "The addresses read in are virtual addresses.\n";
        cout << "\n";
    }
    else
    {
        cout << "The addresses read in are physical addresses.\n";
        cout << "\n";
    }

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

    TLBIndexBits = log2(numTLBSets); //Sets number of TLB index bits according to numTLBSets

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

    pageTableIndexBits = log2(numVirtPages); //Sets number of L1 index bits according to numVirtPages
    pageOffsetBits = log2(pageSize);

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
    tempString = removeNonLetters(tempString); //Remove all non-letter characters from string
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

    L1IndexBits = log2(L1NumSets); //Sets number of L1 index bits according to L1NumSets
    L1OffsetBits = log2(L1LineSize); //Sets number of offset bits according to line size

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
    tempString = removeNonLetters(tempString); //Remove all non-letter characters from string
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

    L2IndexBits = log2(L2NumSets); //Sets number of L2 index bits according to L2NumSets
    L2OffsetBits = log2(L2LineSize); //Sets number of offset bits according to line size

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
    tempString = removeNonLetters(tempString); //Remove all non-letter characters from string
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

    L3IndexBits = log2(L3NumSets); //Sets number of L3 index bits according to L3NumSets
    L3OffsetBits = log2(L3LineSize); //Sets number of offset bits according to line size

    getline(filein, currentLine); //blank line

    //VirtAddressActive
    getline(filein, currentLine);   //"Virtual addresses:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempString = removeNonLetters(tempString); //Remove all non-letter characters from string
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
    tempString = removeNonLetters(tempString); //Remove all non-letter characters from string
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
    tempString = removeNonLetters(tempString); //Remove all non-letter characters from string
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
    getline(filein, currentLine);   //"L3 cache:" config value line
    tempString = currentLine.substr(currentLine.find_first_of(':') + 2); //Store value at end of config line in string
    tempString = removeNonLetters(tempString); //Remove all non-letter characters from string
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

//Method for checking if number is a power of two
//Stolen from https://stackoverflow.com/questions/600293/how-to-check-if-a-number-is-a-power-of-2
//Returns if the input number (x) is a power of 2
bool TraceConfig::IsPowerOfTwo(int x)
{
    return (x != 0) && ((x & (x - 1)) == 0);

}//end IsPowerOfTwo()

//Method for removing all characters that are not letters from string
//Stolen from https://www.geeksforgeeks.org/remove-characters-alphabets-string/
//Returns string with only letter (A-Z,a-z) characters 
string TraceConfig::removeNonLetters(string s)
{
    //For every character in string, delete all non-letter (A-Z,a-z) characters
    for (int i = 0; i < s.size(); i++) 
    {    
        if (s[i] < 'A' || s[i] > 'Z' && s[i] < 'a' || s[i] > 'z')
        {  
            s.erase(i, 1);
            i--;
        }
    }

    return s;
}

//Method for returning the number of bits needed to count to given decimalNum in binary form
//Examples:
//64 (or 0-based 63) = 111111 = 6 bits needed
//16 (or 0-based 15) = 1111 = 4 bits needed
int TraceConfig::numBitsNeeded(int decimalNum)
{
    if(decimalNum > -1 && decimalNum <= 1)
    { return 1; }

    if(decimalNum > 1 && decimalNum <= 4)
    { return 2; }

    if(decimalNum > 4 && decimalNum <= 8)
    { return 3; }

    if(decimalNum > 8 && decimalNum <= 16)
    { return 4; }

    if(decimalNum > 16 && decimalNum <= 32)
    { return 5; }

    if(decimalNum > 32 && decimalNum <= 64)
    { return 6; }

    if(decimalNum > 64 && decimalNum <= 128)
    { return 7; }

    if(decimalNum > 128 && decimalNum <= 256)
    { return 8; }

    if(decimalNum > 256 && decimalNum <= 512)
    { return 9; }

    if(decimalNum > 512 && decimalNum <= 1024)
    { return 10; }
    
    if(decimalNum > 1024 && decimalNum <= 2048)
    { return 11; }

    if(decimalNum > 2048 && decimalNum <= 4096)
    { return 12; }

    if(decimalNum > 4096 && decimalNum <= 8192)
    { return 13; }

    if(decimalNum > 8192 && decimalNum <= 16384)
    { return 14; }

    if(decimalNum > 16384 && decimalNum <= 32768)
    { return 15; }

    if(decimalNum > 32768 && decimalNum <= 65536)
    { return 16; }

    return -1; //Something went wrong if it gets here

    
}