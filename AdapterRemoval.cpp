
//https://github.com/jhhung/PEAT

//https://github.com/jhhung/PEAT/releases
//  - Download for working Trimmer


//Curr Working Adapter Trimmer

// string::substr
#include <iostream>
#include <string>

int main()
{
	std::string str = "AGATCGGAAGAGCACACGTCTGAACTCCAGTCACCAGATCATCTCGTATGCNGTCTTCTGNNTGAAAAAAAAAAACAAATACATAGTATACTTTCTTTATGAAATAAAAAATAAAAACAATAACA";
	// (this is the Strings from Sequence)

	std::string strTest = "GAT";
	std::string strAdap1 = "AGATCGAGTTTCGCGCCGCGGGCAGATCGG";
	std::string strAdap2 = "TTTCCCCATCTTCGCCGTCCTCCTGCTTGC";
	std::string strAdap3 = "CTTGATCACCGGGTCGCCGTTCTTCGGCGG";
	std::string strAdap4 = "AGATCGGAAGAGCACACGTCTGAACTCCGT";
	std::string strAdap5 = "CTGGAATCGCGGACACGACCCGATCTCATT";
	std::string strAdap6 = "TGCCGGAGTTTCGCGCCGCGGGCGCTTCGC";
	std::string strAdap7 = "CCCGCGGCAAACTCGGCTGTTTTGCAAGCA";
	std::string strAdap8 = "AGATCGGAAGAGCACACGTCTGAACTCCAG";
	std::string strAdap9 = "AAACTTCATCCATCCTCCTTCGGTACTGAT";
	std::string strAdap10 = "CTTTTACCGCAACATCATTTCCCCTTCCGA";
	std::string strAdap11 = "AGATCGAAATCAAGGAAGAGGAAGAGGACA";


	int endIndex = strAdap1.length();
	int startIndex = 0;

	int found = 0;
	std::string FoundString = "";

	for (int startIndex = 0; startIndex <= (endIndex - 6); startIndex = startIndex + 1)
	{


		std::string CurrSeq = str.substr(0, (endIndex - startIndex));

		std::string CurrAdap = strAdap1.substr(startIndex, (endIndex));
		std::cout << "CurrAdap for Check: " << CurrAdap << ' ' << CurrSeq << '\n';



		std::size_t found = CurrSeq.find(CurrAdap);
		if (found != std::string::npos) {
			std::cout << "Adaptor found at length: " << CurrSeq.length() << '\n';
			FoundString.assign(CurrSeq);
			std::cout << "Longest Adaptor is currently " << FoundString << '\n';
			found = 1;
			break;
		}


	}


	std::string CurrAdap = str.substr(startIndex, (endIndex - 1));     // "length of adaptor(4)"



	std::size_t pos = str.find(strTest);      // position of "live" in str

	std::string str3 = str.substr(pos);     // get from "live" to the end

											//std::cout << CurrAdap << '\n' << strAdap1 << '\n';

	return 0;
}
