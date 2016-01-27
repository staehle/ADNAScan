GCC=g++
MCC=mpic++

BGCC=/usr/local/gcc-5.3.0/bin/g++
BMCC=/usr/local/gcc-5.3.0/openmpi/bin/mpic++

STD=-std=gnu++0x
CFLAGS=-Wall -Wextra
LFLAGS=-lrt

SINT=init.cpp
SMPI=mpiworld.cpp ReadPair.cpp
SFIN=finish.cpp

TINT=adna-init
TMPI=adna-gompi
TFIN=adna-finish

all:
	$(MAKE) clean
	$(GCC) $(STD) $(CFLAGS) $(SINT) -o ./bin/$(TINT) $(LFLAGS) 
	$(MCC) $(STD) $(CFLAGS) $(SMPI) -o ./bin/$(TMPI) $(LFLAGS) 
	#$(GCC) $(STD) $(CFLAGS) $(SFIN) -o ./bin/$(TFIN) $(LFLAGS)

bart:
	$(MAKE) clean
	
	
clean:
	rm -rf ./bin
	mkdir -p ./bin
