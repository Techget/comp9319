#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;


int main(){
	string str1 = "";
	char c = 'A';

	str1 = str1 + c;

	str1 += 'B';

	str1 = 'c' + str1;

	cout<<str1<<endl;

	string testlength = "a";

	std::cout << "The size of testlength is " << testlength.length() << " bytes.\n";



	// char * pattern = "this is a pattern\n";
	// cout<<pattern<<endl;
}