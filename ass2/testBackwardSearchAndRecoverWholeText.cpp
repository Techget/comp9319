#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;

int occWithoutIndex(char c, int index);
int nextKey(char c);
int inverseOccWithoutIndex(char c, int rank);

FILE * bwt_file = NULL;
string pattern = "ban";

int FIRST_ptr = -1;
int LAST_ptr = -1;
int bwt_file_size = -1;

std::map<char, int> c_array_map;


int main(){	
	c_array_map['1'] = 1;
	c_array_map['2'] = 1;
	c_array_map['a'] = 4;
	c_array_map['b'] = 2;
	c_array_map['n'] = 3;
	c_array_map['['] = 2;
	c_array_map[']'] = 2;

	// c_array_map['2'] = 1;
	// c_array_map['['] = 1;
	// c_array_map[']'] = 1;
	// c_array_map['b'] = 1;
	// c_array_map['a'] = 3;
	// c_array_map['n'] = 2;
	//transform
	map<char, int>::iterator it;
	int previousValue = 0;
	int temp = -1;
	for ( it = c_array_map.begin(); it != c_array_map.end(); it++ )
	{
		temp = it->second;
		c_array_map[it->first] = previousValue;
		previousValue += temp;
	}

	bwt_file = fopen("simple2.bwt", "ro");
	// bwt_file = fopen("simple1.bwt", "ro");
	fseek(bwt_file, 0, SEEK_END);
	bwt_file_size = ftell(bwt_file);
	fseek(bwt_file, 0, SEEK_SET);

	/////////////////// bwt backward search
	int p_len = pattern.length();
	int i = p_len -1;
	char c = pattern.at(i);
	FIRST_ptr = c_array_map[c];

	int next_char = nextKey(c);
	if (next_char == -1){
		// bwt_file_size - 1 is the corresponding index in first column
		LAST_ptr = bwt_file_size - 1;
	} else {
		LAST_ptr = c_array_map[(char)next_char] - 1;
	}

	// printf("%d %d\n",FIRST_ptr, LAST_ptr);

	while (FIRST_ptr <= LAST_ptr && i>=1){
		c = pattern.at(i-1);
		FIRST_ptr = c_array_map[c] + occWithoutIndex(c, FIRST_ptr-1);
		LAST_ptr = c_array_map[c] + occWithoutIndex(c, LAST_ptr) - 1;
		i--;
	}

	// printf("%d %d\n",FIRST_ptr, LAST_ptr);


	if(FIRST_ptr<=LAST_ptr && FIRST_ptr != -1 && LAST_ptr != -1){
		int i;
		for (i=FIRST_ptr; i<=LAST_ptr; i++) {
			//construct the corresponding text
			string output_str = "";
			//forward search to the fisrt [
			int index = i;
			char concat_char;
			int curr_char_rank;
			int skip_flag = 0;
			while(true){
				map<char, int>::iterator it = c_array_map.begin();
				char prev_key = it->first;
				int prev_value = it->second;
				it++;
				char curr_key;
				int curr_value;
				for ( ; it != c_array_map.end(); it++ ) {
					curr_key = it->first;
					curr_value = it->second;

					if(index >= prev_value && index < curr_value){
						concat_char = prev_key;
						break;
					}

					prev_key = curr_key;
					prev_value = curr_value;
				}
				// the last key
				if(prev_key == curr_key){
					concat_char = prev_key;	
				}

				curr_char_rank = index - prev_value;

				// skip, since it's inside []
				if (concat_char == ']') {
					skip_flag = 1;
					break;
				}
				// end forward search;
				if (concat_char == '[') {
					break;
				}

				output_str += concat_char;

				index = inverseOccWithoutIndex(concat_char, curr_char_rank);
			}

			if (skip_flag){
				continue;
			}
			//backward search until the first [
			fseek(bwt_file, i, SEEK_SET);
			char prefix_char = fgetc(bwt_file);
			output_str = prefix_char + output_str;
			index = c_array_map[prefix_char] + occWithoutIndex(prefix_char, i)-1;
			// cout<<prefix_char<<" "<<index<<"\n";		
			while(true){
				fseek(bwt_file, index, SEEK_SET);
				prefix_char = fgetc(bwt_file);
				output_str = prefix_char + output_str;
				if(prefix_char == '['){
					break;
				}
				index = c_array_map[prefix_char] + occWithoutIndex(prefix_char, index)-1;
			}
			cout<<output_str<<"\n";


			//if argc > 4, need to further string match
			// if(argc>4){

			// }
		}
	}

	fclose(bwt_file);
	return 0;
}

// count it from the start of bwt_file
int occWithoutIndex(char c, int index){
	fseek(bwt_file, 0, SEEK_SET);

	int loop = 0;
	int counter = 0;
	char ch;
	while((ch = fgetc(bwt_file))!= EOF && loop <= index){
		if (ch == c) {
			counter++;
		}
		loop++;
	}

	return counter;
}


/*
*	edge conditions:
*		- rank is zero, solved by set temp_rank to -1 when initialization
*		- the first char of bwt_file is the char, and we want rank 0, solved by the second if condition test
**/
int inverseOccWithoutIndex(char c, int rank){
	fseek(bwt_file, 0, SEEK_SET);

	int temp_rank = -1;
	int index = 0;
	char ch;
	while((ch = fgetc(bwt_file))!= EOF && temp_rank < rank){
		if (ch == c) {
			temp_rank++;
		}
		if(temp_rank == rank){
			break;
		}
		index++;
	}

	return index;
}


// auxiliary function nextKey()
// return next key or return NULL if it is the last key-value in the map
int nextKey(char c){
	map<char, int>::iterator it;
	char prev_key;
	char curr_key;
	for ( it = c_array_map.begin(); it != c_array_map.end(); it++ )
	{
	    curr_key = it->first;
	    if (prev_key == c) {
	    	break;
	    }
	    prev_key = it->first;

	}


	if (prev_key == curr_key){
		return -1;
	} else {
		return curr_key;
	}
}