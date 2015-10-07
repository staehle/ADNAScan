/* ADNAScan
 * Main
 */

#include <iostream>
using namespace std;

int main(int argc, char **argv) {
	cout << "ADNAScan" << endl;
	if (argc < 2) {
		cout << "Error: Not enough arguments" << endl;
		cout << "Usage: " << argv[0] << " <input_fastq_file> <options>" << endl;
		return -1;
	}
	
	// Grab FASTQ File
	
	// Split FASTQ File into equal segments (splitter)
	
	// Do multithreading thing
		// For each thread (set as cmd argument) 
			// run scan(splitfile)
		
	// ...
	// Profit
	return 0;
}
