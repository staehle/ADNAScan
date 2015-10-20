/* ADNA
 * FASTQ Sequence Location Finder
 */
 
#include <cstdio>
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <sstream>
//#include <algorithm>
#include <unordered_map>
using namespace std;
string fastq_read1;
string fastq_read2;
int numthreads;
int dead;
int readnum;
unordered_map<string, int>* threadtable1;
unordered_map<string, int>* threadtable2;

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
	threadtable1[threadnum]["test"] = 256; 
	threadtable1[threadnum]["123456789"] = 14;
	cout << readnum << " using test --> " << threadtable1[threadnum]["test"] <<endl;
	cout << readnum << " using nums --> " << threadtable1[threadnum]["123456789"] <<endl;
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
		//if (line.length() < 4) break;
		string head = line.substr(5,37);
		//replace(head.begin(), head.end(), ':', '_');
		//cout << "\thead: " << head << endl;
		if (readnum==1) threadtable1[threadnum][head] = lineno;
		else if (readnum==2) threadtable2[threadnum][head] = lineno;
		else exit(2);
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
	
	threadtable1 = new unordered_map<string, int>[numthreads];
	threadtable2 = new unordered_map<string, int>[numthreads];
	
	/* Splitter for read 1 */
	readnum = 1;
	cerr << "Scan-Find read 1 in main for testing"<<endl;
	dofind(0);
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
	dofind(0);
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
	
	cout << threadtable1[0]["D00550:263:C6V0DANXX:8:1101:1487:2192"] <<endl;
	cout << threadtable2[0]["D00550:263:C6V0DANXX:8:1101:1487:2192"] <<endl;
	
	return 0;
}
