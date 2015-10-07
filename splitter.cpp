/* ADNA
 * FASTQ File Splitter
 */

#include <iostream>
using namespace std;
#define maxbps 128

int main(int argc, char **argv) {
	cout << "ADNAScan" << endl;
	if (argc < 2) {
		cout << "Error: Not enough arguments" << endl;
		cout << "Usage: " << argv[0] << " <input_fastq_file> <options>" << endl;
		return -1;
	}
	
	// Grab FASTQ File
	
	// Split FASTQ File into equal segments

	return 0;
}
