#include <iostream>
#include <fstream>
#include <string>
#include "SimStats.hpp"
using namespace std;

//Constructor
SimStats::SimStats()
{
    //Sets all values to -1 initially before counter preparation
    this->dtlbHitCount = -1;
    this->dtlbMissCount = -1;
    this->ptHitCount = -1;
    this->ptFaultCount = -1;
    this->dcHitCount = -1;
    this->dcMissCount = -1;
    this->l2HitCount = -1;
    this->l2MissCount = -1;
    this->l3HitCount = -1;
    this->l3MissCount = -1;
    this->readsCount = -1;
    this->writesCount = -1;
    this->mainMemRefsCount = -1;
    this->pageTableRefsCount = -1;
    this->diskRefsCount = -1;
}

//Deconstructor
SimStats::~SimStats(){}

//Method for setting all counters to 0 to prepare for hit/miss/read/write/reference counting
void SimStats::prepareCounters()
{
    //Sets all counters to 0 to prepare for hit/miss/read/write/reference counting
    this->dtlbHitCount = 0;
    this->dtlbMissCount = 0;
    this->ptHitCount = 0;
    this->ptFaultCount = 0;
    this->dcHitCount = 0;
    this->dcMissCount = 0;
    this->l2HitCount = 0;
    this->l2MissCount = 0;
    this->l3HitCount = 0;
    this->l3MissCount = 0;
    this->readsCount = 0;
    this->writesCount = 0;
    this->mainMemRefsCount = 0;
    this->pageTableRefsCount = 0;
    this->diskRefsCount = 0;

}//end prepareCounters()

//Method for outputting the final simulation statistics
void SimStats::outputSimulationStatistics(bool TLBActive, bool L2Active, bool L3Active)
{
    cout << "\n";
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
            cout << "dtlb hit ratio: " << (double)((double)dtlbHitCount/((double)dtlbHitCount+(double)dtlbMissCount)) << "\n";
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
        cout << "pt hit ratio: " << (double)((double)ptHitCount/((double)ptHitCount+(double)ptFaultCount)) << "\n";
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
        cout << "dc hit ratio: " << (double)((double)dcHitCount/((double)dcHitCount+(double)dcMissCount)) << "\n";
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
            cout << "L2 hit ratio: " << (double)((double)l2HitCount/((double)l2HitCount+(double)l2MissCount)) << "\n";
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
            cout << "L3 hit ratio: " << (double)((double)l3HitCount/((double)l3HitCount+(double)l3MissCount)) << "\n";
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
        cout << "Ratio of Reads: " << (double)((double)readsCount/((double)readsCount+(double)writesCount)) << "\n";
    }
    cout << "\n";
    cout << "main memory refs: " << mainMemRefsCount << "\n";
    cout << "page table refs: " << pageTableRefsCount << "\n";
    cout << "disk refs: " << diskRefsCount << "\n";
    cout << "\n";
    
}//end outputSimulationStatistics()
