#include "Dictionary.h"

/*
Sets the name of the dictionary to a given name
@param file_name: the name of the dictionary to be changed to
*/
void Dictionary::set_name(string file_name)
{
	name = file_name;
}
//=====================================================================================
/*
Fills the multimap with the information in the dictionary file
*/
void Dictionary::extract_dict()
{
	ifstream dict_file;
	dict_file.open(name);
	while (!dict_file.eof())
	{
		string line;
		string entry;
		string synonim;

		getline(dict_file, line);
		entry = line.substr(0, line.find_first_of(':'));
		line = line.substr(line.find_first_of(':') + 1);

		while ((line.find_first_of(',') != string::npos) && (line != ""))
		{
			synonim = line.substr(1, line.find_first_of(',') - 1);
			line = line.substr(line.find_first_of(',') + 1);
			transform(entry.begin(), entry.end(), entry.begin(), ::toupper);
			transform(synonim.begin(), synonim.end(), synonim.begin(), ::toupper);

			Dict_sin.insert(pair<string, string>(entry, synonim));
		}
		if (line != "")
		{
			synonim = line.substr(1);
			transform(entry.begin(), entry.end(), entry.begin(), ::toupper);
			transform(synonim.begin(), synonim.end(), synonim.begin(), ::toupper);
			Dict_sin.insert(pair<string, string>(entry, synonim));
		}
	}
	dict_file.close();
}
//=====================================================================================
/*
Returns true if it finds a word in the multimap keys and false otherwise
@param word: word to be found
*/
bool Dictionary::find_word(string word)const
{
	return (Dict_sin.find(word) != Dict_sin.end());
}
//=====================================================================================
/*
Returns the name of the dictionary
*/
string Dictionary::get_name() const
{
	return name;
}
//=====================================================================================
/*
Returns a vector with all the keys in the multimap
*/
vector<string> Dictionary::get_words()
{
	vector <string> new_vector;
	string last_word = "?";
	for (auto keys : Dict_sin)
	{
		if (last_word != keys.first)
		{
			new_vector.push_back(keys.first);
			last_word = keys.first;
		}
	}
	return new_vector;
}
//=====================================================================================
/*
Returns a vector with all the synonims in the multimap
*/
vector<string> Dictionary::get_synonims(string word)
{
	vector <string> new_vector;
	for (auto keys : Dict_sin)
	{
		if (word == keys.first)
		{
			new_vector.push_back(keys.second);
		}
	}
	return new_vector;
}
//=====================================================================================
