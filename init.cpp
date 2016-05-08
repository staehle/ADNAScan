/* adna - the Asynchronous-process DNA fastq checker and trimmer
 * Job Initialization Program
 */

#include <iostream>
#include <cstring>
#include "adnashm.hpp"
#include <sstream>
#include <ctime>
#include <chrono>
#include <unistd.h>

using namespace std;

int main(int argc, char **argv) {
	cout << "adna - the Asynchronous-process DNA fastq checker and trimmer" << endl << endl;
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
	char fq1r[512], fq2r[512];
	realpath(argv[2], fq1r);
	realpath(argv[3], fq2r);
	strcpy(thisJob->fq1n, fq1r);
	strcpy(thisJob->fq2n, fq2r);
	if (argc < 5) {
		string fqo = string(argv[2]);
		fqo = fqo.substr(fqo.find_last_of('/')+1);
		fqo = fqo.substr(0, fqo.find_last_of('.')-1);
		fqo = fqo+"A";
		strcpy(thisJob->jobname, fqo.c_str());
	} else {
		strcpy(thisJob->jobname, argv[4]);
	}
	thisJob->numProcs = numProcs;
	thisJob->starttime = chrono::system_clock::to_time_t(chrono::system_clock::now());
	
	//set results dir
	tm *loctime = localtime(&thisJob->starttime);
	char cwd[512];
	if (getcwd(cwd, sizeof(cwd)) == NULL) perror("getcwd() error");
	stringstream resdir;
	resdir << cwd << "/results";
	stringstream newdir;
	newdir << cwd << "/results/" << thisJob->jobname << "." << (1900+loctime->tm_year) << "-" << (1+loctime->tm_mon) << "-" << loctime->tm_mday << "-" << (1+loctime->tm_hour) << "h" << (1+loctime->tm_min) << "m";
	strcpy(thisJob->jobdir, newdir.str().c_str());
	
	cout << "Init: Creating job: '" << thisJob->jobname << "'" << endl;
	cout << "Init: Setting job for " << thisJob->numProcs << " processes" << endl;
	cout << "Init: Using FASTQ read 1: " << thisJob->fq1n << endl;
	cout << "Init: Using FASTQ read 2: " << thisJob->fq2n << endl;
	cout << "Init: Results will be placed in: " << thisJob->jobdir << endl;
	
	//create directories
	stringstream cmd;
	cmd << "mkdir -p " << resdir.str() << "; mkdir " << thisJob->jobdir << "; mkdir " << thisJob->jobdir << "/ind";
	system(cmd.str().c_str());
	
	// Init process table
	_stat thisStat[numProcs];
	for (int i=0; i<numProcs; i++) {
		thisStat[i].PID = 0;
		thisStat[i].readsAssigned = 0;
		thisStat[i].readsComplete = 0;
		thisStat[i].section = 0;
	}
	
	// Initialize shared memory
	int statsize = sizeof(_stat)*numProcs; 
	int jobsize = sizeof(_job);
	
	int fdt = shm_open(TABKEY, O_CREAT|O_RDWR|O_EXCL, 0666);
	int fdj = shm_open(JOBKEY, O_CREAT|O_RDWR|O_EXCL, 0666);
	ftruncate(fdt, 4096);
	ftruncate(fdj, 4096);
	void* mapptrt = mmap(NULL, statsize, PROT_READ|PROT_WRITE, MAP_SHARED, fdt, 0);
	if (mapptrt==MAP_FAILED) {
		cerr<< "Error: Unable to set shared memory for stat table. Did you run adna-finish after your last job?" <<endl;
		exit(1);
	}
	void* mapptrj = mmap(NULL, jobsize, PROT_READ|PROT_WRITE, MAP_SHARED, fdj, 0);
	if (mapptrj==MAP_FAILED) {
		cerr<< "Error: Unable to set shared memory for job table. Did you run adna-finish after your last job?" <<endl;
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
	
	cout << "Init: Success!" << endl;
	cout << "Next, run `nohup mpirun -np <numProcs> ./bin/adna-gompi &`" << endl;
	cout << "   where numProcs is what you assigned here" << endl;
	cout << "After that, run `adna-check` to view job progress and perform tasks" << endl;
	return 0;
}




