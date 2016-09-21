#ifndef STRINGTINDER_H
#define STRINGTINDER_H

#include <boost/mpi.hpp>
namespace mpi = boost::mpi;

int StringTinder(boost::mpi::communicator, char*, char*);

#endif

