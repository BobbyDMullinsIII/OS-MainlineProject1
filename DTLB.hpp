#ifndef DTLB_HPP
#define DTLB_HPP

#pragma once

class DTLB
{
public:
    DTLB(int numSets, int setSize);
    ~DTLB();

    int numSets;
    int setSize;
    //std::vector<std::vector<DTLBEntry>> dtlbSets();

    void insertRecentAddress(int address);
    bool checkForAddress(int checkAddr);

    private:

};


#endif
