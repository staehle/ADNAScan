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
ReadPair::ReadPair(std::string d1, std::string r1, std::string q1, int threadNum)
{
	ID1 = d1;
	read1 = r1;
	qual1 = q1;
	fRead = "";
	fQual = "";
	tNum = threadNum;
	std::vector < std::string > PrintLongestArray;
}

// Constructor for the ReadPair class. Takes the bases and quality of both sides of a read
ReadPair::ReadPair(std::string d1, std::string d2, std::string r1, std::string q1,std::string r2, std::string q2, int threadNum)
{
	ID1 = d1;
	ID2 = d2;
	read1 = r1;
	read2 = r2;
	qual1 = q1;
	qual2 = q2;
	fRead = "";
	fQual = "";
	tNum = threadNum;
	std::vector < std::string > PrintLongestArray;
	Compile();
}

// Adds the data for read 2 and then compiles.
void ReadPair::addR2(std::string d2, std::string r2, std::string q2)
{
	ID2 = d2;
	read2 = r2;
	qual2 = q2;
	//Compile(); //call after
}

// Quality test for the two parts of this read. Since we aren't yet sure what will be done after finding the
// bad pairs, and because many of the reads look to be failing, I currently just have a failure/pass print
int ReadPair::qualPass()
{
	if(fRead.compare("") == 0)
	{
		//test first half
		int ret = 0;
		int qual = -1;
		int failCtr = 0;
		for(std::string::iterator c = qual1.begin(); c != qual1.end(); ++c)
		{
			qual = int(*c)-33;
			//printf("%c : %i\n", *c, qual);
			if(int(*c)-33 < 15)
			{
				failCtr+=1;
				//printf("%i\n", failCtr);
			}
		}
		printf("FAILURES : %i\n\n",failCtr);
		if (failCtr < 10)
		{
			printf("Read 1 passed qual test\n");
			ret += 1;
		}
		else
		{
			printf("Read 1 failed qual test\n");
		}

		//test second half
		qual = -1;
		failCtr = 0;
		for(std::string::iterator c = qual2.begin(); c != qual2.end(); ++c)
		{
			qual = int(*c)-33;
			//printf("%c : %i\n", *c, qual);
			if(int(*c)-33 < 15)
			{
				failCtr+=1;
				//printf("%i\n", failCtr);
			}
		}
		printf("FAILURES : %i\n\n",failCtr);
		if (failCtr < 10)
		{
			printf("Read 2 passed qual test\n");
			ret += 2;
		}
		else
		{
			printf("Read 2 failed qual test\n");
		}
		return ret;
	}

	// if reads were combined
	else
	{
		int qual = -1;
		int failCtr = 0;
		for(std::string::iterator c = fQual.begin(); c != fQual.end(); ++c)
		{
			qual = int(*c)-33;
			//printf("%c : %i\n", *c, qual);
			if(int(*c)-33 < 15)
			{
				failCtr+=1;
				//printf("%i\n", failCtr);
			}
		}
		printf("FAILURES : %i\n\n",failCtr);
		if (failCtr < 10)
		{
			printf("Combined read passed qual test\n");
			return 4;
		}
		else
		{
			printf("Combined read failed qual test\n");
			return 0;
		}
	}
}

// This method strips the leading T and trailing 2 T's by changing their quality scores to #(2)
void ReadPair::tStrip()
{
    if(read1.at(0) == 'T')
    {
        printf("found a T\n");
        qual1[0] = '#';
    }
    if((read1.at(read1.length() - 1) == 'T') && (read1.at(read1.length() - 2) == 'T'))
    {
        printf("found ending T's\n");
        qual1[qual1.length() - 1] = '#';
        qual1[qual1.length() - 2] = '#';
    }

    if(read2.at(0) == 'T')
    {
        printf("found a T\n");
        qual2[0] = '#';
    }
    if((read2.at(read2.length() - 1) == 'T') && (read2.at(read2.length() - 2) == 'T'))
    {
        printf("found ending T's\n");
        qual2[qual2.length() - 1] = '#';
        qual2[qual2.length() - 2] = '#';
    }
}

