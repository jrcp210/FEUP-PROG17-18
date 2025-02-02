#include"Dictionary.h"
#include"Board.h"

using namespace std;

bool allWordsValidity(Board *boardP, Dictionary *dictP)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	vector<string> verticalWords = boardP->verifyVertical(); //vector with all vertical words formed in the board
	vector<string> horizontalWords = boardP->verifyHorizontal(); //vector with all horizontal words formed in the board
	string errorMessageHeadline = "\nThe word %s is not a valid word in the dictionary\n";

	for (unsigned int i = 0; i < verticalWords.size(); i++) //loop in vector
	{
		if (!dictP->headlineExists(verticalWords[i])) //if word doesnt belong in the dictionary
		{
			SetConsoleTextAttribute(hConsole, 244);
			printf(errorMessageHeadline.c_str(), verticalWords[i].c_str()); //error message
			SetConsoleTextAttribute(hConsole, 15);
			return false;
		}
	}
	for (unsigned int i = 0; i < horizontalWords.size(); i++) //loop in vector
	{
		if (!dictP->headlineExists(horizontalWords[i])) //if word doesnt belong in the dictionary
		{
			SetConsoleTextAttribute(hConsole, 244);
			printf(errorMessageHeadline.c_str(), horizontalWords[i].c_str()); //error message
			SetConsoleTextAttribute(hConsole, 15);
			return false;
		}
	}
	SetConsoleTextAttribute(hConsole, 15); //normal color
	return true;
}
 
bool checkValidity(Dictionary *dictP, Board *boardP, string word, string position) 
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//check the existance of the word
	if (!dictP->headlineExists(word)) //word doesnt exist
	{
		SetConsoleTextAttribute(hConsole, 15);
		return false;
	}

	if (!boardP->unusedWord(word)) //word used
	{
		SetConsoleTextAttribute(hConsole, 15);
		return false;
	}
	if (!boardP->checkSpace4Word(word, position)) //no space for the word
	{
		SetConsoleTextAttribute(hConsole, 15);
		return false;
	}
	return true;
}

