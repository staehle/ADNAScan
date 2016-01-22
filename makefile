CC=mpic++
BCC=/usr/local/gcc-5.3.0/openmpi/bin/mpic++
STD=-std=gnu++0x
CFLAGS=-Wall -Wextra

SRC=adna.cpp ReadPair.cpp fCompile.cpp
TARGET=adna fCompile

#PROCS=4
#FASTQ1=test1.fastq
#FASTQ2=test2.fastq

all:
	$(MAKE) clean
	mkdir -p ./bin
	$(CC) $(STD) $(CFLAGS) $(SRC) -o ./bin/$(TARGET)
	
clean:
	rm -f *.o
	rm -f ./bin/*

bart:
	$(MAKE) clean
	mkdir -p ./bin
	$(BCC) $(STD) $(CFLAGS) $(SRC) -o ./bin/$(TARGET) 

#test:
#	mpirun -np $(PROCS) ./bin/$(TARGET) ./test/$(FASTQ1) ./test/$(FASTQ2) 
