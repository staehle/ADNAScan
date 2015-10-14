// Example program
#include <iostream>
#include <string>
using namespace std;


// returns 1 if string passed qual test: < 10 bases with < 15 qual score
// symbol ! = 0, symbol 0 = 15 <-Threshold
int qualPass(string tString)
{
    int qual = -1;
	int failCtr = 0;
	for(char& c : tString)
	{
	    qual = int(c)-33;
	    printf("%c : %i\n", c, qual);
		if(int(c)-33 < 15)
		{
			failCtr+=1;
			printf("%i\n", failCtr);
		}
	}
	printf("FAILURES : %i\n\n",failCtr);
	if (failCtr < 10)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//check for a leading T or 2 trailing T's and replace the corresponding quality scores to 2 (#)
//tStrip takes the nd and 4th lines of a read and returns an updated qual string
string tStrip(string seq, string qual)
{
    if(seq.at(0) == 'T')
    {
        printf("found a T\n");
        qual[0] = '#';
    }
    if((seq.at(seq.length() - 1) == 'T') && (seq.at(seq.length() - 2) == 'T'))
    {
        printf("found ending T's\n");
        qual[qual.length() - 1] = '#';
        qual[qual.length() - 2] = '#';
    }
    
    return qual;
}

//All the print statements in this file are currently for debuggung only, will not be in final software
int main()
{
	//sequence to test the T search and qual replacements
	string x = "TGCCGGAGTTTCGCGCCGCGGGCGCTTCGCAGGCGATAACCTGAAACTTGANGGAAGAGCNNCCGCAGTTGACCACCAGTACATTCAGATCGGAAGAGCACACGTCTGAACTCCAGTCACCAGTT";
	string s = "BC@BCGGGGGGGGGGGGGGGGGGGGGGGGGFGGGGGGGGGGC@FFGFGGGG#==E<FFGG##=<:EGGEGACEC</0<000800000008000<0;0000;000/8///69.6//9/...9////";
	s = tStrip(x,s);
	qualPass(s);
	string t = "BBCCCGGGGGGGGGGGGGGGGGGEGGGGGGGGGGCFGGG@DGBCGGGGGGG#;;CFFCEG##=::EGG@EGGGGGGGGGCGGGG..@GGGGCGGGGGGCG.8.:DEDG.9.CEGG<DDGD<CDG/";
	qualPass(t);
	string v = "CCCCCGGGGGGGDGGGGGGGGGGGGGGGGGGGGGEGGGGGGGGGGGGFGGG##=##==EF###===FGGGGFFGGGGGGGG0FDGGGGG.CG@GGBGGGGGGGGGGGGGGGGGGGGG/EB@G/8/";
	qualPass(v);
}
