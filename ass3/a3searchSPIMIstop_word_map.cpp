#include <iostream>
#include <fstream>
#include <string>
#include "porter2_stemmer.h"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <regex>
#include <map>
#include <set>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

/**************** global variables *****************/

// inverted index block size for index creation, this block size is the size of map 
#define IIMAP_BLOCK_SIZE 20000
// the index file name, which is going to be used in search, and is the final result of creating index
#define INDEX_FILE_NAME "index_file"
// stop words
// std::set<std::string> stop_word_set = {};
std::map<char, std::set<std::string> > stop_word_map = {
	{'a', {"a", "about", "above", "across", "after", "afterwards", "again", "against", "all", "almost", "alone", "along", "already", "also","although","always","am","among", "amongst", "amoungst", "amount",  "an", "and", "another", "any","anyhow","anyone","anything","anyway", "anywhere", "are", "around", "as",  "at"}},
	{'b', {"back","be","became", "because","become","becomes", "becoming", "been", "before", "beforehand", "behind", "being", "below", "beside", "besides", "between", "beyond", "bill", "both", "bottom","but", "by"}},
	{'c', {"call", "can", "cannot", "cant", "co", "con", "could", "couldnt", "cry"}},
	{'d', {"de", "describe", "detail", "do", "done", "down", "due", "during"}},
	{'e', {"each", "eg", "eight", "either", "eleven","else", "elsewhere", "empty", "enough", "etc", "even", "ever", "every", "everyone", "everything", "everywhere", "except", "entire"}},
	{'f', {"few", "fifteen", "fify", "fill", "find", "first", "five", "for", "former", "formerly", "forty", "found", "four", "from", "front", "full", "further"}},
	{'g', {"get", "give", "go", "great"}},
	{'h', {"had", "has", "hasnt", "have", "he", "hence", "her", "here", "hereafter", "hereby", "herein", "hereupon", "hers", "herself", "him", "himself", "his", "how", "however", "hundred"}},
	{'i', {"ie", "if", "in", "inc", "indeed", "interest", "into", "is", "it", "its", "itself"}},
	{'j', {"just", "jit"}},
	{'k', {"keep"}},
	{'l', {"last", "latter", "latterly", "least", "less", "ltd"}},
	{'m', {"made", "many", "may", "me", "meanwhile", "might", "mill", "mine", "more", "moreover", "most", "mostly", "move", "much", "must", "my", "myself"}},
	{'n', {"name", "namely", "neither", "never", "nevertheless", "next", "nine", "no", "nobody", "none", "noone", "nor", "not", "nothing", "now", "nowhere"}},
	{'o', {"of", "off", "often", "on", "once", "one", "only", "onto", "or", "other", "others", "otherwise", "our", "ours", "ourselves", "out", "over", "own"}},
	{'p', {"part", "per", "perhaps", "please", "put"}},
	{'q', {"quit", "quick", "quicker"}},
	{'r', {"rather", "re", "real"}},
	{'s', {"same", "see", "seem", "seemed", "seeming", "seems", "serious", "several", "she", "should", "show", "side", "since", "sincere", "six", "sixty", "so", "some", "somehow", "someone", "something", "sometime", "sometimes", "somewhere", "still", "such", "system"}},
	{'t', {"take", "ten", "than", "that", "the", "their", "them", "themselves", "then", "thence", "there", "thereafter", "thereby", "therefore", "therein", "thereupon", "these", "they", "thickv", "thin", "third", "this", "those", "though", "three", "through", "throughout", "thru", "thus", "to", "together", "too", "top", "toward", "towards", "twelve", "twenty", "two"}},
	{'u', {"un", "under", "until", "up", "upon", "us"}},
	{'v', {"very", "via"}},
	{'w', {"was", "we", "well", "were", "what", "whatever", "when", "whence", "whenever", "where", "whereafter", "whereas", "whereby", "wherein", "whereupon", "wherever", "whether", "which", "while", "whither", "who", "whoever", "whole", "whom", "whose", "why", "will", "with", "within", "without", "would"}},
	{'x', {"x", "xx"}},
	{'y', {"yet", "you", "your", "yours", "yourself", "yourselves"}},
	{'z', {"z", "zz"}}
};

/************** functions declaration ***************/

