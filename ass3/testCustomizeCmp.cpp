#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
// #include <utility>
#include <iterator>

using namespace std;

bool cmp(const pair<std::string, int>  &p1, const pair<std::string, int> &p2){
    return p1.second != p2.second ? p1.second < p2.second : p1.first < p2.first;
}

int main() {
	std::map<std::string, int> myMap;
	myMap["here"] = 5;
	myMap["there"] = 3;

	vector<pair<std::string, int> > v;	
    std::copy(myMap.begin(), myMap.end(), back_inserter(v));

    std::sort(v.begin(), v.end(), cmp);

    for(int i = 0; i < v.size(); ++i)
        cout << v[i].first << " : " << v[i].second << endl;
    return 0;
}