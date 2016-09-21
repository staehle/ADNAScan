/* adna
   
   the Asynchronous-process DNA fastq checker and trimmer
   an Awesome DNA utility

   (C) 2016 Jake Staehle <jacob@staehle.us>
 */

/* main process

   Exec'd from adna-run

 */

#include "adna.hpp"
#include <boost/mpi.hpp>
#include <boost/serialization/string.hpp>
#include "color.hpp"
#include <exception>
#include <iostream>
#include <sstream>
#include "StringTinder.hpp"

using namespace std;
namespace mpi = boost::mpi;

int main(int argc, char **argv) {
	mpi::environment env;
	mpi::communicator world;
	std::stringstream ss_temp;
	std::string my_name;
	int i_rank = world.rank();
	int i_max = world.size();
	int gate;
	string s_msg;

	switch(i_rank) {
	case GATEKEEPER: my_name = "Gatekeeper";
		break;
	case STRINGTINDER: my_name = "StringTinder";
		break;
	case SORTINGHAT: my_name = "SortingHat";
		break;
	case SATANHIMSELF: my_name = "SatanHimself";
		break;
	default: my_name = "idk my bff jill?";
	}
	
	if (i_rank == GATEKEEPER) {
		/*for (int i=0; i<argc; i++) {
			cout << i << " -> " << argv[i] << endl;
			}*/

		int alive = 3;
	    cerr << P_ADNA(ADNA) << endl;
		cerr << P_ADNA(COPYRIGHT) << endl << endl;
		cerr << P_INFO(STD_TAB << "MPI Initialized with " << i_max << " processes") << endl << endl;

		PRINTGK(START);
		sleep(.1);
		
		// TODO: change to needed processes
		try {
			if ((string(argv[1]).compare(MAGICKEY) != 0)) { 
				throw std::runtime_error("nope");
			}
		} catch(const std::exception& e) {
			EXIT_ERROR("Not so fast, buckaroo! You didn't start adna correctly. Please use " << HOWTORUN);
		}
		
		broadcast(world, gate, GATEKEEPER);

		while(alive > 0) {
			world.recv(mpi::any_source, PASSWORD, s_msg);
			if (s_msg.find('@') == string::npos) { // standard message
				for (int w = s_msg.find(':'); w < REPORT_WIDTH; w++)
					cout << " ";
				cout << s_msg << endl;
			} else { // exit message
				alive--;
			}
		}

		PRINTGK(P_EXIT("Thank you for using adna. Your results are located:"));
		PRINTGK(STD_TAB << argv[5]);
		MPI_Finalize();														\
		exit(0);

	} else {
	    broadcast(world, gate, GATEKEEPER); // wait for gatekeeper
		if (i_rank == STRINGTINDER) { 
			PRINT(START);
			sleep(.1);
			int e_st = StringTinder(world, argv[2], argv[3]);
			if (!e_st) { EXIT_CLEAN; }
			else { EXIT_ERROR("StringTinder exited with status " << e_st); }

		} else if (i_rank == SORTINGHAT) { 
			broadcast(world, gate, STRINGTINDER);
			PRINT(START);
			sleep(.1);

			broadcast(world, gate, SORTINGHAT);
			EXIT_CLEAN;

		} else if (i_rank == SATANHIMSELF) { 
			broadcast(world, gate, SORTINGHAT);
			PRINT(START);
			sleep(.1);
			
			EXIT_CLEAN;
		
		} else {
			EXIT_ERROR("hey dummy you started too many processes");
		}
	}

	exit(1);
}
