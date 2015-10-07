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

int splicer(int filenum) {
	ifstream fin;
	fin.open(filename, ios::in);
	if (!fin.is_open()) {
		cerr << "Error: Cannot open file" << endl;
		return -1;
	}
	while(!fin.eof()) {
		
	}
}

int main(int argc, char **argv) {
	cout << "ADNA Split" << endl;
	if (argc < 2) {
		cout << "Error: Not enough arguments" << endl;
		cout << "Usage: " << argv[0] << " <input_fastq_file> <options>" << endl;
		return -1;
	}
	
	fastq_orig = argv[1]; 

	// Split FASTQ File into equal segments

	return 0;
}