void helpInsertWord(string position, Board *boardP, Dictionary *dictP) 
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	bool repeatNoMatchingWords = true;
	while (repeatNoMatchingWords)
	{
		repeatNoMatchingWords = false;
		string wordToCreate;
		string errorMessageLength = "A word with so many letters don't fit!\n\nIf the help is no longer needed, press CTRL-Z";
		string errorMessageInput = "Only a numerical value is valid in here! Try again";
		string errorMessageBoolInput = "Only a 0 or a 1 is a valid option here! Try again";
		string returnMessage = "You chose to leave the help!";
		string warningMessageNoWords = "I'm sorry but no words were found that match the place you chose...\nDo you want to try again?\n";
		string errorInputWord = "You miswrote the word you chose!\nTry Again: ";
		bool errorInWordSize, errorInput;
		//input number of letters, input error robust (checks for space in the board and the right input in first question) and creates word and checks for space
		do 
		{
			//inicialize flags
			errorInWordSize = false;
			errorInput = false;
			SetConsoleTextAttribute(hConsole, 15);
			//size of the word
			cout << "Insert the number of letters you want the word to have (CTRL-Z to leave the help) : ";
			int lettersNumber; cin >> lettersNumber;
			SetConsoleTextAttribute(hConsole, 244);
			if (cin.eof())
			{
				cin.clear();
				cout << returnMessage;
				SetConsoleTextAttribute(hConsole, 15);
				return;
			}
			if (cin.fail()) 
			{
				cin.clear();
				cin.ignore(1000000, '\n');
				cout << errorMessageInput;
				SetConsoleTextAttribute(hConsole, 15);
				errorInput = true;
				continue;
			}
			
			SetConsoleTextAttribute(hConsole, 15);
			//word creation with the asked size
			for (int i = 0; i < lettersNumber; i++)
				wordToCreate += "?";
																																																																																																																																																																																																																																																																																						//is there any space available?
			if (!boardP->checkSpace4Word(wordToCreate, position)) errorInWordSize = true;
		} while (errorInput || errorInWordSize);

		boardP->getLettersRight(wordToCreate, position);
		vector<string> resultWord = dictP->matchingWords(wordToCreate);

		if (resultWord.size() == 0) 
		{
			//waring message
			SetConsoleTextAttribute(hConsole, 244);
			cout << warningMessageNoWords;
			SetConsoleTextAttribute(hConsole, 15);
			//choose what to do
			bool repeat;
			cout << "0 - Finish help\n1 - Try again\n\tAnswer -> ";
			cin >> repeat;
			//input error robustness
			while (cin.fail()) 
			{
				cin.clear();
				cin.ignore(1000000, '\n');
				SetConsoleTextAttribute(hConsole, 244);
				cout << errorMessageBoolInput;
				SetConsoleTextAttribute(hConsole, 15);
				cout << "\n\tAnswer -> ";
				cin >> repeat;
			}
			//repeat the funciton
			if (repeat)
			{
				repeatNoMatchingWords = true; continue;
			}
			//leave the function
			else if (!repeat) 
			{
				cout << returnMessage; return;
			}
		}
		else {
			//show the content of the vector with the resulting words
			cout << endl;
			for (unsigned int i = 0; i < resultWord.size(); i++)
				cout << "\t" << resultWord[i];
			cout << "\n\nChoose one of them to add to the board [CTRL-Z if you want to cancel help]: ";
			bool wordMatchesVector;
			string answer;
			do 
			{
				cin.clear();
				wordMatchesVector = false;
				cin >> answer;
				if (cin.eof())
				{
					cin.clear(); cout << returnMessage;  return;
				}
				if (cin.fail()) 
				{
					cin.clear();
					cin.ignore(1000000, '\n');
					SetConsoleTextAttribute(hConsole, 244);
					cout << errorInputWord;
					SetConsoleTextAttribute(hConsole, 15);
					continue;
				}
				
				transform(answer.begin(), answer.end(), answer.begin(), ::toupper); //upper case
				for (unsigned int i = 0; i < resultWord.size(); i++)
					if (resultWord[i] == answer)
					{	//searches for the word in the vector, because all the checking was done before 
						wordMatchesVector = true;	//and a mistaken word here would break the "security" of the board
						break;
					}
				
			} while (cin.fail() || !wordMatchesVector);
			boardP->addWord(answer, position);
		}
	}
}

bool finishingCreate(Board *boardP, Dictionary *dictP) 
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	bool finishedBoard, errorInput;

	do
	{
		string errorMessageResume = "\nThat is not a valid answer to this question! Try again\n";
		errorInput = false;
		//cout << "Is your board finished (1) or you will continue later(0) ?\n\tAnswer [1 or 0] -> ";
		cout << "What do you want to do:\n\tSave finished board (1)\n\tSave unfinished board (0)\n\tDon't save board at all (CTRL-Z)\n\t\tAnswer -> ";
		cin >> finishedBoard;

		if (cin.eof())
		{
			cin.clear();
			cin.ignore(1000, '\n');
			return true;
		}

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(10000, '\n');
			SetConsoleTextAttribute(hConsole, 244);
			cout << errorMessageResume;
			SetConsoleTextAttribute(hConsole, 15);
			errorInput = true;
		}	
	} while (errorInput);

	if (finishedBoard)
	{		
		bool validity = allWordsValidity(boardP, dictP);
		if (validity)
		{
			boardP->hashtagFill();
			boardP->extraction(dictP->fileNameInput);
		
		}
		else 
		{
			bool finishExtraction;
			do 
			{
				string errorMessageResume = "\nThat is not a valid answer to this question! Try again\n";
				errorInput = false;

				cout << "Do you want to:\n\t finish with the wrong words (1)\n\t to replace the wrong words (0)\n\t\tAnswer -> ";
				cin >> finishExtraction;

				if (cin.fail())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					SetConsoleTextAttribute(hConsole, 244);
					cout << errorMessageResume;
					SetConsoleTextAttribute(hConsole, 15);
					errorInput = true;
				}

				if (cin.eof())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					errorInput = true;
					SetConsoleTextAttribute(hConsole, 244);
					cout << errorMessageResume;
					SetConsoleTextAttribute(hConsole, 15);
				}
			} while (errorInput);

			if (finishExtraction)
			{
				boardP->extraction(dictP->fileNameInput); //redundante?????
			}
			else return false;
		}
	}
	else
	{
		boardP->extraction(dictP->fileNameInput);
	}
	return true;
}

