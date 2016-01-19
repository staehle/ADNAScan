#include <string>
#include <vector>

class ReadPair
{
	std::string ID1, ID2, read1, read2, qual1, qual2, fRead, fQual;
	int tNum, lAdap, rAdap, merged, tRem, badRead, badSide;
	std::vector <std::string> PrintLongestArray;
	//char fRead;
public:
	ReadPair(std::string, std::string, std::string,std::string, std::string,std::string , int);
	ReadPair(std::string, std::string, std::string, int);
	void addR2(std::string, std::string, std::string);
	int qualPass();
	void tStrip();
	int oCheck();
	int aRemove();
	int getLeftA();
	int getRightA();
	int getBad();
	int isMerged();
	int tStripped();
	void passOutFile();
	void failOutFile();
	void Compile();
};
