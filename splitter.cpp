/* ADNA
 * FASTQ File Splitter
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;
#define BP_SPLIT_SIZE 128; // max size of each split file
string fastq_orig;
int numthreads;
int alive;
//int workload;
//int workload_last;

void splicer(int threadnum) {
	cerr << "Starting thread " << threadnum <<endl;
	ifstream fin;
	ofstream curof;
	string line;
	int lineno;
	fin.open(fastq_orig, ios::in);
	if (!fin.is_open()) {
		cerr << "Error: Cannot open file" << endl;
		exit(1);
	}
	int roundnum = 0;
	// where should i start? 
	//int start = (threadnum*4)+(numthreads*4*roundnum);
	for(int i=0; i<(threadnum*4); ++i) {
		fin.ignore(numeric_limits<streamsize>::max(),'\n');
	}
	lineno = (threadnum*4);
	
	getline(fin, line);
	string head = "./raw/"+line.substr(5)+".fastq";
	replace(head.begin(), head.end(), ':', '_');
	replace(head.begin(), head.end(), ' ', '_');
	curof.open(head, ios::out);
	curof<<line<<endl;
	for (int i=0; i<3; i++) {
		getline(fin, line);
		curof<<line<<endl;
	}
	curof.close();
	
	// need to split my workload by BP_SPLIT_SIZE.
	/*int my_workload;
	if (threadnum == numthreads-1) {
		my_workload = workload_last;
	} else {
		my_workload = workload;
	}*/
	
	
	// output file names will be named "fastq_orig" - ".fastq" + ".fq####", where #### = (filenum * numthreads) + threadnum 
	
	
	// while(!fin.eof()) {}
	/*std::streamstring eofreport;
	eofreport << "Thread #" << threadnum << " has completed " << my_workload << " BPs\n";
	cout << eofreport;*/
	alive--;
	fin.close();
	exit(0);
}

int main(int argc, char **argv) {
	cout << "ADNA Split" << endl;
	if (argc < 2) {
		cout << "Error: Not enough arguments" << endl;
		cout << "Usage: " << argv[0] << " <input_fastq_file> -t <threads> -o <output> <OPTIONS>" << endl;
		return -1;
	}
	fastq_orig = argv[1];
	//int fastq_size = 4293; // number of base pairs in fastq file.   !!! TO-DO: calculate each bp set: number of lines in the fastq file / 4 (each set)
	
	istringstream ss(argv[3]);
	if (!(ss >> numthreads)) { 
		cerr << "Error: Invalid number of threads" << argv[3] << '\n';
	}
	
	cerr << "Starting pool with " << numthreads << " threads"<<endl;
	thread threadpool[numthreads];
	alive = numthreads;
	
	/* example file distribution:
		BP_SPLIT_SIZE = 128
		numthreads = 8
		fastq_size = 4293
		workload = 536 (.625)
		workload_last = 541 = 536+5
	*/
	//workload = (int)fastq_size / numthreads; // number of base pairs each thread will work on.
	//workload_last = (int)workload+(fastq_size % numthreads); // last thread might have a few more bps at the end.
	
	for (int t=0; t < numthreads; t++) {
		threadpool[t] = thread(splicer, t);
	}
	
	for (int t=0; t < numthreads; t++) {
		threadpool[t].join();
	}
	cout << "The thread pool has terminated." << endl;
	return 0;
}










