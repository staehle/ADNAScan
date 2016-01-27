/* ADNA
 * Main driver for shared memory initialization 
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "main.hpp"

using namespace std;
using namespace adna;

int main(int argc, char **argv) {
	cout << "adna -- Asynchronous process DNA trimmer and analyzer" << endl;
	if (argc < 4) {
		cerr << "Error: Not enough arguments" << endl;
		cerr << "Usage: " << argv[0] << " <num_processes> <input_fastq_file_1> <input_fastq_file_2>" << endl;
		exit(1);
	}
	int numProcs = atoi(argv[1]);
	if (numProcs < 1 or numProcs > 512) {
		cerr << "Error: num_processes outside of acceptable range of 1-512" << endl;
		exit(1);
	}
	
	//init process table
	_table Monitor[numProcs];
	for (int i=0; i<numProcs; i++) {
		Monitor[i].PID = 0;
		Monitor[i].readsAssigned = 0;
		Monitor[i].readsComplete = 0;
	}
	int monsize = sizeof(_table)*numProcs; 
	
	//initialize shared memory
	shm_unlink(MEMKEY);
	int fd = shm_open(MEMKEY, O_CREAT|O_RDWR, 0666);
	ftruncate(fd, 4096);
	void* mapptr = mmap(NULL, monsize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (mapptr==MAP_FAILED) {
		cerr<< "Error: Unable to set shared memory" <<endl;
		exit(1);
	}
	
	
	
	
	
	//copy table to shared memory
	memcpy(mapptr, Monitor, monsize);
	if (msync(mapptr, monsize, MS_SYNC) != 0) {
		cerr << "Error: Cannot write to shared memory" << endl;
		exit(1);
	}
}
