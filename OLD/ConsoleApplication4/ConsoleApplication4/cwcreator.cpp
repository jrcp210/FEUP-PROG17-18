#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map> //<-------------------------------
#include <algorithm>

using namespace std;

class Dict {
private:
	//vector < vector <string> > wordSynonyms;
	map <string, vector <string> > wordSynonyms;
	//vector <string> newEmptyVector;
	ifstream f;

public:
	string fileNameInput;

private:
	bool isHeadline(string line)
	{
		int doisPontos, virgula;
		doisPontos = line.find(":");
		virgula = line.find(",");

		if (doisPontos < virgula) return true;
		else return false;
	}

	string singleWord(string& Line) //retirar a proxima palavra da linha
	{
		//check if the first char is a letter
		//and if not cut the chars till we reach a letter, and then, a word
		while (!((Line[0] >= 65 && Line[0] <= 90) || (Line[0] >= 97 && Line[0] <= 122))) {
			Line.erase(0, 1);
		}

		string delimiter = " ,:"; //delimitadores
		string singleWord = Line.substr(0, Line.find(delimiter)); //palavra individual
		//erase the given words
		Line.erase(0, Line.find(delimiter));

		
		return singleWord;
	}

	bool validLine(string Line)
	{
		//check if a line is valid by comparing char by char, so lines with [] and {} are ignored
		for (unsigned int i = 0; i < Line.size(); i++)
		{
			if (!((Line[i] >= 65 && Line[i] <= 90) || (Line[i] >= 97 && Line[i] <= 122) || Line[i] == 32 || Line[i] == 58 || Line[i] == 44 || Line[i]==45))
			{
				return false;
				break;
			}
		}
		return true;
	}

public:
	void extraction()
	{
		string completeLine;
		string word;
		//int i = -1;

		f.open(fileNameInput);
		if (!f.is_open()) {
			cerr << "File " << fileNameInput << " could not be found!\n";
			system("pause");
			return;
		}

		cout << "extrating the words and synonyms...\n";

		//first line, except lines with "[ (...) ]"
		do {
			getline(f, completeLine);
		} while (!validLine(completeLine));

		//line and headline analysis
		while (!f.eof()) {
			string headline;
			vector <string> synonyms;
			while (completeLine != "\0") {
				if (isHeadline(completeLine)) {
					//i++;
					//wordSynonyms.push_back(newEmptyVector);
					//wordSynonyms[i].push_back(singleWord(completeLine));
					headline = singleWord(completeLine);
					transform(headline.begin(), headline.end(), headline.begin(), ::toupper);
				}
				else {
					//wordSynonyms[i].push_back(singleWord(completeLine)); 
					word = singleWord(completeLine);
					transform(word.begin(), word.end(), word.begin(), ::toupper);
					synonyms.push_back(word);
				}
			}

			wordSynonyms[headline] = synonyms;

			//next line
			do {
				getline(f, completeLine);
			} while (!validLine(completeLine));
		}
		cout << "end of extraction\n";
		f.close();
	}

	bool headlineExists(string word) //<=================================0
	{
		return true;
	}     //<====================================================

};


class Board {
private:
	unsigned int lines, columns;
	vector <vector <char> > layout;
	vector <char> newEmpty;
	vector <string> wordsPlaced;

	void addVertical(string word, int line, int column)
	{
		if (0 < line) layout[column].at(line - 1) = '#';
		for (unsigned int i = 0; i < word.size(); i++)
			layout[column][line + i] = word[i];
		if (line + word.size() < lines) layout[column].at(line + word.size()) = '#';
	}

	void addHorizontal(string word, int line, int column)
	{
		if (0 < column) layout.at(column - 1)[line] = '#';
		for (unsigned int i = 0; i < word.size(); i++)
			layout[column + i][line] = word[i];
		if (column + word.size() < columns) layout.at(column + word.size())[line] = '#';
	}

