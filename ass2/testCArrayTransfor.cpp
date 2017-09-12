#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;
std::map<char, int> c_array_map;

int main(){
	// map is sorted according to key automatically
	c_array_map['s'] = 4;
	c_array_map['#'] = 1;
	c_array_map['i'] = 4;
	c_array_map['m'] = 1;
	c_array_map['p'] = 2;
	
	map<char, int>::iterator it;
	int previousValue = 0;
	int temp = -1;
	for ( it = c_array_map.begin(); it != c_array_map.end(); it++ )
	{
		temp = it->second;
		c_array_map[it->first] = previousValue;
		previousValue += temp;
	}

	for ( it = c_array_map.begin(); it != c_array_map.end(); it++ )
	{
	    std::cout << it->first  // string (key)
	              << ':'
	              << it->second   // string's value 
	              << std::endl ;
	}

	return 0;
}