void create_idxes(std::string file_folder, std::string idx_folder);
// auxiliary function of `create_idxes`
void write_to_idx_file(std::string sub_idxname_base, 
	int & sub_idxname_suffix,
	std::map< std::string, std::map<std::string, int> > & inverted_index,
	std::string idx_folder); 
void merge_sub_idx_files(std::string& idx_folder);
void delete_sub_idx_files(std::string& idx_folder);
void key_word_search(std::vector<std::string>& search_terms, 
	std::map< std::string, std::map<std::string, int> > & inverted_index,
	std::string & idx_folder);
void concept_search(std::vector<std::string>& search_terms, 
	std::map< std::string, std::map<std::string, int> > & inverted_index);
void rank_output(std::map< std::string, std::map<std::string, int> > & inverted_index);
bool file_occurrence_cmp(const std::pair<std::string, int>  &p1, 
	const std::pair<std::string, int> &p2);


int main(int argc, char* argv[]) {
	/********************** INDEXING *8**************************/
	
	std::string file_folder = argv[1];
	std::string idx_folder = argv[2];

	DIR * idx_dir;
    if((idx_dir  = opendir(idx_folder.c_str())) == NULL) {
  		// create a new directory and index file
		create_idxes(file_folder, idx_folder);        
    }

	/************************* SEARCH **************************/

	std::string third_arg = argv[3];
	std::vector<std::string> search_terms;
	std::map< std::string, std::map<std::string, int> > inverted_index;

	if( third_arg == "-c") {
		// int i;
		// for (i = 5; i < argc; i++) {
		// 	std::string word = argv[i];
		// 	Porter2Stemmer::trim(word);
		// 	Porter2Stemmer::stem(word);
		// 	search_terms.push_back(word);
		// }

		// std::sort(search_terms.begin(), search_terms.begin()+(argc - 5));

		concept_search(search_terms, inverted_index);
	} else {
		int i;
		// if not , the argc - 3 hardcode offset, may cause segfault
		// if encounter a stop word unfortunately
		int skipped_words_amount = 0;
		for (i = 3; i < argc; i++) {
			std::string word = argv[i];
			// to lower case
			std::transform(word.begin(), word.end(), word.begin(), ::tolower);
  			// check if it is stop word
  			if (stop_word_map[word.at(0)].find(word) 
  				!= stop_word_map[word.at(0)].end()) {
  				skipped_words_amount++;
  				continue;
			}
			// put stemmed search term into the vector
			Porter2Stemmer::trim(word);
			Porter2Stemmer::stem(word);
			search_terms.push_back(word);
		}

		// make it a sorted vector
		// it is sorted so I only need to read through index_file once
		std::sort(search_terms.begin(), search_terms.begin()+(argc - 3 - skipped_words_amount));

		key_word_search(search_terms, inverted_index, idx_folder);
	}

	/**************************** RANK *******************************/

	rank_output(inverted_index);

	inverted_index.clear();
}

// auxiliary function used in std::sort in rank_output
bool file_occurrence_cmp(const std::pair<std::string, int>  &p1, 
	const std::pair<std::string, int> &p2){
	// rank the occurences decreasingly, and filename lexicographically
    return p1.second != p2.second ? p1.second > p2.second : p1.first < p2.first;
}

