CC = g++

project1: main.o TraceConfig.o PageDirectory.o PageTable.o PageTableEntry.o
	$(CC) -o project1 main.o TraceConfig.o PageDirectory.o PageTable.o PageTableEntry.o

main.o: main.cc
	$(CC) -c main.cc

TraceConfig.o: TraceConfig.cc
	$(CC) -c TraceConfig.cc

PageDirectory.o: PageDirectory.cpp
	$(CC) -c PageDirectory.cpp

PageTable.o: PageTable.cc
	$(CC) -c PageTable.cc

PageTableEntry.o: PageTableEntry.cpp
	$(CC) -c PageTableEntry.cpp


clean:
	rm -rf *.o
	rm -rf project1