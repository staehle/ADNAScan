#ifndef ADNA_HPP
#define ADNA_HPP

#define TABKEY "/adna_stat"
#define JOBKEY "/adna_job"

struct _stat {
	pid_t PID;
	int readsAssigned;
	int readsComplete;
};
struct _job {
	std::string jobname;
	std::string fq1n;
	std::string fq2n;
};

#endif
