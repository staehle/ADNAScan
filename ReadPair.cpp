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
		if (failCtr < 10) ret += 1;

		//test second half
		qual = -1;
		failCtr = 0;
		for(string::iterator c = qual2.begin(); c != qual2.end(); ++c) {
			qual = int(*c)-33;
			if(qual < 15) failCtr+=1;
		}
		
		if (failCtr < 10) ret += 2;
		
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
		
		if (failCtr < 10) return 1;
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

int ReadPair::lUniversalTest()
{
	string uAdap = "AATGATACGGCGACCACCGAGATCTACACTCTTTCCCTACACGACGCTCTTCCGATCT";
	int iR = 0;
	int iA = 0;
	int iATemp = 0;
	int missCtr = 0;
	while(iA < (int)uAdap.length() - 5)
	{
		missCtr = 0;
		iATemp = iA;
		iR = 0;
		while (iATemp < (int)uAdap.length())//  && read1[i1] == read2[i2Temp]) 
		{
			//++oCtr;
			if(read2[iR] != uAdap[iATemp])
			{
				++missCtr;
				if(missCtr > (int)((int)uAdap.length() - iA) / 5)
				{
					break;
				}
			}
			if(iATemp == (int)uAdap.length()-1)
			{
				read2 = read2.substr((int)uAdap.length() - iA, (int)read2.length() - (int)uAdap.length() - iA);
				qual2 = qual2.substr((int)uAdap.length() - iA, (int)qual2.length() - (int)uAdap.length() - iA);
				merged += 1;
				return 1;
			}
			++iATemp;
			++iR;
		}
		++iA;
	}
}

int ReadPair::rUniversalTest()
{
	string uAdap = "AATGATACGGCGACCACCGAGATCTACACTCTTTCCCTACACGACGCTCTTCCGATCT";
	int iR = (int)read1.length() - (int)uAdap.length();
	int iRTemp = iR;
	int iA = 0;
	int missCtr = 0;
	while(iR < (int)read1.length() - 5)
	{
		missCtr = 0;
		iRTemp = iR;
		iA = 0;
		while (iRTemp < (int)read1.length())//  && read1[i1] == read2[i2Temp]) 
		{
			//++oCtr;
			if(read1[iRTemp] != uAdap[iA])
			{
				++missCtr;
				if(missCtr > (int)((int)read1.length() - iR) / 5)
				{
					break;
				}
			}
			if(iRTemp == (int)read1.length()-1)
			{
				read1 = read1.substr(0, (int)read1.length() - iR);
				qual1 = qual1.substr(0, (int)qual1.length() - iR);
				merged += 1;
				return 1;
			}
			++iRTemp;
			++iA;
		}
		++iR;
	}
}

int ReadPair::aPrint(string a1, string a2)
{
	string adapters[] =
	{
	    "AATGATACGGCGACCACCGAGATCTACACTCTTTCCCTACACGACGCTCTTCCGATCT",
	    "CAAGCAGAAGACGGCATACGAGATCGGTCTCGGCATTCCTGCTGAACCGCTCTTCCGATCT",
	    "AATGATACGGCGACCACCGAGATCTACACTCTTTCCCTACACGACGCTCTTCCGATCT",
	    "AGATCGGAAGAGCGTCGTGTAGGGAAAGAGTGTAGATCTCGGTGGTCGCCGTATCATT",
	    "CAAGCAGAAGACGGCATACGAGATCGGTCTCGGCATTCCTGCTGAACCGCTCTTCCGATCT",
	    "AGATCGGAAGAGCGGTTCAGCAGGAATGCCGAGACCGATCTCGTATGCCGTCTTCTGCTTG",
	    "TTTTTTTTTTAATGATACGGCGACCACCGAGATCTACAC",
	    "TTTTTTTTTTCAAGCAGAAGACGGCATACGA",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACATCACGATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACCGATGTATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACTTAGGCATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACTGACCAATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACACAGTGATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGCCAATATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACCAGATCATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACACTTGAATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGATCAGATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACTAGCTTATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGGCTACATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACCTTGTAATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACAGTCAACAATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACAGTTCCGTATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACATGTCAGAATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACCCGTCCCGATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGTCCGCACATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGTGAAACGATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGTGGCCTTATCTCGTATGCCGTCTTCTGCTTG"
	};
	int i1 = 0;
	int i2 = 0;
	int match = 0;
	int maxMatch = 0;
	//read1 adapter find
	for(int i = 0; i < 27 ; ++i) //(sizeof(adapters)/sizeof(adapters[0])); ++i)
	{
		i1 = 0;
		i2 = 0;
		match = 0;
		string adap  = adapters[i];
		if((int)a1.length() <= (int)adap.length())
		{
			while(i1 < (int)a1.length())
			{
				if(a1[i1] == adap[i2])
				{
					++match;
				}
				++i1;
				++i2;
			}
			if(match > maxMatch)
			{
				maxMatch = match;
				lAdap=i;
			}
		}
	}
	lAdapLength = (int)a1.length();
	//read 2 adapter find
	maxMatch = 0;
	for(int i = 0; i < 27 ; ++i) //(sizeof(adapters)/sizeof(adapters[0])); ++i)
	{
		i1 = 0;
		i2 = (int)adapters[i].length() - (int)a2.length();
		match = 0;
		string adap  = adapters[i];
		if((int)a1.length() <= (int)adap.length())
		{
			while(i1 < (int)a2.length())
			{
				if(a2[i1] == adap[i2])
				{
					++match;
				}
				++i1;
				++i2;
			}
			if(match > maxMatch)
			{
				maxMatch = match;
				rAdap=i;
			}
		}

	}
	rAdapLength = (int)a2.length();
return 0;
}

