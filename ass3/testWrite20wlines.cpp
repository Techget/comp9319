// basic file operations
#include <iostream>
#include <fstream>
using namespace std;

int main () {
  ofstream myfile;
  myfile.open ("example.txt");
  int i;
  for(i=0 ; i< 200000 ; i++) {
  	myfile << "Writing this to a file: "<< i << "\n";
  }
  
  myfile.close();
  return 0;
}