	void removeVertical(int line, int column)
	{
		if (0 < line) layout[column].at(line - 1) = '.';
		int i = 0;
		while (true)
		{
			if (line + i == lines) return;
			if (layout[column][line + i] == '#')
			{
				layout[column][line + i] = '.';
				return;
			}
			layout[column][line + i] = '.';
			i++;
		}
	//	for (unsigned int i = 0; i < word.size(); i++)
	//		layout[column][line + i] = '.';
	//	if (line + word.size() < lines) layout[column].at(line + word.size()) = '.';
	}

	void removeHorizontal(int line, int column)
	{
		if (0 < column) layout.at(column - 1)[line] = '.';
		int i = 0;
		while (true)
		{
			if (column + i == lines) return;
			if (layout[column + i][line] == '#')
			{
				layout[column + i][line] = '.';
				return;
			}
			layout[column][line + i] = '.';
			i++;
		}
	//	for (unsigned int i = 0; i < word.size(); i++)
	//		layout[column + i][line] = '.';
	//	if (column + word.size() < columns) layout.at(column + word.size())[line] = '.';
	}

public:
	/*
	FAZER LINHAS E COLUNAS +1, PARA QUE OS IDENTIFICADORES DAS LINHAS ESTEJAM DENTRO DO VECTOR?
	OU NOS LOOP QUE FIZERMOS, CONTANDO DO INICIO E NA PRIMEIRA LINHA, METER OS EIXOS "A PARTE" DO VECTOR E DO LOOP DO VECTOR?
	*/

	// from here
	Board()      
	{
		lines = 10;  //default size
		columns = 10; //default size
	}//constructor

	Board(unsigned int lines, unsigned int columns) 
	{
		this->lines = lines; 
		this->columns = columns;
	}	//constructor with arguments					
	// to here

	void pointFill()
	{
		//fill the board with points
		char point = '.';
		//	layout:	outer vector columns
		//			inner vector lines
		for (unsigned int i = 0; i < columns; i++) {
			layout.push_back(newEmpty);
			for (unsigned int j = 0; j < lines; j++) {
				layout[i].push_back(point);
			}
		}
	}

	void show() {
		char a = 'a';
		char A = 'A';
		cout << "  ";
		for (int i = 0; i < columns; i++) {
			char b = a + i;
			cout << " " << b;
		}
		cout << endl;
		for (unsigned int j = 0; j < lines; j++) {
			char B = A + j;
			cout << " " << B;
			for (unsigned int i = 0; i < columns; i++) {
				cout << " " << layout[i][j];
			}
			cout << endl;
		}
	}

	int whichLine(string position)
	{
		//first char of position
		char l = position[0];
		//char - A = position on vector (A-A=0, 1st line, position 0 on the vector)
		int lineNum = l - 65;
		return lineNum;
	}

	int whichColumn(string position)
	{
		//second char of position
		char c = position[1];
		//char - a = position on vector
		int columnNum = c - 97;
		return columnNum;
	}

	void addWord(string word, string position) {
		int lineNum = whichLine(position);
		int columnNum = whichColumn(position);
		char direction = position[2];

		if ('V' == direction || 'v' == direction) addVertical(word, lineNum, columnNum);
		if ('H' == direction || 'v' == direction) addHorizontal(word, lineNum, columnNum);

		wordsPlaced.push_back(word);
	}

