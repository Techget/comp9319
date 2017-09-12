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

// functions
void create_idxes(std::string file_folder, std::string idx_folder);


// global variables
std::map<int, std::string> number_filename_map;
std::set<std::string> stop_word_set = {"a", "about", "above", "across", "after", "afterwards", "again", "against", "all", "almost", "alone", "along", "already", "also","although","always","am","among", "amongst", "amoungst", "amount",  "an", "and", "another", "any","anyhow","anyone","anything","anyway", "anywhere", "are", "around", "as",  "at", "back","be","became", "because","become","becomes", "becoming", "been", "before", "beforehand", "behind", "being", "below", "beside", "besides", "between", "beyond", "bill", "both", "bottom","but", "by", "call", "can", "cannot", "cant", "co", "con", "could", "couldnt", "cry", "de", "describe", "detail", "do", "done", "down", "due", "during", "each", "eg", "eight", "either", "eleven","else", "elsewhere", "empty", "enough", "etc", "even", "ever", "every", "everyone", "everything", "everywhere", "except", "few", "fifteen", "fify", "fill", "find", "fire", "first", "five", "for", "former", "formerly", "forty", "found", "four", "from", "front", "full", "further", "get", "give", "go", "had", "has", "hasnt", "have", "he", "hence", "her", "here", "hereafter", "hereby", "herein", "hereupon", "hers", "herself", "him", "himself", "his", "how", "however", "hundred", "ie", "if", "in", "inc", "indeed", "interest", "into", "is", "it", "its", "itself", "keep", "last", "latter", "latterly", "least", "less", "ltd", "made", "many", "may", "me", "meanwhile", "might", "mill", "mine", "more", "moreover", "most", "mostly", "move", "much", "must", "my", "myself", "name", "namely", "neither", "never", "nevertheless", "next", "nine", "no", "nobody", "none", "noone", "nor", "not", "nothing", "now", "nowhere", "of", "off", "often", "on", "once", "one", "only", "onto", "or", "other", "others", "otherwise", "our", "ours", "ourselves", "out", "over", "own","part", "per", "perhaps", "please", "put", "rather", "re", "same", "see", "seem", "seemed", "seeming", "seems", "serious", "several", "she", "should", "show", "side", "since", "sincere", "six", "sixty", "so", "some", "somehow", "someone", "something", "sometime", "sometimes", "somewhere", "still", "such", "system", "take", "ten", "than", "that", "the", "their", "them", "themselves", "then", "thence", "there", "thereafter", "thereby", "therefore", "therein", "thereupon", "these", "they", "thickv", "thin", "third", "this", "those", "though", "three", "through", "throughout", "thru", "thus", "to", "together", "too", "top", "toward", "towards", "twelve", "twenty", "two", "un", "under", "until", "up", "upon", "us", "very", "via", "was", "we", "well", "were", "what", "whatever", "when", "whence", "whenever", "where", "whereafter", "whereas", "whereby", "wherein", "whereupon", "wherever", "whether", "which", "while", "whither", "who", "whoever", "whole", "whom", "whose", "why", "will", "with", "within", "without", "would", "yet", "you", "your", "yours", "yourself", "yourselves", "the", "b", "c","d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
// temp
// const bool is_in = container.find(element) != container.end();

int main(int argc, char* argv[]) {
	/****************** indexing ***************/
	std::string file_folder = argv[1];
	std::string idx_folder = argv[2];

	DIR * idx_dir;
    if((idx_dir  = opendir(idx_folder.c_str())) == NULL) {
  		// create a new directory
		create_idxes(file_folder, idx_folder);        
    } else {
    	// read filename_number_map in
    }

	/****************** search **************/


	/****************** rank *****************/

}


void create_idxes(std::string file_folder, std::string idx_folder) {
	DIR * file_dir;
	// DIR * idx_dir;
    struct dirent *dirp;

	// If enter this method, means that idx folder does not exist
	mkdir(idx_folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	// idx_dir = opendir(idx_folder.c_str());
	file_dir = opendir(file_folder.c_str());

	// used for filename number mapping, will start from 1
	// there will be 2000 files at most, use short int
	short int file_number = 0;
	// loop through files in file_dir
	while ((dirp = readdir(file_dir)) != NULL) {
		std::string filename = std::string(dirp->d_name);
		if (filename == "." || filename == "..") {
			continue;
		} 
		// std::cout<< "file name :" << dirp->d_name << std::endl;
		// word occurence map for each file
		std::map<std::string, int> stem_word_occurence;

		file_number++;
		// add to filename to number_filename_map
		number_filename_map[file_number] = filename;

		// open file
		std::ifstream in_file;
		if(file_folder.back() == '/'){
			in_file.open(file_folder+filename);
		} else {
			in_file.open(file_folder+"/"+filename);			
		}

   		// read the file line by line, fill in the stem_word_occurence map
		std::string str_temp;
	    while (std::getline(in_file, str_temp)) {
	    	// split string into words
	        std::regex e ("[a-zA-Z]+"); // regex to extract words
	        std::regex_token_iterator<std::string::iterator> reg_tok_itr (str_temp.begin(), str_temp.end(), e);
	  		std::regex_token_iterator<std::string::iterator> rend;
	  		// loop through words in one sentence
	  		while (reg_tok_itr != rend) {
	  			std::string word = *reg_tok_itr;
	  			// tolowercase for every word
	  			std::transform(word.begin(), word.end(), word.begin(), ::tolower);
	  			// stem the word
	  			Porter2Stemmer::stem(word);

	  			// check if it is stop word
	  			if (stop_word_set.find(word) != stop_word_set.end()){
	  				// it is stop word, skip it
	  				reg_tok_itr++;
	  				continue;
	  			} else {
	  				if (stem_word_occurence.find(word) != stem_word_occurence.end()) {
	  					stem_word_occurence[word]++;
	  				} else {
	  					stem_word_occurence[word] = 1;
	  				}
	  			}

	  			reg_tok_itr++;
	  		}
	    }

	    // close file
	    in_file.close();

	    // write to corresponding idx files
	    // loop through stem_word_occurence map
	    std::map<std::string, int>::iterator itr;
		for (itr = stem_word_occurence.begin(); itr != stem_word_occurence.end(); itr++) {
			FILE * stem_word_idx_file;
			std::string idx_file_name;

			if(idx_folder.back() == '/'){
				idx_file_name = idx_folder + itr->first;
			} else {
				idx_file_name = idx_folder + '/' + itr->first;
			}

			// if file not exist, append mode will create a new file as well
			stem_word_idx_file = fopen(idx_file_name.c_str(), "ab");

			// append file number to corresponding file
			short int value[] = {file_number};
			fwrite(value, sizeof(short int), 1, stem_word_idx_file);
			// append stemmed word occurences in this file 
			int occurence[] = {itr->second};
			fwrite(occurence, sizeof(int), 1, stem_word_idx_file);

			fclose(stem_word_idx_file);
		}
	}

	// write filename number map to file.idx in idx folder, for future usage
	std::string idx_file_name;

	if(idx_folder.back() == '/'){
		idx_file_name = idx_folder + "file.idx";
	} else {
		idx_file_name = idx_folder + '/' + "file.idx";
	}

	std::map<int, std::string>::iterator itr;
	// since stemmed word won't match file.idx, so it going to be unique filename, file.idx
	std::ofstream file_number_idx (idx_file_name);
	for (itr = number_filename_map.begin(); itr != number_filename_map.end(); itr++) {
		file_number_idx << itr->second << "\n";
	}
  	file_number_idx.close();
}