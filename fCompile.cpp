#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>

using namespace std;

string line;
int gTotal = 0;
int bTotal = 0;
int mTotal = 0;
int tTotal = 0;

int main(int argc, char **argv) {
  //BEGIN ADAPTER FILE COMPILE
  //max length of adapters currently ~ 60. will change in future
  int adaps[27][70] = {0};

  char aFileName [100];
  sprintf(aFileName, "./results/%s/removedAdapters.temp", argv[1]);
	
  ifstream aFile;
  aFile.open(aFileName);
  line;
  while (std::getline(aFile, line))
  {
    std::istringstream iss(line);
    int a, b, c;
    if (!(iss >> a >> b >> c)) { break; } // error
	adaps[a][b] += c;
  }
  aFile.close();
  
  
  // BEGIN GOOD/BAD FILE COMPILE
  char gBFileName [100];
  sprintf(gBFileName, "./results/%s/goodBadReadsCount.temp", argv[1]);
  
  ifstream gBFile;
  gBFile.open(gBFileName);
  line;
  gTotal = 0;
  bTotal = 0;
  while (std::getline(gBFile, line))
  {
    std::istringstream iss(line);
    int a, b;
    if (!(iss >> a >> b)) { break; } // error
    gTotal += a;
    bTotal += b;
  }
  gBFile.close();
  
    // BEGIN MERGE FILE COMPILE
  char mFileName [100];
  sprintf(mFileName, "./results/%s/mergeCount.temp", argv[1]);
  
  ifstream mFile;
  mFile.open(mFileName);
  line;
  mTotal = 0;
  while (std::getline(mFile, line))
  {
	mTotal += atoi(line.c_str());
  }
  mFile.close();
  
  
  // BEGIN T REMoVAL COUNT FILE COMPILE
  char tFileName [100];
  sprintf(tFileName, "./results/%s/tRemoveCount.temp", argv[1]);
  
  ifstream tFile;
  tFile.open(tFileName);
  line;
  tTotal = 0;
  while (std::getline(tFile, line))
  {
	tTotal += atoi(line.c_str());
  }
  tFile.close();

  
  
  //BEGIN FINAL COMPILE
  ofstream rFile;
  
  char rFileName [100];
  sprintf(rFileName, "./results/%s/results.out", argv[1]);
	
  rFile.open(rFileName, std::ios::app);
  for(int i =0; i < 27; i++)
  {
    rFile << "Adapter " << i << ":\n";
    for(int j=0;j<70;j++)
    {

      if(adaps[i][j] > 0)
      {
        rFile << "  Length: " << j << "  Count: " << adaps[i][j] << "\n";
      }
    }
  }
  rFile << "\n\nPassing Read Count: " << gTotal << " Failing Read Count: " << bTotal << "\n";
  rFile << "\n\nMerged Read Pair Count: " << mTotal  << "\n";
  rFile << "\n\nT Removal Count (Total Pairs): " << tTotal  << "\n";
  
  rFile.close();

}
