GCC=g++
MCC=mpic++

BGCC=/usr/local/gcc-5.3.0/bin/g++
BMCC=/usr/local/gcc-5.3.0/openmpi/bin/mpic++

STD=-std=gnu++0x
CFLAGS=-Wall -Wextra
LFLAGS=-lrt
NCFLGS=-lcurses

SINT=init.cpp
SMPI=mpiworld.cpp ReadPair.cpp
SCHK=checker.cpp
#COMP=fCompile.cpp

TINT=adna-init
TMPI=adna-gompi
TCHK=adna-check
#TCMP=adna-compile

all:
	$(MAKE) clean
	$(GCC) $(STD) $(CFLAGS) $(SINT) -o ./bin/$(TINT) $(LFLAGS) 
	$(MCC) $(STD) $(CFLAGS) $(SMPI) -o ./bin/$(TMPI) $(LFLAGS) 
	$(GCC) $(STD) $(CFLAGS) $(SCHK) -o ./bin/$(TCHK) $(LFLAGS) $(NCFLGS)
	#$(GCC) $(STD) $(CFLAGS) $(COMP) -o ./bin/$(TCMP)

bart:
	$(MAKE) clean
	$(BGCC) $(STD) $(CFLAGS) $(SINT) -o ./bin/$(TINT) $(LFLAGS)
	$(BMCC) $(STD) $(CFLAGS) $(SMPI) -o ./bin/$(TMPI) $(LFLAGS)
	$(BGCC) $(STD) $(CFLAGS) $(SCHK) -o ./bin/$(TCHK) $(LFLAGS) $(NCFLGS)
	#$(BGCC) $(STD) $(CFLAGS) $(COMP) -o ./bin/$(TCMP)

clean:
	rm -rf ./bin
	mkdir -p ./bin
