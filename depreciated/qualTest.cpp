//#include "qualTest.h"
// Example program
#include <iostream>
#include <string>
#include <stdio.h>
#include "ReadPair.h"
#include "FastaSequences.h"

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
	//myRead.tStrip();
	//myRead.qualPass();
	//myRead.oCheck();
	string aFile = "adapters.fasta";
	if (myRead.oCheck())
	{
		printf("Overlap found, new single-read created\n");
	}
	else
	{
		printf("No significant overlap found, reads remain seperate\n");
	}
	FastaSequences adapters(aFile);

	//qualPass(s);
	//qualPass(t);
	//qualPass(v);
	return 0;
}
