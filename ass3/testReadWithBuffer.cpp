#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int main() {
	// std::ifstream fin("dasovich-j.133", std::ios::in);
	// std::vector<char> buffer (128,0); //reads only the first 1024 bytes

	// while(fin.read(buffer.data(), buffer.size())) {
	//     std::streamsize s=fin.gcount();
	//     ///do with buffer
	//     std::cout<<buffer.data()<<std::endl;
	//     std::string str(buffer.begin(),buffer.end());

	//     // std::cout<< str << "**********" << std::endl;
	// }
	// std::cout<<buffer.data()<<std::endl;
	std::ifstream in("dasovich-j.133");
	std::string s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

	std::cout<<s<<std::endl;
}



