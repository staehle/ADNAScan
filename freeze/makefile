CC=mpic++
BCC=/usr/local/gcc-5.3.0/openmpi/bin/mpic++
STD=-std=gnu++0x
CFLAGS=-Wall -Wextra

SRC=adna.cpp ReadPair.cpp
CMP=fCompile.cpp
TARGETA=adna
TARGETC=fCompile

all:
	$(MAKE) clean
	mkdir -p ./bin
	$(CC) $(STD) $(CFLAGS) $(SRC) -o ./bin/$(TARGETA)
	$(CC) $(STD) $(CFLAGS) $(CMP) -o ./bin/$(TARGETC)
	
clean:
	rm -f *.o
	rm -f ./bin/*

bart:
	$(MAKE) clean
	mkdir -p ./bin
	$(BCC) $(STD) $(CFLAGS) $(SRC) -o ./bin/$(TARGETA) 
	$(BCC) $(STD) $(CFLAGS) $(CMP) -o ./bin/$(TARGETC)

