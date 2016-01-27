#ifndef ADNASHM_HPP
#define ADNASHM_HPP

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TABKEY "/adna_stat"
#define JOBKEY "/adna_job"

struct _stat {
	int readsAssigned;
	int readsComplete;
};
struct _job {
	std::string jobname;
	std::string fq1n;
	std::string fq2n;
};

#endif
