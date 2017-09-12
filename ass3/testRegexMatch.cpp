#include <regex>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

int main(){

	ifstream file_in("index_file");

	std::string str_temp;
	while(getline(str_temp, file_in)) {
		std::regex word_reg("^accepteth:");

		if (std::regex_match(str_temp, word_reg)) {
			cout<<match<<endl;
		}

	}

	return 0;
}