void rank_output(std::map< std::string, std::map<std::string, int> > & inverted_index) {	
	if (inverted_index.size() == 0) {
		return;
	} 

	std::map<std::string, int> file_occurence_map;
	if (inverted_index.size() == 1) {
		// output directly
		file_occurence_map = inverted_index.begin()->second;
	} else {
		// find the shortest file list
		std::map< std::string, std::map<std::string, int> >::iterator twod_itr = inverted_index.begin();
		std::string word_with_shortest_list = twod_itr->first;
		unsigned int shortest_list_length = (twod_itr->second).size();
		twod_itr++;
		while (twod_itr != inverted_index.end()) {
			if ((twod_itr->second).size() < shortest_list_length) {
				shortest_list_length = (twod_itr->second).size();
				word_with_shortest_list = twod_itr->first;
			}
			twod_itr++;
		}
		// get intersection of all lists, and add up the occurences
		std::map<std::string, int>::iterator file_list_itr = inverted_index[word_with_shortest_list].begin();
		while (file_list_itr != inverted_index[word_with_shortest_list].end()) {

			int all_exist_flag = 1;

			std::map< std::string, std::map<std::string, int> >::iterator twod_itr_temp;
			for (twod_itr_temp = inverted_index.begin(); twod_itr_temp != inverted_index.end(); twod_itr_temp++) {	
				if (twod_itr_temp->first == word_with_shortest_list) {
					continue;
				}

				if (twod_itr_temp->second.find(file_list_itr->first) != twod_itr_temp->second.end()) {
					// means exist;
					inverted_index[word_with_shortest_list][file_list_itr->first] += 
						twod_itr_temp->second[file_list_itr->first];
				} else {
					all_exist_flag = 0;
					break;
				}
			}		

			if (all_exist_flag == 0) {
				file_list_itr = inverted_index[word_with_shortest_list].erase(file_list_itr);
			} else {
				file_list_itr++;
			}
		}

		if (inverted_index[word_with_shortest_list].size() == 0) {
			return;
		}

		file_occurence_map = inverted_index[word_with_shortest_list];
	}

	std::vector<std::pair<std::string, int> > v;	
	std::copy(file_occurence_map.begin(), file_occurence_map.end(), back_inserter(v));
    std::sort(v.begin(), v.end(), file_occurrence_cmp);

    for(unsigned int i = 0; i < v.size(); ++i)
        std::cout << v[i].first << std::endl;
}


void key_word_search(std::vector<std::string> & search_terms, 
	std::map< std::string, std::map<std::string, int> > & inverted_index,
	std::string & idx_folder) {
	
	// read index_file
	std::string idx_folder_base = idx_folder;
	if(idx_folder.back() != '/'){
		idx_folder_base +='/';
	} 
	std::ifstream in_file(idx_folder_base + INDEX_FILE_NAME, std::fstream::in);

	std::vector<std::string>::iterator itr;
	for (itr = search_terms.begin(); itr != search_terms.end(); ++itr) {
		char start_with = (*itr).at(0);

		std::string str_temp;
		while (std::getline(in_file, str_temp)) {
			// used to speed up search
			if (str_temp.at(0) != start_with) {
				continue;
			}
			// start with this stemmed word
			if (str_temp.find(*itr) == 0) {
				std::regex reg ("[^,:|]+"); // regex to extract words
		        std::regex_token_iterator<std::string::iterator> reg_tok_itr (str_temp.begin(), str_temp.end(), reg);
		  		std::regex_token_iterator<std::string::iterator> rend;

		  		// so I can skip the test of stemmed word exist test of inverted_index map
		  		// in the following while loop
		  		std::string stemmed_word = *reg_tok_itr++;

		  		while (reg_tok_itr != rend) {
		  			// do not need to test the existence of filename/stemmed_word
		  			// since for filename, there should not exist overlapping
		  			std::string filename = *reg_tok_itr++;
		  			std::string occurence = *reg_tok_itr++;
		  			inverted_index[stemmed_word][filename] = std::stoi(occurence);
		  		}
		  		// stop search of current search term, proceed to next search term.s
		  		break;
			}
		}
	}

	in_file.close();
}

void concept_search(std::vector<std::string> & search_terms, 
	std::map< std::string, std::map<std::string, int> > & inverted_index) {

}

