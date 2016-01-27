/* ADNA
 * Main driver for shared memory initialization 
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <unordered_map>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "main.hpp"

using namespace std;

int main(int argc, char **argv) {
	cout << "adna -- Asynchronous process DNA trimmer and analyzer" << endl << endl;
	if (argc < 4) {
		cerr << "Error: Not enough arguments" << endl;
		cerr << "Usage: " << argv[0] << " <num_processes> <input_fastq_file_1> <input_fastq_file_2> (output_name)" << endl;
		cerr << "OPTIONAL:" << endl;
		cerr << "\toutput_name\tSkip fastq name checking and use this custom name" << endl;
		exit(1);
	}
	int numProcs = atoi(argv[1]);
	if (numProcs < 1 or numProcs > 512) {
		cerr << "Error: num_processes outside of acceptable range of 1-512" << endl;
		exit(1);
	}
	
	//init job
	_job thisJob;
	thisJob.fq1n = string(argv[2]);
	thisJob.fq2n = string(argv[3]);
	if (argc < 5) {
		string fq1nt = thisJob.fq1n.substr(thisJob.fq1n.find_last_of('/')+1, 37);
		string fq2nt = thisJob.fq2n.substr(thisJob.fq2n.find_last_of('/')+1, 37);
		if (fq1nt.compare(fq2nt) != 0) {
			cerr << "Error: The two fastq files do not match. Got:\t" << endl << fq1nt << endl << fq2nt << endl;
			exit(1);
		} else {
			thisJob.jobname = fq1nt;
		}
	} else {
		thisJob.jobname = string(argv[4]);
	}
	cout << "Notice: Using job name: '" << thisJob.jobname << "'" << endl;
	
	//init process table
	_stat thisStat[numProcs];
	for (int i=0; i<numProcs; i++) {
		thisStat[i].PID = 0;
		thisStat[i].readsAssigned = 0;
		thisStat[i].readsComplete = 0;
	}
	
	//initialize shared memory
	int statsize = sizeof(_stat)*numProcs; 
	int jobsize = sizeof(thisJob);
	shm_unlink(TABKEY);
	shm_unlink(JOBKEY);
	int fdt = shm_open(TABKEY, O_CREAT|O_RDWR, 0666);
	int fdj = shm_open(JOBKEY, O_CREAT|O_RDWR, 0666);
	ftruncate(fdt, 4096);
	ftruncate(fdj, 4096);
	void* mapptrt = mmap(NULL, statsize, PROT_READ|PROT_WRITE, MAP_SHARED, fdt, 0);
	if (mapptrt==MAP_FAILED) {
		cerr<< "Error: Unable to set shared memory for stat table" <<endl;
		exit(1);
	}
	void* mapptrj = mmap(NULL, jobsize, PROT_READ|PROT_WRITE, MAP_SHARED, fdj, 0);
	if (mapptrj==MAP_FAILED) {
		cerr<< "Error: Unable to set shared memory for job table" <<endl;
		exit(1);
	}
	
	//copy table to shared memory
	memcpy(mapptrt, thisStat, statsize);
	if (msync(mapptrt, statsize, MS_SYNC) != 0) {
		cerr << "Error: Cannot write to shared stat memory" << endl;
		exit(1);
	}
	memcpy(mapptrj, &thisJob, statsize);
	if (msync(mapptrj, jobsize, MS_SYNC) != 0) {
		cerr << "Error: Cannot write to shared job memory" << endl;
		exit(1);
	}
	
	cout << "adna -- initialization complete" << endl;
	return 0;
}
