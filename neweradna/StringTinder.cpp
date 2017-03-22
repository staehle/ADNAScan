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

typedef std::unordered_map<std::string, Match> readmap;

int StringTinder(mpi::communicator world, char* csfp_fq1, char* csfp_fq2) {
	int gate;
	std::stringstream ss_temp;
	std::string my_name = "StringTinder";
	int i_rank = STRINGTINDER;
    string l1, l2;
    size_t len;
    Match *curmatch;
	
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

    curmatch = new Match;

    // Header
    /*getline(ifs_fq1, l1);
    getline(ifs_fq2, l2);

    l1 = l1.substr(0,l1.find(' '));
    l2 = l2.substr(0,l2.find(' '));

    if (l1.compare(l2) != 0) ////////// how tf do i compare if theyre not same

    if (l1.copy(curmatch, l1.length()) == 0)
        EXIT_ERROR("Got line 1 of zero length");
    if (l2.copy(curmatch, l2.length()) == 0)
        EXIT_ERROR("Got line 2 of zero length");

    //cerr << l1 << endl;
    //cerr << l2 << endl;
    */
	broadcast(world, gate, STRINGTINDER);

	ifs_fq1.close();
	ifs_fq2.close();
	return 0;
}
