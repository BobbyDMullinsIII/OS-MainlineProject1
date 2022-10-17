#ifndef SIMSTATS_HPP
#define SIMSTATS_HPP

#pragma once

class SimStats
{
public:
    SimStats();
    ~SimStats();

    //Sim stat counters
    int dtlbHitCount;               //Number of dtlb hits (to be systematically counted)
    int dtlbMissCount;              //Number of dtlb misses (to be systematically counted)
    int ptHitCount;                 //Number of pt hits (to be systematically counted)
    int ptFaultCount;               //Number of pt misses (to be systematically counted)
    int dcHitCount;                 //Number of dc hits (to be systematically counted)
    int dcMissCount;                //Number of dc misses (to be systematically counted)
    int l2HitCount;                 //Number of L2 hits (to be systematically counted)
    int l2MissCount;                //Number of L2 misses (to be systematically counted)
    int l3HitCount;                 //Number of L3 hits (to be systematically counted)
    int l3MissCount;                //Number of L3 misses (to be systematically counted)
    int readsCount;                 //Number of reads (to be systematically counted)
    int writesCount;                //Number of writes (to be systematically counted)
    int mainMemRefsCount;           //Number of main memory refs (to be systematically counted)
    int pageTableRefsCount;         //Number of page table refs (to be systematically counted)
    int diskRefsCount;              //Number of disk refs count (to be systematically counted)

    void prepareCounters();
    void outputSimulationStatistics(bool TLBActive, bool L2Active, bool L3Active);

private:

};


#endif
