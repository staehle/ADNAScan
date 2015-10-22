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
#include <string>
#include <cstdio>
#include "ReadPair.hpp"
using namespace std;

// Constructor for the ReadPair class. Takes the bases and quality of both sides of a read
ReadPair::ReadPair(std::string r1, std::string q1,std::string r2, std::string q2)
{
	read1 = r1;
	read2 = r2;
	qual1 = q1;
	qual2 = q2;
}

// Quality test for the two parts of this read. Since we aren't yet sure what will be done after finding the
// bad pairs, and because many of the reads look to be failing, I currently just have a failure/pass print
void ReadPair::qualPass()
{
	//test first half
    int qual = -1;
	int failCtr = 0;
	for(std::string::iterator c = qual1.begin(); c != qual1.end(); ++c)
	{
	    qual = int(*c)-33;
	    printf("%c : %i\n", *c, qual);
		if(int(*c)-33 < 15)
		{
			failCtr+=1;
			printf("%i\n", failCtr);
		}
	}
	printf("FAILURES : %i\n\n",failCtr);
	if (failCtr < 10)
	{
		printf("Read 1 passed qual test\n");
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
	    printf("%c : %i\n", *c, qual);
		if(int(*c)-33 < 15)
		{
			failCtr+=1;
			printf("%i\n", failCtr);
		}
	}
	printf("FAILURES : %i\n\n",failCtr);
	if (failCtr < 10)
	{
		printf("Read 2 passed qual test\n");
	}
	else
	{
		printf("Read 2 failed qual test\n");
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
	//Start at end of read 1 and start of read 2
	int i1 = read1.length() - 1;
	int i2 = 0;
	//longestOLap will keep the largest number of overlapping base matches found. Since there could
	//POTENTIALLY be a section of overlap past the first instance of a match, we will test the whole read
	//just to be certain we found the full overlap
	int longestOLap = 0;
	//i1Temp will act as temporary i1 to iterate back up the string, searching for continual matches (overlap)
	//oCtr will be an overlap counter, counting up for each continual match.
	int i1Temp, oCtr;
	while (i1 >= 0)
	{
		if (read1[i1] == read2[i2])
		{
			i1Temp = i1;
			while (i1Temp < read1.length() && read1[i1Temp] == read2[i2])
			{
				printf("%c and %c at %i and %i\n", read1[i1Temp], read2[i2], i1Temp, i2);
				++oCtr;
				if (oCtr > 10 && i1Temp == read1.length() - 1)
				{
					printf("found %i overlapping bases\n", oCtr);
					longestOLap = oCtr;
				}
				++i1Temp;
				++i2;
			}
			oCtr = 0;
			i2 = 0;
		}
		--i1;
	}
	return longestOLap;
}
