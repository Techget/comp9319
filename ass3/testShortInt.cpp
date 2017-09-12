#include <iostream>
#include <stdio.h>
using namespace std;

int main() {
	short int a = 5;
	cout<<sizeof(a)<<endl;


	FILE * file = fopen("writeShortIntAppendBinary", "ab");

	short int value[] = {a};
	fwrite(value, sizeof(short int), 1, file);

	int b = 5;
	int value2[] = {b};
	fwrite(value2, sizeof(int), 1, file);

	fclose(file);
	return 0;
}