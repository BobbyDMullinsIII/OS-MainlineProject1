#include <iostream>
#include <fstream>
#include <string>
#include "SimStats.hpp"
using namespace std;

//Constructor
SimStats::SimStats()
{
    //Sets all values to -1 initially before counter preparation
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
SimStats::~SimStats(){}

//Method for setting all counters to 0 to prepare for hit/miss/read/write/reference counting
void SimStats::prepareCounters()
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

}//end prepareCounters()

//Method for outputting the final simulation statistics
void SimStats::outputSimulationStatistics(bool TLBActive, bool L2Active, bool L3Active)
{
    cout << "Simulation Statistics\n";
    cout << "\n";
    if(TLBActive == true) //Only displays actual stats if TLB cache is active
    {
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
    }
    else
    {
        cout << "dtlb hits: N/A\n";
        cout << "dtlb misses: N/A\n";
        cout << "dtlb hit ratio: N/A\n";
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
    if(L2Active == true) //Only displays actual stats if L2 cache is active
    {
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
    }
    else
    {
        cout << "L2 hits: N/A\n";
        cout << "L2 misses: N/A\n";
        cout << "L2 hit ratio: N/A\n";
    }
    cout << "\n";
    if(L3Active == true) //Only displays actual stats if L3 cache is active
    {
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
    }
    else
    {
        cout << "L3 hits: N/A\n";
        cout << "L3 misses: N/A\n";
        cout << "L3 hit ratio: N/A\n";
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
    cout << "\n";
    
}//end outputSimulationStatistics()
