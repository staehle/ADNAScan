/*
 * ReadPair.cpp
 *
 * This class is meant to act as a central read object so that processing through the multiple processors
 * is simple. The class type requires the bases and quality score strings from both halves of the read
 * I figure most of the string operations can be centralized here, reducing text in the main class
 * where we are splitting the files, filling the hashmap, and then working with each pair.
 *
 */
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include "ReadPair.hpp"
using namespace std;

// Partial Constructor for the ReadPair class. Used on the first read.
ReadPair::ReadPair(string d1, string r1, string q1, int threadNum) {
	ID1 = d1;
	read1 = r1;
	qual1 = q1;
	fRead = "";
	fQual = "";
	lAdap = 0;
	lAdapLength = 0;
	rAdap = 0;
	rAdapLength = 0;
	merged = 0;
	tRem = 0;
	badRead = 0;
	badSide = 0;
	tNum = threadNum;
	vector<string> PrintLongestArray;
}

// Constructor for the ReadPair class. Takes the bases and quality of both sides of a read
ReadPair::ReadPair(string d1, string d2, string r1, string q1,string r2, string q2, int threadNum) {
	ID1 = d1;
	ID2 = d2;
	read1 = r1;
	read2 = r2;
	qual1 = q1;
	qual2 = q2;
	fRead = "";
	fQual = "";
	lAdap = 0;
	lAdapLength = 0;
	rAdap = 0;
	rAdapLength = 0;
	merged = 0;
	tRem = 0;
	badRead = 0;
	badSide = 0;
	tNum = threadNum;
	vector<string> PrintLongestArray;
	//Compile();
}

// Adds the data for read 2 and then compiles.
void ReadPair::addR2(string d2, string r2, string q2) {
	ID2 = d2;
	read2 = r2;
	qual2 = q2;
	//Compile(); //call after
}

// Quality test for the two parts of this read. Returns 0 if no passing reads, otherwise returns 1 and records which read failed to be used in output mehtod
int ReadPair::qualPass() {
	if(fRead.compare("") == 0) {
		//test first half
		int ret = 0;
		int qual = -1;
		int failCtr = 0;
		for(string::iterator c = qual1.begin(); c != qual1.end(); ++c) {
			qual = int(*c)-33;
			//if(int(*c)-33 < 15) {
			if(qual < 15) failCtr+=1;
		}
		if (failCtr < 10 && read1.length() > 10) ret += 1;


		//test second half
		qual = -1;
		failCtr = 0;
		for(string::iterator c = qual2.begin(); c != qual2.end(); ++c) {
			qual = int(*c)-33;
			if(qual < 15) failCtr+=1;
			
		}
		
		if (failCtr < 10 && read2.length() > 10) ret += 2;
		
		if(ret == 0) {
			badSide = 3;
			badRead=2;
			
		} else if(ret == 1) {
			fRead = read1;
			fQual = qual1;
			badSide = 2;
			badRead = 1;
			
		} else if (ret == 2) {
			fRead = read2;
			fQual = qual2;
			badSide = 1;
			badRead = 1;
		}
		if(ret != 0)
		{
			ret=1;
		}
		return ret;
		
	} else { // if reads were combined
		int qual = -1;
		int failCtr = 0;
		for(string::iterator c = fQual.begin(); c != fQual.end(); ++c) {
			qual = int(*c)-33;
			if(qual < 15) failCtr+=1;
		}
		
		if (failCtr < 10 && fRead.length() > 10) return 1;
		else {
			badRead = 2;
			return 0;
		}
	}
}

// This method strips the leading T and trailing 2 T's by changing their quality scores to #(2)
void ReadPair::tStrip() {
    if(read1.at(0) == 'T') {
        qual1[0] = '#';
        tRem = 1;
    }
    if((read1.at(read1.length() - 1) == 'T') && (read1.at(read1.length() - 2) == 'T')) {
        qual1[qual1.length() - 1] = '#';
        qual1[qual1.length() - 2] = '#';
        tRem = 1;
    }
    if(read2.at(0) == 'T') {
        qual2[0] = '#';
        tRem = 1;
    }
    if((read2.at(read2.length() - 1) == 'T') && (read2.at(read2.length() - 2) == 'T')) {
        qual2[qual2.length() - 1] = '#';
        qual2[qual2.length() - 2] = '#';
        tRem = 1;
    }
}