// Look for 11 overlapping nucleotides. Returns max overlap over 10
int ReadPair::oCheck()
{
	int i1 = 0; //looks at read1
	int i2 = 0; //looks at read2
	//longestOLap will keep the largest number of overlapping base matches found. Since there could
	//POTENTIALLY be a section of overlap past the first instance of a match, we will test the whole read
	//just to be certain we found the full overlap

	//int longestOLap = 0;

	//i1Temp will act as temporary i1 to iterate up the string, searching for continual matches (overlap)
	//oCtr will be an overlap counter, counting up for each continual match.

	int i1Temp, oCtr;

	while (i1 < read1.length() - 10)
	{
		if (read1[i1] == read2[i2])
		{
			i1Temp = i1;
			while (i1Temp < read1.length()  && read1[i1Temp] == read2[i2])
			{
				//printf("%c and %c at %i and %i\n", read1[i1Temp], read2[i2], i1Temp, i2);
				++oCtr;
				if (oCtr > 10 && i1Temp == read1.length() - 1)
				{
					i2 += 1;
					printf("found %i overlapping bases\n", i2);


					fRead = read1.substr(0,i1) + read2;
					fQual = qual1.substr(0,i1) + qual2;
					std::cout << "New single-read: " << fRead << "\n";
					std::cout << "New single-read quality: " << fQual << "\n";
					return 1;
				}
				++i1Temp;
				++i2;
			}
			oCtr = 0;
			i2 = 0;
		}
		++i1;
	}
	return 0;
}

/////////////////////////////////////////////////////////////

// Many Print statements and Extra Information for Testing Purpose

