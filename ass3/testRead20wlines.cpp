#include <fstream>
#include <string>
#include <iostream>
#include <string> 

// using namespace std;

int main() 
{ 
   	std::ifstream in_file;
   	in_file.open("index_file");

   	std::string str;

    while (std::getline(in_file, str)) {
    	// cout<< str;
    }	

    in_file.close();

    // std::string base = "lalal";
    // int a = 1;
    // std::string combine = base + std::to_string(a);
    // std::cout<<combine<<std::endl;

    return 0;

}
