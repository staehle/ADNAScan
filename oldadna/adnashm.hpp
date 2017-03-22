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
	pid_t PID;
	int readsAssigned;
	int readsComplete;
	int section;
};
struct _job {
	char jobname[512];
	char fq1n[512];
	char fq2n[512];
	char jobdir[512];
	int numProcs;
	time_t starttime;
};

#endif
