CC=g++
STD=-std=gnu++0x
CFLAGS=-Wall -Wextra
LFLAGS=-lrt

SRC=main.cpp
TARGET=adna

all:
	$(MAKE) clean
	mkdir -p ./bin
	$(CC) $(STD) $(CFLAGS) $(SRC) -o ./bin/$(TARGET) $(LFLAGS) 
	
clean:
	rm -f ./bin/*