bool finishingCreate(Board *boardP, Dictionary *dictP, string inputFile)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	bool finishedBoard, errorInput;

	do
	{
		string errorMessageResume = "\nThat is not a valid answer to this question! Try again\n";
		errorInput = false;
		//cout << "Is your board finished (1) or you will continue later(0) ?\n\tAnswer [1 or 0] -> ";
		cout << "What do you want to do:\n\tSave finished board (1)\n\tSave unfinished board (0)\n\tDon't save board at all (CTRL-Z)\n\t\tAnswer -> ";
		cin >> finishedBoard;

		if (cin.eof()) //if ctrl z, dont save the board
		{
			cin.clear();
			cin.ignore(1000, '\n');
			return true;
		}

		if (cin.fail()) //if fail, repeats the loop
		{
			cin.clear();
			cin.ignore(10000, '\n');
			SetConsoleTextAttribute(hConsole, 244);
			cout << errorMessageResume;
			SetConsoleTextAttribute(hConsole, 15);
			errorInput = true;
		}
	} while (errorInput);

	if (finishedBoard)
	{
		bool validity = allWordsValidity(boardP, dictP);
		if (validity)
		{
			boardP->hashtagFill();
			boardP->reExtraction(dictP->fileNameInput, inputFile);

		}
		else
		{
			bool finishExtraction;
			do
			{
				string errorMessageResume = "\nThat is not a valid answer to this question! Try again\n";
				errorInput = false;

				cout << "Do you want to:\n\t finish with the wrong words (1)\n\t to replace the wrong words (0)\n\t\tAnswer -> ";
				cin >> finishExtraction;

				if (cin.fail())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					SetConsoleTextAttribute(hConsole, 244);
					cout << errorMessageResume;
					SetConsoleTextAttribute(hConsole, 15);
					errorInput = true;
				}

				if (cin.eof())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					errorInput = true;
					SetConsoleTextAttribute(hConsole, 244);
					cout << errorMessageResume;
					SetConsoleTextAttribute(hConsole, 15);
				}
			} while (errorInput);

			if (finishExtraction)
			{
				boardP->reExtraction(dictP->fileNameInput, inputFile);
			}
			else return false;
		}
	}
	else
	{
		boardP->reExtraction(dictP->fileNameInput, inputFile);
	}
	return true;
}

