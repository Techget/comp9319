#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;

#define THRESHOLD 2000
#define CHAR_AMOUNT 127

void bwtBackwardSearch(int (*occFunc)(char,int), string pattern);
void recoverTexts(int (*occFunc)(char,int), int (*inverseOccFunc)(char,int), int arg_count, char* arg_v[]);
void readCArrayFromIdx();
int createIdxAndCArray(char* idx_file_name);
int occWithIndex(char c, int index);
int occWithoutIndex(char c, int index);
int inverseOccWithIndex(char c, int rank);
int inverseOccWithoutIndex(char c, int rank);
int nextKey(char c);

// These variables may be accessed by mutltiple function
std::map<char, int> c_array_map;
FILE * bwt_file = NULL;
FILE * idx_file = NULL;
int FIRST_ptr = -1;
int LAST_ptr = -1;
// When C[next(c)], the c is the last char, we need to 
// infer the last_ptr value;
int bwt_file_size = -1;

/*
*	main function
*/
int main(int argc, char *argv[]){
	// as said in spec, pattern should at least has length 1
	string pattern = argv[3];
	// If search term contains '[' ']' quit directly
	int temp_char_pos;
	for(temp_char_pos = 0; temp_char_pos < pattern.length(); temp_char_pos++){
		char temp_char = pattern.at(temp_char_pos);
		if(temp_char == ']' || temp_char == '['){
			return 0;
		}
	}

	bwt_file = fopen(argv[1], "ro");
	idx_file = fopen(argv[2], "rb");

	// get the file size of bwt_file
	fseek(bwt_file, 0, SEEK_END);
   	bwt_file_size = ftell(bwt_file);
	fseek(bwt_file, 0, SEEK_SET);

	// Flag of existence of idx_file
	int idx_file_exist_flag = 0;
	if (idx_file) {
		idx_file_exist_flag = 1;
		readCArrayFromIdx();
		//recover the file pointer
		fseek(idx_file, 0, SEEK_SET);
	} else {
		idx_file_exist_flag = createIdxAndCArray(argv[2]);
		fseek(bwt_file, 0, SEEK_SET);
	}

	// check if each char in pattern is in c_array_map
	int i;
	for(i = 3; i<argc; i++){
		string pattern_temp = argv[i];
		for(temp_char_pos = 0; temp_char_pos < pattern_temp.length(); temp_char_pos++){
			char temp_char = pattern_temp.at(temp_char_pos);
			if (c_array_map.find(temp_char) == c_array_map.end()) {
				fclose(bwt_file);
				if(idx_file){
					fclose(idx_file);
				}
				return 0;
			} 
		}
	}

	// If pattern length is 1, we do not bother to do the search
	if (pattern.length() == 1) {
		// directly return the range we can get from c_array_map
		FIRST_ptr = c_array_map[pattern.at(0)];
		int next_char = nextKey(pattern.at(0));
		if (next_char == -1){
			// bwt_file_size - 1 is the corresponding index in first column
			LAST_ptr = bwt_file_size - 1;
		} else {
			LAST_ptr = c_array_map[(char)next_char] - 1;
		}

		if(idx_file_exist_flag){
			if(idx_file == NULL){
				idx_file = fopen(argv[2], "rb");
			}
			recoverTexts(occWithIndex, inverseOccWithIndex, argc, argv);
		} else {
			recoverTexts(occWithoutIndex, inverseOccWithoutIndex, argc, argv);
		}
	} else if (idx_file_exist_flag) {
		if(idx_file == NULL){
			idx_file = fopen(argv[2], "rb");
		}
		bwtBackwardSearch(occWithIndex, pattern);
		// printf("finish bwtBackwardSearch\n");
		// cout<<FIRST_ptr<<" "<<LAST_ptr<<"\n";
		//return 0;
		if(FIRST_ptr<=LAST_ptr && FIRST_ptr != -1 && LAST_ptr != -1){
			recoverTexts(occWithIndex, inverseOccWithIndex, argc, argv);
		}
	} else {
		bwtBackwardSearch(occWithoutIndex, pattern);
		if(FIRST_ptr<=LAST_ptr && FIRST_ptr != -1 && LAST_ptr != -1){
			recoverTexts(occWithoutIndex, inverseOccWithoutIndex, argc, argv);
		}
	}

	fclose(bwt_file);
	if(idx_file){
		fclose(idx_file);
	}

	return 0;
}


/*
* 	Implementation of bwt backward search algo
*/
void bwtBackwardSearch(int (*occFunc)(char,int), string pattern){
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

	while (FIRST_ptr <= LAST_ptr && i>=1){
		c = pattern.at(i-1);
		FIRST_ptr = c_array_map[c] + (*occFunc)(c, FIRST_ptr-1);
		LAST_ptr = c_array_map[c] + (*occFunc)(c, LAST_ptr) - 1;
		i--;
		// cout<<"FIRST_ptr: "<<FIRST_ptr<<" LAST_ptr:"<< LAST_ptr<<"\n";
	}
}

