#ifndef DTLB_HPP
#define DTLB_HPP

#pragma once

#include "DTLBEntry.hpp"

class DTLB
{
public:
    DTLB(int numSets, int setSize);
    ~DTLB();

    int numSets;
    int setSize;
    std::vector<std::vector<DTLBEntry>> dtlbSets;

    void insertRecentAddress(int address, int frame);
    bool checkForAddress(int checkAddr);

    private:

};


#endif