	void removeWord(string position) {
		int line = whichLine(position);
		int column = whichColumn(position);
		char direction = position[2];

		if ('V' == direction || 'v' == direction) removeVertical(line, column);
		if ('H' == direction || 'h' == direction) removeHorizontal(line, column);
	}

/*	bool clearSpace4Word(int wSize, int lineNum, int columnNum, char direction)
	{
		//check if the beginning is out of boundaries or not
		if (lineNum + 1 > lines || columnNum + 1 > columns) return false;
		//checks if there are space for the word, depending on the initial position, and the word size
		//checks several conditions
		if ('V' == direction || 'v' == direction) {
			if (0 == lineNum) {  //beginning of the column
				for (unsigned int i = 0; i < wSize; i++) //checks every position
					if (layout[columnNum][i] != '.') return false;
				if (!(layout[columnNum].at(wSize) == '#' || layout[columnNum].at(wSize) == '.')) return false; // checks the final position +1
			}
			else {
				if (lineNum + wSize > lines) return false;
				for (unsigned int i = 0; i < wSize; i++) //all position checking
					if (layout[columnNum][i + lineNum] != '.') return false;
				if (!(layout[columnNum].at(wSize + lineNum) == '#' || layout[columnNum].at(wSize + lineNum) == '.')) return false; //empty space in the end
				if (!(layout[columnNum].at(lineNum - 1) == '#' || layout[columnNum].at(lineNum - 1) == '.')) return false; //empty space in the beginning
			}
		}
		else if ('H' == direction || 'h' == direction)
		{
			if (0 == columnNum)
			{
				for (unsigned int i = 0; i < wSize; i++)
					if (layout[i][lineNum] != '.') return false; //all positions
				if (!(layout.at(wSize)[lineNum] == '#' || layout.at(wSize)[lineNum] == '.')) return false; // checks the final position +1
			}
			else {
				if (columnNum + wSize > columns) return false;
				for (unsigned int i = 0; i < wSize; i++) //all position checking
					if (layout[i+columnNum][lineNum] != '.') return false;
				if (!(layout.at(wSize + columnNum)[lineNum] == '#' || layout.at(wSize + columnNum)[lineNum] == '.')) return false; //empty space in the end
				if (!(layout.at(columnNum - 1)[lineNum] == '#' || layout.at(columnNum - 1)[lineNum] == '.')) return false; //empty space in the beginning
			}
		}
	}
	*/

	bool checkSpace4Word(string word, int lineNum, int columnNum, char direction)
	{
		int wSize = word.size();

		//check if the beginning is out of boundaries or not
		if (lineNum + 1 > lines || columnNum + 1 > columns) return false;
		//checks if there are space for the word, depending on the initial position, and the word size
		//checks several conditions
		if ('V' == direction || 'v' == direction) {
			if (0 == lineNum) {  //beginning of the column
				for (unsigned int i = 0; i < wSize; i++) //checks every position
					if (!(layout[columnNum][i] == '.' || layout[columnNum][i] == word[i])) return false;
				if (lineNum + wSize < lines)
					if (!(layout[columnNum].at(wSize) == '#' || layout[columnNum].at(wSize) == '.')) return false; // checks the final position +1
			}
			else {
				if (lineNum + wSize > lines) return false;
				for (unsigned int i = 0; i < wSize; i++) //all position checking
					if (!(layout[columnNum][i + lineNum] == '.' || layout[columnNum][i + lineNum] == word[i])) return false;
				if (lineNum + wSize < lines) 
					if (!(layout[columnNum].at(wSize + lineNum) == '#' || layout[columnNum].at(wSize + lineNum) == '.')) return false; //empty space in the end
				if (!(layout[columnNum].at(lineNum - 1) == '#' || layout[columnNum].at(lineNum - 1) == '.')) return false; //empty space in the beginning
			}
		}
		else if ('H' == direction || 'h' == direction)
		{
			if (0 == columnNum)
			{
				for (unsigned int i = 0; i < wSize; i++)
					if (!(layout[i][lineNum] == '.' || layout[i][lineNum] == word[i])) return false; //all positions
				if (columnNum + wSize < columns) 
					if (!(layout.at(wSize)[lineNum] == '#' || layout.at(wSize)[lineNum] == '.')) return false; // checks the final position +1
			}
			else {
				if (columnNum + wSize > columns) return false;
				for (unsigned int i = 0; i < wSize; i++) //all position checking
					if (!(layout[i + columnNum][lineNum] == '.' || layout[i + columnNum][lineNum] == word[i])) return false;
				if (columnNum + wSize < columns) 
					if (!(layout.at(wSize + columnNum)[lineNum] == '#' || layout.at(wSize + columnNum)[lineNum] == '.')) return false; //empty space in the end
				if (!(layout.at(columnNum - 1)[lineNum] == '#' || layout.at(columnNum - 1)[lineNum] == '.')) return false; //empty space in the beginning
			}
		}
		return true;
	}

