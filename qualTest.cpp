//#include "qualTest.h"
// Example program
#include <iostream>
#include <string>
#include <stdio.h>
#include "ReadPair.h"
using namespace std;

class ReadPair;

//All the print statements in this file are currently for debuggung only, will not be in final software
int main()
{
	//sequence to test the T search and qual replacements
	string xR = "TGCCGGAGTTTCGCGCCGCGGGCGCTTCGCAGGCGATAACCTGAAACTTGANGGAAGAGCNNCCGCAGTTGACCACCAGTACATTCAGATCGGAAGAGCACACGTCTGAACTCCAGTCACCAGTC";
	string xQ = "BC@BCGGGGGGGGGGGGGGGGGGGGGGGGGFGGGGGGGGGGC@FFGFGGGG#==E<FFGG##=<:EGGEGACEC</0<000800000008000<0;0000;000/8///69.6//9/...9////";
	string yR = "TCCAGTCACCAGTCGATAAACGCCCTGCCAAACGGCGCAAGGATGCGCCGCACGAAAATCAAACGCGCAAGTGCTTGATTTTCGGAGCGCGCCGTGGGCGAAGCGGAGCTCTCGCGGATGCACTT";
	string yQ = "CCCCCGGGGGGGDGGGGGGGGGGGGGGGGGGGGGEGGGGGGGGGGGGFGGG##=##==EF###===FGGGGFFGGGGGGGG0FDGGGGG.CG@GGBGGGGGGGGGGGGGGGGGGGGG/EB@G/8/";
	ReadPair myRead(xR,xQ,yR,yQ);
	myRead.tStrip();
	myRead.qualPass();
	printf("\nfound %i as largest section of overlap over 10 bases\n",myRead.oCheck());

	//qualPass(s);
	//qualPass(t);
	//qualPass(v);
	return 0;
}