// Look for 11 overlapping nucleotides. Returns max overlap over 10
int ReadPair::oCheck() 
{
	int maxScore = 0;
	int bestI1 = 0;
	int bestI2 = 0;
	int i1 = 0; //looks at read1
	int i2 = 0; //looks at read2
	//longestOLap will keep the largest number of overlapping base matches found. Since there could
	//POTENTIALLY be a section of overlap past the first instance of a match, we will test the whole read
	//just to be certain we found the full overlap

	//int longestOLap = 0;

	//i1Temp will act as temporary i1 to iterate up the string, searching for continual matches (overlap)
	//oCtr will be an overlap counter, counting up for each continual match.
	int i2Temp = 0;//, oCtr;
	int score = 0;
	//while (i1 < (int)read1.length()) {
	//-15 to accept only matches of length 15 or greater
	while(i2 < (int)read2.length() - 15)
	{
		score = 0;
		i2Temp = i2;
		i1 = 0;
		while (i2Temp < (int)read2.length())//  && read1[i1] == read2[i2Temp]) 
		{
			//++oCtr;
			if(read1[i1] != read2[i2Temp])
			{
				score -= 2;
				//if(missCtr > (int)((int)read2.length() - i2) / 3)
				//{
				//	break;
				//}
			}
			else
			{
				score += 1;
			}
			if (i2Temp == (int)read2.length() -1 )//&& missCtr < ((int)read2.length()-i2)/8) 
			{
				//i2 += 1;
				if(score > maxScore)
				{
					maxScore = score;
					bestI1 = i1;
					bestI2 = i2;
				}
			}
			++i2Temp;
			++i1;
		}
		++i2;	
	}
	if(maxScore > 0)
	{
		fRead = read1.substr(0, bestI1 + 1); //+ read2.substr(i2 + 1, (int)read2.length() - i2 - 1);
		fQual = qual1.substr(0, bestI1 + 1); //+ qual2.substr(i2 + 1, (int)read2.length() - i2 - 1);
		merged = 1;
		if(bestI1 != (int)read1.length()-1)
		{
			aPrint(read1.substr(bestI1 + 1,(int)read1.length() - bestI1), read2.substr(0, bestI2));
		}
	}
	//	++i1;
	//}
	return 1;
}

