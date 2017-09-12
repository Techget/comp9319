#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;

int main(){
	// FILE * simple6idx = fopen("simple6.idx", "ro");
	FILE * bwt_file = fopen("simple6.bwt", "ro");

	int rank = 0;
	int char_count = 0;
	char ch;
	while(char_count<=(18000 - 1) && (ch = fgetc(bwt_file)) !=EOF){
		if(ch == 's'){
			rank++;
		}
		char_count++;
	}

	int temp_rank = rank;
	cout<<"char_count: "<<char_count<<" first part count: "<<temp_rank<<"\n";

	int second_part_start= ftell(bwt_file);

	int i = 0;
	for(i=0; i<=1185; i++){
		ch = fgetc(bwt_file);
		if(ch == 's'){
			rank++;
		}
		char_count++;
	}

	int file_pointer = ftell(bwt_file);

	cout<<"second_part_start: "<<second_part_start<<"\n";
	cout<<"char_count: "<<char_count<<" second part count: "<<(rank-temp_rank)<<" file_pointer: "<<file_pointer<<"\n";


}



