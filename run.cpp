/* adna
   
   the Asynchronous-process DNA fastq checker and trimmer
   an Awesome DNA utility

   (C) 2016 Jake Staehle <jacob@staehle.us>
 */

/* run process

   Gathers data and runs the adna mpi sequence

 */

using namespace std;
#include "adna.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>

void help(const char* path) {
	cerr << "Usage: " << path << " [fast_q_1] [fast_q_2] [fast_a] [output_dir] [n_proc]" << endl;
	cerr << "   fast_q_1 - .FASTQ read one file" << endl;
	cerr << "   fast_q_2 - .FASTQ read two file" << endl;
	cerr << "     fast_a - .FASTA adapter file" << endl;
	cerr << " output_dir - directory to place results" << endl;
	cerr << "     n_proc - number of processes to dedicate for analysis" << endl;
}

int main(int argc, char **argv) {
	if (argc < 6) {
		cerr << "Error: Not enough arguments" << endl;
		help(argv[0]);
		exit(1);
	}
	int i_proc = atoi(argv[5]);
	if (i_proc < 1 or i_proc > 512) { //idk there's some crazy systems out there
		cerr << "Error: n_proc outside of acceptable range of 1-512" << endl;
		help(argv[0]);
		exit(1);
	}

	char csfp_fq1[512], csfp_fq2[512], csfp_fa[512], csfp_res[512];
	realpath(argv[1], csfp_fq1);
	realpath(argv[2], csfp_fq2);
	realpath(argv[3], csfp_fa);
	realpath(argv[4], csfp_res);
	
	//iproc+=3; //gatekeeper, st, and sh

	stringstream ss_proc;
	ss_proc << i_proc;

	int ret = execlp(const_cast<char*>(MPIRUN),
					 const_cast<char*>(MPIRUN),
					 const_cast<char*>(MPINPV),
					 const_cast<char*>(&ss_proc.str()[0u]),
					 const_cast<char*>(MPINAME),
					 const_cast<char*>(MAGICKEY),
					 const_cast<char*>(csfp_fq1),
					 const_cast<char*>(csfp_fq2),
					 const_cast<char*>(csfp_fa),
					 const_cast<char*>(csfp_res),
					 NULL
		);
    cerr << ret << endl;
	
	return 1;

	
}
