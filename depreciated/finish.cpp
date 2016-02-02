/* ADNA
 * Cleanup driver
 */

#include <iostream>
#include <cstring>
#include "adnashm.hpp"
using namespace std;

int main() {
	cout << "-------------------------------------------" << endl;
	cout << "- adna -- post-job statistics and cleaner -" << endl;
	cout << "-------------------------------------------" << endl;

	int jobsize = sizeof(_job);
	int fdj = shm_open(JOBKEY, O_RDWR, 0666);
	void* mapptrj = mmap(NULL, jobsize, PROT_READ|PROT_WRITE, MAP_SHARED, fdj, 0);
	if (mapptrj==MAP_FAILED) {
		cerr << "Finish Error: Unable to find job shared memory" << endl;
		exit(1);
	}
	_job* myJob = static_cast<_job*>(mapptrj);
	
	int statsize = sizeof(_stat)*myJob->numProcs;
	int fdt = shm_open(TABKEY, O_RDWR, 0666);
	void* mapptrt = mmap(NULL, statsize, PROT_READ|PROT_WRITE, MAP_SHARED, fdt, 0);
	if (mapptrt==MAP_FAILED) {
		cerr << "Finish Error: Unable to find stat shared memory" << endl;
		exit(1);
	}
	_stat* myStat = static_cast<_stat*>(mapptrt);
	
	cout << "Job name: " << myJob->jobname << endl;
	cout << "fastq #1: " << myJob->fq1n << endl;
	cout << "fastq #2: " << myJob->fq2n << endl;
	for(int i=0; i<myJob->numProcs; i++) {
		cout << "-------------------------------------------" << endl;
		cout << "Proc " << i << " stats:" << endl;
		cout << "    Reads Assigned: " << myStat[i].readsAssigned << endl;
	}
	
	cout << endl << "-------------------------------------------" << endl;
	cout << "- adna -- cleanup success. job finished!  -" << endl;
	cout << "-------------------------------------------" << endl;
	
	
	if (shm_unlink(JOBKEY)!=0) cerr << "Cleaner error: Unable to unlink shared memory with: " << JOBKEY << endl;
	if (shm_unlink(TABKEY)!=0) cerr << "Cleaner error: Unable to unlink shared memory with: " << TABKEY << endl;
	
	return 0;
}








