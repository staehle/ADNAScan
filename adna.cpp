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
#include "ReadPair.hpp"
using namespace std;

typedef unordered_map<string, ReadPair> readmap;

int main(int argc, char **argv) {
	/* MPI Start */
    MPI::Init(argc, argv);
    int comm_sz = MPI::COMM_WORLD.Get_size(); // number of processes
    int my_rank = MPI::COMM_WORLD.Get_rank(); // process rank
    
    if (my_rank==0){
		cout << "ADNA-Finder -- Scan FASTQ reads and return sequence locations" << endl;
		if (argc < 3) {
			cerr << "Error: Not enough arguments" << endl;
			cerr << "Usage: " << argv[0] << " <input_fastq_file_1> <input_fastq_file_2>" << endl;
			exit(1); // is this the proper way to exit MPI?
		}
	}
	// other mpi processes need to wait for process 0 to check arguments
	
	string line;
	readmap readdb; // <"header", "whole read information">
	readmap::hasher hashfn = readdb.hash_function();
	//int roundnum;
	
	/* READ ONE */
	ifstream readOne;
	readOne.open(argv[1], ios::in);
	if (!readOne.is_open()) {
		cerr << "Error: Cannot open file " << argv[1] << endl;
		exit(1); // again, proper exit?
	}
	//roundnum = 0;
	while(!readOne.eof()) { 
		getline(readOne, line); //must be a header line
		if (line.length() < 4) break; //somethings wrong, this is not a header line
		string header = line.substr(5,37);
		//ReadPair readData;
		//long header_hash = (long) header;
		size_t header_hash = hashfn(header);
		if ((header_hash % comm_sz) == (size_t)my_rank) { // this process will use this header
			getline(readOne, line);
			string lineRead = line;
			readOne.ignore(numeric_limits<streamsize>::max(),'\n');
			getline(readOne, line);
			string lineQual = line;
			
			ReadPair readData = ReadPair(lineRead, lineQual);
			readdb.emplace(header, readData);
			
		} else { // this process will not use this header
			for(int i=0; i<4; ++i) {
				readOne.ignore(numeric_limits<streamsize>::max(),'\n');
			}
		}
	}
	readOne.close();
	
	/* READ TWO */
    ifstream readTwo;
    readTwo.open(argv[2], ios::in);
    if (!readTwo.is_open()) {
		cerr << "Error: Cannot open file " << argv[2] << endl;
		exit(1); // again, proper exit?
	}
    //roundnum = 0;
	while(!readTwo.eof()) { 
		getline(readTwo, line); //must be a header line
		if (line.length() < 4) break; //somethings wrong, this is not a header line
		string header = line.substr(5,37);
		//long header_hash = (long) header;
		size_t header_hash = hashfn(header);
		if ((header_hash % comm_sz) == (size_t)my_rank) { // this process will use this header
			getline(readTwo, line);
			string lineRead = line;
			readTwo.ignore(numeric_limits<streamsize>::max(),'\n');
			getline(readTwo, line);
			string lineQual = line;
			
			ReadPair temp = readdb.at(header);
			readdb.erase(header);
			temp.addR2(lineRead, lineQual);
			temp.Compile();
			readdb.emplace(header, temp);

		} else { // this process will not use this header
			for(int i=0; i<4; ++i) {
				readTwo.ignore(numeric_limits<streamsize>::max(),'\n');
			}
		}
	}
    readTwo.close();
    
    MPI_Finalize();
    return 0;
}
