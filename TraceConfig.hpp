#ifndef TRACECONFIG_HPP
#define TRACECONFIG_HPP

#pragma once

class TraceConfig
{
public:
    TraceConfig();
    ~TraceConfig();

    //Data TLB Configuration
    int numTLBSets;                 //Number of sets within TLB (Max: 16 sets, 1 set = direct mapped, 16 sets = fully associative, Must be power of 2)
    int TLBSetSize;                 //Number of entries in each set (Max: 64 entries, Must be power of 2)

    //Page Table Configuration
    int numVirtPages;               //Number of virtual pages in page table (Max: 8192 pages, Must be power of 2)
    int numPhysPages;               //Number of physical pages in page table (Max: 2048 pages, Must be power of 2)
    int pageSize;                   //Size (in bytes) of each page (Max: 4096 bytes (4kb), Must be power of 2)

    //L1 Cache Configuration
    int L1NumSets;                  //Number of sets in L1 (Max: 8 sets, 1 set = direct mapped, Must be power of 2)
    int L1SetSize;                  //Number of entries in each set in L1 (Max: 128 entries, Must be power of 2)
    int L1LineSize;                 //Size (in bytes) of each line in L1 (Min: 8 bytes, Max: 64 bytes, Must be power of 2)
    bool L1WriteThrough;            //Switch for L1 to be write-through or not (y/n = true/false)

    //L2 Cache Configuration
    int L2NumSets;                  //Number of sets in L2 (Max: 16 sets, 1 set = direct mapped, Must be power of 2)
    int L2SetSize;                  //Number of entries in each set in L2 (Max: 512 entries, Must be power of 2)
    int L2LineSize;                 //Size (in bytes) of each line in L2 (Min: 8 bytes, Max: 64 bytes, Must be power of 2)
    bool L2WriteThrough;            //Switch for L2 to be write-through or not (y/n = true/false)

    //Active Modules Configuration
    bool VirtAddressActive;         //Determines if addresses read in are virtual or physical (y/n = true/false)
    bool TLBActive;                 //Switch for TLB to be active or not (y/n = true/false)
    bool L2Active;                  //Switch for L2 Cache to be active or not (y/n = true/false)

    void outputRawConfigValues();
    bool IsPowerOfTwo(int x);
    void insertConfig();

private:

};

#endif