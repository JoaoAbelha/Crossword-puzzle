#ifndef BOARD_H
#define BOARD_H
#include "Dictionary.h"
#include "Color.h"

class Board: public Dictionary
{

public:
	//Constructors
	Board();
	Board(size_t height, size_t width, string dict_file_name);
	//
	void extract_board(string board_file_name);
	void set_playing(bool playing);
	void show() const;
	void hide_words();
	bool new_operation(size_t &help_counts);
	void fill_spaces_end();
	void save_finished_board();
	void save_unfinished_board();
	bool final_check();
	bool correct_guess();
	string name_final_file();
	void set_synonims();
		
private:

	bool word_valid() const;
	bool word_not_repeted() const;
	bool word_fit() const;
	void add_word();
	void add_to_matrix();
	void remove_word();
	void clear_board();
	void help_creator();
	void help_player(size_t &help_counts);
	void auxiliar_file(string save_file_name);

	unsigned int height; //height of the board
	unsigned int width;	//width of the board
	bool playing = false; //mode: true = playing, false = creating
	struct entry {
		string word; //the word itself
		char posx, posy; //inicial posicion of a word
		char orientation; //orientation of a word
	} board_word; //user entered word
	struct tray {
		vector <vector <char>> matrix; //matrix of letters
		vector <entry> used_words; //vector with all the words in a tray
	} answer, guess; //2 trays: answer is the tray created, guess is the tray of a player guess
	vector <vector <entry>> synonims; //vector with one or more(in case the player needed help) synonims for each word on the board
};
#endif