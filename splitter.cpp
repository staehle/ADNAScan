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
string fastq_read1;
string fastq_read2;
int numthreads;
int dead;
int readnum;

void dosplit(int threadnum) {
	string report = "Read #" + to_string(readnum) + ": Starting splitter thread " + to_string(threadnum) + "\n";
	cerr << report;
	ifstream fin;
	ofstream curof;
	string line;
	fin.open(fastq_read1, ios::in);
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
		string head = "./raw/"+line.substr(5,37)+"_X_"+line.substr(45);
		replace(head.begin(), head.end(), ':', '_');
		if (readnum==1) curof.open(head, ios::out);
		else if (readnum==2) curof.open(head, ios::app);
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
	report = "Read #" + to_string(readnum) + ": Thread " + to_string(threadnum) + " has completed\n";
	cerr << report;
	dead++;
	fin.close();
}

int main(int argc, char **argv) {
	cout << "ADNAify -- Split and combine FASTQ into ADNA format" << endl;
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
	
	/* Splicer for read 2 */
	readnum = 2;
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
	
	cerr << "ADNA-Split has completed!"<<endl;
	
	return 0;
}



