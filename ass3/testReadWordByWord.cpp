#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <regex>

using namespace std;

int main() 
{ 
   	ifstream in_file;
   	in_file.open("dasovich-j.133");

   	std::string str;

    while (std::getline(in_file, str))
    {
        std::regex e ("[a-zA-Z]+"); // whitespace
        std::regex_token_iterator<std::string::iterator> a (str.begin(), str.end(), e);
        // default constructor = end-of-sequence:
  		std::regex_token_iterator<std::string::iterator> rend;
  		while (a!=rend) std::cout << " [" << *a++ << "]";
    }

    in_file.close();
    
    return 0;
}