int ReadPair::aRemove() {
    const int MinLengthAdapters = 12; // Min Length for accepted Adapters
    const int NumberOfAdapters = 27; // Amount of Adapters we are using // I use this instead of length, for more flexibility

    // These are for Return
    // Returns 1 if Only removed from First read
    // 2 if Only removed from Second read
    // 3 if removed from both
    // 0 if none removed
    int AdapterRemoved = 0;
    bool AdapterFound = false;
    bool AdapterFound2 = false;

    
    stringstream testr1;
    testr1 << "./results/testr1.txt";
    ofstream test1;
    stringstream testr2;
    testr2 << "./results/testr2.txt";
    ofstream test2;
    

	string Adapters[] =
	{
		"",
	    "AATGATACGGCGACCACCGAGATCTACACTCTTTCCCTACACGACGCTCTTCCGATCT",
	    "CAAGCAGAAGACGGCATACGAGATCGGTCTCGGCATTCCTGCTGAACCGCTCTTCCGATCT",
	    "AATGATACGGCGACCACCGAGATCTACACTCTTTCCCTACACGACGCTCTTCCGATCT",
	    "AGATCGGAAGAGCGTCGTGTAGGGAAAGAGTGTAGATCTCGGTGGTCGCCGTATCATT",
	    "CAAGCAGAAGACGGCATACGAGATCGGTCTCGGCATTCCTGCTGAACCGCTCTTCCGATCT",
	    "AGATCGGAAGAGCGGTTCAGCAGGAATGCCGAGACCGATCTCGTATGCCGTCTTCTGCTTG",
	    "TTTTTTTTTTAATGATACGGCGACCACCGAGATCTACAC",
	    "TTTTTTTTTTCAAGCAGAAGACGGCATACGA",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACATCACGATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACCGATGTATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACTTAGGCATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACTGACCAATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACACAGTGATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGCCAATATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACCAGATCATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACACTTGAATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGATCAGATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACTAGCTTATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGGCTACATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACCTTGTAATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACAGTCAACAATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACAGTTCCGTATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACATGTCAGAATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACCCGTCCCGATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGTCCGCACATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGTGAAACGATCTCGTATGCCGTCTTCTGCTTG",
	    "GATCGGAAGAGCACACGTCTGAACTCCAGTCACGTGGCCTTATCTCGTATGCCGTCTTCTGCTTG"
	};


        string str1 = read1;
		// (this is the String from Sequence)

		int counter = 0; // Counter for Adapters
		int RemoveAdapter = 0; // Longest Adapter that is found in sequence (The number in my array) ETC Adapter Number 4
		AdapterFound = false; // If Adapter Found

		while (counter < NumberOfAdapters){ // While loop to get every Adapter.
			string AdapterSearch = ""; // Assigning string AdapterSearch to the current Adapter we are searching through
			AdapterSearch.assign(Adapters[counter]);
			int endIndex = AdapterSearch.length(); // The length of the adapter
			string FoundString = ""; // Temp value for current string

			// From 0 -> Str.length - AdapterLength. Searching from 0 to end
			for (int startIndex = 0; startIndex <= (str1.length() - endIndex); startIndex = startIndex + 1) {
				// Creating Temp Strings for match - Printing
				
				
				int whilecounter = endIndex;
				int positivewhilecounter = 0;
				// While endindex(Length of Adapter) is bigger than 12(Min Length of Adapter) endIndex - 1
				while (whilecounter >= MinLengthAdapters){
					
					//This is to match the first part of string, with last part of adapter
					
					string CurrSeq = str1.substr(startIndex, (whilecounter));
					string CurrAdap = AdapterSearch.substr(positivewhilecounter, (endIndex));
					
				//Adapter Found
				
					size_t found = CurrSeq.find(CurrAdap);  // Search String for Adapter
					if (found != string::npos) {		// If Found
						FoundString.assign(CurrSeq);	//Setting temp string to found adapter string

					//If First Adapter Found
						if (AdapterFound == false) {
							AdapterFound = true;
							RemoveAdapter = startIndex;
						
		
							test1.open(testr1.str(), ios::app); //For Testing Prints
							test1 << "Full Sequence Read 1 : \n"<< str1 << "\n" << "Current Sequence : Matching Adapter \n" << CurrSeq <<"\n"<< CurrAdap <<  "\nNew Full Sequence Read 1: \n" << (str1.substr(0, RemoveAdapter)) <<"\n";
							test1.close();
						
							break;
						} else { // If another adapter is found
							if(RemoveAdapter < startIndex) { // If longer than previous (Currently < )
								RemoveAdapter = startIndex;	    //assigning adapter to remove
							
							
								test1.open(testr1.str(), ios::app); //For Testing Prints
								test1 << "Found Bigger : Full Sequence Read 1 : \n"<< str1 << "\n" << "Current Sequence : Matching Adapter \n" << CurrSeq <<"\n"<< CurrAdap <<  "\nNew Full Sequence Read 1: \n" << (str1.substr(0, RemoveAdapter)) <<"\n";
								test1.close();
								break;
							} else break;
						}
					}
					positivewhilecounter ++;
					whilecounter = whilecounter - 1;
				}
			}
			counter ++;
		}


		//Second
		
		
		
		string str2 = read2;
		// (this is the String from Sequence)

		int counter2 = 0; // Counter for Adapters
		int RemoveAdapter2 = 0; // Longest Adapter that is found in sequence (The number in my array) ETC Adapter Number 4
		AdapterFound2 = false; // If Adapter Found

		while (counter2 < NumberOfAdapters){ // While loop to get every Adapter.
			string AdapterSearch = ""; // Assigning string AdapterSearch to the current Adapter we are searching through
			AdapterSearch.assign(Adapters[counter]);
			int endIndex = AdapterSearch.length(); // The length of the adapter
			string FoundString = ""; // Temp value for current string

			// From length - length of adapter ->   12(minLength). Search from back to front
			for (int startIndex = (str2.length() - endIndex); startIndex <= 0; startIndex = startIndex - 1) {
				// Creating Temp Strings for match - Printing
				
				int whilecounter = endIndex;
				int positivewhilecounter = 0;
				
				
				while (whilecounter >= MinLengthAdapters){
					
					//This is to match the last part of string, with first part of adapter
					
					string CurrSeq = str2.substr((startIndex + positivewhilecounter), (whilecounter));
					string CurrAdap = AdapterSearch.substr(0, (whilecounter)); // Looking at FIRST part of the sequence

				//Adapter Found
					size_t found = CurrSeq.find(CurrAdap); // Search String for Adapter
					if (found != string::npos) {		// If Found
						FoundString.assign(CurrSeq);	//Setting temp string to found adapter string

					//If First Adapter Found
						if (AdapterFound2 == false) {
							AdapterFound2 = true;
							RemoveAdapter2 = startIndex;
						
		
							test2.open(testr2.str(), ios::app); //For Testing Prints
							test2 << "Full Sequence Read 2 : \n"<< str2 << "\n" << "Current Sequence : Matching Adapter \n" << CurrSeq <<"\n"<< CurrAdap <<  "\nNew Full Sequence Read 2: \n" << (str2.substr(RemoveAdapter2, str2.length())) <<"\n";
							test2.close();
						
							break;
						} else { // If another adapter is found
							if(RemoveAdapter2 > startIndex) { // If longer than previous (Currently >= !!)
								RemoveAdapter2 = startIndex;	    //assigning adapter to remove
							
							
								test2.open(testr2.str(), ios::app); //For Testing Prints
								test2 << "Found Bigger : Full Sequence Read 2 : \n"<< str2 << "\n" << "Current Sequence : Matching Adapter \n" << CurrSeq <<"\n"<< CurrAdap <<  "\nNew Full Sequence Read 2: \n" << (str2.substr(RemoveAdapter2, str2.length())) <<"\n";
								test2.close();
								break;
							} else break;
						}
					}
					positivewhilecounter ++;
					whilecounter = whilecounter - 1;
				}
			}
			counter2 ++;
		}

		//Use these if replacing the actual Reads


		//read1 = str1.substr(0, RemoveAdapter);
		test1.open(testr1.str(), ios::app); //For Testing Prints
		test1 << "\n\n**********************************\nOld Read 1: "<< str1 << "\n**********************************\nNew Read 1: " << (str1.substr(0, RemoveAdapter)) << "\n**********************************\n" <<"\n";
		test1.close();
		//read2 = str2.substr(RemoveAdapter2, str2.length());
		test2.open(testr2.str(), ios::app); //For Testing Prints
		test2 << "\n\n**********************************\nOld Read 2: "<< str2 << "\n**********************************\nNew Read 2: " << (str2.substr(RemoveAdapter2, str2.length())) << "\n**********************************\n" <<"\n";
		test2.close();
    //}

	if ((AdapterFound == true) && (AdapterFound2 == true)) {
		AdapterRemoved = 3;
		
	} else if ((AdapterFound == true) && (AdapterFound2 == false)) {
		AdapterRemoved = 1;
	
	} else if ((AdapterFound == false) && (AdapterFound2 == true)){
		AdapterRemoved = 2;
	
	} else {
		AdapterRemoved = 0;
	}

    // Returns 1 if only First Half Removed
    // Returns 2 if only Second Half Removed
    // Returns 3 if both Removed
    
	return AdapterRemoved;
}

