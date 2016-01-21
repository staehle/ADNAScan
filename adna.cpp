/* ADNA
 * Main driver
 */
 
#include <mpi.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <unordered_map>
#include "ReadPair.hpp"
using namespace std;

typedef unordered_map<string, ReadPair> readmap;

int adaps[27][70] = {0};


void fillAdapters(int x, int xl, int y, int yl)
{
   if (x != 0)
   {
      adaps[x-1][xl] = adaps [x-1][xl] + 1;
   }
   if (y != 0)
   {
      adaps[y-1][yl] = adaps [y-1][yl] + 1;
   }
	
}



int main(int argc, char **argv) {
	/* MPI Start */
    MPI::Init(argc, argv);
    int comm_sz = MPI::COMM_WORLD.Get_size(); // number of processes
    int my_rank = MPI::COMM_WORLD.Get_rank(); // process rank
    
    if (my_rank==0){
		cout << "ADNA-Finder -- Scan FASTQ reads and return sequence locations" << endl;
		if (argc < 3) {
			cerr << "Error: Not enough arguments" << endl;
			cerr << "Usage: " << argv[0] << " <input_fastq_file_1> <input_fastq_file_2>" << endl;
			exit(1); // is this the proper way to exit MPI?
		} 
	}
	// other mpi processes need to wait for process 0 to check arguments
	
	string line;
	readmap readdb; // <"header", "whole read information">
	readmap::hasher hashfn = readdb.hash_function();
	
	/* READ ONE */
	ifstream readOne;
	readOne.open(argv[1], ios::in);
	if (!readOne.is_open()) {
		cerr << "Error: Cannot open file " << argv[1] << endl;
		exit(1); // again, proper exit?
	}
	int readsassigned = 0;
	int merges = 0;
	int tRems = 0;
	int badReads = 0;
	while(!readOne.eof()) { 
		getline(readOne, line); //must be a header line
		if (line.length() < 4) break; //somethings wrong, this is not a header line
		string header = line.substr(5,37);
		
		size_t header_hash = hashfn(header);
		if ((header_hash % comm_sz) == (size_t)my_rank) { // this process will use this header
			
			getline(readOne, line);
			string lineRead = line;
			readOne.ignore(numeric_limits<streamsize>::max(),'\n');
			getline(readOne, line);
			string lineQual = line;
			
			ReadPair readData = ReadPair(header, lineRead, lineQual, my_rank);
			readdb.emplace(header, readData);
			readsassigned++;
		} else { // this process will not use this header
			for(int i=0; i<3; i++) {
				readOne.ignore(numeric_limits<streamsize>::max(),'\n');
				//getline(readOne, line);
			}
		}
	}
	readOne.close();
	
	stringstream report;
	report<<my_rank<<": has completed read one with "<<readsassigned<<" assigned reads"<<endl;
	cout<<report.str();
	
	/* READ TWO */
    ifstream readTwo;
    readTwo.open(argv[2], ios::in);
    int progress = 0;
    int ctr = 0;
    if (!readTwo.is_open()) {
		cerr << "Error: Cannot open file " << argv[2] << endl;
		exit(1); // again, proper exit?
	}
	while(!readTwo.eof()) { 
		getline(readTwo, line); //must be a header line
		if (line.length() < 4) break; //somethings wrong, this is not a header line
		string header = line.substr(5,37);
		
		size_t header_hash = hashfn(header);
		if ((header_hash % comm_sz) == (size_t)my_rank) { // this process will use this header
			ctr++;
			if(my_rank == 1)
			{
				if (progress == 0 && ctr > (readsassigned/10))
				{
					progress++;
					std::cout << "adna is 10% through the second reads...\n";
				}
				else if (progress == 1 && ctr > (2 * readsassigned/10))
				{
					progress++;
					std::cout << "adna is 20% through the second reads...\n";
				}
				else if (progress == 2 && ctr > (3 * readsassigned/10))
				{
					progress++;
					std::cout << "adna is 30% through the second reads...\n";
				}
				else if (progress == 3 && ctr > (4 * readsassigned/10))
				{
					progress++;
					std::cout << "adna is 40% through the second reads...\n";
				}
				else if (progress == 4 && ctr > (5 * readsassigned/10))
				{
					progress++;
					std::cout << "adna is 50% through the second reads...\n";
				}
				else if (progress == 5 && ctr > (6 * readsassigned/10))
				{
					progress++;
					std::cout << "adna is 60% through the second reads...\n";
				}
				else if (progress == 6 && ctr > (7 * readsassigned/10))
				{
					progress++;
					std::cout << "adna is 70% through the second reads...\n";
				}
				else if (progress == 7 && ctr > (8 * readsassigned/10))
				{
					progress++;
					std::cout << "adna is 80% through the second reads...\n";
				}
				else if (progress == 8 && ctr > (9 * readsassigned/10))
				{
					progress++;
					std::cout << "adna is 90% through the second reads...\n";
				}
			}
		
			getline(readTwo, line);
			string lineRead = line;
			readTwo.ignore(numeric_limits<streamsize>::max(),'\n');
			getline(readTwo, line);
			string lineQual = line;
			try {
				ReadPair temp = readdb.at(header);
				readdb.erase(header);
				temp.addR2(header, lineRead, lineQual);
				temp.Compile();
				fillAdapters(temp.getLeftA(), temp.getLeftAL(), temp.getRightA(), temp.getRightAL());
				badReads+=temp.getBad();
				if (temp.isMerged())
				{
					merges++;
				}
				if (temp.tStripped())
				{
					tRems++;
				}
				readdb.emplace(header, temp);
			} catch(const std::out_of_range& oor) {
				throw std::runtime_error("Cannot find read for header in read two.");
			}

		} else { // this process will not use this header
			for(int i=0; i<3; i++) {
				readTwo.ignore(numeric_limits<streamsize>::max(),'\n');
			}
		}
	}
    readTwo.close();
    
    if(my_rank == 1)
    {
    	std::cout << "adna is 100% through the second reads on its first process...\n";
    }
    
    stringstream report2;
	report2<<my_rank<<": has completed read two"<<endl;
	cout<<report2.str();
	   
    MPI_Finalize();
    
    char aFileName [30];
    sprintf(aFileName, "./results/aRem_%i.out", my_rank);
    ofstream adapOFile;
    adapOFile.open(aFileName, std::ios::app);
    for (int i = 0 ; i < 27 ; i++)
    {
    	for (int y = 0; y < 70; y++)
    	{
    		if (adaps[i][y] > 0){
    			adapOFile << "Adapter " << i + 1 << " length [" << y + 1 << "] -" << adaps[i][y] << "- times from process " << my_rank << ".\n";
    		}
    	}
    	//adapOFile << "Adapter " << i + 1 << " removed " << adaps[i] << " times from process " << my_rank << ".\n";
    }
    adapOFile.close();
    
   char tFileName [30];
   sprintf(tFileName, "./results/tRem_%i.out", my_rank);
   ofstream oFile1;
   oFile1.open(tFileName, std::ios::app);
   oFile1 << "Number of reads from process " << my_rank << " with removed T's: " << tRems << "\n";
   oFile1.close();
   
   char mFileName [30];
   sprintf(mFileName, "./results/merges_%i.out", my_rank);
   ofstream oFile2;
   oFile2.open(mFileName, std::ios::app);
   oFile2 << "Number of reads from process " << my_rank << " that were merged due to overlap: " << merges << "\n";
   oFile2.close();

   char bFileName [30];
   sprintf(bFileName, "./results/badReads_%i.out", my_rank);
   ofstream oFile3;
   oFile3.open(bFileName, std::ios::app);
   oFile3 << "Number of reads (halves) from process " << my_rank << " that were below quality threshold: " << badReads << "\n";
   oFile3 << "Good (halves): " << (2 * readsassigned - badReads - merges) << "\n";
   oFile3.close();

    return 0;
}