/*
* 	After find matches, recover the whole sentence, and match the second&third search
*	term if there is any.
*/
void recoverTexts(int (*occFunc)(char,int), int (*inverseOccFunc)(char,int), int arg_count, char* arg_v[]){
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
			//cout<<"in first while true\n";
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

			index = (*inverseOccFunc)(concat_char, curr_char_rank);
		}

		if (skip_flag){
			continue;
		}

		// backward search until the first [
		// i is the index of file reading, do not need to do -1
		fseek(bwt_file, i, SEEK_SET);
		char prefix_char = fgetc(bwt_file);
		output_str = prefix_char + output_str;
		index = c_array_map[prefix_char] + (*occFunc)(prefix_char, i)-1;
		// cout<<prefix_char<<" "<<index<<"\n";		
		while(true){
			fseek(bwt_file, index, SEEK_SET);
			prefix_char = fgetc(bwt_file);
			output_str = prefix_char + output_str;
			if(prefix_char == '['){
				break;
			}
			index = c_array_map[prefix_char] + (*occFunc)(prefix_char, index)-1;
		}


		int multiple_term_match_flag = 1;
		//if argc > 4, need to further string match
		if(arg_count>4){
			// extract text, remove the record number, since for following match
			// we do not match record either.
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

			int i;
			for(i = 4; i<arg_count ; i++){
				int found = output_str_without_record.find(arg_v[i]);
				if (found == std::string::npos) {
				    multiple_term_match_flag = 0;
				    break;
				}
			}
		}

		// Output the string
		if(multiple_term_match_flag){
			cout<<output_str<<"\n";
		}
	}
}

/* 	
*	Fill in the c_array_map by reading the idx file.
*	In idx file, the last 127 value is the c_array_map value.
*/
void readCArrayFromIdx(){
	int read_arr[1];
	int i;
	for(i=0;i<CHAR_AMOUNT;i++){
		// when it goes backwardly, should use 127 - i instead of 126 - i
		fseek(idx_file, -4*(CHAR_AMOUNT-i), SEEK_END);
		fread(read_arr,sizeof(int),1,idx_file);
		//cout<<((char)i)<<":"<<read_arr[0]<<"\n";
		c_array_map[(char)i] = read_arr[0];
	}

	// remove the zero elements, and remove the distinguishing parameter 1.
	map<char, int>::iterator it;
	for(it = c_array_map.begin(); it != c_array_map.end(); it++ ){
		if(it->second == 0){
			c_array_map.erase(it->first);
		} 
	}
	for(it = c_array_map.begin(); it != c_array_map.end(); it++ ){
		c_array_map[it->first]--;
	}
}

// return 1 if create idx file else return 0
// may create idx and will alwarys fill in the c_array_map
int createIdxAndCArray(char* idx_file_name){
	// construct c_array_map, and if the amount is out of a THRESHOLD, use idx file
	int char_count = 0; // notice char_count start with 1
	char c;

	// Since char_count start from 0, only file size that is >=2001
	// will this program try to use idx file
	if(bwt_file_size > THRESHOLD){
		idx_file = fopen(idx_file_name, "wb");
	}
	fseek(bwt_file, 0, SEEK_SET);

	while((c = fgetc(bwt_file)) != EOF) {
		// write to idx file the occ value for position 2000 * n
        // every 2000 chars
        if(char_count%THRESHOLD == 0 && char_count != 0){
        	int i;
			int zero[] = {0};
			for(i=0; i<CHAR_AMOUNT;i++){
				if (c_array_map.find((char)i) != c_array_map.end()) {
					int value[] = {c_array_map[(char)i]};
					fwrite(value, sizeof(int), 1, idx_file);
				} else {
					fwrite(zero, sizeof(int), 1, idx_file);
				}
			}
        }

        if (c_array_map.find(c) == c_array_map.end()) {
        	c_array_map[c] = 1;
        } else {
        	c_array_map[c]++;
        }

        char_count++;
    }	

    // seems that this part won't be used, but still keep it,
    // and char_count%THRESHOLD != 0 is bit buggy, try to input bwt file
    // whose file_size is 2000/4000
    // write last round occ and c_array_map 
    if(char_count>THRESHOLD && char_count%THRESHOLD != 0){
		// if the modulo is 0, it is flushed to the idx file already
	    int i;
		int zero[] = {0};
		for(i=0; i<CHAR_AMOUNT;i++){
			if (c_array_map.find((char)i) != c_array_map.end()) {
				int value[] = {c_array_map[(char)i]};
				fwrite(value, sizeof(int), 1, idx_file);
			} else {
				fwrite(zero, sizeof(int), 1, idx_file);
			}
		}
    }

    // transform the c_array_map to the style of the first column, which we can use it 
    // in the backwardsearch, c_array_map first key-value pair's value is 0
    map<char, int>::iterator it;
	int previousValue = 0;
	int temp;
	for ( it = c_array_map.begin(); it != c_array_map.end(); it++ )
	{
		temp = it->second;
		c_array_map[it->first] = previousValue;
		previousValue += temp;
	}
	
	// write the transformed c_array_map to idx file if idx file is needed
    if (bwt_file_size > THRESHOLD) {
    	int i;
		int zero[] = {0};
		for(i=0; i<CHAR_AMOUNT;i++){
			if (c_array_map.find((char)i) != c_array_map.end()) {
				// use + 1 here, used in distinguish in idx file
				int v = c_array_map[(char)i] + 1;
				int value[] = {v};
				fwrite(value, sizeof(int), 1, idx_file);
			} else {
				fwrite(zero, sizeof(int), 1, idx_file);
			}
		}
		// close here, won't affect reading
		fclose(idx_file);
		// should set the closed idx_file to null explicitly, otherwise the pointer
		// would a random value instead of NULL
		idx_file = NULL;
    	// then return 1 to indicate that we make use of idx file
    	return 1;
    }

    return 0;
}

