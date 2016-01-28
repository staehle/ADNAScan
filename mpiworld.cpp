/* ADNA
 * Main driver
 */
 
#include <mpi.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include "adnashm.hpp"
#include "mpiworld.hpp"
using namespace std;

int main(int argc, char **argv) {
	MPI::Init(argc, argv);
	int comm_sz = MPI::COMM_WORLD.Get_size(); // number of processes
	int my_rank = MPI::COMM_WORLD.Get_rank(); // process rank
	
	string line;
	readmap readdb;
	readmap::hasher hashfn = readdb.hash_function();

	int statsize = sizeof(_stat)*comm_sz;
	int jobsize = sizeof(_job);
	int fdj = shm_open(JOBKEY, O_RDWR, 0666);
	void* mapptrj = mmap(NULL, jobsize, PROT_READ|PROT_WRITE, MAP_SHARED, fdj, 0);
	if (mapptrj==MAP_FAILED) {
		stringstream error;
		error << "MPI Proc "<< my_rank << " Error: Unable to set job shared memory" << endl;
		cerr << error;
		exit(1);
	}
	int fdt = shm_open(TABKEY, O_RDWR, 0666);
	void* mapptrt = mmap(NULL, statsize, PROT_READ|PROT_WRITE, MAP_SHARED, fdt, 0);
	if (mapptrt==MAP_FAILED) {
		stringstream error;
		error << "MPI Proc "<< my_rank << " Error: Unable to set stat shared memory" << endl;
		cerr << error;
		exit(1);
	}
	_job* myJob = static_cast<_job*>(mapptrj);
	_stat* myStat = static_cast<_stat*>(mapptrt);
	
	myStat[my_rank].PID = getpid();

	stringstream test;
	test << "MPI Proc " << my_rank << ": has PID: " << getpid() << endl;
	test << "MPI Proc " << my_rank << ": starting job name: " << myJob->jobname << endl;
	test << "MPI Proc " << my_rank << ": has status PID of: " << myStat[my_rank].PID << endl;
	cout << test.str();
	
	// Start Read One
	ifstream readOne;
	readOne.open(myJob->fq1n, ios::in);
	if (!readOne.is_open()) {
		cerr << "Error: Cannot open file " << myJob->fq1n << endl;
		exit(1); 
	} 
	
	while(!readOne.eof()) { 
		getline(readOne, line); //must be a header line
		if (line.length() < 4) break; //somethings wrong, this is not a header line
		string header = line.substr(5,37);
		
		size_t header_hash = hashfn(header);
		if ((header_hash % comm_sz) == (size_t)my_rank) { // this process will use this header
			getline(readOne, line);
			string lineRead = line;
			readOne.ignore(numeric_limits<streamsize>::max(),'\n');
			getline(readOne, line);
			string lineQual = line;
			
			//ReadPair readData = ReadPair(header, lineRead, lineQual, my_rank); //TODO
			//readdb.emplace(header, readData);
			myStat[my_rank].readsAssigned++;
		} else { // this process will not use this header
			for(int i=0; i<3; i++) {
				readOne.ignore(numeric_limits<streamsize>::max(),'\n');
			}
		}
	}
	readOne.close();
	
	stringstream report;
	report << "MPI Proc " << my_rank << ": assigned "<< myStat[my_rank].readsAssigned << " reads" << endl;
	cout << report.str();

	MPI_Finalize();
	return 0;
}
