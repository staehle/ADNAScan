/* ADNA
 * Main driver
 */
 
#include <mpi.h>
//#include <cstdio>
#include <iostream>
//#include <fstream>
#include <sstream>
//#include <unordered_map>
using namespace std;

string fastq_read1;
string fastq_read2;
unordered_map<string, string>* readdb; // <"header", ReadClass>

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
			exit(1);
		}
		fastq_read1 = argv[1];
		fastq_read2 = argv[2];
		
		
	}
	
	
    
    MPI_Finalize();
    return 0;
}