int ReadPair::findAdapSlow()
{
	//"AGATCGGAAGAG",
	string adapters[] =
	{
	    "AATGATACGGCGACCACCGAGATCTACACTCTTTCCCTACACGACGCTCTTCCGATCT", // PCR_Primer1
	    "CAAGCAGAAGACGGCATACGAGATCGGTCTCGGCATTCCTGCTGAACCGCTCTTCCGATCT", // PCR_Primer2
	    "CGTCGTGTAGGGAAAGAGTGTAGATCTCGGTGGTCGCCGTATCATT", // PCR_Primer1_rc
	    "CAAGCAGAAGACGGCATACGAGATCGGTCTCGGCATTCCTGCTGAACCGCTCTTCCGATCT", // PCR_Primer2
	    "AGATCGGAAGAGCGGTTCAGCAGGAATGCCGAGACCGATCTCGTATGCCGTCTTCTGCTTG", // PCR_Primer2_rc
	    "TTTTTTTTTTAATGATACGGCGACCACCGAGATCTACAC", // FlowCell1
	    "TTTTTTTTTTCAAGCAGAAGACGGCATACGA", // FlowCell2
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACATCACGATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index1
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACCGATGTATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index2
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACTTAGGCATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index3
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACTGACCAATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index4
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACACAGTGATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index5
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGCCAATATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index6
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACCAGATCATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index7
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACACTTGAATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index8
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGATCAGATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index9
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACTAGCTTATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index10
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGGCTACATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index11
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACCTTGTAATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index12
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACAGTCAACAATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index13
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACAGTTCCGTATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index14
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACATGTCAGAATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index15
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACCCGTCCCGATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index16
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGTCCGCACATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index18
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGTGAAACGATCTCGTATGCCGTCTTCTGCTTG", // TruSeq_Adapter_Index19
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGTGGCCTTATCTCGTATGCCGTCTTCTGCTTG" // TruSeq_Adapter_Index20
	};
	//string universalAdapter = "AGATCGGAAGAG";
	/*
	Illumina Small RNA Adapter	ATGGAATTCTCG
	Nextera Transpose Sequence	CTGTCTCTTATA
	*/

	//float ratio = 0;
	//float bestRatio = 0;
	int bestAdap = 0;
	int iFinal = 0;
	int aFinal = 0;
	int score = 0;
	int bestScore = 0;
	//string adapter;
	
	for(int x = 0; x < (int)(sizeof(adapters)/sizeof(*adapters)); x++){
		//int iIndex = 0;
		//int aIndex = 0;	
		int i= 0; //looks at reads
		int a = 0; //looks at adapter
		string adapter = adapters[x];

		//will act to iterate up the string, searching for continual matches
		int iTemp = 0;
		while(i < (int)read1.length() - 1)
		{
			iTemp = i;
			a = 0;
			score = 0;
			while (a < (int)adapter.length() && iTemp < (int)read1.length()) // && read1[i] == universalAdapter[a]) 
			{
				
				if(read1[iTemp] != adapter[a])
				{
					score -= 7;
				}
				else
				{
					score += 5;
				}
				
				if (iTemp == (int)read1.length() -1 || a == (int)adapter.length() - 1)//&& missCtr < ((int)read2.length()-i2)/8) 
				{
					//float ratio = score / (a + 1);
					if(score >  bestScore)
					{
						
						bestScore = score;
						bestAdap = x+1;
						
						iFinal = i;
						aFinal = a;
						
						//bestScore = score;
						//iIndex = i;
						//aIndex = a;
					}
					//lAdap = x + 1;
					//read1 = read1.substr(0, i); //+ read2.substr(i2 + 1, (int)read2.length() - i2 - 1);
					//qual1 = qual1.substr(0, i); //+ qual2.substr(i2 + 1, (int)read2.length() - i2 - 1);
					//lAdapLength = a + 1;
					//aPrint(read1.substr(i, (int)read1.length() - i), read2.substr(0, bestI2));
				}
				++iTemp;
				++a;
			}
			//if (lAdap != 0)
			//{
			//	break;
			//}
			++i;	
		}

	}
	
	if(bestScore > 50)
	{
		read1 = read1.substr(0, iFinal + 1); //+ read2.substr(i2 + 1, (int)read2.length() - i2 - 1);
		qual1 = qual1.substr(0, iFinal + 1); //+ qual2.substr(i2 + 1, (int)read2.length() - i2 - 1);
		lAdapLength = aFinal + 1;
		lAdap = bestAdap;
	}

	//ratio = 0;
	//bestRatio = 0;
	bestAdap = 0;
	iFinal = 0;
	aFinal = 0;
	score = 0;
	bestScore = 0;
	
	for(int x = 0; x < (int)(sizeof(adapters)/sizeof(*adapters)); x++){

		int i = read2.length() - 6;
		int a = 0;
		//int iIndex = 0;
		//int aIndex = 0;
		int iTemp = i;
		int score = 0;
		string adapter = adapters[x];
		
		while(i >= 0)
		{
			iTemp = i;
			a = adapter.length() - 1;
			score = 0;
			while (iTemp >= 0 && a >= 0) 
			{
				if(read2[iTemp] != adapter[a])
				{
					score -= 7;
				}
				else
				{
					score += 5;
				}
				if (iTemp == 0 || a == 0)//&& missCtr < ((int)read2.length()-i2)/8) 
				{
					/*
					if (iTemp == 0)
					{
						float ratio = score / (i+1);
					}
					else
					{
						float ratio = score / (adapter.length());
					}
					*/
					if(score > bestScore)
					{
						bestAdap = x+1;
						bestScore = score;
						iFinal = i;
						aFinal = a;
						//bestScore = score;
						//iIndex = i;
						//aIndex = a;
					}
					//read1 = read1.substr(0, i); //+ read2.substr(i2 + 1, (int)read2.length() - i2 - 1);
					//qual1 = qual1.substr(0, i); //+ qual2.substr(i2 + 1, (int)read2.length() - i2 - 1);
					//lAdapLength = a + 1;
					//aPrint(read1.substr(i, (int)read1.length() - i), read2.substr(0, bestI2));
				}

				--iTemp;
				--a;
			}
			--i;	
		}
	}
	
	if(bestScore > 50)
	{
		read2 = read2.substr(iFinal + 1, read2.length() - iFinal - 1); //+ read2.substr(i2 + 1, (int)read2.length() - i2 - 1);
		qual2 = qual2.substr(iFinal + 1, qual2.length() - iFinal - 1);
		rAdapLength = adapters[bestAdap - 1].length() - aFinal;
		rAdap = bestAdap;
		
		/*
			stringstream ofr1fn;
			ofr1fn << "./results/curjob/ind/read1Fail_p" << tNum << ".fastq";
			ofstream ofr1fs;
			ofr1fs.open(ofr1fn.str(), ios::app);
			ofr1fs << ID1 << "\n" << read1 << "\n+\n" << qual1 << "\n";
			ofr1fs.close();
		*/
	}
	
	return 1;
}


