/* adna - the Asynchronous-process DNA fastq checker and trimmer
 * MPI heavy processing section
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

int adaps[28][70] = {0};

void fillAdapters(int x, int xl, int y, int yl) {
	if (x != 0) {
		adaps[x-1][xl] = adaps [x-1][xl] + 1;
	}
	if (y != 0) {
		adaps[y-1][yl] = adaps [y-1][yl] + 1;
	}
}

int main(int argc, char **argv) {
	MPI::Init(argc, argv);
	int comm_sz = MPI::COMM_WORLD.Get_size(); // number of processes
	int my_rank = MPI::COMM_WORLD.Get_rank(); // process rank
	
	string line;
	readmap readdb;
	readmap::hasher hashfn = readdb.hash_function();
	
	int merges = 0;
	int tRems = 0;
	int badReads = 0;

	int statsize = sizeof(_stat)*comm_sz;
	int jobsize = sizeof(_job);
	int fdj = shm_open(JOBKEY, O_RDWR, 0666);
	void* mapptrj = mmap(NULL, jobsize, PROT_READ|PROT_WRITE, MAP_SHARED, fdj, 0);
	
	int fdt = shm_open(TABKEY, O_RDWR, 0666);
	void* mapptrt = mmap(NULL, statsize, PROT_READ|PROT_WRITE, MAP_SHARED, fdt, 0);
	_stat* myStat = static_cast<_stat*>(mapptrt);
	
	if (mapptrj==MAP_FAILED) {
		stringstream error;
		error << "MPI Proc "<< my_rank << " Error: Unable to set job shared memory";
		myStat[my_rank].section = -1;
		throw std::runtime_error(error.str().c_str());
	}

	if (mapptrt==MAP_FAILED) {
		stringstream error;
		error << "MPI Proc "<< my_rank << " Error: Unable to set stat shared memory";
		myStat[my_rank].section = -2;
		throw std::runtime_error(error.str().c_str());
	}
	_job* myJob = static_cast<_job*>(mapptrj);

	
	if ((my_rank==0) and (comm_sz != myJob->numProcs)) {
		stringstream error;
		error << "MPI Proc "<< my_rank << " Error: Job Process Number (" << myJob->numProcs;
		error << ") does not match number of running MPI Processes (" << comm_sz << ")";
		myStat[my_rank].section = -3;
		throw std::runtime_error(error.str().c_str());
	}
	
	myStat[my_rank].PID = getpid();
	myStat[my_rank].section = 1;
	
	// Start Read One
	ifstream readOne;
	readOne.open(myJob->fq1n, ios::in);
	if (!readOne.is_open()) {
		stringstream error;
		error << "Error: Cannot open file " << myJob->fq1n;
		myStat[my_rank].section = -10;
		throw std::runtime_error(error.str().c_str());
	} 
	getline(readOne, line); //must be a header line
	while(!readOne.eof()) { 
		if (line.length() < 4) { //somethings wrong, this is not a header line
			stringstream error;
			error << "Read two attempting to use header \"" << line << "\"";
			myStat[my_rank].section = -11;
			throw std::runtime_error(error.str().c_str());
		}
		string headerf = line;
		string header = line.substr(0,line.find(' '));
		size_t header_hash = hashfn(header);
		if ((header_hash % comm_sz) == (size_t)my_rank) { // this process will use this header
			getline(readOne, line);
			string lineRead = line;
			readOne.ignore(numeric_limits<streamsize>::max(),'\n');
			getline(readOne, line);
			string lineQual = line;
			
			ReadPair readData = ReadPair(headerf, lineRead, lineQual, my_rank);
			readdb.emplace(header, readData);
			myStat[my_rank].readsAssigned++;
		} else { // this process will not use this header
			for(int i=0; i<3; i++) {
				readOne.ignore(numeric_limits<streamsize>::max(),'\n');
			}
		}
		getline(readOne, line); //must be a header line
	}
	readOne.close();
	
	myStat[my_rank].section = 2;
	
	ifstream readTwo;
	readTwo.open(myJob->fq2n, ios::in);
	if (!readTwo.is_open()) {
		stringstream error;
		error << "Error: Cannot open file " << myJob->fq2n;
		myStat[my_rank].section = -20;
		throw std::runtime_error(error.str().c_str());
	}
	getline(readTwo, line); //must be a header line
	while(!readTwo.eof()) {
		if (line.length() < 4) { //somethings wrong, this is not a header line
			stringstream error;
			error << "Read two attempting to use header \"" << line << "\"";
			myStat[my_rank].section = -21;
			throw std::runtime_error(error.str().c_str());
		}
		string headerf = line;
		string header = line.substr(0,line.find(' '));
		size_t header_hash = hashfn(header);
		if ((header_hash % comm_sz) == (size_t)my_rank) { // this process will use this header
			getline(readTwo, line);
			string lineRead = line;
			readTwo.ignore(numeric_limits<streamsize>::max(),'\n');
			getline(readTwo, line);
			string lineQual = line;
			try {
				ReadPair temp = readdb.at(header);
				readdb.erase(header);
				temp.addR2(headerf, lineRead, lineQual);
				temp.Compile(myJob->jobdir);
				fillAdapters(temp.getLeftA(), temp.getLeftAL(), temp.getRightA(), temp.getRightAL());
				badReads += temp.getBad();
				if (temp.isMerged() == 1) merges++;
				if (temp.tStripped()) tRems++;
				readdb.emplace(header, temp);
				myStat[my_rank].readsComplete++;
			} catch(const std::out_of_range& oor) {
				stringstream error;
				error << "Cannot find read for header in read two: " << header;
				myStat[my_rank].section = -22;
				throw std::runtime_error(error.str().c_str());
			}
			
		} else { // this process will not use this header
			for(int i=0; i<3; i++) {
				readTwo.ignore(numeric_limits<streamsize>::max(),'\n');
			}
		}
		getline(readTwo, line); //must be a header line
	}
	readTwo.close();
	
	myStat[my_rank].section = 3;
	
	stringstream ofna;
	ofna << myJob->jobdir << "/ind/adaptersRemoved_p" << my_rank << ".txt";
	ofstream ofsa;
	ofsa.open(ofna.str(), ios::out);
	for (int i=0; i<28; i++) {
		for (int y=0; y<70; y++) {
			if (adaps[i][y] > 0) {
				ofsa << i+1 << " " << y+1 << " " << adaps[i][y] << endl;
			}
		}
	}
	ofsa.close();
	
	stringstream ofnt;
	ofnt << myJob->jobdir << "/ind/tRemoveCount_p" << my_rank << ".txt";
	ofstream ofst;
	ofst.open(ofnt.str(), ios::out);
	ofst << tRems << endl;
	ofst.close();
	
	stringstream ofnm;
	ofnm << myJob->jobdir << "/ind/mergeCount_p" << my_rank << ".txt";
	ofstream ofsm;
	ofsm.open(ofnm.str(), ios::out);
	ofsm << merges << endl;
	ofsm.close();
	
	stringstream ofnb;
	ofnb << myJob->jobdir << "/ind/goodBadReadsCount_p" << my_rank << ".txt";
	ofstream ofsb;
	ofsb.open(ofnb.str(), ios::out);
	ofsb << (2 * myStat[my_rank].readsAssigned - badReads) << " " << badReads << endl;
	ofsb.close();
	
	// End of MPI Process
	myStat[my_rank].section = 99;
	MPI_Finalize();
	return 0;
}








