/* adna - the Asynchronous-process DNA fastq checker and trimmer
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
#include <signal.h>
#include <fstream>
using namespace std;

int main() {
	int jobsize = sizeof(_job);
	int fdj = shm_open(JOBKEY, O_RDWR, 0666);
	void* mapptrj = mmap(NULL, jobsize, PROT_READ, MAP_SHARED, fdj, 0);
	if (mapptrj==MAP_FAILED) {
		cerr << "Checker Error: Unable to find job shared memory. Did you initialize a new job?" << endl;
		exit(1);
	}
	_job* myJob = static_cast<_job*>(mapptrj);
	
	int statsize = sizeof(_stat)*myJob->numProcs;
	int fdt = shm_open(TABKEY, O_RDWR, 0666);
	void* mapptrt = mmap(NULL, statsize, PROT_READ, MAP_SHARED, fdt, 0);
	if (mapptrt==MAP_FAILED) {
		cerr << "Checker Error: Unable to find stat shared memory. Did you initialize a new job?" << endl;
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
	waddstr(stdscr, (char*)"adna - the Asynchronous-process DNA fastq checker and trimmer");
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
	string jobstm = " Job started: "+string(ctime(&myJob->starttime));
	
	wmove(stdscr, 3, 2);
	waddstr(stdscr, (char*)jobstr.c_str());
	wmove(stdscr, 4, 2);
	waddstr(stdscr, (char*)fq1str.c_str());
	wmove(stdscr, 5, 2);
	waddstr(stdscr, (char*)fq2str.c_str());
	wmove(stdscr, 6, 2);
	waddstr(stdscr, (char*)mpistr.str().c_str());
	wmove(stdscr, 7, 2);
	waddstr(stdscr, (char*)jobstm.c_str());
	
	wrefresh(stdscr);
	
	WINDOW * statscr = newwin(LINES-11, COLS-4, 8, 2);
	timeout(1000);
	
	int clear = 0;
	int error = 0;
	
	while(1) {
		wclear(statscr);
		wmove(statscr, 0, 0);
		time_t curtime = chrono::system_clock::to_time_t(chrono::system_clock::now());
		string timestr = "Last checked: "+string(ctime(&curtime));
		waddstr(statscr, (char*)timestr.c_str());
		
		clear = myJob->numProcs;
		int line = 1;
		for (int i=0; i<myJob->numProcs; i++) {
			wmove(statscr, line, 0);
			line++;
			stringstream procstat;
			procstat << "Proc " << i << " (" << myStat[i].PID << "): ";
			if (myStat[i].section == 0) {
				procstat << "Initialized and ready to start";
			} else if (myStat[i].section == 1) {
				procstat << "Crawling read ONE. Assigned " << myStat[i].readsAssigned << " reads";
			} else if (myStat[i].section == 2) {
				double progressdbl = (myStat[i].readsComplete / (double)myStat[i].readsAssigned) * 100;
				procstat << setprecision(2) << fixed << "Comparing with read TWO: " << myStat[i].readsComplete << " / " << myStat[i].readsAssigned << " (" << progressdbl << "%)";
			} else if (myStat[i].section == 3) {
				procstat << "Compiling metrics";
			} else if (myStat[i].section == 99) {
				clear--;
				procstat << "Completed " << myStat[i].readsComplete << " reads (assigned " << myStat[i].readsAssigned << ")";
			} else if (myStat[i].section < 0) {
				procstat << "ERROR: ";
				if (myStat[i].section == -1) procstat<<"Unable to set job shared memory";
				else if (myStat[i].section == -2) procstat<<"Unable to set stat shared memory";
				else if (myStat[i].section == -3) procstat<<"Process number mismatch. See log.";
				else if (myStat[i].section == -10) procstat<<"Cannot open file " << myJob->fq1n;
				else if (myStat[i].section == -11) procstat<<"R1 Bad Header. See log.";
				else if (myStat[i].section == -20) procstat<<"Cannot open file " << myJob->fq2n;
				else if (myStat[i].section == -21) procstat<<"R2 Bad Header. See log.";
				else if (myStat[i].section == -22) procstat<<"R2 Header did not find R1 match. See log.";
				else procstat << "Code " << myStat[i].section;
				clear--;
				error++;
			} else {
				procstat << "ERROR Unknown state: " << myStat[i].section;
			}
			waddstr(statscr, (char*)procstat.str().c_str());
		}
		
		wrefresh(statscr);
		
		if (ch == 'k' || ch == 'K' || ch == 'q' || ch == 'Q') break;
		if (clear == 0) { //all procs have status 99
			wmove(stdscr, LINES-2, 2);
			wclrtoeol(stdscr);
			if (error<1) waddstr(stdscr, (char*)"adna has completed the job! Press any key to exit.");
			else waddstr(stdscr, (char*)"ERROR! adna has encountered a problem. Press any key to exit.");
			timeout(-1);
			getch();
			break;
		}
		ch = getch();
	}
	
	endwin();
	
	if (clear==0) {
		cerr << "Please wait while adna-check compiles the resulting data." << endl;
		
		int adaps[28][70] = {0};
		//string resdir = "./results/curjob/";
		string line;
		int gTotal = 0;
		int bTotal = 0;
		int mTotal = 0;
		int tTotal = 0;
		time_t curtime = chrono::system_clock::to_time_t(chrono::system_clock::now());
		//tm *loctime = localtime(&curtime);
		time_t jobtime = myJob->starttime;
		//tm *srttime = localtime(&jobtime);
		
		// combine fastq files
		ofstream ofr1ps;
		stringstream ofr1pn;
		ofr1pn << myJob->jobdir << "/" << myJob->jobname << "_pass_1.fastq";
		ofr1ps.open(ofr1pn.str(), ios_base::binary);
		ifstream ifr1ps[myJob->numProcs];
		for (int i=0; i<myJob->numProcs; i++) {
			stringstream ifr1pn;
			ifr1pn << myJob->jobdir << "/ind/read1Pass_p" << i << ".fastq";
			ifr1ps[i].open(ifr1pn.str(), ios_base::binary);
			ofr1ps << ifr1ps[i].rdbuf();
			ifr1ps[i].close();
		}
		ofr1ps.close();
		
		ofstream ofr2ps;
		stringstream ofr2pn;
		ofr2pn << myJob->jobdir << "/" << myJob->jobname << "_pass_2.fastq";
		ofr2ps.open(ofr2pn.str(), ios_base::binary);
		ifstream ifr2ps[myJob->numProcs];
		for (int i=0; i<myJob->numProcs; i++) {
			stringstream ifr2pn;
			ifr2pn << myJob->jobdir << "/ind/read2Pass_p" << i << ".fastq";
			ifr2ps[i].open(ifr2pn.str(), ios_base::binary);
			ofr2ps << ifr2ps[i].rdbuf();
			ifr2ps[i].close();
		}
		ofr2ps.close();
		
		ofstream ofr1fs;
		stringstream ofr1fn;
		ofr1fn << myJob->jobdir << "/" << myJob->jobname << "_fail_1.fastq";
		ofr1fs.open(ofr1fn.str(), ios_base::binary);
		ifstream ifr1fs[myJob->numProcs];
		for (int i=0; i<myJob->numProcs; i++) {
			stringstream ifr1fn;
			ifr1fn << myJob->jobdir << "/ind/read1Fail_p" << i << ".fastq";
			ifr1fs[i].open(ifr1fn.str(), ios_base::binary);
			ofr1fs << ifr1fs[i].rdbuf();
			ifr1fs[i].close();
		}
		ofr1fs.close();
		
		ofstream ofr2fs;
		stringstream ofr2fn;
		ofr2fn << myJob->jobdir << "/" << myJob->jobname << "_fail_2.fastq";
		ofr2fs.open(ofr2fn.str(), ios_base::binary);
		ifstream ifr2fs[myJob->numProcs];
		for (int i=0; i<myJob->numProcs; i++) {
			stringstream ifr2fn;
			ifr2fn << myJob->jobdir << "/ind/read2Fail_p" << i << ".fastq";
			ifr2fs[i].open(ifr2fn.str(), ios_base::binary);
			ofr2fs << ifr2fs[i].rdbuf();
			ifr2fs[i].close();
		}
		ofr2fs.close();
		
		ofstream ofrsps;
		stringstream ofrspn;
		ofrspn << myJob->jobdir << "/" << myJob->jobname << "_pass_single.fastq";
		ofrsps.open(ofrspn.str(), ios_base::binary);
		ifstream ifrsps[myJob->numProcs];
		for (int i=0; i<myJob->numProcs; i++) {
			stringstream ifrspn;
			ifrspn << myJob->jobdir << "/ind/singleReadPass_p" << i << ".fastq";
			ifrsps[i].open(ifrspn.str(), ios_base::binary);
			ofrsps << ifrsps[i].rdbuf();
			ifrsps[i].close();
		}
		ofrsps.close();
		
		ofstream ofrsfs;
		stringstream ofrsfn;
		ofrsfn << myJob->jobdir << "/" << myJob->jobname << "_fail_single.fastq";
		ofrsfs.open(ofrsfn.str(), ios_base::binary);
		ifstream ifrsfs[myJob->numProcs];
		for (int i=0; i<myJob->numProcs; i++) {
			stringstream ifrsfn;
			ifrsfn << myJob->jobdir << "/ind/singleReadFail_p" << i << ".fastq";
			ifrsfs[i].open(ifrsfn.str(), ios_base::binary);
			ofrsfs << ifrsfs[i].rdbuf();
			ifrsfs[i].close();
		}
		ofrsfs.close();
		
		// removed adapters stream
		ofstream ofarems;
		stringstream ofaremn;
		ofaremn << myJob->jobdir << "/adaptersRemoved.txt";
		ofarems.open(ofaremn.str(), ios_base::binary);
		ifstream ifarems[myJob->numProcs];
		for (int i=0; i<myJob->numProcs; i++) {
			stringstream ifaremn;
			ifaremn << myJob->jobdir << "/ind/adaptersRemoved_p" << i << ".txt";
			ifarems[i].open(ifaremn.str(), ios_base::binary);
			ofarems << ifarems[i].rdbuf();
			ifarems[i].close();
			ifarems[i].open(ifaremn.str(), ios::in);
			while (getline(ifarems[i], line)) {
				stringstream ssarem(line);
				int a, b, c;
				if (!(ssarem >> a >> b >> c)) { break; } // error
				adaps[a][b] += c;
			}
			ifarems[i].close();
		}
		ofarems.close();
		
		// good/bad file compiling
		ofstream ofgbs;
		stringstream ofgbn;
		ofgbn << myJob->jobdir << "/goodBadReadsCount.txt";
		ofgbs.open(ofgbn.str(), ios_base::binary);
		ifstream ifgbs[myJob->numProcs];
		for (int i=0; i<myJob->numProcs; i++) {
			stringstream ifgbn;
			ifgbn << myJob->jobdir << "/ind/goodBadReadsCount_p" << i << ".txt";
			ifgbs[i].open(ifgbn.str(), ios_base::binary);
			ofgbs << ifgbs[i].rdbuf();
			ifgbs[i].close();
			ifgbs[i].open(ifgbn.str(), ios::in);
			while (getline(ifgbs[i], line)) {
				stringstream ssgb(line);
				int a, b;
				if (!(ssgb >> a >> b)) { break; } // error
				gTotal += a;
				bTotal += b;
			}
			ifgbs[i].close();
		}
		ofgbs.close();
		
		// merge file compiling
		ofstream ofmcs;
		stringstream ofmcn;
		ofmcn << myJob->jobdir << "/mergeCount.txt";
		ofmcs.open(ofmcn.str(), ios_base::binary);
		ifstream ifmcs[myJob->numProcs];
		for (int i=0; i<myJob->numProcs; i++) {
			stringstream ifmcn;
			ifmcn << myJob->jobdir << "/ind/mergeCount_p" << i << ".txt";
			ifmcs[i].open(ifmcn.str(), ios_base::binary);
			ofmcs << ifmcs[i].rdbuf();
			ifmcs[i].close();
			ifmcs[i].open(ifmcn.str(), ios::in);
			while (getline(ifmcs[i], line)) {
				mTotal += atoi(line.c_str());
			}
			ifmcs[i].close();
		}
		ofmcs.close();
		
		// t removal count
		ofstream oftrs;
		stringstream oftrn;
		oftrn << myJob->jobdir << "/tRemoveCount.txt";
		oftrs.open(oftrn.str(), ios_base::binary);
		ifstream iftrs[myJob->numProcs];
		for (int i=0; i<myJob->numProcs; i++) {
			stringstream iftrn;
			iftrn << myJob->jobdir << "/tRemoveCount_p" << i << ".txt";
			iftrs[i].open(iftrn.str(), ios_base::binary);
			oftrs << iftrs[i].rdbuf();
			iftrs[i].close();
			iftrs[i].open(iftrn.str(), ios::in);
			while (getline(iftrs[i], line)) {
				tTotal += atoi(line.c_str());
			}
			iftrs[i].close();
		}
		oftrs.close();
		
		// output results text
		stringstream ofrn;
		ofrn << myJob->jobdir << "/results.txt";
		ofstream ofrs;
		ofrs.open(ofrn.str(), ios::out);
		ofrs << "adna - the Asynchronous-process DNA fastq checker and trimmer\n";
		ofrs << "\n          job name: " << myJob->jobname;
		ofrs << "\n fastq read 1 file: " << myJob->fq1n;
		ofrs << "\n fastq read 2 file: " << myJob->fq2n;
		ofrs << "\n       job started: " << ctime(&jobtime);
		ofrs << "     job completed: " << ctime(&curtime);
		ofrs << "\n  Total Read Count: " << gTotal+bTotal;
		ofrs << "\nPassing Read Count: " << gTotal;
		ofrs << "\nFailing Read Count: " << bTotal;
		ofrs << "\n         Fail Rate: " << setprecision(6) << ((double)bTotal/(gTotal+bTotal));
		ofrs << "\n\n Merged Pair Count: " << mTotal;
		ofrs << "\n   T Removal Count: " << tTotal << " pairs";
		ofrs << "\n\nAdapter Removal Stats:\n";
		for(int i=0; i < 28; i++) {
			stringstream temp, tempa, toAdd;
			toAdd << "Adapter " << i << " occurences ("; //TODO get adapter name from fasta file
			temp << adaps[i][0];
			int last = 0;
			int total = adaps[i][0];
			for(int j=0; j<70; j++) {
				tempa << ", " << adaps[i][j];
				if (adaps[i][j] > 0) {
					total += adaps[i][j];
					last = j;
					temp << tempa.str();
					tempa.str(std::string()); //clear
				}
			}
			toAdd << total << " total):\n[";
			toAdd << temp.str();
			toAdd << "]\n\n";
			if(last>0) ofrs << toAdd.str();
		}
		ofrs.close();
		
		// output results html
		stringstream ofrhn;
		ofrhn << myJob->jobdir << "/results.html";
		ofstream ofrhs;
		ofrhs.open(ofrhn.str(), ios::out);
		ofrhs << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n";
		ofrhs << "<title>adna: " << myJob->jobname << "</title>\n";
		ofrhs << "<link rel=\"stylesheet\" type=\"text/css\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap.min.css\" />\n";
		ofrhs << "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js\"></script>\n";
		ofrhs << "<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/js/bootstrap.min.js\"></script>\n";
		ofrhs << "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/Chart.js/1.0.2/Chart.min.js\"></script>\n";
		ofrhs << "</head><body><div class=\"container\">\n";
		ofrhs << "<h1>adna <small>the Asynchronous-process DNA fastq checker and trimmer</small></h1>\n";
		ofrhs << "<h2>job name: <small>" << myJob->jobname;
		ofrhs << "</h2>\n<h3>fastq read 1 file: <small>" << myJob->fq1n;
		ofrhs << "</small></h3>\n<h3>fastq read 2 file: <small>" << myJob->fq2n;
		ofrhs << "</small></h3>\n<h3>job started: <small>" << ctime(&jobtime) << "</small></h3>\n";		
		ofrhs << "</small></h3>\n<h3>job completed: <small>" << ctime(&curtime) << "</small></h3>\n";
		ofrhs << "<br><h2>Job data stats</h2>\n<div class=\"row\"><div class=\"col-md-6\">\n";
		ofrhs << "<canvas id=\"counts\" width=\"300\" height=\"300\"></canvas></div><div class=\"col-md-6\">\n";
		ofrhs << "<table class=\"table table-striped table-bordered\">\n";
		ofrhs << "<tr><td>Total Read Count</td><td>" << gTotal+bTotal;
		ofrhs << "</td></tr><tr><td>Passing Read Count</td><td>" << gTotal;
		ofrhs << "</td></tr><tr><td>Failing Read Count</td><td>" << bTotal;
		ofrhs << "</td></tr><tr><td>Failing Rate</td><td>" << setprecision(6) << ((double)bTotal/(gTotal+bTotal));
		ofrhs << "</td></tr><tr><td>Merged Read Pair Count</td><td>" << mTotal;
		ofrhs << "</td></tr><tr><td>T Removal Count (Total Pairs)</td><td>" << tTotal;
		ofrhs << "</td></tr></table></div>\n<script>var data = [ { value: " << bTotal << ", color:\"#F7464A\", highlight: \"#FF5A5E\", label: \"Failed Reads\" }, { value: " << gTotal << ", color: \"#46BFBD\", highlight: \"#5AD3D1\", label: \"Passed Reads\" }, { value: " << mTotal << ", color: \"#FDB45C\", highlight: \"#FFC870\", label: \"Merged Reads\" } ];\n";
		ofrhs << "var ctx = document.getElementById(\"counts\").getContext(\"2d\");\n";
		ofrhs << "var countChart = new Chart(ctx).Pie(data, {});</script></div>\n";
		ofrhs << "<br><h2>Adapter removal stats</h2>\n<div class=\"row\">\n";
		int tablecount = 0;
		for(int i=0; i < 28; i++) {
			stringstream temp;
			int doPrint = 0;
			temp << "<div class=\"col-md-3 col-sm-6\"><h3>Adapter " << i << ":</h3>\n<table class=\"table table-striped table-bordered\">\n";
			temp << "<tr><th>Length</th><th>Count</th></tr>\n";
			for(int j=0;j<70;j++) {
				if(adaps[i][j] > 0) {
					temp << "<tr><td>" << j << "</td><td>" << adaps[i][j] << "</td>\n";
					doPrint = 1;
				}
			}
			
			temp << "</table></div>\n";
			if (doPrint==1) { 
				if (tablecount>3) {
					tablecount = 0;
					ofrhs << "\n</div>\n<div class=\"row\">";
				}
				ofrhs << temp.str();
				tablecount++;
			}
		}
		ofrhs << "</div></div></body>\n</html>\n";
		ofrhs.close();
		
		// delete the individual thread files
		stringstream cmd;
		cmd << "rm -rf " << myJob->jobdir << "/ind";
		system(cmd.str().c_str());
		
		if (shm_unlink(JOBKEY)!=0) cerr << "Cleaner error: Unable to unlink job shared memory with: " << JOBKEY << endl;
		if (shm_unlink(TABKEY)!=0) cerr << "Cleaner error: Unable to unlink stat shared memory with: " << TABKEY << endl;
		if (error<1) cerr << "Exited adna-check with job completion.";
		else cerr << "Exited adna-check with job ERRORS.";
		cerr << " adna is NOT running and job info has been cleaned." << endl;
	}
	if (ch == 'q' || ch == 'Q') {
		cerr << "Exited adna-check without killing the adna job. adna is still running." << endl;
		
	} else if (ch == 'k' || ch == 'K') {
		for (int i=0; i<myJob->numProcs; i++) {
			if (myStat[i].PID > 0) {
				kill(myStat[i].PID, SIGTERM);
			}
		}
		if (shm_unlink(JOBKEY)!=0) cerr << "Cleaner error: Unable to unlink job shared memory with: " << JOBKEY << endl;
		if (shm_unlink(TABKEY)!=0) cerr << "Cleaner error: Unable to unlink stat shared memory with: " << TABKEY << endl;
		cerr << "Exited adna-check AND killed the adna job. adna is NOT running and job info has been cleaned." << endl;
	}
	
	return 0;
}








