/* ADNA
 * FASTQ File Splitter
 */

#include <cstdio>
#include <iostream>
#include <mpi.h>
using namespace std;
#define maxbps 128

int main(int argc, char **argv) {
    int comm_sz; // number of processes
    int my_rank; // process rank
    
    int token = 32;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	int next_rank = my_rank + 1;
    if (next_rank >= comm_sz) next_rank = 0;

    int prev_rank = my_rank - 1;
    if (prev_rank < 0) prev_rank = comm_sz - 1;
    
    MPI_Request request;
    MPI_Status status;
    
    if (my_rank == 0) {
    	if (token > 0) token--;
    	cout << "[process " << my_rank << "]: sending  " << token << endl;
    	
    	MPI_Isend(&token,
                 1,
                 MPI_INT,
                 next_rank,
                 0,
                 MPI_COMM_WORLD,
                 &request);
        MPI_Wait(&request, &status);
    } 
    
    while (token - comm_sz >= 0) {
        MPI_Irecv(&token,
                 1,
                 MPI_INT,
                 prev_rank,
                 0,
                 MPI_COMM_WORLD,
                 &request);

        //You could do some processing in here if you wanted.

        MPI_Wait(&request, &status);

//        cout << "[process " << my_rank << "]: received " << token << endl;
        token--;

        if (token >= 0) {
            cout << "[process " << my_rank << "]: sending  " << token << endl;
            MPI_Isend(&token,
                     1,
                     MPI_INT,
                     next_rank,
                     0,
                     MPI_COMM_WORLD,
                     &request);

            MPI_Wait(&request, &status);
        }
    }

    MPI_Finalize();
    return 0;
}

/*
#include <iostream>
using namespace std;
int main(int argc, char **argv) {
	cout << "ADNAScan" << endl;
	if (argc < 2) {
		cout << "Error: Not enough arguments" << endl;
		cout << "Usage: " << argv[0] << " <input_fastq_file> <options>" << endl;
		return -1;
	}
	
	// Grab FASTQ File
	ifstream fin;
	fin.open(filename, ios::in);
	if (!fin.is_open()) {
		cerr << "Error: Cannot open file" << endl;
		return -1;
	}
	while(!fin.eof()) {
		
	}
	
	// Split FASTQ File into equal segments

	return 0;
}*/

