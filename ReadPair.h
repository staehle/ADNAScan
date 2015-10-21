#include <string>

class ReadPair
{
	std::string read1, read2, qual1, qual2;
public:
	ReadPair(std::string, std::string,std::string, std::string);
	void qualPass();
	void tStrip();
	int oCheck();
};
