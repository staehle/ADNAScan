/* ADNA
 * FASTQ File Splitter
 */

#include <cstdio>
#include <iostream>
#include <thread>
#include <string>
using namespace std;
#define BP_SPLIT_SIZE 128; // max size of each split file
string fastq_orig;
int numthreads;
int workload;
int workload_last;

int splicer(int threadnum) {
	ifstream fin;
	fin.open(filename, ios::in);
	if (!fin.is_open()) {
		cerr << "Error: Cannot open file" << endl;
		return -1;
	}
	// need to split my workload by BP_SPLIT_SIZE.
	int my_workload;
	if (threadnum == numthreads-1) {
		my_workload = workload_last;
	} else {
		my_workload = workload;
	}
	
	// each thread will access fastq_orig on an offset of (threadnum + numthreads) * 4
	
	
	// while(!fin.eof()) {}
	string eofreport = "Thread #"+threadnum+" has completed "+my_workload+" BPs\n";
}

int main(int argc, char **argv) {
	cout << "ADNA Split" << endl;
	if (argc < 2) {
		cout << "Error: Not enough arguments" << endl;
		cout << "Usage: " << argv[0] << " <input_fastq_file> <OPTIONS> -t <threads> -o <output> " << endl;
		return -1;
	}
	fastq_orig = argv[1];
	fastq_size = 4293; // number of base pairs in fastq file.   !!! TO-DO: calculate each bp set: number of lines in the fastq file / 4 (each set)
	
	numthreads = argv[4];
	thread threadpool[numthreads]; // is this actually a thing? i dunno i haven't tested it. 
	
	/* example file distribution:
		BP_SPLIT_SIZE = 128
		numthreads = 8
		fastq_size = 4293
		workload = 536 (.625)
		workload_last = 541 = 536+5
	*/
	
	workload = (int)fastq_size / numthreads; // number of base pairs each thread will work on.
	workload_last = (int)workload+(fastq_size % numthreads); // last thread might have a few more bps at the end.
	
	for (int t=0; t < numthreads; t++) {
		threadpool[t](splicer, n);
	}
	
	for (int t=0; t < numthreads; t++) {
		threadpool[t].join();
	}
	cout << "The thread pool has terminated." << endl;
	return 0;
}










