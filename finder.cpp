/* ADNA
 * FASTQ Sequence Location Finder
 */
 
#include <cstdio>
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <sstream>
#include <unordered_map>
using namespace std;
string fastq_read1;
string fastq_read2;
int numthreads;
int dead;
int readnum;
unordered_map<string, int>* read1table;
unordered_map<string, int>* read2table;

void dofind(int threadnum) {
	ifstream fin;
	string line;
	if (readnum==1) fin.open(fastq_read1, ios::in);
	else if (readnum==2) fin.open(fastq_read2, ios::in);
	if (!fin.is_open()) {
		cerr << "Error: Cannot open file #" << readnum << endl;
		exit(1);
	}
	//something something hash function idk
	/*
	read1table[threadnum]["test"] = 256; 
	read1table[threadnum]["123456789"] = 14;
	cout << readnum << " using test --> " << read1table[threadnum]["test"] <<endl;
	cout << readnum << " using nums --> " << read1table[threadnum]["123456789"] <<endl;
	*/
	int roundnum = 0;
	int lineno = 0;
	// where should i start? 
	// start of each is (threadnum*4)+(numthreads*4*roundnum);
	for(int i=0; i<(threadnum*4); ++i) {
		fin.ignore(numeric_limits<streamsize>::max(),'\n');
		lineno++;
	}
	//cout << "starting at line " << lineno << endl;
	getline(fin, line);
	while(!fin.eof()) {
		//cout << "reading at line " << lineno;
		if (line.length() < 4) break;
		string head = line.substr(5,37);
		//cout << "\thead: " << head << endl;
		if (readnum==1) read1table[threadnum][head] = lineno;
		else if (readnum==2) read2table[threadnum][head] = lineno;
		roundnum++;
		for(int i=0; i<(numthreads*4)-1; ++i) {
			fin.ignore(numeric_limits<streamsize>::max(),'\n');
			lineno++;
		}
		getline(fin, line);
		lineno++;
	}
	dead++;
	fin.close();
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
	
	read1table = new unordered_map<string, int>[numthreads];
	read2table = new unordered_map<string, int>[numthreads];
	
	/* Splitter for read 1 */
	readnum = 1;
	/*cerr << "Scan-Find read 1 in main for testing"<<endl;
	dofind(0);*/
	dead = 0;
	cerr << "Scan-Find read 1 with thread pool of "<<numthreads<<" threads"<<endl;
	thread threadpool[numthreads];
	for (int t=0; t < numthreads; t++) {
		threadpool[t] = thread(dofind, t);
	}
	for (int t=0; t < numthreads; t++) {
		threadpool[t].join();
	}
	while (dead<numthreads); // poor man's thread block
	cerr << "The finder pool for read 1 has terminated." << endl;
	
	/* Splicer for read 2 */
	readnum = 2;
	/*cerr << "Scan-Find read 2 in main for testing"<<endl;
	dofind(0);*/
	dead = 0;
	cerr << "Scan-Find read 2 with thread pool of "<<numthreads<<" threads"<<endl;
	for (int t=0; t < numthreads; t++) {
		threadpool[t] = thread(dofind, t);
	}
	for (int t=0; t < numthreads; t++) {
		threadpool[t].join();
	}
	while (dead<numthreads); // poor man's thread block
	cerr << "The finder pool for read 2 has terminated." << endl;
	
	cerr << "ADNA-Finder has completed!"<<endl;
	
	const char* testfind = "D00550:263:C6V0DANXX:8:1101:1270:2171";
	cout << "Test: Find read id " << testfind << " in read one: " << endl;
	for (int i=0; i<numthreads; i++) {
		cout << "\tread1table["<<i<<"]:\t"<< read1table[i][testfind] <<endl;
	}
	cout << "Test: Find read id " << testfind << " in read two: " << endl;
	for (int i=0; i<numthreads; i++) {
		cout << "\tread2table["<<i<<"]:\t"<< read2table[i][testfind] <<endl;
	}
	
	return 0;
}
