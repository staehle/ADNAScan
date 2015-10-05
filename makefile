CC=g++
CFLAGS=-Wall -Wextra
TARGET=runscan
SOURCES=adnascan.cpp split.cpp scanner.cpp

all:
	$(MAKE) clean
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)
	
clean:
	rm -f *.o
	rm -f $(TARGET)