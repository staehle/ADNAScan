/* ADNA
 * Main driver
 */
 
#include <mpi.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <unordered_map>
#include "adnashm.hpp"
#include "mpiworld.hpp"
#include "ReadPair.hpp"
using namespace std;

//typedef unordered_map<string, ReadPair> readmap;

int main(int argc, char **argv) {
	MPI::Init(argc, argv);
	//int comm_sz = MPI::COMM_WORLD.Get_size(); // number of processes
    int my_rank = MPI::COMM_WORLD.Get_rank(); // process rank
    
    //int statsize = sizeof(_stat)*comm_sz;
    int jobsize = sizeof(_job);
    int fdj = shm_open(JOBKEY, O_RDWR, 0666);
	void* mapptrj = mmap(NULL, jobsize, PROT_READ|PROT_WRITE, MAP_SHARED, fdj, 0);
	if (mapptrj==MAP_FAILED) {
		stringstream error;
		error << "MPI Proc "<< my_rank << " Error: Unable to set job shared memory" << endl;
		cerr << error;
		exit(1);
	}
	/*
	int fdt = shm_open(TABKEY, O_RDWR, 0666);
	void* mapptrt = mmap(NULL, statsize, PROT_READ|PROT_WRITE, MAP_SHARED, fdt, 0);
	if (mapptrt==MAP_FAILED) {
		stringstream error;
		error << "MPI Proc "<< my_rank << " Error: Unable to set stat shared memory" << endl;
		cerr << error;
		exit(1);
	}*/
	
	_job* myJob = static_cast<_job*>(mapptrj);
    
    cout << myJob << endl;
    /*stringstream test;
    test << "MPI Proc " << my_rank << ": job name: " << myJob.jobname << endl;
    cout << test;*/
    
    MPI_Finalize();
    return 0;
}
