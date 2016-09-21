/* adna
   
   the Asynchronous-process DNA fastq checker and trimmer
   an Awesome DNA utility

   (C) 2016 Jake Staehle <jacob@staehle.us>
 */

/* StringTinder

   Matches (get it?) two read identifers across two FASTQ files

 */

#include "adna.hpp"
#include <boost/mpi.hpp>
#include <boost/serialization/string.hpp>
#include "color.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "StringTinder.hpp"
#include <unordered_map>

using namespace std;
namespace mpi = boost::mpi;

struct Match {
	char seq[2][128];
	char qual[2][128];
};

typedef std::unordered_map<std::string, Match> readmap;

int StringTinder(mpi::communicator world, char* csfp_fq1, char* csfp_fq2) {
	int gate;
	std::stringstream ss_temp;
	std::string my_name = "StringTinder";
	int i_rank = STRINGTINDER;
	
	readmap readdb;
	readmap::hasher hashfn = readdb.hash_function();
	
	ifstream ifs_fq1, ifs_fq2;
	ifs_fq1.open(csfp_fq1, ios::in);
	ifs_fq2.open(csfp_fq2, ios::in);
	if (!ifs_fq1.is_open()) {
		EXIT_ERROR("Cannot open r1 file: " << csfp_fq1);
	} else {
		PRINT(P_INFO("Opened read one file: " << csfp_fq1));
	}
	if (!ifs_fq2.is_open()) {
		EXIT_ERROR("Cannot open r2 file: " << csfp_fq2);
	}  else {
		PRINT(P_INFO("Opened read two file: " << csfp_fq2));
	}

	PRINT("StringTinder reporting okay.");

	broadcast(world, gate, STRINGTINDER);

	ifs_fq1.close();
	ifs_fq2.close();
	return 0;
}