int ReadPair::aRemove()
{
    // if FREAD  == ""// read1 and read2
    // if FREAD == !" = Check both sides

    const int MinLengthAdapters = 6; // Min Length for accepted Adapters
    const int NumberOfAdapters = 26; // Amount of Adapters we are using // I use this instead of length, for more flexibility

    // These are for Return
    // Returns 1 if Only removed from First read
    // 2 if Only removed from Second read
    // 3 if removed from both
    // 0 if none removed
    int AdapterRemoved = 0;
    bool AdapterFound = false;
    bool AdapterFound2 = false;

    std::string PrintLongestAdapter;
    std::string PrintLongestAdapter2;



    	// Array of the adapter
	std::string Adapters[] =
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



	// If fRead(Overlap) occured
    if (!fRead.empty()){

		std::string str = fRead;
		// (this is the String from Sequence)



		std::string LongestAdapter = ""; // Longest Adapter Found. - This is what SHOULD be Removed
		int counter = 0; // Counter for Adapters
		int RemoveAdapter; // Longest Adapter that is found in sequence (The number in my array) ETC Adapter Number 4
		std::vector< int > NumbOfArrays; //Array for all the adapters found - For testing
		AdapterFound = false; // If Adapter Found

		// str.length to avoid too small sequences
		 while (counter < NumberOfAdapters){ // While loop to get every Adapter.

			//std::cout << "Adapter " << Adapters[counter] << " is being checked. \n" << '\n'; // see adapters being checked
			std::string AdapterSearch = ""; // Assigning string AdapterSearch to the current Adapter we are searching through
			AdapterSearch.assign(Adapters[counter]);
			int endIndex = AdapterSearch.length(); // Again not needed, could use AdapterSearch.length() in the for loop

			std::string FoundString = "";

		// For loop, From the first Character, to the length of the Adapter - Minimum characters matching (6) in our case
		// (We don't want to look for matching 1/2/3/4/5 adapter matches, since these couldve matched randomly.)
		// (C T G A The chances of one of our etc 30 adapters to have a matching any 2 character start is 2/30, but 6 matching randomly is 0.007/30)
		// Reducing the chance for random matches
			for (int startIndex = 0; startIndex <= (endIndex - MinLengthAdapters); startIndex = startIndex + 1)
			{
						// Creating Temp Strings for match - Printing
				std::string CurrSeq = str.substr(0, (endIndex - startIndex));
				std::string CurrAdap = AdapterSearch.substr(startIndex, (endIndex)); // Looking at LAST part of the sequence
				//std::cout << CurrAdap << ' ' << CurrSeq << '\n'; // see the actual sequence

						//Adapter Found
				std::size_t found = CurrSeq.find(CurrAdap);
				if (found != std::string::npos) {
					//std::cout << "\nAdapter found at length: " << CurrSeq.length() << '\n';
					FoundString.assign(CurrSeq);
					NumbOfArrays.push_back(counter);

						//If First Adapter Found
					if (AdapterFound == false){
						//std::cout << "Longest Adapter is currently " << FoundString << '\n';
						LongestAdapter.assign(FoundString);
						AdapterFound = true;
						RemoveAdapter = counter; // assigning adapter to remove
						break;
					}
					else{ // If another adapter is found
						if(FoundString.length() >= LongestAdapter.length()){ // If longer than previous (Currently >= !!)
							LongestAdapter.assign(FoundString);
							//std::cout << "New Longest Adapter is currently " << FoundString << '\n';
							RemoveAdapter = counter;
							break;}
						else { // If Not longer than previous
							//std::cout << "Adapter Found, but not longer than current Longest Adapter " << FoundString <<  " vs " << LongestAdapter <<'\n';
							break;}
						} // For the else
					} // If Found
				} // For Loop

			//std::cout << "\nAdapter " << Adapters[counter] << " is searched. \n\n" << '\n'; //see adapters being checked
		// Counter for Adapters
				counter ++;
			} // While loop

		// Print Statements
		/*
		if (AdapterFound == false)
			std::cout << "\n\n\n No Adapters Found in First Half of sequence" << '\n';
		else{
			std::cout << " \n\n\n Longest Adapter : " << Adapters[RemoveAdapter] << '\n';
			std::cout << " Is found with length " << LongestAdapter.length() << " : " << LongestAdapter << "\n" << '\n';
			std::cout << " Adapter(s) were found in First Half of sequence " << '\n';
			for(int i = 0; i < NumbOfArrays.size(); i++) {
				std::cout << " Adapter." << NumbOfArrays[i] << " : " << Adapters[NumbOfArrays[i]] << '\n';
			} // For loop
		} // Else loop
			std::cout << "\n\n\n" << '\n';
		*/
		//Basically the same as first Half, the only thing different is the For loop
		// Since it is now checking the last part of the sequence, going in a different direction

			counter = 0; // Counter for Adapters
			std::string LongestAdapter2 = ""; // Longest Adapter Found. - This is what SHOULD be Removed
			int RemoveAdapter2; // Longest Adapter that is found in sequence (The number in my array)
			std::vector< int > NumbOfArrays2; //Array for all the adapters found - For testing
			AdapterFound2 = false; // If Adapter Found

		// str.length to avoid too small sequences
			while (counter < NumberOfAdapters){ //

				//std::cout << "Adapter " << Adapters[counter] << " is being checked. \n" << '\n'; // see adapters being checked
				std::string AdapterSearch = "";
				AdapterSearch.assign(Adapters[counter]);
				int endIndex = AdapterSearch.length();
				std::string FoundString = "";

				int CurrentIncrementer = 0; // Incrementer for the substring Current Adapter

			// from last character minus the length of the adapter. to the last character minus MinLengthAdapters

				for (int startIndex = (str.length()- endIndex); startIndex <= (str.length()-MinLengthAdapters); startIndex = startIndex + 1)
				{

				  //  int teststrlength = str.length();
					//std::cout << "Length of string : " << teststrlength << " Length of Start Index : " << startIndex << '\n';
					std::string CurrSeq = str.substr(startIndex, (startIndex - endIndex));
					std::string CurrAdap = AdapterSearch.substr(0, (endIndex - CurrentIncrementer)); //Looking at FIRST part of Sequence
					//std::cout << CurrAdap << ' ' << CurrSeq << '\n'; // see the actual sequence


					std::size_t found = CurrSeq.find(CurrAdap);
					if (found != std::string::npos) {
						//std::cout << "\nAdapter found at length: " << CurrSeq.length() << '\n';
						FoundString.assign(CurrSeq);
						NumbOfArrays2.push_back(counter);


						if (AdapterFound2 == false){ // If first Adapter Found
							//std::cout << "Longest Adapter is currently " << FoundString << '\n';
							LongestAdapter2.assign(FoundString);
							AdapterFound2 = true;
							RemoveAdapter2 = counter; // assigning adapter to remove
							break;
						}
						else{ // If another adapter is found
							if(FoundString.length() >= LongestAdapter2.length()){
								LongestAdapter2.assign(FoundString);
								//std::cout << "New Longest Adapter is currently " << FoundString << '\n';
								RemoveAdapter2 = counter;
								break;}
							else {
								//std::cout << "Adapter Found, but not longer than current Longest Adapter " << FoundString <<  " vs " << LongestAdapter <<'\n';
							break;}
						} // else
					} // if found
					CurrentIncrementer++;
				}

				//std::cout << "\nAdapter " << Adapters[counter] << " is searched. \n\n" << '\n'; // see adapters being checked
			// Counter for Adapters
				counter ++;
			}

			// Print Statements
			/*
			if (AdapterFound2 == false)
				std::cout << "\n\n\n No Adapters Found in Second Half of sequence" << '\n';
			else {
				std::cout << " \n\n\n Longest Adapter : " << Adapters[RemoveAdapter2] << '\n';
				std::cout << " Is found with length " << LongestAdapter2.length() << " : " << LongestAdapter2 << "\n" << '\n';
				std::cout << " Adapter(s) were found in Second Half of sequence " << '\n';
				for(int i = 0; i < NumbOfArrays2.size(); i++) {
					std::cout << " Adapter." << NumbOfArrays2[i] << " : " << Adapters[NumbOfArrays2[i]] << '\n';
				} // for statement
			} // else
			*/
		// Adapter Removal


		int FirstLength = LongestAdapter.length();
		int SecondLength = LongestAdapter2.length();

		PrintLongestAdapter.assign(Adapters[RemoveAdapter]);
    		PrintLongestAdapter2.assign(Adapters[RemoveAdapter2]);

		//For testing purpose, not replacing the original string, just creating a new



					// REPLACE WITH FREAD///////////////////////////////////////////
		fRead = str.substr(FirstLength, (str.length() - FirstLength - SecondLength));
		std::cout <<"\n\n\n Adapter Removal Complete \n" << '\n';
		std::cout <<" Original Sequence : [" << str << "]" << '\n';
		std::cout <<" New Sequence      : [" << fRead << "]" << '\n';



		} // if fread is not empty
    else {

        std::string str1 = read1;
		// (this is the String from Sequence)



		std::string LongestAdapter = ""; // Longest Adapter Found. - This is what SHOULD be Removed
		int counter = 0; // Counter for Adapters
		int RemoveAdapter; // Longest Adapter that is found in sequence (The number in my array) ETC Adapter Number 4
		std::vector< int > NumbOfArrays; //Array for all the adapters found - For testing
		AdapterFound = false; // If Adapter Found

		// str.length to avoid too small sequences
		 while (counter < NumberOfAdapters){ // While loop to get every Adapter.

			//std::cout << "Adapter " << Adapters[counter] << " is being checked. \n" << '\n'; // see adapters being checked
			std::string AdapterSearch = ""; // Assigning string AdapterSearch to the current Adapter we are searching through
			AdapterSearch.assign(Adapters[counter]);
			int endIndex = AdapterSearch.length(); // Again not needed, could use AdapterSearch.length() in the for loop

			std::string FoundString = "";

		// For loop, From the first Character, to the length of the Adapter - Minimum characters matching (6) in our case
		// (We don't want to look for matching 1/2/3/4/5 adapter matches, since these couldve matched randomly.)
		// (C T G A The chances of one of our etc 30 adapters to have a matching any 2 character start is 2/30, but 6 matching randomly is 0.007/30)
		// Reducing the chance for random matches
			for (int startIndex = 0; startIndex <= (endIndex - MinLengthAdapters); startIndex = startIndex + 1)
			{
			// Creating Temp Strings for match - Printing
				std::string CurrSeq = str1.substr(0, (endIndex - startIndex));
				std::string CurrAdap = AdapterSearch.substr(startIndex, (endIndex)); // Looking at LAST part of the sequence
				//std::cout << CurrAdap << ' ' << CurrSeq << '\n'; // see the actual sequence

			//Adapter Found
				std::size_t found = CurrSeq.find(CurrAdap);
				if (found != std::string::npos) {
					//std::cout << "\nAdapter found at length: " << CurrSeq.length() << '\n';
					FoundString.assign(CurrSeq);
					NumbOfArrays.push_back(counter);

						//If First Adapter Found
					if (AdapterFound == false){
						//std::cout << "Longest Adapter is currently " << FoundString << '\n';
						LongestAdapter.assign(FoundString);
						AdapterFound = true;
						RemoveAdapter = counter; // assigning adapter to remove
						break;
					}
					else{ // If another adapter is found
						if(FoundString.length() >= LongestAdapter.length()){ // If longer than previous (Currently >= !!)
							LongestAdapter.assign(FoundString);
							//std::cout << "New Longest Adapter is currently " << FoundString << '\n';
							RemoveAdapter = counter;
							break;}
						else { // If Not longer than previous
							//std::cout << "Adapter Found, but not longer than current Longest Adapter " << FoundString <<  " vs " << LongestAdapter <<'\n';
							break;}
						} // For the else
					} // If Found
				} // For Loop

			//std::cout << "\nAdapter " << Adapters[counter] << " is searched. \n\n" << '\n'; //see adapters being checked
		// Counter for Adapters
				counter ++;
			} // While loop

		// Print Statements
		/*
		if (AdapterFound == false)
			std::cout << "\n\n\n No Adapters Found in First Half of sequence" << '\n';
		else{
			std::cout << " \n\n\n Longest Adapter : " << Adapters[RemoveAdapter] << '\n';
			std::cout << " Is found with length " << LongestAdapter.length() << " : " << LongestAdapter << "\n" << '\n';
			std::cout << " Adapter(s) were found in First Half of sequence " << '\n';
			for(int i = 0; i < NumbOfArrays.size(); i++) {
				std::cout << " Adapter." << NumbOfArrays[i] << " : " << Adapters[NumbOfArrays[i]] << '\n';
			} // For loop
		} // Else loop
			std::cout << "\n\n\n" << '\n';
		*/
		//Basically the same as first Half, the only thing different is the For loop
		// Since it is now checking the last part of the sequence, going in a different direction

		//Second
			std::string str2 = read2;

			counter = 0; // Counter for Adapters
			std::string LongestAdapter2 = ""; // Longest Adapter Found. - This is what SHOULD be Removed
			int RemoveAdapter2; // Longest Adapter that is found in sequence (The number in my array)
			std::vector< int > NumbOfArrays2; //Array for all the adapters found - For testing
			AdapterFound2 = false; // If Adapter Found

		// str.length to avoid too small sequences
			while (counter < NumberOfAdapters){ //

				//std::cout << "Adapter " << Adapters[counter] << " is being checked. \n" << '\n'; // see adapters being checked
				std::string AdapterSearch = "";
				AdapterSearch.assign(Adapters[counter]);
				int endIndex = AdapterSearch.length();
				std::string FoundString = "";

				int CurrentIncrementer = 0; // Incrementer for the substring Current Adapter

			// from last character minus the length of the adapter. to the last character minus MinLengthAdapters

				for (int startIndex = (str2.length()- endIndex); startIndex <= (str2.length()-MinLengthAdapters); startIndex = startIndex + 1)
				{

				  //  int teststrlength = str.length();
					//std::cout << "Length of string : " << teststrlength << " Length of Start Index : " << startIndex << '\n';
						std::string CurrSeq = str2.substr(startIndex, (startIndex - endIndex));
					std::string CurrAdap = AdapterSearch.substr(0, (endIndex - CurrentIncrementer)); //Looking at FIRST part of Sequence
					//std::cout << CurrAdap << ' ' << CurrSeq << '\n'; // see the actual sequence


					std::size_t found = CurrSeq.find(CurrAdap);
					if (found != std::string::npos) {
						//std::cout << "\nAdapter found at length: " << CurrSeq.length() << '\n';
						FoundString.assign(CurrSeq);
						NumbOfArrays2.push_back(counter);


						if (AdapterFound2 == false){ // If first Adapter Found
							//std::cout << "Longest Adapter is currently " << FoundString << '\n';
							LongestAdapter2.assign(FoundString);
							AdapterFound2 = true;
							RemoveAdapter2 = counter; // assigning adapter to remove
							break;
						}
						else{ // If another adapter is found
							if(FoundString.length() >= LongestAdapter2.length()){
								LongestAdapter2.assign(FoundString);
								//std::cout << "New Longest Adapter is currently " << FoundString << '\n';
								RemoveAdapter2 = counter;
								break;}
							else {
								//std::cout << "Adapter Found, but not longer than current Longest Adapter " << FoundString <<  " vs " << LongestAdapter <<'\n';
							break;}
						} // else
					} // if found
					CurrentIncrementer++;
				}

				//std::cout << "\nAdapter " << Adapters[counter] << " is searched. \n\n" << '\n'; // see adapters being checked
			// Counter for Adapters
				counter ++;
			}

			// Print Statements
			/*
			if (AdapterFound2 == false)
				std::cout << "\n\n\n No Adapters Found in Second Half of sequence" << '\n';
			else {
				std::cout << " \n\n\n Longest Adapter : " << Adapters[RemoveAdapter2] << '\n';
				std::cout << " Is found with length " << LongestAdapter2.length() << " : " << LongestAdapter2 << "\n" << '\n';
				std::cout << " Adapter(s) were found in Second Half of sequence " << '\n';
				for(int i = 0; i < NumbOfArrays2.size(); i++) {
					std::cout << " Adapter." << NumbOfArrays2[i] << " : " << Adapters[NumbOfArrays2[i]] << '\n';
				} // for statement
			} // else
			*/
		// Adapter Removal


		int FirstLength = LongestAdapter.length();
		int SecondLength = LongestAdapter2.length();

		PrintLongestAdapter.assign(Adapters[RemoveAdapter]);
    		PrintLongestAdapter2.assign(Adapters[RemoveAdapter2]);


						///////// REPLACE WITH READ1 //////////
		read1 = str1.substr(FirstLength, (str1.length() - FirstLength));
						///////// REPLACE WITH READ2 /////////
		read2 = str2.substr(0, (str2.length() - SecondLength));
		//For testing purpose, not replacing the original string, just creating a new


		std::cout <<"\n\n\n Adapter Removal Complete \n" << '\n';
		std::cout <<" Original Sequence Read 1: [" << str1 << "]" << '\n';
		std::cout <<" New Sequence      : [" << read1 << "]" << '\n';
		std::cout <<" Original Sequence Read 2: [" << str2 << "]" << '\n';
		std::cout <<" New Sequence      : [" << read2 << "]" << '\n';



    }

      if ((AdapterFound == true) && (AdapterFound2 == true)){
    	PrintLongestArray.push_back(PrintLongestAdapter);
    	PrintLongestArray.push_back(PrintLongestAdapter2);
    	std::cout << "\n Adapter Removed from both sides\n" << '\n';
        AdapterRemoved = 3;}
    else if ((AdapterFound == true) && (AdapterFound2 == false)){
    	PrintLongestArray.push_back(PrintLongestAdapter);
    	std::cout << "\n Adapter Removed from only first side\n" << '\n';
        AdapterRemoved = 1;}
    else if ((AdapterFound == false) && (AdapterFound2 == true)){
    	std::cout << "\n Adapter Removed from only second side\n" << '\n';
    	PrintLongestArray.push_back(PrintLongestAdapter2);
        AdapterRemoved = 2;}
    else {
    	std::cout << "\n No Adapters Removed\n" << '\n';
        AdapterRemoved = 0;}




    // Returns 1 if only First Half Removed
    // Returns 2 if only Second Half Removed
    // Returns 3 if both Removed
	return AdapterRemoved;

}

