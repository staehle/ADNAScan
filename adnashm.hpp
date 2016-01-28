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
};
struct _job {
	char jobname[64];
	char fq1n[64];
	char fq2n[64];
};

#endif
