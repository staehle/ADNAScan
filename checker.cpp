/* ADNA
 * Checker application
 */

#include <iostream>
#include <cstring>
#include "adnashm.hpp"
#include <curses.h>
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
	
	/*
	cout << "Job name: " << myJob->jobname << endl;
	cout << "fastq #1: " << myJob->fq1n << endl;
	cout << "fastq #2: " << myJob->fq2n << endl;
	for(int i=0; i<myJob->numProcs; i++) {
		cout << "-------------------------------------------" << endl;
		cout << "Proc " << i << " stats:" << endl;
		cout << "               PID: " << myStat[i].PID << endl;
		cout << "   Current Section: " << myStat[i].section << endl;
		cout << "    Reads Assigned: " << myStat[i].readsAssigned << endl;
	}
	
	cout << "-------------------------------------------" << endl;
	cout << "- adna -- current job is still running    -" << endl;
	cout << "-------------------------------------------" << endl;
	*/
	
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
	waddstr(stdscr, (char*)"Press 'q' to exit. The adna job will continue to run.");
	wrefresh(stdscr);
	
	WINDOW * statscr = newwin(LINES-6, COLS-2, 3, 1);
	
	
	
	wrefresh(statscr);
	
	while((ch = wgetch(stdscr))) {
		if (ch == 'q') break;
	}
	
	endwin();
	
	return 0;
}








