#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;

int main(){
	FILE * bwt_file = fopen("simple1.bwt","ro");

	fseek(bwt_file, 7-1, SEEK_SET);

	char c = fgetc(bwt_file);

	cout<<c<<"\n";

	fclose(bwt_file);

	return 0;
}