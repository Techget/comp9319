#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <fstream>

using namespace std;

std::map<char, int> c_array_map;

int main(){

	c_array_map['1'] = 1;
	c_array_map['2'] = 1;
	c_array_map['a'] = 4;
	c_array_map['b'] = 2;
	c_array_map['n'] = 3;
	c_array_map['['] = 2;
	c_array_map[']'] = 2;

	FILE * idx_file = fopen("testWriteCArrayMapInBinary.bin", "wb");

	int i;

	int zero[] = {0};
	for(i=0; i<127;i++){
		if (c_array_map.find((char)i) != c_array_map.end()) {
			int value[] = {c_array_map[(char)i]};
			fwrite(value, sizeof(int), 1, idx_file);
		} else {
			fwrite(zero, sizeof(int), 1, idx_file);
		}
	}

	fclose(idx_file);

	idx_file = fopen("testWriteCArrayMapInBinary.bin", "ab");

	//int zero[] = {0};
	for(i=0; i<127;i++){
		if (c_array_map.find((char)i) != c_array_map.end()) {
			int value[] = {c_array_map[(char)i]};
			fwrite(value, sizeof(int), 1, idx_file);
		} else {
			fwrite(zero, sizeof(int), 1, idx_file);
		}
	}

	fclose(idx_file);



	idx_file = fopen("testWriteCArrayMapInBinary.bin", "rb");
	int read_arr[1];
	//for(i=0;i<127;i++){
	int offset = (int('b'))*4;
	fseek(idx_file, offset, SEEK_SET);
	fread(read_arr,sizeof(int),1,idx_file);
	cout<<"b:"<<read_arr[0]<<"\n";

	char ch = 'a';
	offset = 127*4 + ((int)ch) * 4;
	cout<<offset<<"\n";
	//fseek(idx_file, 0, SEEK_SET);
	fseek(idx_file, offset, SEEK_SET);
	fread(read_arr,sizeof(int),1,idx_file);
	cout<<"a:"<<read_arr[0]<<"\n";
	//}


	offset = (int('b'))*4;
	fseek(idx_file, offset, SEEK_SET);
	fread(read_arr,sizeof(int),1,idx_file);
	cout<<"b:"<<read_arr[0]<<"\n";
	fclose(idx_file);

	// idx_file = fopen("testWriteCArrayMapInBinary.bin", "rb");
	// int offset = ((int)'a')*4;
	// int read_arr_temp[1];
	// fseek(idx_file, offset, SEEK_SET);
	// fread(read_arr_temp,sizeof(int),1,idx_file);
	// cout<<"a:"<<read_arr_temp[0]<<"\n";
	// fclose(idx_file);

	// idx_file = fopen("testWriteCArrayMapInBinary.bin", "rb");
	// int read_arr[1];
	// for(i=0;i<127;i++){
	// 	// when it goes backwardly, should use 127 - i instead of 126 - i
	// 	fseek(idx_file, -4*(127-i), SEEK_END);
	// 	fread(read_arr,sizeof(int),1,idx_file);
	// 	//cout<<((char)i)<<":"<<read_arr[0]<<"\n";
	// }
	// fclose(idx_file);


	// std::ofstream ofile("testWriteCArrayMapInBinary.bin", std::ios::binary);

	// //ofile.write((char*) &f, sizeof(float));
	// int i;
	// int zero = 0;
	// for(i=0; i<127;i++){
	// 	if (c_array_map.find((char)i) != c_array_map.end()) {
	// 		int value = c_array_map[(char)i];
	// 		ofile.write((int*) &value, sizeof(int));
	// 	} else {
	// 		ofile.write((int*) &zero, sizeof(int));
	// 	}
	// }

}

