CC = g++

project1: main.o TraceConfig.o SimStats.o PageDirectory.o PageTable.o PageTableEntry.o DTLB.o DTLBEntry.o Memory.o
	$(CC) -o project1 main.o TraceConfig.o SimStats.o PageDirectory.o PageTable.o PageTableEntry.o DTLB.o DTLBEntry.o Memory.o

main.o: main.cc
	$(CC) -c main.cc

TraceConfig.o: TraceConfig.cc
	$(CC) -c TraceConfig.cc

SimStats.o: SimStats.cc
	$(CC) -c SimStats.cc

PageDirectory.o: PageDirectory.cpp
	$(CC) -c PageDirectory.cpp

PageTable.o: PageTable.cc
	$(CC) -c PageTable.cc

PageTableEntry.o: PageTableEntry.cpp
	$(CC) -c PageTableEntry.cpp

DTLB.o: DTLB.cc
	$(CC) -c DTLB.cc

DTLBEntry.o: DTLBEntry.cc
	$(CC) -c DTLBEntry.cc

Memory.o: Memory.cpp
	$(CC) -c Memory.cpp


clean:
	rm -f *.o
	rm -f project1