#ifndef STRINGTINDER_H
#define STRINGTINDER_H

#include <boost/mpi.hpp>
namespace mpi = boost::mpi;

struct Match {
    char header[2][MAXLINELEN];
	char seq[2][MAXLINELEN];
	char qual[2][MAXLINELEN];
};

int StringTinder(boost::mpi::communicator, char*, char*);

#endif