void create_idxes(std::string file_folder, std::string idx_folder){
	/******** CREATE multiple sub index files *******/

	DIR * file_dir;
    struct dirent *dirp;

	// If enter this method, means that idx folder does not exist
	// S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH
	mkdir(idx_folder.c_str(), ACCESSPERMS);
	// open destination file folder for later looping
	file_dir = opendir(file_folder.c_str());

	// sub_idx file name pattern
	std::string sub_idxname_base = "sub_idx_";
	int sub_idxname_suffix = 1;

	// inverted index map
	std::map< std::string, std::map<std::string, int> > inverted_index;

	std::string file_folder_base = file_folder;
	if(file_folder.back() != '/'){
		file_folder_base +='/';
	} 
	// loop through files in file_dir
	while ((dirp = readdir(file_dir)) != NULL) {
		std::string filename = std::string(dirp->d_name);
		if (filename == "." || filename == "..") {
			continue;
		} 

		// open file in destination file folder
		std::ifstream in_file(file_folder_base+filename, std::fstream::in);

		// if(file_folder.back() == '/'){
		// 	in_file.open(file_folder+filename);
		// } else {
		// 	in_file.open(file_folder+"/"+filename);			
		// }

		// read entire file into buffer directly
		// std::string str_temp((std::istreambuf_iterator<char>(in_file)), 
		// 	std::istreambuf_iterator<char>());

		// close read file
	    // in_file.close();

		// read the file line by line, fill in the inverted_index map
		std::string str_temp;
	    while (std::getline(in_file, str_temp)) {

		    // split string into words
	        std::regex reg ("[a-zA-Z]+"); // regex to extract words
	        std::regex_token_iterator<std::string::iterator> reg_tok_itr (str_temp.begin(), str_temp.end(), reg);
	  		std::regex_token_iterator<std::string::iterator> rend;
	  		// loop through words in one sentence
	  		while (reg_tok_itr != rend) {
	  			std::string word = *reg_tok_itr;
	  			// tolowercase for every word
	  			std::transform(word.begin(), word.end(), word.begin(), ::tolower);
	  			// check if it is stop word
	  			if (stop_word_map[word.at(0)].find(word) 
	  				!= stop_word_map[word.at(0)].end()) {
	  				// it is stop word, skip it
	  				reg_tok_itr++;
	  				continue;
	  			}

	  			// stem the word
	  			Porter2Stemmer::trim(word);
				Porter2Stemmer::stem(word);

				// add to inverted_index map
				if (inverted_index.find(word) != inverted_index.end()) {
					if(inverted_index[word].find(filename) != inverted_index[word].end()) {
						inverted_index[word][filename]++;
					} else {
						inverted_index[word][filename] = 1;
					}
				} else {
					inverted_index[word][filename] = 1;
				}

	  			reg_tok_itr++;
	  		}
	    }
	    
	    // if inverted_index map reach the block size, write to a separate sub idx file
	    if (inverted_index.size() >= IIMAP_BLOCK_SIZE) {
	    	write_to_idx_file(sub_idxname_base, 
	    		sub_idxname_suffix, 
	    		inverted_index,
	    		idx_folder);
	    }
	}

	// inverted_index map is not empty, write to a sub idx
	if (!inverted_index.empty()) {
		if (sub_idxname_suffix == 1) {
			// means there will be only one sub_idx file
			// make that we should make as the final index file directly
			int place_holder = -1;
			write_to_idx_file(INDEX_FILE_NAME, 
				place_holder, 
				inverted_index,
				idx_folder);
			return;
		} else {
			write_to_idx_file(sub_idxname_base, 
				sub_idxname_suffix, 
				inverted_index,
				idx_folder);
		}
	}

	/********* MERGE sub_idx_files ******/
	merge_sub_idx_files(idx_folder);

	/********* DELETE temp sub idx files *******/
	delete_sub_idx_files(idx_folder);
}

// remove temporary sub indexes
void delete_sub_idx_files(std::string& idx_folder) {
	DIR * idx_dir;
    struct dirent *dirp;
    idx_dir = opendir(idx_folder.c_str());

    std::string idx_folder_base = idx_folder;
	if(idx_folder.back() != '/'){
		idx_folder_base += '/';
	} 

    // loop through idx folder, delete all the sub idx file
    while ((dirp = readdir(idx_dir)) != NULL) {
    	std::string filename = std::string(dirp->d_name);
    	if (filename == INDEX_FILE_NAME) {
    		continue;
    	}

    	filename = idx_folder_base + filename;
    	remove(filename.c_str());
    }
}	

