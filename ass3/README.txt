Readme.txt
1. General
I make use of inverted index technique to tackle the problems listed in assignment

2. Structure of index file
There will be one final index file named "index_file" in the index folder, index file contains inverted index about the files in target folder, general format as following:
	stemmed_word:filename1,occurences|filename2,occurences|....

3. External libray used
I make use of porter2_stmmer to do stemming work for me, and only use part of its functionality, so that words won't be over stemmed.

4. General design of this function
	### Indexing ###
	First check the existence of index folder, go through files in target folder and at the same time create some sub_idx files, after looping, merge sub_idx into the final index file

	### Searching ###
	Depends on command line input, separate into keyword search and concept search. Search terms will be changed to lowercase and stemmed before entering the search part. Both keyword search and concept search will return inverted-index data structure as following:
		std::map<std::string words, std::map<std::string file,int occurences>>
	Words will only be the input search terms.

	### Ranking ###
	Operations taken on the data structure show previously, will transform into a data structure as following:
		std::map<std::string file, int weights>
	After sorting, this map according to the requirements from assignment, output it.

5. Keyword search design
Go through index files and put that into the data structure mentioned previously.

6. Concept search design
I try to realize concept search by making use of synonyms, there is a list of synonyms submit with all the other files. 
For each search term, I will attach it with a synonym set respectively, when searching, do keyword search for search term, and then, the weights&filenames contributed by synonyms will be added to corresponding search term. So the return value of concept search looks alike with the return value of keyword search. Both only contains search terms and corresponding inverted-index, then the rank and output part is reusable. 