int ReadPair::findPrimer()
{
		
	string primer = "CGTCGTGTAGGGAAAGAGTGTAGATCTCGGTGGTCGCCGTATCATT";
	
	//Illumina Small RNA Adapter	ATGGAATTCTCG
	//Nextera Transpose Sequence	CTGTCTCTTATA
	

	//float bestRatio = 0;
	//int bestAdap = 0;
	int iFinal = 0;
	int aFinal = 0;
	int bestScore = 0;
	int score = 0;
	//string adapter;
	
	int i= 0; //looks at reads
	int a = 0; //looks at adapter

	//will act to iterate up the string, searching for continual matches
	int iTemp = 0;
	while(i < (int)read2.length() - 1)
	{
		iTemp = i;
		a = 0;
		score = 0;
		while (a < (int)primer.length() && iTemp < (int)read2.length()) // && read1[i] == universalAdapter[a]) 
		{
			if(read2[iTemp] != primer[a])
			{
				score -= 7;
			}
			else
			{
				score += 5;
			}
			if (iTemp == (int)read2.length() -1 || a == (int)primer.length() - 1)//&& missCtr < ((int)read2.length()-i2)/8) 
			{
				//float ratio = score / a + 1;
				if(score >  bestScore)
				{
					bestScore = score;
					iFinal = i;
					aFinal = a;
					
					//bestScore = score;
					//iIndex = i;
					//aIndex = a;
				}
				//lAdap = x + 1;
				//read1 = read1.substr(0, i); //+ read2.substr(i2 + 1, (int)read2.length() - i2 - 1);
				//qual1 = qual1.substr(0, i); //+ qual2.substr(i2 + 1, (int)read2.length() - i2 - 1);
				//lAdapLength = a + 1;
				//aPrint(read1.substr(i, (int)read1.length() - i), read2.substr(0, bestI2));
			}
			++iTemp;
			++a;
		}
		//if (lAdap != 0)
		//{
		//	break;
		//}
		++i;	
	}

	if(bestScore > 50)
	{
		read2 = read2.substr(0, iFinal + 1); //+ read2.substr(i2 + 1, (int)read2.length() - i2 - 1);
		qual2 = qual2.substr(0, iFinal + 1); //+ qual2.substr(i2 + 1, (int)read2.length() - i2 - 1);
		rAdapLength = aFinal + 1;
		rAdap = 4;
	}

	
	
	return 1;
	
}


