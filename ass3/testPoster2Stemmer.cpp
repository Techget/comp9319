#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "porter2_stemmer.h"
#include <regex>

using namespace std;

int main() {
	// std::string word = "banana";
	// Porter2Stemmer::stem(word);

	// std::string word2 = "iphone";
	// Porter2Stemmer::trim(word2);

	// cout<<word<<" "<<word2<<endl;

	std::ifstream synonym_file("dict.txt", std::fstream::in);
	std::ofstream out_file("synonym_dict.txt", std::ofstream::out);

	std::string str_temp;
	while (std::getline(synonym_file, str_temp)) {
		std::regex reg ("[a-z]+"); // regex to extract words
		std::regex_token_iterator<std::string::iterator> reg_tok_itr (str_temp.begin(), str_temp.end(), reg);
		std::regex_token_iterator<std::string::iterator> rend;

		// std::string word = *reg_tok_itr++;
		// Porter2Stemmer::stem(word);
		// Porter2Stemmer::trim(word);
		// out_file << word;
		while (reg_tok_itr != rend) {
			std::string word = *reg_tok_itr++;
			Porter2Stemmer::stem(word);
			Porter2Stemmer::trim(word);
			out_file << " " << word;
  		}
  		out_file << " \n";
	}

	synonym_file.close();
	out_file.close();


	// std::set<std::string> stop_word_set = {"a", "about", "above", "across", "after", "afterwards", "again", "against", "all", "almost", "alone", "along", "already", "also","although","always","am","among", "amongst", "amoungst", "amount",  "an", "and", "another", "any","anyhow","anyone","anything","anyway", "anywhere", "are", "around", "as",  "at", "back","be","became", "because","become","becomes", "becoming", "been", "before", "beforehand", "behind", "being", "below", "beside", "besides", "between", "beyond", "bill", "both", "bottom","but", "by", "call", "can", "cannot", "cant", "co", "con", "could", "couldnt", "cry", "de", "describe", "detail", "do", "done", "down", "due", "during", "each", "eg", "eight", "either", "eleven","else", "elsewhere", "empty", "enough", "etc", "even", "ever", "every", "everyone", "everything", "everywhere", "except", "few", "fifteen", "fify", "fill", "find", "fire", "first", "five", "for", "former", "formerly", "forty", "found", "four", "from", "front", "full", "further", "get", "give", "go", "had", "has", "hasnt", "have", "he", "hence", "her", "here", "hereafter", "hereby", "herein", "hereupon", "hers", "herself", "him", "himself", "his", "how", "however", "hundred", "ie", "if", "in", "inc", "indeed", "interest", "into", "is", "it", "its", "itself", "keep", "last", "latter", "latterly", "least", "less", "ltd", "made", "many", "may", "me", "meanwhile", "might", "mill", "mine", "more", "moreover", "most", "mostly", "move", "much", "must", "my", "myself", "name", "namely", "neither", "never", "nevertheless", "next", "nine", "no", "nobody", "none", "noone", "nor", "not", "nothing", "now", "nowhere", "of", "off", "often", "on", "once", "one", "only", "onto", "or", "other", "others", "otherwise", "our", "ours", "ourselves", "out", "over", "own","part", "per", "perhaps", "please", "put", "rather", "re", "same", "see", "seem", "seemed", "seeming", "seems", "serious", "several", "she", "should", "show", "side", "since", "sincere", "six", "sixty", "so", "some", "somehow", "someone", "something", "sometime", "sometimes", "somewhere", "still", "such", "system", "take", "ten", "than", "that", "the", "their", "them", "themselves", "then", "thence", "there", "thereafter", "thereby", "therefore", "therein", "thereupon", "these", "they", "thickv", "thin", "third", "this", "those", "though", "three", "through", "throughout", "thru", "thus", "to", "together", "too", "top", "toward", "towards", "twelve", "twenty", "two", "un", "under", "until", "up", "upon", "us", "very", "via", "was", "we", "well", "were", "what", "whatever", "when", "whence", "whenever", "where", "whereafter", "whereas", "whereby", "wherein", "whereupon", "wherever", "whether", "which", "while", "whither", "who", "whoever", "whole", "whom", "whose", "why", "will", "with", "within", "without", "would", "yet", "you", "your", "yours", "yourself", "yourselves", "the", "b", "c","d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
	
	// std::string word = "about";

	// if (stop_word_set.find(word) != stop_word_set.end()){	
		
	// }

}