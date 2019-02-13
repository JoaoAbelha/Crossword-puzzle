#include "Board.h"

int main()
{
	cout << "CROSSWORD PUZZLE CREATOR" << endl;
	cout << "=============================================" << endl << endl;
	cout << "INSTRUCTIONS:" << endl <<endl;
	cout << "+===============================================================================================================+\n";
	cout << "-> You may choose one of the option given,you can either create a puzzle, resume it or exit the program;" << endl;
	cout << "-> The board created must have the maximum width and length of 26;" << endl;
	cout << "-> When creating a puzzle you can resume it later with option 2;" << endl;
	cout << "-> You may write the name of the dictionary;" << endl;
	cout << "-> The words in the board must be in the dictionary;" << endl;
	cout << "-> You will be noticed if any error input is made;" << endl;
	cout << "-> If you want to resume the programm, you may provide the name of the file;" << endl;
	cout << "-> If the board is completely finished, it will be saved automatically with the name bXXX.txt, where XXX stands \nfor the number of the file created;" << endl;
	cout << "-> Position (LcD / Ctrl-Z = stop );" << endl;
	cout << "; Note that:" << endl;
	cout << "  LCD stands for line, column and direction, respectively;" << endl;
	cout << "  In case you are having difficulties choosing a word, instead of the word, write \"?\";" << endl;
	cout << "-> If you want to remove a placed word in the board, instead of writing the word write \"-\";" << endl;
	cout << "-> You can get back to the main menu by using \"Ctrl-Z\";" << endl;
	cout << "-> In the end, all the words in the board are going to be checked if they are valid or not;" << endl;
	cout << "-> Finally, enjoy!" << endl;
	cout << "+===============================================================================================================+\n";

	int option;
	do
	{
		bool invalid_option = true;

		cout << "---------------------------------------------" << endl << endl;
		cout << "OPTIONS:" << endl;
		cout << "1-Create Puzzle" << endl;
		cout << "2-Resume Puzzle" << endl;
		cout << "0-Exit" << endl;

		while (invalid_option)
		{
			cout << "Option ? ";
			cin >> option;

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
			else if (option == 1 || option == 2 || option == 0)
			{
				invalid_option = false;
			}
			if (invalid_option)
				cout << "Invalid Option" << endl;
		}

		size_t helps = 0;

		switch (option)
		{
			case 1:
			{
				cout << endl << "---------------------------------------------" << endl;
				cout << "CREATE PUZZLE" << endl;
				cout << "---------------------------------------------" << endl;

				ifstream dict_file;

				string dict_file_name;
				cout << "Dictionary file name ? ";
				cin >> dict_file_name;
				dict_file.open(dict_file_name);

				if (dict_file.fail())
				{
					cerr << "Input file opening failed.\n";
				}
				else
				{
					size_t height, width;
					bool valid_input = false;
					do
					{
						cout << "Board size (lines columns) ? ";
						if (cin >> height >> width)
						{
							if (height >= 1 && height <= 26 && width >= 1 && width <= 26)
							{
								if (height != 1 || width != 1)
									valid_input = true;
							}
							cin.clear();
							cin.ignore(1000, '\n');
						}
						else
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
						if (!valid_input)
							cout << "Invalid Size." << endl;

					} while (!valid_input);

					Board B(height, width, dict_file_name);

					bool invalid_board = true;
					do
					{
						do
						{
							B.show();
						} while (B.new_operation(helps));

						cout << "1-End and Save." << endl;
						cout << "2-Save to Finish later." << endl;
						cout << "3-Discard puzzle." << endl;

						int option2;

						bool invalid_option = true;

						while (invalid_option)
						{
							cout << "Option ? ";
							cin >> option2;

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
							else if (option2 == 1 || option2 == 2 || option2 == 3)
							{
								invalid_option = false;
							}
							if (invalid_option)
								cout << "Invalid Option" << endl;
						}

						switch (option2)
						{
							case 1:
							{
								if (B.final_check())
								{
									invalid_board = false;
									B.fill_spaces_end();
									B.save_finished_board();
									cout << "The board was saved successfully" << endl;
								}
								else
									cout << "The board has invalid words. Be carefull with intersections." << endl;
								break;
							}
							case 2:
							{
								B.save_unfinished_board();
								invalid_board = false;
								break;
							}
							case 3:
							{
								invalid_board = false;
							}
						}
					} while (invalid_board);
				}

				break;
			}
			case 2:
			{
				ifstream board_file;
				
				string board_file_name;
				cout << "Board file name ? ";
				cin >> board_file_name;
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
					board_file.open(board_file_name);

					if (board_file.fail())
					{
						cerr << "Input file opening failed.\n";
						break; // go back to menu if file failed to open - pode não existir nenhum ficheiro
					}
					else
						board_file.close();

					Board B;
					B.extract_board(board_file_name);

					bool invalid_board = true;
					do
					{
						do
						{
							B.show();
						} while (B.new_operation(helps));

						cout << "1-End and Save." << endl;
						cout << "2-Save to Finish later." << endl;
						cout << "3-Discard puzzle." << endl;

						int option2;

						bool invalid_option = true;

						while (invalid_option)
						{
							cout << "Option ? ";
							cin >> option2;

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
							else if (option2 == 1 || option2 == 2 || option2 == 3)
							{
								invalid_option = false;
							}
							if (invalid_option)
								cout << "Invalid Option" << endl;
						}

						switch (option2)
						{
							case 1:
							{
								if (B.final_check())
								{
									invalid_board = false;
									B.fill_spaces_end();
									B.save_finished_board();
									cout << "The board was saved successfully" << endl;
								}
								else
									cout << "The board is invalid." << endl;
								break;
							}
							case 2:
							{
								B.save_unfinished_board();
								invalid_board = false;
								break;
							}
							case 3:
							{
								invalid_board = false;
							}
						}
					} while (invalid_board);
				}
				break;
			}
		}
	} while (option != 0);
	return 0;
}