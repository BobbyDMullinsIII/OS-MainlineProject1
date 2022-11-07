#ifndef DTLBENTRY_HPP
#define DTLBENTRY_HPP

#pragma once

#include <vector>

class DTLBEntry
{
public:
    DTLBEntry();
    DTLBEntry(int newBit, int newIndex, int newTag, int virtAddr, int physAddr, int lru);
    ~DTLBEntry();
    
    bool validBit;  //Valid bit
    int index;      //TLB address index
    int tag;        //TLB address tag
    int virtAddr;   //Also known as the virtual address mapping
    int frameNum;   //Also known as the physical address mapping
    int LRUNum;     //Number for keeping track of least recently used item in DTLB

    private:

};


#endif
