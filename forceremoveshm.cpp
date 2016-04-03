/* ADNA
 * Cleanup driver
 */

#include <iostream>
#include <cstring>
#include "adnashm.hpp"
using namespace std;

int main() {
	
	if (shm_unlink(JOBKEY)!=0) cerr << "Cleaner error: Unable to unlink shared memory with: " << JOBKEY << endl;
	if (shm_unlink(TABKEY)!=0) cerr << "Cleaner error: Unable to unlink shared memory with: " << TABKEY << endl;
	
	return 0;
}








