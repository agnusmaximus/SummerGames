/*
 *  ScrabbleChecker.h
 *  Scrabble
 *
 *  Created by Max Lam on 7/23/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 *  This file represents the class that will check for invalid moves.
 *  To implement this class, a web-like structure will be used,
 *  with the root of the tree specifying the first letter played down.
 * 
 *  The first letter that will specify the web origin has to
 *  be in the center of the map.
 *
 *  If there are any lone letters on the map, a method that checks valid
 *  Scrabble moves will return false.
 */

#pragma once

#include "ScrabbleLetter.h"
#include "ScrabbleBoard.h"
#include "ScrabbleScore.h"
#include "set.h"
#include "grid.h"
#include "lexicon.h"
#include "map.h"

class ScrabbleChecker {

public:
	//Constructor and destructor
	ScrabbleChecker();
	~ScrabbleChecker();
	
	//Check move
	bool checkMove(Set<ScrabbleLetter *> &moves, int who);
	
	//Get points earned
	int getPointsEarnedLastTurn();
	
	//Shared object
	static ScrabbleChecker * sharedObject();
	
	Grid<ScrabbleLetter *> getLettersOnBoard();
	
	//Computer or player
	static const int PLAYER;
	static const int COMPUTER;
	static const int SILENT;
	
	bool isFirstMove();
	
	void reset();
	
private:
	//The lexicon
	Lexicon dict;
	
	//The grid representing letters placed
	Grid<ScrabbleLetter *> placedLetters;
	
	//A map representing made words
	Map<Vector<Set<ScrabbleLetter *> > >constructedWords;
	Map<Vector<Set<ScrabbleLetter *> > >possibleConstructedWords;
	
	//The root of the web
	ScrabbleLetter *root;
	
	//Size of the tree
	int treeSize;
	
	//Construct the grid (first thing to do)
	bool constructGrid(Set<ScrabbleLetter *> &moves);
	
	//Removes from the grid the elements in the set
	void removeFromGrid(Set<ScrabbleLetter *> &moves);
	
	//Was the move in a straight line
	bool moveInLine(Set<ScrabbleLetter *> &moves);
	
	int createWeb(ScrabbleLetter *&rt);
	int createWebRec(ScrabbleLetter *&rt, Grid<ScrabbleLetter *> &elem, Grid<bool> &used);
	
	int validateWeb(ScrabbleLetter *rt);
	int validateWebRec(ScrabbleLetter *rt, Grid<bool> &used);
	int validateWebWordsRec(ScrabbleLetter *rt, string soFar, Grid<bool> &used, ScrabbleLetter *last, int direction);

	//This method saves the a newly formed word given the word a direction and a last node of the letter
	void saveWord(ScrabbleLetter *last, int direction, string word);
	
	//This method, given a root word, returns a set with all letters
	//that form this word
	Set<ScrabbleLetter *> lettersFormingWord(ScrabbleLetter *current, int direction);
	void lettersFormingWordRec(ScrabbleLetter *current, Set<ScrabbleLetter *> &soFar, Grid<bool> &used, int direction);
	
	//Converts every letter back to black
	void lettersToBlack();
	
	//Points user earned in this turn
	int pointsEarnedTurn;
	
	//who is the chcker checking
	int checkWho;
};
