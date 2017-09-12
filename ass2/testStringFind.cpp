#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;

int main(){

	string str1 = "There is a needle\n";
	string str2 = "needle\n";

	int found = str1.find(str2);
  	if (found != std::string::npos){
    	std::cout << "'needle' found at: " << found << '\n';
  	} else {
  		cout<<"not found\n";
  	}
}