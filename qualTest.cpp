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

int main()
{
	string s = "BC@BCGGGGGGGGGGGGGGGGGGGGGGGGGFGGGGGGGGGGC@FFGFGGGG#==E<FFGG##=<:EGGEGACEC</0<000800000008000<0;0000;000/8///69.6//9/...9////";
	qualPass(s);
    string t = "BBCCCGGGGGGGGGGGGGGGGGGEGGGGGGGGGGCFGGG@DGBCGGGGGGG#;;CFFCEG##=::EGG@EGGGGGGGGGCGGGG..@GGGGCGGGGGGCG.8.:DEDG.9.CEGG<DDGD<CDG/";
	qualPass(t);
	string v = "CCCCCGGGGGGGDGGGGGGGGGGGGGGGGGGGGGEGGGGGGGGGGGGFGGG##=##==EF###===FGGGGFFGGGGGGGG0FDGGGGG.CG@GGBGGGGGGGGGGGGGGGGGGGGG/EB@G/8/";
	qualPass(v);
}
