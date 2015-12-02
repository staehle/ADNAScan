/*
 * FastaSequences.cpp
 *
 * This class is meant to act as a central read object for the adapter sequences. It will have
 * descriptions from the FASTA file as well as the sequence reads themselves. Will be called from
 * the main program sending the command line FASTA file argument to the constructor.
 *
 */
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <cstddef>
#include "FastaSequences.h"
using namespace std;

struct Node
{
	std::string desc;
	std::string seq;
	struct Node *next;
	//Node(std::string d, std::string s, Node n)
	//{
	//	desc = d;
	//	seq = s;
	//	next = *n;
	//}
};

Node *head = NULL;

// Constructor for the ReadPair class. Takes the bases and quality of both sides of a read
FastaSequences::FastaSequences(std::string inF)
{
	ifstream inFile(inF.c_str());

	if (inFile.good())
	{
		//inFile.open();
		std::string output;
		if (inFile.is_open())
		{
			Node *cur = head;
			while (!inFile.eof())
			{

				inFile>>output;
				//cout << output << "\n";

				if( output.at(0) == '>')
				{
					Node *temp = new Node;
					temp->desc = output;
					temp->next = NULL;
					if(head == NULL)
					{
						head = temp;
						cur = temp;
					}
					else
					{
						cur->next = temp;
						cur = temp;
					}
				}
				else
				{
					if(cur != NULL)
					{
						cur->seq += output;
					}
				}
				//cout<<output;
			}
		}
		inFile.close();
		//Node *head = NULL;
		//new Node("str","s",NULL);
	}
	else
	{
		printf("No\n");
	}

}
