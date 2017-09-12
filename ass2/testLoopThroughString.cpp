#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;

int main(){
	string output_str = "[12345]abcdefg";
	int start_text_flag = 0;
	int char_pos_in_output_str = 0;
	string output_str_without_record;
	for(char_pos_in_output_str = 0; char_pos_in_output_str < output_str.length(); char_pos_in_output_str++){
		char temp_char = output_str.at(char_pos_in_output_str);
		if(start_text_flag){
			output_str_without_record += temp_char;
		}
		if(temp_char == ']'){
			start_text_flag = 1;
		}
	}

	cout<<output_str_without_record<<"\n";
}