#include <iostream>
#include <fstream>

#include <string>
#include <cstdio>

using namespace std;

int main(int argc, char **argv) {
  //Compile adapter file
  //max length of adapters currently ~ 60. will change in future
  int adaps[27][70] = {0};
  
  int arr[3];
  int i = 0;

  char aFileName [30];
	sprintf(aFileName, "./results/%s/removedAdapters.temp", argv[1]);
	
  ifstream adapFile;
  adapFile.open(aFileName);
  char line[15];
  if (adapFile.is_open())
  {
    while(!adapFile.eof())
    {
      adapFile >> line;
      //stringstream ssin(line);
      //while (ssin.good() && i < 3)
      //{
      //  string l = ssin.str();
      //  atoi(l.c_str()) >> arr[i];
      //  ++i;
      //}
      //adaps[arr[0]][arr[1]] += arr[2];
    }
  }
  adapFile.close();
  
  ofstream adapOFile;
  
  char aOFileName [30];
	sprintf(aOFileName, "./results/%s/removedAdapters.out", argv[1]);
	
  adapOFile.open(aOFileName, std::ios::app);
  for(int i =0; i < 27; i++)
  {
    for(int j=0;j<70;j++)
    {
      adapOFile << "Adapter " << i << ":\n";
      if(adaps[i][j] > 0)
      {
        adapOFile << "  Length: " << j << "  Count: " << adaps[i][j] << "\n";
      }
    }
  }
  
  adapOFile.close();



}