void ReadPair::passOutFile() {
	if(fRead.compare("") == 0) {
		/*char fileName1[15];  //STOP USING SPRINTF. THIS IS WHAT NOT TO DO.
		sprintf(fileName1, "./results/read1Pass_%i.fastq", tNum); 
		ofstream oFile1;
		oFile1.open(fileName1, ios::app);
		oFile1 << ID1 << "\n" << read1 << "\n+\n" << qual1 << "\n";
		oFile1.close();*/
		
		stringstream ofr1pn;  //This is the correct way to do it.
		ofr1pn << "./results/curjob/ind/read1Pass_p" << tNum << ".fastq";
		ofstream ofr1ps;
		ofr1ps.open(ofr1pn.str(), ios::app);
		ofr1ps << ID1 << "\n" << read1 << "\n+\n" << qual1 << "\n";
		ofr1ps.close();

		stringstream ofr2pn;
		ofr2pn << "./results/curjob/ind/read2Pass_p" << tNum << ".fastq";
		ofstream ofr2ps;
		ofr2ps.open(ofr2pn.str(), ios::app);
		ofr2ps << ID2 << "\n" << read2 << "\n+\n" << qual2 << "\n";
		ofr2ps.close();
		
	} else {
		stringstream ofsrpn;
		ofsrpn << "./results/curjob/ind/singleReadPass_p" << tNum << ".fastq";
		ofstream ofsrps;
		ofsrps.open(ofsrpn.str(), ios::app);
		ofsrps << ID1 << "\n" << fRead << "\n+\n" << fQual << "\n";
		ofsrps.close();
		
		if (badSide == 2) {
			stringstream ofr2fn;
			ofr2fn << "./results/curjob/ind/read2Fail_p" << tNum << ".fastq";
			ofstream ofr2fs;
			ofr2fs.open(ofr2fn.str(), ios::app);
			ofr2fs << ID2 << "\n" << read2 << "\n+\n" << qual2 << "\n";
			ofr2fs.close();
			
		} else if (badSide == 1) {
			stringstream ofr1fn;
			ofr1fn << "./results/curjob/ind/read1Fail_p" << tNum << ".fastq";
			ofstream ofr1fs;
			ofr1fs.open(ofr1fn.str(), ios::app);
			ofr1fs << ID1 << "\n" << read1 << "\n+\n" << qual1 << "\n";
			ofr1fs.close();
		}
	}
}

void ReadPair::failOutFile() {
	if(fRead.compare("") == 0) {
		stringstream ofr1fn;
		ofr1fn << "./results/curjob/ind/read1Fail_p" << tNum << ".fastq";
		ofstream ofr1fs;
		ofr1fs.open(ofr1fn.str(), ios::app);
		ofr1fs << ID1 << "\n" << read1 << "\n+\n" << qual1 << "\n";
		ofr1fs.close();

		stringstream ofr2fn;
		ofr2fn << "./results/curjob/ind/read2Fail_p" << tNum << ".fastq";
		ofstream ofr2fs;
		ofr2fs.open(ofr2fn.str(), ios::app);
		ofr2fs << ID2 << "\n" << read2 << "\n+\n" << qual2 << "\n";
		ofr2fs.close();

	} else {
		stringstream ofsrfn;
		ofsrfn << "./results/curjob/ind/singleReadFail_p" << tNum << ".fastq";
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

void ReadPair::Compile() {
	tStrip();
	aRemove();
	oCheck();
	//lUniversalTest();
	//rUniversalTest();
	int p = qualPass();
	if(p == 1) passOutFile();
	else failOutFile();
}



