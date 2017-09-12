#include <string>
#include <iostream>

using namespace std;

int main() {

	string word = "about";

	if (word.back() == '/') {
		cout << "1" << endl;
	} else {
		cout << "2" << endl;
	}

	return 0;
}