void puzzleCreate()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//dictionary creation and extraction
	Dictionary dict;
	Dictionary *dictA = &dict;
	bool errorOpeningFile;
	do
	{
		errorOpeningFile = false;
		cin.clear(); //reseting the flag
		string errorMessageFileInput = "That input is not valid! Try again\n";
		cout << "Dictionary file name ? ";
		cin >> dict.fileNameInput;
		if (cin.fail()) //if fail
		{
			cin.ignore(1000000, '\n');
			SetConsoleTextAttribute(hConsole, 244);
			cout << errorMessageFileInput;
			SetConsoleTextAttribute(hConsole, 15);
			continue;
		}
		if (cin.eof()) //if ctrl z
		{
			cin.ignore(100, '\n');
			SetConsoleTextAttribute(hConsole, 244);
			cout << errorMessageFileInput;
			SetConsoleTextAttribute(hConsole, 15);
			continue;
		}
		if (!dict.loadToProgram()) errorOpeningFile = true;

	} while (cin.fail() || errorOpeningFile); //if error or not opened


	//creation of the board
	int lines, columns;

	//ask the user how many lines and columns
	//error proof, the do while loop will work when there are errors in the input and the user has more chances

	bool cinError;
	do
	{									
		string errorMessage26 = "\nI'm sorry but that number is too big! Write that again.\n";
		string errorMessageCinFail = "\nThose aren't valid sizes for the board! Try again...\n";
		cinError = false;
		cout << "Board size[max size=26] (lines columns)? "; cin >> lines >> columns;

		SetConsoleTextAttribute(hConsole, 244);
		if (cin.fail()) //fail in input
		{
			cin.clear();
			cin.ignore(1000000, '\n');
			cout << errorMessageCinFail;
			cinError = true; //error flag
		}
		else if (lines > 26 || columns > 26) //if number over 26
		{
			cout << errorMessage26;
			cinError = true;
		}
		SetConsoleTextAttribute(hConsole, 15);
	} while (cinError); //if error, repeats


	//construct the board
	Board board(lines, columns);
	Board *boardA = &board;

	//fill it with a point ('.') in every position
	board.pointFill();

	//loop to fill in the board
	string position, word;

	while (true)
	{
		board.show(); //show the board
					  //the user choose what to do
		cout << "Position ( LCD / CTRL-Z = stop ) ? "; cin >> position; //<======================= tornar mais robusto
																		//CTRL-Z to get out of the loop
		if (cin.eof()) //if ctrl z, breaks
		{
			cin.clear();
			bool finishBoard = finishingCreate(boardA, dictA);
			if (finishBoard) break;
			else continue;
		}

		cout << "Word ( - = remove / ? = help ) . ? "; cin >> word;
		transform(word.begin(), word.end(), word.begin(), ::toupper); //upper case the word

		if (cin.eof()) //if ctrl z, does nothing
		{
			cin.clear();
			cin.ignore(10000, '\n');
			SetConsoleTextAttribute(hConsole, 244);
			cout << "That is not a valid answer!";
			SetConsoleTextAttribute(hConsole, 15);
			continue;
		}
																	  
		if ((position.size() != 3) || (!board.validPosition(position))) //check if position input is correct
		{
			SetConsoleTextAttribute(hConsole, 244);
			cout << "That position is not valid... \nTry Again!\n";
			SetConsoleTextAttribute(hConsole, 15);
		}
		//to remove a word
		else if ("-" == word)
		{
			board.removeWord(position);
		}
		//to help the user
		else if ("?" == word)
		{
			helpInsertWord(position, boardA, dictA);
		}
		//to add the respective word
		else if (checkValidity(dictA, boardA, word, position)) board.addWord(word, position);

	}
	
}