// merge temporary sub indexes
void merge_sub_idx_files(std::string& idx_folder) {
	DIR * idx_dir;
    struct dirent *dirp;
    idx_dir = opendir(idx_folder.c_str());

    std::vector<std::ifstream *> ifstrs_vec;

	// loop through idx folder, open all the idx file
    while ((dirp = readdir(idx_dir)) != NULL) {
    	std::string filename = std::string(dirp->d_name);

    	if (filename == "." || filename == "..") {
			continue;
		} 

		// open file in destination file folder
		std::ifstream* in_file = new std::ifstream;
		if(idx_folder.back() == '/'){
			in_file->open(idx_folder+filename);
		} else {
			in_file->open(idx_folder+"/"+filename);			
		}

		ifstrs_vec.push_back(in_file);
    }

    // open the output file
    std::ofstream out_file;
	if(idx_folder.back() == '/'){
		out_file.open(idx_folder+INDEX_FILE_NAME, std::ofstream::app);
	} else {
		out_file.open(idx_folder+"/"+INDEX_FILE_NAME, std::ofstream::app);			
	}

    // final inverted-index map vector
    std::map<std::string,std::string> inverted_index; 

    std::regex reg ("[^:]+"); // regex to extract words
    while (!ifstrs_vec.empty()) {
    	std::vector<std::ifstream *>::iterator itr;
	    for ( itr = ifstrs_vec.begin(); itr != ifstrs_vec.end(); itr++) {
	    	// read in next line for each file, put into map,
	    	// until reach a limit 
	    	std::string str_temp;
	    	if(!std::getline(*(*itr), str_temp)){
	    		// end of this file
	    		int index = std::distance(ifstrs_vec.begin(), itr);
	    		ifstrs_vec[index]->close();
	    		ifstrs_vec.erase(ifstrs_vec.begin()+index);
	    		break;
	    	}
	    	
	        std::regex_token_iterator<std::string::iterator> reg_tok_itr (str_temp.begin(), str_temp.end(), reg);

	  		std::string stemmed_word = *reg_tok_itr++;
	  		
	  		if (inverted_index.find(stemmed_word) != inverted_index.end()){
	  			inverted_index[stemmed_word] += *reg_tok_itr++;
	  		} else {
	  			inverted_index[stemmed_word] = *reg_tok_itr++;
	  		}
    	}

    	// if inverted_index map reach the block size, write to a separate sub idx file
	    if (inverted_index.size() >= IIMAP_BLOCK_SIZE) {
			// remove IIMAP_BLOCK_SIZE - 50 keys.
			int record_index = 0;
			int threshold = IIMAP_BLOCK_SIZE - 50;
			std::map<std::string, std::string>::iterator itr;
			for(itr = inverted_index.begin(); itr != inverted_index.end(); itr++) {
				record_index++;
				out_file << itr->first << ":" << itr->second << "\n";
				if (record_index > threshold) {
					inverted_index.erase(inverted_index.begin(), itr);
					break;
				}
			}
	    }
    }

    if (!inverted_index.empty()) {		
		std::map<std::string, std::string>::iterator itr;
		for(itr = inverted_index.begin(); itr != inverted_index.end(); itr++) {
			out_file << itr->first << ":" << itr->second << "\n";
		}

		inverted_index.clear();
    }

    // close the output file
    out_file.close();
}

/* 
*	sub_idxname_suffix and map is passed in by refernce, 
*	so it will update the corresponding value in `create_idx`
*/
void write_to_idx_file(std::string sub_idxname_base, 
	int & sub_idxname_suffix,
	std::map< std::string, std::map<std::string, int> > & inverted_index,
	std::string idx_folder) {

	// sub_idx file name construction
	std::string sub_idx_filename;
	if (sub_idxname_suffix == -1 ) {
		// there will be only one sub_idx file, make that as the 
		// final index file
		sub_idx_filename = sub_idxname_base;
	} else {
	 	sub_idx_filename = sub_idxname_base + std::to_string(sub_idxname_suffix);
	 	sub_idxname_suffix++;
	}
	
	// open sub_idx file
	std::ofstream out_file;
	if(idx_folder.back() == '/'){
		out_file.open(idx_folder+sub_idx_filename, std::ofstream::app);
	} else {
		out_file.open(idx_folder+"/"+sub_idx_filename, std::ofstream::app);			
	}

	// loop through inverted_index map
	std::map< std::string, std::map<std::string, int> >::iterator twod_itr;
	for (twod_itr = inverted_index.begin(); twod_itr != inverted_index.end(); twod_itr++) {
		std::map<std::string, int>::iterator oned_itr;
		// stem word
		std::string output_str = twod_itr->first + ":";

		for (oned_itr = twod_itr->second.begin(); oned_itr != twod_itr->second.end(); oned_itr++) {
			// filename and its occurence
			output_str += oned_itr->first + "," + std::to_string(oned_itr->second) + "|";
		}

		out_file << output_str << "\n";
	}

	out_file.close();

	// clear inverted_index map
	inverted_index.clear();
}


