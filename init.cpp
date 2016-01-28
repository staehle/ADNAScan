/* ADNA
 * Job Initialization
 */

#include <iostream>
#include <cstring>
#include "adnashm.hpp"

using namespace std;

int main(int argc, char **argv) {
	cout << endl << "adna -- Asynchronous process DNA trimmer and analyzer" << endl;
	if (argc < 4) {
		cerr << "Error: Not enough arguments" << endl;
		cerr << "Usage: " << argv[0] << " <num_processes> <input_fastq_file_1> <input_fastq_file_2> (output_name)" << endl;
		cerr << "OPTIONAL:" << endl;
		cerr << "\toutput_name\tType a job name, otherwise it will generate based on input_fastq_file_1" << endl;
		exit(1);
	}
	int numProcs = atoi(argv[1]);
	if (numProcs < 1 or numProcs > 512) {
		cerr << "Error: num_processes outside of acceptable range of 1-512" << endl;
		exit(1);
	}
	
	// Init job
	_job* thisJob = new _job();
	strcpy(thisJob->fq1n, argv[2]);
	strcpy(thisJob->fq2n, argv[3]);
	if (argc < 5) {
		string fqo = string(argv[2]);
		fqo = fqo.substr(fqo.find_last_of('/')+1);
		fqo = fqo.substr(0, fqo.find_last_of('.')-1);
		strcpy(thisJob->jobname, fqo.c_str());
	} else {
		strcpy(thisJob->jobname, argv[4]);
	}
	cout << "Init: Creating job name: '" << thisJob->jobname << "'" << endl;
	
	// Init process table
	_stat thisStat[numProcs];
	for (int i=0; i<numProcs; i++) {
		thisStat[i].PID = 0;
		thisStat[i].readsAssigned = 0;
		thisStat[i].readsComplete = 0;
	}
	
	// Initialize shared memory
	int statsize = sizeof(_stat)*numProcs; 
	int jobsize = sizeof(_job);
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
	
	// Copy table to shared memory
	memcpy(mapptrt, thisStat, statsize);
	if (msync(mapptrt, statsize, MS_SYNC) != 0) {
		cerr << "Error: Cannot write to shared stat memory" << endl;
		exit(1);
	}
	memcpy(mapptrj, thisJob, jobsize);
	if (msync(mapptrj, jobsize, MS_SYNC) != 0) {
		cerr << "Error: Cannot write to shared job memory" << endl;
		exit(1);
	}
	
	cout << "adna -- initialization complete" << endl;
	return 0;
}