int ReadPair::findUAdap() //Used to find Universal adapter in read2
{
	
	string universalAdapter = "AGATCGGAAGAG";
	
	//Illumina Small RNA Adapter	ATGGAATTCTCG
	//Nextera Transpose Sequence	CTGTCTCTTATA
	
	//float ratio = 0;
	//float bestRatio = 0;
	int score = 0;
	int bestScore = 0;
	int iFinal = 0;
	int aFinal = 0;	
	int i = 0;
	int a = 0;
	int iTemp = 0;

	//int iIndex = 0;
	//int aIndex = 0;	


	//will act to iterate up the string, searching for continual matches

	while(i < (int)read1.length() - 1)
	{
		iTemp = i;
		a = 0;
		score = 0;
		while (a < (int)universalAdapter.length() && iTemp < (int)read1.length()) // && read1[i] == universalAdapter[a]) 
		{
			if(read1[iTemp] != universalAdapter[a])
			{
				score -= 7;
			}
			else
			{
				score += 5;
			}
			if (iTemp == (int)read1.length() -1 || a == (int)universalAdapter.length() - 1)//&& missCtr < ((int)read2.length()-i2)/8) 
			{
				//float ratio = score / (a + 1);
				if(score >  bestScore)
				{
					
					bestScore = score;
					iFinal = i;
					aFinal = a;
					
					//bestScore = score;
					//iIndex = i;
					//aIndex = a;
				}
				//lAdap = x + 1;
				//read1 = read1.substr(0, i); //+ read2.substr(i2 + 1, (int)read2.length() - i2 - 1);
				//qual1 = qual1.substr(0, i); //+ qual2.substr(i2 + 1, (int)read2.length() - i2 - 1);
				//lAdapLength = a + 1;
				//aPrint(read1.substr(i, (int)read1.length() - i), read2.substr(0, bestI2));
			}
			++iTemp;
			++a;
		}
		//if (lAdap != 0)
		//{
		//	break;
		//}
		++i;	
	}

	
	if(bestScore > 30)
	{
		read1 = read1.substr(0, iFinal + 1); //+ read2.substr(i2 + 1, (int)read2.length() - i2 - 1);
		qual1 = qual1.substr(0, iFinal + 1); //+ qual2.substr(i2 + 1, (int)read2.length() - i2 - 1);
		lAdapLength = aFinal + 1;
		lAdap = 27;
	}

	iTemp = 0;
	i = (int)read2.length() - 1;
	a = 0;
	iFinal = 0;
	aFinal = 0;
	score = 0;
	bestScore = 0;
	
	while(i >= 5)
	{
		iTemp = i;
		a = (int)universalAdapter.length() - 1;
		score = 0;
		while (iTemp >= 0 && a >= 0) 
		{
			if(read2[iTemp] != universalAdapter[a])
			{
				score -= 7;
			}
			else
			{
				score += 5;
			}
			if (iTemp == 0 || a == 0)//&& missCtr < ((int)read2.length()-i2)/8) 
			{
				/*
				if (iTemp == 0)
				{
					float ratio = score / (i+1);
				}
				else
				{
					float ratio = score / (universalAdapter.length());
				}
				*/
				if(score >= bestScore)
				{
					bestScore = score;
					iFinal = i;
					aFinal = a;
				}
				//read1 = read1.substr(0, i); //+ read2.substr(i2 + 1, (int)read2.length() - i2 - 1);
				
				//qual1 = qual1.substr(0, i); //+ qual2.substr(i2 + 1, (int)read2.length() - i2 - 1);
				//lAdapLength = a + 1;
				//aPrint(read1.substr(i, (int)read1.length() - i), read2.substr(0, bestI2));
			}
			--iTemp;
			--a;
		}
		--i;	
	}

	
	if(bestScore > 30)
	{
		//ERRORS ON THESE 2 LINES
		read2 = read2.substr(iFinal + 1, read2.length() - iFinal - 1); //+ read2.substr(i2 + 1, (int)read2.length() - i2 - 1);
		qual2 = qual2.substr(iFinal + 1, qual2.length() - iFinal - 1);
		//
		rAdapLength = universalAdapter.length() - aFinal;
		rAdap = 27;
	}
	
	return 1;
}


int ReadPair::findALoop()
{
	//int iTemp = 0;
	int i = 0;
	//int flag = 0;
	//int a = 0;
	int iFinal = 0;
	//int aFinal = 0;
	int score = 0;
	int bestScore = 0;
	
	while(i < (int)read2.length() - 1)
	{
		//iTemp = i;
		//a = 0;
		//score = 0;
		//while (iTemp < (int)read1.length()) // && read1[i] == universalAdapter[a]) 
		//{
			if(read2.at(i) != 'A')
			{
				score -= 7;
			}
			else
			{
				score += 5;
			}
			//if (i == (int)read2.length() -1)//&& missCtr < ((int)read2.length()-i2)/8) 
			//{
				//float ratio = score / (a + 1);
				if(score >  bestScore)
				{
					
					bestScore = score;
					iFinal = i;
					//aFinal = a;
					
					//bestScore = score;
					//iIndex = i;
					//aIndex = a;
				}
				//lAdap = x + 1;
				//read1 = read1.substr(0, i); //+ read2.substr(i2 + 1, (int)read2.length() - i2 - 1);
				//qual1 = qual1.substr(0, i); //+ qual2.substr(i2 + 1, (int)read2.length() - i2 - 1);
				//lAdapLength = a + 1;
				//aPrint(read1.substr(i, (int)read1.length() - i), read2.substr(0, bestI2));
			//}
			//++iTemp;
			//++a;
		//}
		//if (lAdap != 0)
		//{
		//	break;
		//}
		++i;	
	}


	
	if(bestScore >= 20)
	{
		//ERRORS ON THESE 2 LINES
		read2 = read2.substr(iFinal + 1, read2.length() - iFinal - 1); //+ read2.substr(i2 + 1, (int)read2.length() - i2 - 1);
		qual2 = qual2.substr(iFinal + 1, qual2.length() - iFinal - 1);
		//
		rAdapLength = read2.length() - iFinal - 1;
		//rAdap = 27;
	}
	
	return 1;
	
}


