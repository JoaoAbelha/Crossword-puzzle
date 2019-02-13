#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <map>
#include <vector>
#include <sstream>
#include <ctime>

using namespace std;

class Dictionary
{
public:
	void set_name(string file_name);
	void extract_dict();
	bool find_word(string word) const;
	string get_name() const;
	vector <string> get_words();
	vector <string> get_synonims(string word);

private:
	multimap <string, string> Dict_sin; //multimap with all the words in the dictionary and all its synonims
	string name; //name of the dictionary
};

#endif
