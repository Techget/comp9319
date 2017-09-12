#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;

void testPassMap(std::map<char, int>& c_array_map);

int main(){
	std::map<char, int> c_array_map;

	char special = 10;

	c_array_map['c'] = 5;
	c_array_map['a'] = 6;
	c_array_map[special] = 1;

	// int a = 128;
	// char ascii128 = (char)a;
	// c_array_map[ascii128] = 0;

	testPassMap(c_array_map);
	map<char, int>::iterator it;
	for ( it = c_array_map.begin(); it != c_array_map.end(); it++ )
	{
	    std::cout << it->first  // string (key)
	              << ':'
	              << it->second   // string's value 
	              << std::endl ;
	}



	// int temp = std::next(it, 1);
	// cout<<temp<<"\n";
}


void testPassMap(std::map<char, int>& c_array_map){
	c_array_map['a']++;
	c_array_map['c']++;
	map<char, int>::iterator it;
	for ( it = c_array_map.begin(); it != c_array_map.end(); it++ )
	{
	    std::cout << it->first  // string (key)
	              << ':'
	              << it->second   // string's value 
	              << std::endl ;
	}
}