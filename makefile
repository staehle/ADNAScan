CC=mpic++
STD=-std=gnu++0x
CFLAGS=-Wall -Wextra

SRC=adna.cpp ReadPair.cpp
TARGET=adna

PROCS=4

FASTQ1=./test/test1.fastq
FASTQ2=./test/test2.fastq

all:
	$(MAKE) clean
	mkdir -p ./bin
	$(CC) $(STD) $(CFLAGS) $(SRC) -o ./bin/$(TARGET)
	
clean:
	rm -f *.o
	rm -f ./bin/*

test:
	mpirun -np $(PROCS) ./bin/$(TARGET) $(FASTQ1) $(FASTQ2) 
