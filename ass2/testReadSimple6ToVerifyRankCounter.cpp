#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;
std::map<char, int> c_array_map;
int main(){
	// brute force count
	FILE * simple6 = fopen("simple6.bwt", "ro");
	FILE * simple6BruteForceCounter = fopen("simple6BruteForceCounter", "wr");

	char ch;
	int char_count=0;
	while((ch = fgetc(simple6))!=EOF){
		if(char_count % 2000 == 0 && char_count != 0){
			int i;
			for(i=0; i<127;i++){
				if (c_array_map.find((char)i) != c_array_map.end()) {
					cout<<(char)i<<" "<<c_array_map[(char)i]<<"\n";
					// string input_line = (char)i + " " + c_array_map[(char)i] + "\n";
					// fputs(input_line);
				} else {
					cout<<(char)i<<" "<<"0\n";
					// string input_line = (char)i + " 0\n";
					// fputs(input_line);
				}
			}
		}

		if(char_count == 2000){
			break;
		}

		if (c_array_map.find(ch) == c_array_map.end()) {
        	c_array_map[ch] = 1;
        } else {
        	c_array_map[ch]++;
        }

		char_count++;
	}

	fclose(simple6);
	fclose(simple6BruteForceCounter);

	// read the index output the values 
	FILE * simple6IndexRead = fopen("simple6.idx", "rb");
	fseek(simple6IndexRead, 0, SEEK_END);
   	int idx_file_size = ftell(simple6IndexRead);
	fseek(simple6IndexRead, 0, SEEK_SET);

	int offset = 0;
	int i = 0;
	while(offset <= idx_file_size - 4){
		offset = i * 4;
		int value[1];
		fseek(simple6IndexRead, offset, SEEK_SET);
		fread(value,sizeof(int),1,simple6IndexRead);
		cout<<(char)(i%127)<<" "<<value[0]<<"\n";
		i++;
		if(i == 127){
			break;
		}
	}

	fclose(simple6IndexRead);
}