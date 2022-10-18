#ifndef DTLBENTRY_HPP
#define DTLBENTRY_HPP

#pragma once

#include <vector>

class DTLBEntry
{
public:
    DTLBEntry();
    DTLBEntry(int newBit, int virtAddr, int physAddr);
    ~DTLBEntry();
    
    bool validBit;  //Valid bit
    int pageNum;    //Also known as the virtual address mapping
    int frameNum;   //Also known as the physical address mapping

    private:

};


#endif