void ReadPair::passOutFile(char* jobdir) {
	if(fRead.compare("") == 0) {
		
		stringstream ofr1pn; 
		ofr1pn << jobdir << "/ind/read1Pass_p" << tNum << ".fastq";
		ofstream ofr1ps;
		ofr1ps.open(ofr1pn.str(), ios::app);
		ofr1ps << ID1 << "\n" << read1 << "\n+\n" << qual1 << "\n";
		ofr1ps.close();

		stringstream ofr2pn;
		ofr2pn << jobdir << "/ind/read2Pass_p" << tNum << ".fastq";
		ofstream ofr2ps;
		ofr2ps.open(ofr2pn.str(), ios::app);
		ofr2ps << ID2 << "\n" << read2 << "\n+\n" << qual2 << "\n";
		ofr2ps.close();
		
	} else {
		stringstream ofsrpn;
		ofsrpn << jobdir << "/ind/singleReadPass_p" << tNum << ".fastq";
		ofstream ofsrps;
		ofsrps.open(ofsrpn.str(), ios::app);
		ofsrps << ID1 << "\n" << fRead << "\n+\n" << fQual << "\n";
		ofsrps.close();
		
		if (badSide == 2) {
			stringstream ofr2fn;
			ofr2fn << jobdir << "/ind/read2Fail_p" << tNum << ".fastq";
			ofstream ofr2fs;
			ofr2fs.open(ofr2fn.str(), ios::app);
			ofr2fs << ID2 << "\n" << read2 << "\n+\n" << qual2 << "\n";
			ofr2fs.close();
			
		} else if (badSide == 1) {
			stringstream ofr1fn;
			ofr1fn << jobdir << "/ind/read1Fail_p" << tNum << ".fastq";
			ofstream ofr1fs;
			ofr1fs.open(ofr1fn.str(), ios::app);
			ofr1fs << ID1 << "\n" << read1 << "\n+\n" << qual1 << "\n";
			ofr1fs.close();
		}
	}
}

void ReadPair::failOutFile(char* jobdir) {
	if(fRead.compare("") == 0) {
		stringstream ofr1fn;
		ofr1fn << jobdir << "/ind/read1Fail_p" << tNum << ".fastq";
		ofstream ofr1fs;
		ofr1fs.open(ofr1fn.str(), ios::app);
		ofr1fs << ID1 << "\n" << read1 << "\n+\n" << qual1 << "\n";
		ofr1fs.close();

		stringstream ofr2fn;
		ofr2fn << jobdir << "/ind/read2Fail_p" << tNum << ".fastq";
		ofstream ofr2fs;
		ofr2fs.open(ofr2fn.str(), ios::app);
		ofr2fs << ID2 << "\n" << read2 << "\n+\n" << qual2 << "\n";
		ofr2fs.close();

	} else {
		stringstream ofsrfn;
		ofsrfn << jobdir << "/ind/singleReadFail_p" << tNum << ".fastq";
		ofstream ofsrfs;
		ofsrfs.open(ofsrfn.str(), ios::app);
		ofsrfs << ID1 << "\n" << fRead << "\n+\n" << fQual << "\n";
		ofsrfs.close();
	}
}

int ReadPair::getLeftA() {
	return lAdap;
}
int ReadPair::getLeftAL() {
	return lAdapLength;
}
int ReadPair::getRightA() {
	return rAdap;
}
int ReadPair::getRightAL() {
	return rAdapLength;
}
int ReadPair::getBad() {
	return badRead;
}
int ReadPair::isMerged() {
	return merged;
}
int ReadPair::tStripped() {
	return tRem;
}

void ReadPair::Compile(char* jobdir) {
	tStrip();
	//aRemove();
	//oCheck();
	//findUAdapQuick();
	findAdapSlow();
	if((int)read2.length() > 9) findPrimer();
	if((int)read2.length() > 9) findUAdap();
	if((int)read2.length() > 9 && ((read2.at(0) == 'A') + (read2.at(1) == 'A') + (read2.at(2) == 'A') + (read2.at(3) == 'A') >= 3)) findALoop();
	int p = qualPass();
	if(p == 1) passOutFile(jobdir);
	else failOutFile(jobdir);
}



