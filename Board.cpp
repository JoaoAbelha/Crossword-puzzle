#include "Board.h"

/* Default Constructor
Constructs a board with heigth and width 0
@param height: important to set the height of the board
@param width: important to set the width of the board
*/
Board::Board()
{
	this->height = 0;
	this->width = 0;
}
//=====================================================================================
/* Constructor
Constructs a board with the heigth and width provided by the user, placing a dot in each element of the answer.matrix
@param height: important to set the height of the board
@param width: important to set the width of the board
*/
Board::Board(size_t heigth, size_t width, string dict_file_name)
{
	this->height = heigth;
	this->width = width;

	vector < vector <char>> v1(height, vector<char>(width));
	answer.matrix = v1;
	guess.matrix = v1;

	for (size_t lines = 0; lines < this->height; lines++)
		for (size_t columns = 0; columns < this->width; columns++)
			answer.matrix.at(lines).at(columns) = '.';

	set_name(dict_file_name);
	extract_dict();
}
//=====================================================================================
/*
Fills the matrix and the vector of the tray answer with the information in the file given
@param board_file_name: name of the file to extract from
*/
void Board::extract_board(string board_file_name)
{
	ifstream board_file;
	board_file.open(board_file_name);
	string dict_file_name;
	getline(board_file, dict_file_name);
	board_file.close();

	set_name(dict_file_name);
	extract_dict();

	board_file.open(board_file_name);
	string line;

	getline(board_file, line);
	size_t chars_to_matrix = line.size() + 4;
	board_file.seekg(chars_to_matrix); // +4 -> dois endl -> vai para o incio de linhas e colunas

									   // numero de colunas
	getline(board_file, line); // aqui nao é a 1 linha do ficheiro
	size_t columns = line.length() / 2; // por cada carater ha um espaço
										// numero de linhas
	board_file.seekg(chars_to_matrix);
	size_t lines = 0;

	while (!line.empty())
	{
		getline(board_file, line);
		lines++;
	}

	lines--;

	vector< vector<char> > matrix(lines, vector<char>(columns));
	this->answer.matrix = matrix;
	this->guess.matrix = matrix;
	this->height = lines;
	this->width = columns;

	board_file.seekg(chars_to_matrix);

	for (size_t q = 0; q < lines; q++)
	{
		getline(board_file, line);
		size_t e = 1;

		for (size_t w = 0; w < columns; w++)
		{
			this->answer.matrix.at(q).at(w) = line.at(e);
			e += 2;
		}
	}

	getline(board_file, line);
	getline(board_file, line);

	while (!board_file.eof())
	{
		board_word.posy = line.at(1);
		board_word.posx = line.at(2);
		board_word.orientation = line.at(3);
		board_word.word = line.substr(5);
		answer.used_words.push_back(board_word);
		getline(board_file, line);
	}
	board_file.close();
}
//=====================================================================================
/*
Changes the playing flag
@param playing: boolean value for the flag to be changed
*/
void Board::set_playing(bool playing)
{
	this->playing = playing;
}
//=====================================================================================
/*
Prints on the screen the content of one of the trays(guess in playing mode or answer otherwise)
If in playing mode, also prints one synonim of the words in the board
*/
void Board::show() const
{
	cout << endl;
	cout << " ";

	cout << " ";
	for (size_t columns = 0; columns < this->width; columns++)
	{
		setcolor(12);
		cout << " ";
		cout << char(columns + 97);
	}

	cout << endl;
	setcolor(0, 15);

	for (size_t lines = 0; lines < this->height; lines++)
	{
		setcolor(12, 0);

		cout << char(lines + 65);
		cout << " ";
		setcolor(0, 15);

		for (size_t columns = 0; columns < width; columns++)
		{

			cout << " ";
			if (playing)
			{
				if (guess.matrix.at(lines).at(columns) == '#')
					setcolor(15, 0);
				cout << guess.matrix.at(lines).at(columns);
			}	
			else
			{
				if (answer.matrix.at(lines).at(columns) == '#')
					setcolor(15, 0);
				cout << answer.matrix.at(lines).at(columns);
			}
			setcolor(0, 15);
		}
		cout << " ";
		cout << endl;

	}
	setcolor(15, 0);
	if (playing)
	{
		cout << endl << "---------VERTICAL---------" << endl;
		for (auto syns : synonims)
		{
			if (syns.at(syns.size()-1).orientation == 'V')
				cout << syns.at(syns.size() - 1).posy << syns.at(syns.size() - 1).posx << "  " << syns.at(syns.size() - 1).word << endl;
		}
		cout << endl << "--------HORIZONTAL---------" << endl;
		for (auto syns : synonims)
		{
			if (syns.at(syns.size() - 1).orientation == 'H')
				cout << syns.at(syns.size() - 1).posy << syns.at(syns.size() - 1).posx << "  " << syns.at(syns.size() - 1).word << endl;
		}
	}
}
//=====================================================================================
/*
Fills the matrix of the tray guess with the information of the tray answer, hiding all letters
*/
void Board::hide_words()
{
	for (size_t lines = 0; lines < this->height; lines++)
		for (size_t columns = 0; columns < this->width; columns++)
			if (answer.matrix.at(lines).at(columns) != '#')
				guess.matrix.at(lines).at(columns) = '.';
			else
				guess.matrix.at(lines).at(columns) = '#';
}
//=====================================================================================
/*
Interacts with the user, determining what the user wants to change in the board and calling the needed funtions
Returns false if the user doesn't want to do a new operation, returns true otherwise
@param help_counts: important to increment in case of help
*/
bool Board::new_operation(size_t &help_counts)
{
	bool invalid_input = true;

	do
	{
		cout << "Position ( LCD / CTRL-Z = stop ) ? ";

		cin >> board_word.posy >> board_word.posx; // optei por cin porque so os sinonimos é que tem com espaço... (ver se resolve qq input error...)
		cin >> board_word.orientation;

		if (cin.fail() && cin.eof())
		{
			cin.clear();
			return false;
		}
		else
		{
			cin.clear();
			cin.ignore(1000, '\n');

			board_word.orientation = toupper(board_word.orientation);

			if ((unsigned int)board_word.posx >= 97 && (unsigned int)board_word.posx <= 122 && (unsigned int)board_word.posy >= 65 && (unsigned int)board_word.posy <= 90
				&& (board_word.orientation == 'V' || board_word.orientation == 'H'))
			{
				if (playing)     
				{
					for (size_t i = 0; i < answer.used_words.size(); i++)
						if (answer.used_words.at(i).posx == board_word.posx && answer.used_words.at(i).posy == board_word.posy && answer.used_words.at(i).orientation == board_word.orientation)
							invalid_input = false;
				}
				else
					invalid_input = false;
			}
		}
		if (invalid_input)
			cout << "Invalid position" << endl;
		cout << endl;
	} while (invalid_input);

	cout << "Word ( - =  remove / ? = help ) .. ? ";
	cin >> board_word.word;

	if (cin.fail())
	{
		if (cin.eof())
		{
			cin.clear();
		}
		else
		{
			cin.clear();
			cin.ignore(1000, '\n');
		}
	}
	else
	{
		std::transform(board_word.word.begin(), board_word.word.end(), board_word.word.begin(), ::toupper);

		if (board_word.word == "-")
			remove_word();
		else if (board_word.word == "?")
		{
			if (playing)
				help_player(help_counts);
			else
				help_creator();
		}
		else
		{
			if (!(word_valid() && word_not_repeted()))
			{
				cout << "The word does not exist in the dictionary or it is already in the board " << endl;
			}
			else if (word_fit())
				add_word();
			else
			{
				cerr << "The word does not fit in the selected position " << endl;
			}
		}
	}

	return true;
}
//=====================================================================================
/*
Fills the matrix of the tray answer with '#', where there are '.'
*/
void Board::fill_spaces_end()
{
	for (size_t lines = 0; lines < height; lines++)
		for (size_t columns = 0; columns < width; columns++)
		{
			if (answer.matrix.at(lines).at(columns) == '.')
				answer.matrix.at(lines).at(columns) = '#';
		}
}
//=====================================================================================
/*
Calls the function to determine the name of the next board and the function to save the content of the tray answer
*/
void Board::save_finished_board()
{
	ofstream save_file;
	string save_file_name = name_final_file();
	auxiliar_file(save_file_name);
}
//=====================================================================================
/*
Asks the user the name of the file to save the unfinished tray answer and calls the function to save it
*/
void Board::save_unfinished_board()
{
	ofstream save_file;
	string save_file_name;
	
	bool invalid_name = true;

	while (invalid_name)
	{
		cout << "Board file name to save the unfinished puzzle? " << endl;
		cin >> save_file_name;

		if (cin.fail())
		{
			if (cin.eof())
			{
				cin.clear();
			}
			else
			{
				cin.clear();
				cin.ignore(1000, '\n');
			}
		} 
		else invalid_name = false;

		if (invalid_name)
			cout << "Invalid Name to Save File." << endl;
	}

	auxiliar_file(save_file_name);
}
//=====================================================================================
/*
Does a final check to ensure that all the words on the tray(which one depends on the playing flag
*/
bool Board::final_check()
{
	vector<string> new_vector;
	bool still_same_word = false;;
	size_t word_size = 0;
	string word;
	
	if (playing)
	{
		for (size_t l = 0; l < height; l++)
		{
			for (size_t c = 0; c < width; c++)
			{
				if (guess.matrix.at(l).at(c) >= 65 && guess.matrix.at(l).at(c) <= 90)
				{
					still_same_word = true;
					word_size++;
					word += guess.matrix.at(l).at(c);
				}
				else
				{
					if (word_size > 1)
					{
						new_vector.push_back(word);
					}
					word_size = 0;
					word = "";
					still_same_word = false;
				}
			}

			if (word_size > 1)
			{
				new_vector.push_back(word);
				word_size = 0;
				word = "";
				still_same_word = false;
			}
		}

		for (size_t c = 0; c < width; c++)
		{
			for (size_t l = 0; l < height; l++)
			{
				if (guess.matrix.at(l).at(c) >= 65 && guess.matrix.at(l).at(c) <= 90)
				{
					still_same_word = true;
					word_size++;
					word += guess.matrix.at(l).at(c);
				}
				else
				{
					if (word_size > 1)
					{
						new_vector.push_back(word);
					}
					word_size = 0;
					word = "";
					still_same_word = false;
				}
			}

			if (word_size > 1)
			{
				new_vector.push_back(word);
				word_size = 0;
				word = "";
				still_same_word = false;
			}
		}
	}
	else
	{
		for (size_t l = 0; l < height; l++)
		{
			for (size_t c = 0; c < width; c++)
			{
				if (answer.matrix.at(l).at(c) >= 65 && answer.matrix.at(l).at(c) <= 90)
				{
					still_same_word = true;
					word_size++;
					word += answer.matrix.at(l).at(c);
				}
				else
				{
					if (word_size > 1)
					{
						new_vector.push_back(word);
					}
					word_size = 0;
					word = "";
					still_same_word = false;
				}
			}

			if (word_size > 1)
			{
				new_vector.push_back(word);
				word_size = 0;
				word = "";
				still_same_word = false;
			}
		}

		for (size_t c = 0; c < width; c++)
		{
			for (size_t l = 0; l < height; l++)
			{
				if (answer.matrix.at(l).at(c) >= 65 && answer.matrix.at(l).at(c) <= 90)
				{
					still_same_word = true;
					word_size++;
					word += answer.matrix.at(l).at(c);
				}
				else
				{
					if (word_size > 1)
					{
						new_vector.push_back(word);
					}
					word_size = 0;
					word = "";
					still_same_word = false;
				}
			}

			if (word_size > 1)
			{
				new_vector.push_back(word);
				word_size = 0;
				word = "";
				still_same_word = false;
			}
		}
	}

	if (new_vector.size() > 2)
	{
		for (size_t iv1 = 0; iv1 != new_vector.size(); iv1++)
		{
			if (iv1 != new_vector.size() - 1)
			{
				for (size_t iv2 = iv1+1; iv2 != new_vector.size(); iv2++)
				if (new_vector.at(iv1) == new_vector.at(iv2))
					return false;
			}
			board_word.word = new_vector.at(iv1);
			if (!word_valid())
				return false;
		}
	}
	return true;
}
//=====================================================================================
/*
Checks if the tray guess matches the answer guess and returns true in that case, returning false otherwise
*/
bool Board::correct_guess()
{
	for (size_t l = 0; l < height; l++)
		for (size_t c = 0; c < width; c++)
		{
			if (guess.matrix.at(l).at(c) != answer.matrix.at(l).at(c))
				return false;
		}
	return true;
}
//=====================================================================================
/*
Determines the name of the next ile to save the answer tray
*/
string Board::name_final_file()
{
	ifstream file;
	size_t counter = 0;
	string name_file = "B00" + to_string(counter) + ".txt";
	file.open(name_file);
	while (!file.fail())
	{
		file.close();
		counter++;

		if (counter < 10)
			name_file = "B00" + to_string(counter) + ".txt";
		else if (counter < 100)
			name_file = "B0" + to_string(counter) + ".txt";
		else
			name_file = "B" + to_string(counter) + ".txt";

		file.open(name_file);
		

	}
	file.close();
	return name_file;
}
//=====================================================================================
/*
Gives a random synonim to each word of the board
*/
void Board::set_synonims()
{
	vector <vector <entry>> synonims;
	for (auto word : answer.used_words)
	{
		entry synonim;
		synonim = word;
		vector <string> synonims_vector = get_synonims(word.word);
		size_t synonim_index = rand() % synonims_vector.size();
		synonim.word = synonims_vector.at(synonim_index);
		vector <entry> syns;
		syns.push_back(synonim);
		synonims.push_back(syns);
	}
	this->synonims = synonims;
}
//=====================================================================================
/*
Returns true if the user entered word exists on the dictionary and false otherwise
*/
bool Board::word_valid()const
{
	return (find_word(board_word.word));
}
//=====================================================================================
/*
Returns false if the user entered word is repeted on a tray(depending on the playing flag) and true otherwise
*/
bool Board::word_not_repeted() const
{
	if (playing)
	{
		for (auto x : guess.used_words)
			if (x.word == board_word.word)
				return false;
	}
	else
	{
		for (auto x : answer.used_words)
			if (x.word == board_word.word)
				return false;
	}
	

	return true;
}
//=====================================================================================
/*
Determines if the user entered word fits on a tray
Returns true if the user entered word word fits on a tray(depending on the playing flag) and false otherwise
*/
bool Board::word_fit() const
{
	size_t size_of_word = board_word.word.length() - 1;
	unsigned int y = (unsigned int)board_word.posy - 65;
	unsigned int x = (unsigned int)board_word.posx - 97;

	if (playing)
	{
		if (board_word.orientation == 'V' && (signed int)y < (signed int)(this->height - size_of_word))
		{
			if (y != 0 && guess.matrix.at(y - 1).at(x) != '.' && guess.matrix.at(y - 1).at(x) != '#')
				return false;
			size_t i = 0;
			while (i <= size_of_word)
			{

				if ((guess.matrix.at(y).at(x) == '.' || guess.matrix.at(y).at(x) == board_word.word.at(i)) && (guess.matrix.at(y).at(x) != '#'))
				{
					y++;
					i++;
				}
				else
					return false;
			}
			if (y != (this->height) && guess.matrix.at(y).at(x) != '#')
				return false;
			else
				return true;

		}
		else if (board_word.orientation == 'H' && (signed int)x < (signed int)(this->width - size_of_word))
		{
			if (x != 0 && guess.matrix.at(y).at(x - 1) != '.' && guess.matrix.at(y).at(x - 1) != '#')
				return false;
			size_t i = 0;
			while (i <= size_of_word)
			{
				if ((guess.matrix.at(y).at(x) == '.' || guess.matrix.at(y).at(x) == board_word.word.at(i)) && (guess.matrix.at(y).at(x) != '#'))
				{
					x++;
					i++;
				}
				else
					return false;
			}
			if (x != (this->width) && guess.matrix.at(y).at(x) != '#')
				return false;
			else
				return true;
		}
		else
			return false;
	}
	else
	{
		if (board_word.orientation == 'V' && (signed int)y < (signed int)(this->height - size_of_word))
		{
			if (y != 0 && answer.matrix.at(y - 1).at(x) != '.' && answer.matrix.at(y - 1).at(x) != '#')
				return false;
			size_t i = 0;
			while (i <= size_of_word)
			{

				if ((answer.matrix.at(y).at(x) == '.' || answer.matrix.at(y).at(x) == board_word.word.at(i)) && (answer.matrix.at(y).at(x) != '#'))
				{
					y++;
					i++;
				}
				else
					return false;
			}
			if (y != (this->height) && answer.matrix.at(y).at(x) != '.' && answer.matrix.at(y).at(x) != '#')
				return false;
			else
				return true;

		}
		else if (board_word.orientation == 'H' && (signed int)x < (signed int)(this->width - size_of_word))
		{
			if (x != 0 && answer.matrix.at(y).at(x - 1) != '.' && answer.matrix.at(y).at(x - 1) != '#')
				return false;
			size_t i = 0;
			while (i <= size_of_word)
			{
				if ((answer.matrix.at(y).at(x) == '.' || answer.matrix.at(y).at(x) == board_word.word.at(i)) && (answer.matrix.at(y).at(x) != '#'))
				{
					x++;
					i++;
				}
				else
					return false;
			}
			if (x != (this->width) && answer.matrix.at(y).at(x) != '.' && answer.matrix.at(y).at(x) != '#')
				return false;
			else
				return true;
		}
		else
			return false;
	}
}
//=====================================================================================
/*
Adds the user entered word to a tray(depending on the playing flag)
*/
void Board::add_word()
{
	add_to_matrix();
	if (playing)
		guess.used_words.push_back(board_word);
	else
		answer.used_words.push_back(board_word);
}
//=====================================================================================
/*
Adds the user entered word to the matrix of a tray(depending on the playing flag)
*/
void Board::add_to_matrix()
{
	unsigned int y = (unsigned int)board_word.posy - 65;
	unsigned int x = (unsigned int)board_word.posx - 97;
	char direction = board_word.orientation;

	if (direction == 'V')
	{
		if (y != 0)
		{
			if (playing)
				guess.matrix.at(y - 1).at(x) = '#';
			else
				answer.matrix.at(y - 1).at(x) = '#';

			for (size_t i = 0; i < board_word.word.length(); i++)
			{
				if (playing)
					guess.matrix.at(y).at(x) = board_word.word.at(i);
				else
					answer.matrix.at(y).at(x) = board_word.word.at(i);
				y++;
			}

			if (y != height) // ja acabou no ciclo n preciso de y+1
			{
				if (playing)
					guess.matrix.at(y).at(x) = '#';
				else
					answer.matrix.at(y).at(x) = '#';
			}
		}

		else
		{
			for (size_t i = 0; i < board_word.word.length(); i++)
			{
				if (playing)
					guess.matrix.at(y).at(x) = board_word.word.at(i);
				else
					answer.matrix.at(y).at(x) = board_word.word.at(i);
				y++;
			}

			if (y != height)
			{
				if (playing)
					guess.matrix.at(y).at(x) = '#';
				else
					answer.matrix.at(y).at(x) = '#';
			}
		}
	}
	else if (direction == 'H')
	{
		if (x != 0)
		{
			if (playing)
				guess.matrix.at(y).at(x - 1) = '#';
			else
				answer.matrix.at(y).at(x - 1) = '#';
			for (size_t i = 0; i < board_word.word.length(); i++)
			{
				if (playing)
					guess.matrix.at(y).at(x) = board_word.word.at(i);
				else
					answer.matrix.at(y).at(x) = board_word.word.at(i);
				x++;

			}

			if (x != width)
			{
				if (playing)
					guess.matrix.at(y).at(x) = '#';
				else
					answer.matrix.at(y).at(x) = '#';
			}	
		}
		else
		{
			for (size_t i = 0; i < board_word.word.length(); i++)
			{
				if (playing)
					guess.matrix.at(y).at(x) = board_word.word.at(i);
				else
					answer.matrix.at(y).at(x) = board_word.word.at(i);
				x++;
			}

			if (x != width)
			{
				if (playing)
					guess.matrix.at(y).at(x) = '#';
				else
					answer.matrix.at(y).at(x) = '#';
			}
		}
	}
}
//=====================================================================================
/*
Removes the user entered word of a tray(depending on the playing flag)
*/
void Board::remove_word()
{
	if (playing)
	{
		if (guess.used_words.size() > 0)
		{
			for (vector<entry>::iterator itv = guess.used_words.begin(); itv != guess.used_words.end(); itv++)
			{
				if (itv->posx == board_word.posx && itv->posy == board_word.posy && itv->orientation == board_word.orientation)
				{
					guess.used_words.erase(itv);
					clear_board();
					for (auto words : guess.used_words)
					{
						board_word = words;
						add_to_matrix();
					}
					return;
				}

			}
			cout << "That position has no word to be removed." << endl;
			return;
		}
		else
		{
			cout << "The board is empty." << endl;
		}
	}
	else
	{
		if (answer.used_words.size() > 0)
		{
			for (vector<entry>::iterator itv = answer.used_words.begin(); itv != answer.used_words.end(); itv++)
			{
				if (itv->posx == board_word.posx && itv->posy == board_word.posy && itv->orientation == board_word.orientation)
				{
					answer.used_words.erase(itv);
					clear_board();
					for (auto words : answer.used_words)
					{
						board_word = words;
						add_to_matrix();
					}
					return;
				}
			}
			cout << "That position has no word to be removed." << endl;
			return;
		}
		else
		{
			cout << "The board is empty." << endl;
		}
	}
}
//=====================================================================================
/*
Clears the matrix of a tray(depending on the playing flag)
*/
void Board::clear_board()
{
	for (size_t lines = 0; lines < this->height; lines++)
		for (size_t columns = 0; columns < this->width; columns++)
			if (playing)
			{
				if (guess.matrix.at(lines).at(columns) != '#')
					guess.matrix.at(lines).at(columns) = '.';
			}
			else
				answer.matrix.at(lines).at(columns) = '.';
}
//=====================================================================================
/*
Prints on the screen all the words that fit on the position given by the user
*/
void Board::help_creator()
{
	size_t counter = 0;
	cout << "Avaiable words:" << endl;
	vector <string> new_vector = get_words();

	for (auto words : new_vector)
	{
		board_word.word = words;
		if (word_fit() && word_not_repeted())
		{
			cout << setw(15) << board_word.word;
			counter++;
			if (counter % 4 == 0)
				cout << endl << setw(0);
		}
	}
	if (counter == 0)
		cout << "No words fit there." << endl;
}
//=====================================================================================
/*
Gives a word a new synonim
@param help_counts: important to increment the number of helps of the player
*/
void Board::help_player(size_t &help_counts)
{
	for (size_t i = 0; i < answer.used_words.size(); i ++)
	{
		if (answer.used_words.at(i).posx == board_word.posx &&
			answer.used_words.at(i).posy == board_word.posy &&
			answer.used_words.at(i).orientation == board_word.orientation)
		{
			vector <string> synonims_vector = get_synonims(answer.used_words.at(i).word);
			if (synonims_vector.size() > synonims.at(i).size())
			{
				bool same_synonim;
				entry synonim;
				do
				{
					same_synonim = false;
					size_t synonim_index = rand() % synonims_vector.size();
					synonim = answer.used_words.at(i);
					synonim.word = synonims_vector.at(synonim_index);

					for (auto syns : synonims.at(i))
					{
						if (syns.word == synonim.word)
						{
							same_synonim = true;
							break;
						}
					}
				} while (same_synonim);
				this->synonims.at(i).push_back(synonim);
				help_counts++;
				return;
			}
			else
			{
				cout << "No more synonims available." << endl;
				return;
			}
		}
	}
}
//=====================================================================================
/*
Saves the content of the tray answer in a given file
@param save_file_name: name of the file to save to
*/
void Board::auxiliar_file(string save_file_name)
{
	ofstream save_file;
	save_file.open(save_file_name);
	save_file << get_name() << endl << endl;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			save_file << " ";
			save_file << answer.matrix.at(i).at(j);
		}
		save_file << endl;
	}

	save_file << endl;

	for (size_t i = 0; i < answer.used_words.size(); i++)
	{
		save_file << " " << answer.used_words.at(i).posy << answer.used_words.at(i).posx <<
			answer.used_words.at(i).orientation << " " << answer.used_words.at(i).word << endl;
	}

	save_file.close();
}
//=====================================================================================