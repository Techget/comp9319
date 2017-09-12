#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <algorithm>
using namespace std;

int main() {
	// std::string word = "ABC QQC";
	// std::transform(word.begin(), word.end(), word.begin(), ::tolower);
	// std::cout<<word<<std::endl;
	ofstream myfile ("example.txt");
  	myfile << "Writing this to a file.\n";
  	myfile << "Writing this to a file.\n";
  	myfile.close();
	return 0;
}