void puzzleResume()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	string inputFile;
	//dictionary creation and extraction
	Dictionary dict;
	Dictionary *dictA = &dict;
	bool errorOpeningFile;
	
	fstream f;
	fstream *fA = &f;

	do
	{
		cin.clear();
		errorOpeningFile = false;
		string errorMessageFileInput = "That input is not valid! Try again\n";
		string errorMessageOpeningFile = "It was not possible to open the %s file\n";
		cout << "File name? ";
		cin >> inputFile;
		if (cin.fail()) //fail in input
		{
			cin.ignore(1000000, '\n');
			SetConsoleTextAttribute(hConsole, 244);
			cout << errorMessageFileInput;
			SetConsoleTextAttribute(hConsole, 15);
			continue;
		}
		if (cin.eof()) //ctrl z in input
		{
			cin.ignore(100, '\n');
			SetConsoleTextAttribute(hConsole, 244);
			cout << errorMessageFileInput;
			SetConsoleTextAttribute(hConsole, 15);
			continue;
		}
		f.open(inputFile); //open the file
		if (!f.is_open()) //if is not opened, next iteration and next question
		{
			SetConsoleTextAttribute(hConsole, 244);
			printf(errorMessageOpeningFile.c_str(), inputFile.c_str());
			SetConsoleTextAttribute(hConsole, 15);
			errorOpeningFile = true;
		}
	} while (cin.fail() || errorOpeningFile); //repeats if error in input or not opening file

	Board board;
	Board *boardA = &board;

	getline(f, dict.fileNameInput); //get first line, with the name of the dictionary
	dict.loadToProgram(); //load the dictionary to the program

	board.loadFromFile(fA); //load the board to the program
	f.close(); //close

	//loop to fill in the board
	string position, word;
	while (true)
	{
		board.show(); //show the board
					  //the user choose what to do
		cout << "Position ( LCD / CTRL-Z = stop ) ? "; cin >> position; //<======================= tornar mais robusto
		if (cin.eof()) //if ctrl z, finishing function
		{
			cin.clear();
			bool finishBoard = finishingCreate(boardA, dictA); //finishing function
			if (finishBoard) break; //if true, breaks
			else continue;
		}

		cout << "Word ( - = remove / ? = help ) . ? "; cin >> word;
		transform(word.begin(), word.end(), word.begin(), ::toupper); //upper case the word

		if (cin.eof()) //if ctrl z
		{
			cin.clear();
			cin.ignore(10000, '\n');
			SetConsoleTextAttribute(hConsole, 244);
			cout << "That is not a valid answer!";
			SetConsoleTextAttribute(hConsole, 15);
			continue;
		}
																	  
		if (!board.validPosition(position)) //check if position input is correct
		{
			SetConsoleTextAttribute(hConsole, 244);
			cout << "That position is not valid... Try Again!\n";
			SetConsoleTextAttribute(hConsole, 15);
		}
		//to remove a word
		else if ("-" == word)
		{
			board.removeWord(position);
		}
		//to help the user
		else if ("?" == word)
		{
			helpInsertWord(position, boardA, dictA);
		}
		//to add the respective word
		else if (checkValidity(dictA, boardA, word, position)) board.addWord(word, position);

	}
}

int main()
{
	//program layout
	cout << "               CROSSWORD PUZZLE CREATOR\n";
	cout << "=======================================================\n\n";
	cout << "INSTRUCTIONS:\n\n";

	cout << " -Position ( LCD / CTRL-Z = stop )\n";
	cout << "   LCD stands for Line Column and Direction\n";
	cout << "     Line and Column represented by its name\n";
	cout << "      . upper case for lines\n";
	cout << "      . lower case for columns\n";
	cout << "    Direction can be vertical (V) or horizontal (H)\n";
	cout << "   CTRL-Z will end the board creation and start the file extraction process\n";
	cout << " -Word ( - = remove / ? = help )\n";
	cout << "   There are several options here:\n";
	cout << "      . typing a word to be added to the board\n";
	cout << "      . '-' to remove a word from the crossword board\n";
	cout << "      . '?' gives you a list of possible words to add to the board";
	while (true)
	{
		cout << "\n\n-------------------------------------------------------\n\n";
		cout << "OPTIONS:\n";
		cout << " 0 - Exit\n";
		cout << " 1 - Create puzzle\n";
		cout << " 2 - Resume puzzle\n\n";

		//error-proof input 
		string chosenOption;
		while (true)
		{
			cout << "Option ? ";
			cin >> chosenOption; //option
			if (cin.eof()) //ctrl-z, does nothing
			{
				cin.clear();
				cin.ignore(1000, '\n');
				continue;
			}
			if ("0" == chosenOption) return 0; //exit
			if ("1" == chosenOption)
			{
				cout << "\n\n                     CREATE PUZZLE \n";
				cout << "=======================================================\n\n";
				puzzleCreate(); //create from scratch
				break;
			}
			if ("2" == chosenOption)
			{
				cout << "\n\n                    RESUME PUZZLE \n";
				cout << "=======================================================\n\n";
				puzzleResume(); //continue the creation
				break;
			}
		}
	}
}