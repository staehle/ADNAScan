#include <string>

class ReadPair
{
	std::string read1, read2, qual1, qual2, fRead;
public:
	ReadPair(std::string, std::string,std::string, std::string);
	int qualPass();
	void tStrip();
	int oCheck();
	int aRemove();
	void Compile();
};
