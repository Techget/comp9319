#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

std::map<std::string, std::map<std::string, int> > mymap;
int passbyRef = 5;

int func1(std::map<std::string, std::map<std::string, int> > & mymap, int & passbyRef) {
	cout<< mymap["stem"]["file"] << endl;
	mymap["stem2"]["file2"] = 2;
	cout<< mymap["stem2"]["file2"] << endl;

	cout<<passbyRef<<endl;
	passbyRef = 6;
	cout<<passbyRef<<endl;
}

int main () {
	mymap["stem"]["file"] = 1;
	func1(mymap, passbyRef);
	cout<< mymap["stem"]["file"] << endl;
	cout<< mymap["stem2"]["file2"] << endl;
	cout<< "********\n";
	cout<<passbyRef<<endl;
	cout<<passbyRef<<endl;


	std::vector<int> aVector;
	aVector.push_back(1);
	aVector.push_back(2);
	std::vector<int>::iterator it;
	cout<<"********\n";
	for (it = aVector.begin(); it != aVector.end(); ++it) {
	    int index = std::distance(aVector.begin(), it);
	    cout<<index<<" "<<*it<<endl;
	}
}


