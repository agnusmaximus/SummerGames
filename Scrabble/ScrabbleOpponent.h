/*
 *  ScrabbleOpponent.h
 *  Scrabble
 *
 *  Created by Max Lam on 7/25/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 *  This class defines what the user will play against -- the computer.
 *
 */

#include "ScrabbleLetter.h"
#include "ScrabbleBoard.h"
#include "ScrabbleRandom.h"
#include "ScrabbleChecker.h"
#include "vector.h"
#include "set.h"
#include "lexicon.h"

#define COMPUTER_VISUALIZATION_TIMING 0	//Milliseconds

struct partialSolution {
	char character;
	int indexX, indexY;
};

struct solutionSet {
	Vector<partialSolution> description;
	Set<ScrabbleLetter *> set;
};

class ScrabbleOpponent {
	
public:
	//Constructor
	ScrabbleOpponent();
	
	~ScrabbleOpponent();
	
	//Computer's turn
	void computerMove();

	//The actual board used to place letters on
	Grid<ScrabbleTile *> board;
	
	//Number of consecutive passes
	int numConsecutivePasses;
	
	//no letters left
	bool lettersLeft();
	
	int getScore();
	
	//Cheat
	void computerInventory();
	void forceInventory(string newLetters);
	void instawin();
		
private:

	//Computer's letters
	Set<ScrabbleLetter *> inventory;
	
	//Dictionary
	Lexicon dict;
	
	//Letters on board
	Grid<ScrabbleLetter *> lettersOnBoard;
	
	
	int generateBestSolution(Vector<int> &letterIndices, int rowOrCol, Vector<char> &boardCharacters,
									   Set<ScrabbleLetter *> inventory, solutionSet &bestSet);
	
	void possibleWordsWithInventory(Vector<string> &validWords, Vector<char> &inventory, string soFar, char mandatory);

	Set<ScrabbleLetter *> partialScrabbleLettersSolution(Set<ScrabbleLetter *> inventory, string wordToForm,
														int indexOfCharOnBoard, int charOnBoard, int rowOrCol);

	ScrabbleLetter * scrabbleLetterWithCharacter(char search, Set<ScrabbleLetter *> inventory);
	int tryPartialSolution(Set<ScrabbleLetter *> partial);

	void commitSolution(Set<ScrabbleLetter *> &final, bool shouldDrawTiles);
	
	void saveSolutionSet(solutionSet &solution, Set<ScrabbleLetter *> save);
	void restoreSolutionSet(solutionSet &restore);
	
	//Bounds check given letters in a set
	bool checkBoundsOfLetters(Set<ScrabbleLetter *> check);
	
	//Checks if there is a letter in the set that has this position
	bool letterWithPosition(int x, int y, Grid<ScrabbleLetter *> inv);
	
	//Computer's score
	int score;
	int scoreRef;
	
	//Updates score given amount of increase
	void updateScore(int increase);
	
	//Which way should the computer form words
	int letterFormation;
	
	//Draw letters (as in pick up)
	void addLettersToInventory();
	
	//Make first move
	void makeFirstMove();
};

