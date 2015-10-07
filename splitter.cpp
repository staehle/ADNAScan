/* ADNA
 * FASTQ File Splitter
 */

#include <cstdio>
#include <iostream>
#include <thread>
#include <string>
using namespace std;
#define BP_SPLIT_SIZE 128;

string fastq_orig;
int workload;

int splicer(int filenum) {
	ifstream fin;
	fin.open(filename, ios::in);
	if (!fin.is_open()) {
		cerr << "Error: Cannot open file" << endl;
		return -1;
	}
	while(!fin.eof()) {
		
	}
	
	workload--;
}

int main(int argc, char **argv) {
	cout << "ADNA Split" << endl;
	if (argc < 2) {
		cout << "Error: Not enough arguments" << endl;
		cout << "Usage: " << argv[0] << " <input_fastq_file> <OPTIONS> -t <threads> -o <output> " << endl;
		return -1;
	}
	fastq_orig = argv[1];
	fastq_size = 2048; // calculate each bp set: number of lines in the fastq file / 4 (each set)
	
	int numthreads = argv[4];
	thread threadpool[numthreads];
	
	workload = fastq_size / BP_SPLIT_SIZE;
	
	for (int t=0; t < numthreads; t++) {
		threadpool[t](splicer, n);
	}
	
	for (int t=0; t < numthreads; t++) {
		threadpool[t].join();
	}
	cout << "The threadpool has terminated." << endl;
	return 0;
}










