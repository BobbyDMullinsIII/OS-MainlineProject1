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
    int pageTableIndexBits;//!! not implemented yet         //Number of bits used for the page table index 
    int pageOffsetBits; //!! not implemented yet            //Number of bits used for the page table offset 

    //L1 Cache Configuration
    int L1NumSets;                  //Number of sets in L1 (Max: 8 sets, 1 set = direct mapped, Must be power of 2)
    int L1SetSize;                  //Number of entries in each set in L1 (Min: 1 entry, Max: 128 entries, Must be power of 2)
    int L1LineSize;                 //Size (in bytes) of each line in L1 (Min: 8 bytes, Max: 64 bytes, Must be power of 2)
    bool L1WriteThrough;            //Switch for L1 to be write-through or not (y/n = true/false)
    int L1IndexBits;//!! not implemented yet    //Number of bits used for the L1 index 
    int L1OffsetBits;//!! not implemented yet   //Number of bits used for the L1 Offset

    //L2 Cache Configuration
    int L2NumSets;                  //Number of sets in L2 (Max: 16 sets, 1 set = direct mapped, Must be power of 2)
    int L2SetSize;                  //Number of entries in each set in L2 (Min: 4 entries, Max: 512 entries, Must be power of 2)
    int L2LineSize;                 //Size (in bytes) of each line in L2 (Min: 8 bytes, Max: 64 bytes, Must be power of 2)
    bool L2WriteThrough;            //Switch for L2 to be write-through or not (y/n = true/false)
    int L2IndexBits;//!! not implemented yet    //Number of bits used for the L2 index 
    int L2OffsetBits;//!! not implemented yet   //Number of bits used for the L2 Offset

    //L3 Cache Configuration
    int L3NumSets;                  //Number of sets in L3 (Max: 16 sets, 1 set = direct mapped, Must be power of 2)
    int L3SetSize;                  //Number of entries in each set in L3 (Min: 32 entries, Max: 4096 entries, Must be power of 2)
    int L3LineSize;                 //Size (in bytes) of each line in L3 (Min: 8 bytes, Max: 64 bytes, Must be power of 2)
    bool L3WriteThrough;            //Switch for L3 to be write-through or not (y/n = true/false)
    int L3IndexBits;//!! not implemented yet    //Number of bits used for the L3 index 
    int L3OffsetBits;//!! not implemented yet   //Number of bits used for the L3 Offset

    //Active Modules Configuration
    bool VirtAddressActive;         //Determines if addresses read in are virtual or physical (y/n = true/false)
    bool TLBActive;                 //Switch for TLB to be active or not (y/n = true/false, Will be immediately set to false if VirtAddressActive is already false)
    bool L2Active;                  //Switch for L2 Cache to be active or not (y/n = true/false)
    bool L3Active;                  //Switch for L3 Cache to be active or not (y/n = true/false, Will be immediately set to false if L2Active is already false)

    //Sim stat counters
    int dtlbHitCount;//!! not implemented yet                //Number of dtlb hits (to be systematically counted)
    int dtlbMissCount;//!! not implemented yet               //Number of dtlb misses (to be systematically counted)
    int ptHitCount;//!! not implemented yet                  //Number of pt hits (to be systematically counted)
    int ptFaultCount;//!! not implemented yet                //Number of pt misses (to be systematically counted)
    int dcHitCount;//!! not implemented yet                  //Number of dc hits (to be systematically counted)
    int dcMissCount;//!! not implemented yet                 //Number of dc misses (to be systematically counted)
    int l2HitCount;//!! not implemented yet                  //Number of L2 hits (to be systematically counted)
    int l2MissCount;//!! not implemented yet                 //Number of L2 misses (to be systematically counted)
    int l3HitCount;//!! not implemented yet                  //Number of L3 hits (to be systematically counted)
    int l3MissCount;//!! not implemented yet                 //Number of L3 misses (to be systematically counted)
    int readsCount;//!! not implemented yet                  //Number of reads (to be systematically counted)
    int writesCount;//!! not implemented yet                 //Number of writes (to be systematically counted)
    int mainMemRefsCount;//!! not implemented yet            //Number of main memory refs (to be systematically counted)
    int pageTableRefsCount;//!! not implemented yet          //Number of page table refs (to be systematically counted)
    int diskRefsCount;//!! not implemented yet               //Number of disk refs count (to be systematically counted)

    void outputRawConfigValues();
    void insertConfig();
    void prepareCounters();
    bool IsPowerOfTwo(int x);
    std::string removeNonLetters(std::string s);

private:

};

#endif
