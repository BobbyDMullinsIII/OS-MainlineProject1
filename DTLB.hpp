#ifndef DTLB_HPP
#define DTLB_HPP

#pragma once

#include "DTLBEntry.hpp"

class DTLB
{
public:
    DTLB(int numSets, int setSize);
    ~DTLB();

    int numSets;    //Number of sets within DTLB
    int setSize;    //Size of each set within DTLB
    int LRUCounter; //Counter for the LRUNum within each DTLBEntry (Counts up every time addresses are modified in any way)

    std::vector<std::vector<DTLBEntry>> dtlbSets;   //2D vector of address translations

    void insertRecentAddress(int index, int tag, int address, int frame);
    bool checkDTLB(int checkTag, int checkIndex);

    private:

};


#endif
