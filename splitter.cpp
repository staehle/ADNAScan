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
string fastq_orig;
int numthreads;
int dead;

void splitter(int threadnum) { // Read 1: Split entire FASTQ into a separate file per read
	string report = "Starting splitter thread " + to_string(threadnum);
	cerr << report << endl;
	ifstream fin;
	ofstream curof;
	string line;
	fin.open(fastq_orig, ios::in);
	if (!fin.is_open()) {
		cerr << "Error: Cannot open file" << endl;
		exit(1);
	}
	int roundnum = 0;
	// where should i start? 
	// start of each is (threadnum*4)+(numthreads*4*roundnum);
	for(int i=0; i<(threadnum*4); ++i) {
		fin.ignore(numeric_limits<streamsize>::max(),'\n');
	}	
	while(!fin.eof()) {
		getline(fin, line);
		if (line.length() < 4) break;
		string head = "./raw/"+line.substr(5)+"--t"+to_string(threadnum);
		replace(head.begin(), head.end(), ':', '_');
		replace(head.begin(), head.end(), ' ', '_');
		curof.open(head, ios::out);
		curof<<line<<endl;
		for (int i=0; i<3; i++) {
			getline(fin, line);
			curof<<line<<endl;
		}
		curof.close();
		roundnum++;
		for(int i=0; i<(numthreads*4)-4; ++i) {
			fin.ignore(numeric_limits<streamsize>::max(),'\n');
		}
	}
	report = "Thread " + to_string(threadnum) + " has completed";
	cerr << report << endl;
	dead++;
	fin.close();
}

void splicer(int threadnum) { // Read 2: Splice read 2 into existing split files from read 1
	string report = "Starting splicer thread " + to_string(threadnum);
	cerr << report << endl;
}

int main(int argc, char **argv) {
	cout << "ADNAify -- Split and combine FASTQ into ADNA format" << endl;
	if (argc < 2) {
		cerr << "Error: Not enough arguments" << endl;
		cerr << "Usage: " << argv[0] << " <input_fastq_file_1> <input_fastq_file_2> -t <threads>" << endl;
		exit(1);
	}
	fastq_orig = argv[1];
	
	istringstream ss(argv[3]);
	if (!(ss >> numthreads)) { 
		cerr << "Error: Invalid number of threads\n";
		exit(1);
	}
	
	dead = 0;
	cerr << "Splitting read 1 with thread pool of " << numthreads << " threads"<<endl;
	thread threadpool[numthreads];
	for (int t=0; t < numthreads; t++) {
		threadpool[t] = thread(splitter, t);
	}
	for (int t=0; t < numthreads; t++) {
		threadpool[t].join();
	}
	while (dead<numthreads);
	cout << "The splitter pool for read 1 has terminated." << endl;
	
	dead = 0;
	cerr << "Opening read 2 and adding to split files with thread pool of " << numthreads << " threads"<<endl;
	fastq_orig = argv[2];
	for (int t=0; t < numthreads; t++) {
		threadpool[t] = thread(splicer, t);
	}
	for (int t=0; t < numthreads; t++) {
		threadpool[t].join();
	}
	while (dead<numthreads);
	cout << "The splitter pool for read 2 has terminated." << endl;
	
	// now, we need to combine the first and second read files into a new file. more threads!
	
	
	return 0;
}



