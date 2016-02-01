/* ADNA
 * Checker application
 */

#include <iostream>
#include <cstring>
#include "adnashm.hpp"
#include <curses.h>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>
using namespace std;

int main() {
	/*cout << "-------------------------------------------" << endl;
	cout << "- adna -- current process checker         -" << endl;
	cout << "-------------------------------------------" << endl;*/

	int jobsize = sizeof(_job);
	int fdj = shm_open(JOBKEY, O_RDWR, 0666);
	void* mapptrj = mmap(NULL, jobsize, PROT_READ, MAP_SHARED, fdj, 0);
	if (mapptrj==MAP_FAILED) {
		cerr << "Finish Error: Unable to find job shared memory" << endl;
		exit(1);
	}
	_job* myJob = static_cast<_job*>(mapptrj);
	
	int statsize = sizeof(_stat)*myJob->numProcs;
	int fdt = shm_open(TABKEY, O_RDWR, 0666);
	void* mapptrt = mmap(NULL, statsize, PROT_READ, MAP_SHARED, fdt, 0);
	if (mapptrt==MAP_FAILED) {
		cerr << "Finish Error: Unable to find stat shared memory" << endl;
		exit(1);
	}
	_stat* myStat = static_cast<_stat*>(mapptrt);
	
	int ch;
	initscr();
	raw();
	nonl();
	noecho();
	intrflush(stdscr, FALSE);	
	(void)keypad(stdscr, TRUE);
	
	wclear(stdscr);
	wborder(stdscr, 0, 0, 0, 0, 0, 0, 0, 0);
	wmove(stdscr, 1, 2);
	waddstr(stdscr, (char*)"adna -- current job checker");
	wmove(stdscr, 2, 1);
	whline(stdscr, ACS_HLINE, COLS-2);
	//wmove(stdscr, 3, 1);
	//wprintw(stdscr, (char*)"This window is %d lines and %d columns", LINES, COLS);
	wmove(stdscr, LINES-3, 1);
	whline(stdscr, ACS_HLINE, COLS-2);
	wmove(stdscr, LINES-2, 2);
	waddstr(stdscr, (char*)"Press 'q' to exit and continue job. Press 'k' to exit and kill job.");
	
	string jobstr = "Job name: "+string(myJob->jobname);
	string fq1str = "FASTQ Read 1: "+string(myJob->fq1n);
	string fq2str = "FASTQ Read 2: "+string(myJob->fq2n);
	stringstream mpistr;
	mpistr << "Number of Processes: " << myJob->numProcs;
	
	wmove(stdscr, 3, 2);
	waddstr(stdscr, (char*)jobstr.c_str());
	wmove(stdscr, 4, 2);
	waddstr(stdscr, (char*)fq1str.c_str());
	wmove(stdscr, 5, 2);
	waddstr(stdscr, (char*)fq2str.c_str());
	wmove(stdscr, 6, 2);
	waddstr(stdscr, (char*)mpistr.str().c_str());
	
	wrefresh(stdscr);
	
	WINDOW * statscr = newwin(LINES-10, COLS-4, 7, 2);
	timeout(5000);
	
	while(1) {
		wclear(statscr);
		wmove(statscr, 0, 0);
		time_t curtime = chrono::system_clock::to_time_t(chrono::system_clock::now());
		string timestr = "Updating every 5 seconds. Last checked: "+string(ctime(&curtime));
		waddstr(statscr, (char*)timestr.c_str());
		
		int line = 1;
		for (int i=0; i<myJob->numProcs; i++) {
			wmove(statscr, line, 0);
			line++;
			stringstream procstat;
			procstat << "Proc " << i << " (" << myStat[i].PID << "): ";
			if (myStat[i].section == 0) {
				procstat << "Initialized and ready to start";
			} else if (myStat[i].section == 1) {
				procstat << "Assigning reads from read ONE. Currently has " << myStat[i].readsAssigned << " assigned reads";
			} else if (myStat[i].section == 2) {
				double progressdbl = myStat[i].readsComplete / (double)myStat[i].readsAssigned;
				procstat << setprecision(2) << "Comparing with read TWO data: " << progressdbl << "% (" << myStat[i].readsComplete << " / " << myStat[i].readsAssigned << ")";
			}
			
			waddstr(statscr, (char*)procstat.str().c_str());
		}
		
		wrefresh(statscr);
		
		ch = getch();
		
		if (ch == 'q' || ch == 'Q') {
			break;
		}
	}
	
	/*while((ch = wgetch(stdscr))) {
		if (ch == 'q') break;
	}*/
	
	endwin();
	
	
	return 0;
}








