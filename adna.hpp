#ifndef ADNA_H
#define ADNA_H

#define ADNA "adna - the Asynchronous-process DNA fastq checker and trimmer"
#define COPYRIGHT "(C) 2016 Jake Staehle <jacob@staehle.us>"

#define HOWTORUN "./adna"

#define MPINAME "./mpi-adna"

#define MPIRUN "mpirun"
#define MPINPV "-np"

// Process Definitions
#define GATEKEEPER 0
#define HUMANCENTIPEDE 99
#define STRINGTINDER 1
#define SORTINGHAT 2
#define SATANHIMSELF 3

#define MAGICKEY "password"
#define PASSWORD 12345

// Printing
#define STD_TAB "   "
#define REPORT_WIDTH 24
#define REPORT_FORMAT "["<<i_rank<<" / "<<my_name<<"]: "

#define PRINT_REPORT(text)						\
	ss_temp << P_BOLD(REPORT_FORMAT) << text;	\

#define SSTEMP_CLEAR							\
	ss_temp.str(std::string());

#define PRINT(text)										\
	PRINT_REPORT(text)									\
	world.send(GATEKEEPER, PASSWORD, ss_temp.str());	\
	SSTEMP_CLEAR

#define PRINTGK(text)												\
	PRINT_REPORT(text)												\
	for (int w = ss_temp.str().find(':'); w < REPORT_WIDTH; w++)	\
		cout << " ";												\
	cout << ss_temp.str() << endl;									\
	SSTEMP_CLEAR 

#define START P_START("Reporting for duty!")

// Exiting
#define EXIT_CLEAN														\
	PRINT_REPORT(P_EXIT("Looks like my job here is done! See ya next time!")) \
	world.send(GATEKEEPER, PASSWORD, ss_temp.str());					\
	SSTEMP_CLEAR;														\
	world.send(GATEKEEPER, PASSWORD, "@exit");							\
	MPI_Finalize();														\
	exit(0);

#define EXIT_ERROR(text)						\
	PRINTGK(P_ERROR("EXITING WITH ERROR:"));	\
	PRINTGK(P_ERROR(STD_TAB << text));			\
	exit(1);

#endif