	bool unusedWord(string word)
	{
		for (unsigned int i = 0; i < wordsPlaced.size(); i++)
			if (word == wordsPlaced[i]) return false;
		return true;
	}
};


 
bool checkValidity(Dict *dictP, Board *boardP, string word, string position) 
{
	//check the existance of the word
	if (!dictP->headlineExists(word)) return false;
	
	//process the position string into line number, column number and direction
	int lineNum = boardP->whichLine(position);
	int columnNum = boardP->whichColumn(position);
	char direction = position[2];

//	if (!boardP->clearSpace4Word(word.size(), lineNum, columnNum, direction)) return false;
	if (!boardP->unusedWord(word)) return false;
	if (!boardP->checkSpace4Word(word, lineNum, columnNum, direction)) return false;
	return true;
}

void puzzleCreate()
{
	//dictionary creation and extraction
	Dict dict;
	Dict *dictA = &dict;
	cout << "Dictionary file name ? "; cin >> dict.fileNameInput; 
	dict.extraction();
																									//from here
	//creation of the board
	int lines, columns;												
	//ask the user how many lines and columns
	cout << "Board size (lines columns)? "; cin >> lines >> columns;
	//construct the board
	Board board(lines, columns);
	Board *boardA = &board;

	//fill it with a point ('.') in every position
	board.pointFill();

	//loop to fill in the board
	string position, word;
	while (true) {
		board.show(); //show the board
		//the user choose what to do
		cout << "Position ( LCD / CTRL-Z = stop ) ? "; cin >> position;
		if (cin.eof()) { cin.clear(); break; } //CTRL-Z to get out of the loop

		cout << "Word ( - = remove / ? = help ) . ? "; cin >> word;
		transform(word.begin(), word.end(), word.begin(), ::toupper);
		//to remove a word
		if ("-" == word) {
			board.removeWord(position);
		}
		else if ("?" == word) {
			//TO BE COMPLETED <=========================================================================================
		}

		else if (checkValidity(dictA, boardA, word, position)) board.addWord(word, position);



	} 
	cout << "\n\n\nYOU GOT OUT!\n\n\n";
																									//to here
}																		
/*
void wordsTest()
{

} */

int main()
{
	//program layout
	cout << "         CROSSWORD PUZZLE CREATOR\n";
	cout << "==========================================\n\n";
	cout << "INSTRUCTIONS:\n\n";

	cout << "Position ( LCD / CTRL-Z = stop )\n";
	cout << "  LCD stands for Line Column and Direction\n";
	cout << "    Line and Column represented by its name\n";
	cout << "      upper case for lines\n";
	cout << "      lower case for columns\n";
	cout << "    Direction can be vertical (V) or horizontal (H)\n\n";
	cout << "Word ( - = remove / ? = help )\n";
	cout << "  there are several options here:\n";
	cout << "    typing a word to be added to the board\n";
	cout << "      - to remove a word from the crossword board\n";
	cout << "      ? gives you a list of possible words to add to the board\n\n";
	while (true) {
		cout << "-----------------------------------------------------\n\n";
		cout << "OPTIONS:\n";
		cout << " 1 - Create puzzle\n";
		cout << " 2 - Resume puzzle\n";
		cout << " 0 - Exit\n\n";
		//error-proof input 
		string chosenOption;
		while(true) {
			cout << "Option ? ";
			cin >> chosenOption;
			if ("0" == chosenOption) return 0;
			if ("1" == chosenOption) {
				puzzleCreate(); 
				break;
			}
			if ("2" == chosenOption) {
				//wordsTest();
				break;
			}
			//if ("2" == chosenOption) { puzzleResume(); break; }
		} 
	}
}