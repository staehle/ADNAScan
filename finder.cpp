/* ADNA
 * FASTQ Sequence Location Finder
 */
 
#include <cstdio>
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;
string fastq_read1;
string fastq_read2;

void dofind(int threadnum) {
	ifstream fin;
	string line;
	if (readnum==1) fin.open(fastq_read1, ios::in);
	else if (readnum==2) fin.open(fastq_read2, ios::in);
	if (!fin.is_open()) {
		cerr << "Error: Cannot open file" << endl;
		exit(1);
	}
	
	
}

int main(int argc, char **argv) {
	cout << "ADNA-Finder -- Scan FASTQ reads and return sequence locations" << endl;
	if (argc < 5) {
		cerr << "Error: Not enough arguments" << endl;
		cerr << "Usage: " << argv[0] << " <input_fastq_file_1> <input_fastq_file_2> -t <threads>" << endl;
		exit(1);
	}
	fastq_read1 = argv[1];
	fastq_read2 = argv[2];
	
	istringstream ss(argv[4]);
	if (!(ss >> numthreads)) { 
		cerr << "Error: Invalid number of threads\n";
		exit(1);
	}
	
	/* Splitter for read 1 */
	readnum = 1;
	cerr << "Scan-Find read 1 in main for testing"<<endl;
	dofind(1);
	/*
	dead = 0;
	cerr << "Splitting read 1 with thread pool of "<<numthreads<<" threads"<<endl;
	thread threadpool[numthreads];
	for (int t=0; t < numthreads; t++) {
		threadpool[t] = thread(dosplit, t);
	}
	for (int t=0; t < numthreads; t++) {
		threadpool[t].join();
	}
	while (dead<numthreads); // poor man's thread block
	cerr << "The splitter pool for read 1 has terminated." << endl;
	*/
	
	/* Splicer for read 2 */
	readnum = 2;
	cerr << "Scan-Find read 2 in main for testing"<<endl;
	dofind(1);
	/*
	dead = 0;
	cerr << "Opening read 2 and adding to split files with thread pool of "<<numthreads<<" threads"<<endl;
	for (int t=0; t < numthreads; t++) {
		threadpool[t] = thread(dosplit, t);
	}
	for (int t=0; t < numthreads; t++) {
		threadpool[t].join();
	}
	while (dead<numthreads); // poor man's thread block
	cerr << "The splicer pool for read 2 has terminated." << endl;
	*/
	
	cerr << "ADNA-Finder has completed!"<<endl;
	
	return 0;
}