/*
*	Use idx file to compute occ rank
*/
int occWithIndex(char c, int index){
	if (index >= THRESHOLD) {
		int read_arr[1];
		int rank = 0;

		int number_of_block = index / THRESHOLD; 
		int amount_need_brute_force = index % THRESHOLD;

		int offset_of_idx_file = 4*CHAR_AMOUNT*(number_of_block - 1) + ((int)c)*4;
		fseek(idx_file, offset_of_idx_file, SEEK_SET);
		fread(read_arr, sizeof(int), 1, idx_file);

		rank += read_arr[0];

		int offset_of_bwt_file = number_of_block * THRESHOLD;
		fseek(bwt_file, offset_of_bwt_file, SEEK_SET);
		char ch;
		int i = 0;
		while(i <= amount_need_brute_force){
			ch = fgetc(bwt_file);
			if(ch == c){
				rank++;
			}
			i++;
		}
		return rank;
	} else {
		return occWithoutIndex(c, index);
	}
}

/*
*	Count occ rank from the start of bwt_file,
*	do it by brute force
*/
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
*	Use idx file to compute corresponding occ rank's first occurence
*	index, this function will be used when recover a whole sentence
*/
int inverseOccWithIndex(char c, int rank){
	int read_arr[1];

	int char_offset = (int(c))*4;
	int curr_idx_file_offset = char_offset;
	int number_of_block = 1;
	int rank_temp;

	fseek(idx_file, curr_idx_file_offset, SEEK_SET);
	fread(read_arr, sizeof(int), 1, idx_file);
	rank_temp = read_arr[0];

	if(rank_temp > rank){
		return inverseOccWithoutIndex(c, rank);
	}

	int rank_prev;
	while(rank_temp < rank){
		rank_prev = rank_temp;

		curr_idx_file_offset = number_of_block * 4 * CHAR_AMOUNT + char_offset;
		fseek(idx_file, curr_idx_file_offset, SEEK_SET);
		fread(read_arr, sizeof(int), 1, idx_file);

		rank_temp = read_arr[0];

		number_of_block++;
	}

	rank_temp = rank_prev;
	// do it bluntly, need more consideration
	if(number_of_block == 1){
		return inverseOccWithoutIndex(c, rank);
	}

	int offset_of_bwt_file = (number_of_block - 1) * THRESHOLD;
	int index = offset_of_bwt_file;
	fseek(bwt_file, offset_of_bwt_file, SEEK_SET);
	// cout<<"inverseOccWithIndex before while loop bwt_file pointer: "<<ftell(bwt_file)<<"\n";
	char ch;
	// since the initial value for rank should be -1, so I do decrement here
	rank_temp--;
	while(rank_temp < rank){
		ch = fgetc(bwt_file);	
		if(ch == c){
			rank_temp++;
		}
		if(rank_temp == rank){
			break;
		}
		index++;
	}
	// cout<<"inverseOccWithIndex after while loop bwt_file pointer: "<<ftell(bwt_file)<<"\n";

	return index;
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
	while(temp_rank < rank && (ch = fgetc(bwt_file))!= EOF){
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

// auxiliary function 
// return integer value of next key or return -1 if it is the last key-value in the map
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