void ReadPair::passOutFile()
{
	if(fRead.compare("") == 0)
	{
		char fileName1 [15];
		sprintf(fileName1, "read1Pass_%i.fastq", tNum);
		ofstream oFile1;
		oFile1.open(fileName1);
		oFile1 << ID1 << "\n" << read1 << "\n+\n" << qual1 << "\n";
		oFile1.close();

		char fileName2 [15];
		sprintf(fileName2, "read2Pass_%i.fastq", tNum);
		ofstream oFile2;
		oFile2.open(fileName2);
		oFile1 << ID2 << "\n" << read2 << "\n+\n" << qual2 << "\n";
		oFile2.close();

	}
	else
	{
		char fileName [15];
		sprintf(fileName, "singleReadPass_%i.fastq", tNum);
		ofstream oFile;
		oFile.open(fileName);
		oFile << ID1 << "\n" << fRead << "\n+\n" << fQual << "\n";
		oFile.close();
	}
}

void ReadPair::failOutFile()
{
	if(fRead.compare("") == 0)
	{
		char fileName1 [15];
		sprintf(fileName1, "read1Fail_%i.fastq", tNum);
		ofstream oFile1;
		oFile1.open(fileName1);
		oFile1 << ID1 << "\n" << read1 << "\n+\n" << qual1 << "\n";
		oFile1.close();

		char fileName2 [15];
		sprintf(fileName2, "read2Fail_%i.fastq", tNum);
		ofstream oFile2;
		oFile2.open(fileName2);
		oFile2 << ID2 << "\n" << read2 << "\n+\n" << qual2 << "\n";
		oFile2.close();
	}
	else
	{
		char fileName [15];
		sprintf(fileName, "singleReadFail_%i.fastq", tNum);
		ofstream oFile;
		oFile << ID1 << "\n" << fRead << "\n+\n" << fQual << "\n";
		oFile.close();
	}
}



//Main

/////////////////////////////////////////////////////////////////

void ReadPair::Compile()
{
	tStrip();
	oCheck();
	aRemove();

	for (int i = 0; i < PrintLongestArray.size(); i++) {
    	std::cout << " Adapter Removed " << PrintLongestArray[i]  << '\n';
	}
	//qualPass();
	int p = qualPass();
	if(p == 3 || p == 4)
	{
		passOutFile();
	}
	else
	{
		failOutFile();
	}
	//printf("hi\n");

}
