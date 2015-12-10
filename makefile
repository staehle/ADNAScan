CC=mpicc
STD=-std=c++11
CFLAGS=-Wall -Wextra -g

SRC=adna.cpp ReadPair.cpp
TARGET=adna

PROCS=4

#FASTQ1=test1.fastq
#FASTQ2=test2.fastq

all:
	$(MAKE) clean
	mkdir -p ./bin
	$(CC) $(STD) $(CFLAGS) $(SRC) -o ./bin/$(TARGET)
	
clean:
	rm -f *.o
	rm -f ./bin/*

#test:
#	mpirun -np $(PROCS) ./bin/$(TARGET) ./test/$(FASTQ1) ./test/$(FASTQ2) 
