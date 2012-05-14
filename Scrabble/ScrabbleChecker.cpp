/*
 *  ScrabbleChecker.cpp
 *  Scrabble
 *
 *  Created by Max Lam on 7/23/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 */

#include "ScrabbleChecker.h"

//Direction types
#define VERT 1 
#define HOR 2

//Computer or player
const int ScrabbleChecker::PLAYER = 1;
const int ScrabbleChecker::COMPUTER = 2;
const int ScrabbleChecker::SILENT = 3;

/* Init the scrabblechecker class */
ScrabbleChecker::ScrabbleChecker() {
	root = NULL;
	treeSize = 0;
	placedLetters.resize(ScrabbleBoard::NUM_ROWS_COLS, ScrabbleBoard::NUM_ROWS_COLS);
	
	//Initialize the grid to null and the memoization grid
	for (int x = 0; x < placedLetters.numCols(); x++) {
		for (int y = 0; y < placedLetters.numRows(); y++) {
			placedLetters[x][y] = NULL;
		}
	}
	
	dict.addWordsFromFile("ScrabbleResources/lexicon.dat");
	pointsEarnedTurn = 0;
}

/* destroy the scrabblechecker class */
ScrabbleChecker::~ScrabbleChecker() {
}

/* return shared scrabbleCheckerObject */
ScrabbleChecker * ScrabbleChecker::sharedObject() {
	static ScrabbleChecker *checker = NULL;
	
	if (checker == NULL) {
		checker = new ScrabbleChecker();
	}
	return checker;
}

/* Destroy everything */
void ScrabbleChecker::reset() {
	for (int i = 0; i < ScrabbleBoard::NUM_ROWS_COLS; i++) {
		for (int j = 0; j < ScrabbleBoard::NUM_ROWS_COLS; j++) {
			delete placedLetters[i][j];
			placedLetters[i][j] = NULL;
		}
	}
	constructedWords.clear();
	possibleConstructedWords.clear();
	root = NULL;
	treeSize = 0;
}

/* @return true if it is the first move */
bool ScrabbleChecker::isFirstMove() {
	return treeSize == 0;
}

/* What this method does is that it checks the moves committed by the user
 *
 * @param moves contains ScrabbleLetters that have been moved onto the board
 * @param outputAndCommit determines if the letters are to be committed if they are valid.
 *
 * @return true if it was a valid move, false otherwise
 */
bool ScrabbleChecker::checkMove(Set<ScrabbleLetter *> &moves, int who) {
	pointsEarnedTurn = 0;
		
	checkWho = who;
	
	//Check if user layed two pieces on each other
	foreach (ScrabbleLetter * letter in moves) {
		//Check moves in set
		foreach(ScrabbleLetter * check in moves) {
			if (letter->tileOn == check->tileOn && letter != check) {
				if (who == PLAYER)
					ScrabbleGraphics::sharedObject(NULL)->notifyUser("Two letters are overlapping");
				return false;
			}
		}
		//Check moves in grid
		foreach(ScrabbleLetter * check in placedLetters) {
			if (check != NULL && letter->tileOn == check->tileOn) {
				if (who == PLAYER)
					ScrabbleGraphics::sharedObject(NULL)->notifyUser("Two letters are overlapping");
				return false;
			}
		}
	}
	
	//User must make moves that form a line
	if (!moveInLine(moves)) {
		ScrabbleGraphics::sharedObject(NULL)->notifyUser("Moves must be made in a straight line");
		return false;
	}
		
	//Populate grid
	if (!constructGrid(moves)) {
		if (who == PLAYER) 
			ScrabbleGraphics::sharedObject(NULL)->notifyUser("No move was made");
		
		//Computer output
		else if (checkWho == COMPUTER) 
			ScrabbleGraphics::sharedObject(NULL)->notifyUser("The computer made no move");
		
		return false;
	}
		
	
	//Root of tree
	root = placedLetters[ceil(ScrabbleBoard::NUM_ROWS_COLS/2)][ceil(ScrabbleBoard::NUM_ROWS_COLS/2)];
				
	//Center of must not be null
	if (root != NULL) {
		//Construct the web
		if (createWeb(root) == treeSize) {
			//Set the possible constructed words to constructed words
			possibleConstructedWords = constructedWords;
			
			if (validateWeb(root) > 0) {
				
				//Don't commit changes if the caller doesn't want to 
				if (who == SILENT) {
					removeFromGrid(moves);
					return true;
				}
				
				foreach(ScrabbleLetter *letter in moves) {
					letter->setCommitted();
				}
				
				//Commit changes to the known words set
				constructedWords = possibleConstructedWords;
				
				//Letters to black
				lettersToBlack();
				
				return true;
			}
		}
	}
	else {
		//Notify user that he must place a piece in the center square
		ScrabbleGraphics::sharedObject(NULL)->notifyUser("The first word must have a letter on the center square");
	}
					
	//Error occurred -- depopulate grid
	removeFromGrid(moves);
	root = NULL;
	
	return false;
}

/* This method initializes the grid of scrabbleLetters, which represents
 * where each letter is on the board
 *
 * @returns true if there was a change, false if there wasn't
 */
bool ScrabbleChecker::constructGrid(Set<ScrabbleLetter *> &moves) {		
	bool didChange = false;
		
	foreach(ScrabbleLetter *letter in moves) {
		didChange = true;
		
		placedLetters[letter->gridIndexX][letter->gridIndexY] = letter;
		treeSize++;
	}
	
	return didChange;
}

/* This method removes from the grid the elements of the set */
void ScrabbleChecker::removeFromGrid(Set<ScrabbleLetter *> &moves) {
	foreach(ScrabbleLetter *letter in moves) {
		placedLetters[letter->gridIndexX][letter->gridIndexY] = NULL;
		treeSize--;
	}
}

/* Returns true if the move was in a straight line */
bool ScrabbleChecker::moveInLine(Set<ScrabbleLetter *> &moves) {
	bool indexSpecified = false, moveVertically = true, moveHorizontally = true;
	int indexX, indexY;
	
	foreach(ScrabbleLetter *letter in moves) {
		if (indexSpecified == false) {
			indexX = letter->gridIndexX;
			indexY = letter->gridIndexY;
			indexSpecified = true;
		}
		if (indexX != letter->gridIndexX)
			moveHorizontally = false;
		if (indexY != letter->gridIndexY)
			moveVertically = false;
	}
	
	return moveVertically || moveHorizontally;
}

/* This method is a wrapper to the recursive function
 * for creating the web
 */
int ScrabbleChecker::createWeb(ScrabbleLetter *&rt) {
	Grid<bool> used(ScrabbleBoard::NUM_ROWS_COLS,ScrabbleBoard::NUM_ROWS_COLS);
	
	for (int x = 0; x < used.numCols(); x++) {
		for (int y = 0; y < used.numRows(); y++) {
			used[x][y] = false;
		}
	}	
	return createWebRec(rt, placedLetters, used);
}

/* This method recursively creates the letter web 
 *
 * @return number of elements in the web
 */
int ScrabbleChecker::createWebRec(ScrabbleLetter *&rt, Grid<ScrabbleLetter *> &elem, Grid<bool> &used) {
	int count = 0;
	
	if (rt == NULL)
		return count;
	
	if (used[rt->gridIndexX][rt->gridIndexY] == true)
		return count;
	
	//This element is now used
	used[rt->gridIndexX][rt->gridIndexY] = true;
	
	count = 1;
	
	//Check bounds before recursing
	if (rt->gridIndexX > 0) {
		rt->letterLeft = elem[rt->gridIndexX-1][rt->gridIndexY];
		
		count += createWebRec(rt->letterLeft, elem, used);
	}
	if (rt->gridIndexX < ScrabbleBoard::NUM_ROWS_COLS-1) {
		rt->letterRight = elem[rt->gridIndexX+1][rt->gridIndexY];
		
		count += createWebRec(rt->letterRight, elem, used);
	}
	if (rt->gridIndexY > 0) {
		rt->letterDown = elem[rt->gridIndexX][rt->gridIndexY-1];
		
		count += createWebRec(rt->letterDown, elem, used);
	}
	if (rt->gridIndexY < ScrabbleBoard::NUM_ROWS_COLS-1) {
		rt->letterUp = elem[rt->gridIndexX][rt->gridIndexY+1];
		
		count += createWebRec(rt->letterUp, elem, used);
	}

	return count;
}

/* This is the wrapper function for validating the web */
int ScrabbleChecker::validateWeb(ScrabbleLetter *rt) {
	Grid<bool> used(ScrabbleBoard::NUM_ROWS_COLS,ScrabbleBoard::NUM_ROWS_COLS);
	
	for (int x = 0; x < used.numCols(); x++) {
		for (int y = 0; y < used.numRows(); y++) {
			used[x][y] = false;
		}
	}
		
	return validateWebRec(rt, used);
}

/* This function validates the web given the root of the web
 *
 * @return true if the web is valid
 */
int ScrabbleChecker::validateWebRec(ScrabbleLetter *rt, Grid<bool> &used) {
	if (rt->letterLeft == NULL && 
		rt->letterUp == NULL) {
		
		if (used[rt->gridIndexX][rt->gridIndexY] == false) {
			
			used[rt->gridIndexX][rt->gridIndexY] = true;
			
			if (validateWebWordsRec(rt, "", used, NULL, VERT) <= 0)
				return -1;
			return 1;
		}
		else {
			//Neutral
			return 0;
		}	
	}
	
	//Otherwise recurse left and up
	if (rt->letterLeft != NULL) 
		return validateWebRec(rt->letterLeft, used);
	
	if (rt->letterUp != NULL)
		return validateWebRec(rt->letterUp, used);
	
	return -1;
}

/* This function validates the webs' words given a starting node 
 *
 * @return true if every letter is a word
 */
int ScrabbleChecker::validateWebWordsRec(ScrabbleLetter *rt, string soFar, Grid<bool> &used, ScrabbleLetter *last, int direction) {	
	//Check base cases:
	//At end of string
	if (rt == NULL) {
		//Continue if the word is length 1
		if (soFar.length() == 1)
			return 0;
		
		//Check if soFar is a string and leave a notification
		if (dict.containsWord(soFar)) {
			
			//Save this new word
			saveWord(last, direction, soFar);
			
			return 0;
		}
		
		if (checkWho == PLAYER) {
			//Otherwise return and leave a notification
			ScrabbleGraphics::sharedObject(NULL)->notifyUser("'" + soFar + "'" + " is not a word ");
		}
		
		return -1;
	}
	
	if (!dict.containsPrefix(soFar)) {
		
		if (checkWho == PLAYER) {
			//Leave a notifictation
			ScrabbleGraphics::sharedObject(NULL)->notifyUser("No words begin with '" + soFar + "'");
		}
		return -1;
	}
	
	last = rt;
	
	//used[rt->gridIndexX][rt->gridIndexY] = true;

	//If not, recurse right and downward
	string newString = soFar + rt->randomCharacter;
	
	//The string when changing directions
	string resetString = "";
	resetString.append(1, rt->randomCharacter);
	
	int result = 0;
	
	//Don't backtrack
	if (direction != VERT) {
		//Check if you can go up even more
		if (rt->letterUp != NULL) 
			if (validateWebRec(rt->letterUp, used) < 0)
				return -1;
		
		//Keep string and recurse right
		result = validateWebWordsRec(rt->letterRight, newString, used, last, HOR);
		if (result < 0)
			return result;
		
		if (rt->letterUp == NULL) {
			//Changing direction, destroy string keep current letter and recurse down
			result = validateWebWordsRec(rt->letterDown, resetString, used, last, VERT);
			if (result < 0)
				return result;
		}
	}
	//Don't backtrack
	if (direction != HOR) {
		if (rt->letterLeft != NULL)
			if (validateWebRec(rt->letterLeft, used) < 0)
				return -1;
		
		//Keep string and recurse down
		result = validateWebWordsRec(rt->letterDown, newString, used, last, VERT);
		if (result < 0)
			return result;
		
		if (rt->letterLeft == NULL) {
			//Destroy string and recurse right
			result = validateWebWordsRec(rt->letterRight, resetString, used, last, HOR);
			if (result < 0)
				return result;
		}
	}

	//Everything worked
	return 1;
}

void ScrabbleChecker::saveWord(ScrabbleLetter *last, int direction, string word) {	
	//If the key is not found, save it
	if (!possibleConstructedWords.containsKey(word)) {
		Set<ScrabbleLetter *>newSet = lettersFormingWord(last, direction);
		possibleConstructedWords[word].add(newSet);
		
		int ptEarned = ScrabbleScore::sharedObject()->pointsForWord(newSet);
		pointsEarnedTurn += ptEarned;
		
		if (checkWho == PLAYER) {
			//Notify user of correct word
			ScrabbleGraphics::sharedObject(NULL)->notifyUser("The word '" + word + "' is valid.");
		
			//Count points
			ScrabbleGraphics::sharedObject(NULL)->notifyUser("You earned: " + 
															IntegerToString(ptEarned) + 
															" points");
		}
		else if (checkWho == COMPUTER) {
			//Computer played a word.
			ScrabbleGraphics::sharedObject(NULL)->notifyUser("Computer played '" + word + "'");
			ScrabbleGraphics::sharedObject(NULL)->notifyUser("Computer gained " + IntegerToString(ptEarned) + " points");
		}
	}
	//Make an exception if the same word is formed twice
	else {
		//Exception is true at first
		bool except = true;
		
		Set<ScrabbleLetter *> newSet = lettersFormingWord(last, direction);
		
		//Check this new set with all other sets that form same word
		foreach (Set<ScrabbleLetter *>oldSets in possibleConstructedWords[word]) {
			bool isSameWordFound = true;
			foreach (ScrabbleLetter *letter in newSet) {
				//If found letters aren't the same, smae word isn't found
				if (!oldSets.contains(letter))
					isSameWordFound = false;
			}
			//If found the same word, dont count it twice. No exception
			if (isSameWordFound) {	
				except = false;
			}
		}
		
		//The exception
		if (except) {
			//Add to the vector of sets
			possibleConstructedWords[word].add(newSet);
			
			//Points earned
			int ptEarned = ScrabbleScore::sharedObject()->pointsForWord(newSet);
			pointsEarnedTurn += ptEarned;
			
			if (checkWho == PLAYER) {
				//Notify user of correct word
				ScrabbleGraphics::sharedObject(NULL)->notifyUser("The word '" + word + "' is valid.");
			
				//Count points
				ScrabbleGraphics::sharedObject(NULL)->notifyUser("You earned: " + 
																 IntegerToString(ptEarned) + 
																 " points");
			}
			else if (checkWho == COMPUTER) {
				//Computer played a word.
				ScrabbleGraphics::sharedObject(NULL)->notifyUser("Computer played '" + word + "'");
				ScrabbleGraphics::sharedObject(NULL)->notifyUser("Computer gained " + IntegerToString(ptEarned) + " points");
			}
		}
	}
}

/* What this function does is it returns a set containing all the letters that construct a given word.
 * Although this word is not passed directly, the end of the word, marked by NULL, is passed with the
 * direction that this word was constructed. The word can therefore be re-constructed again
 *
 * @param current is NULL and marks the end of the word
 * @param direction is which way the word was constructed
 *
 * @return a set of all the letters that form the word
 */
Set<ScrabbleLetter *> ScrabbleChecker::lettersFormingWord(ScrabbleLetter *current, int direction) {
	
	//Call the recursive definition
	Set<ScrabbleLetter *> returnSet;
	
	//The grid containing already counted words
	Grid<bool> used(ScrabbleBoard::NUM_ROWS_COLS,ScrabbleBoard::NUM_ROWS_COLS);
	
	for (int x = 0; x < used.numCols(); x++) {
		for (int y = 0; y < used.numRows(); y++) {
			used[x][y] = false;
		}
	}
	
	lettersFormingWordRec(current, returnSet, used, direction);
	
	return returnSet;
}

/* Creates a set of all the letters that form a word */
void ScrabbleChecker::lettersFormingWordRec(ScrabbleLetter *current, Set<ScrabbleLetter *> &soFar, 
										 Grid<bool> &used, int direction) {
	//Done
	if (current == NULL)
		return;
	
	//Check already counted
	if (used[current->gridIndexX][current->gridIndexY])
		return;
	
	soFar.add(current);
	used[current->gridIndexX][current->gridIndexY] = true;
	
	if (direction == HOR) {
		lettersFormingWordRec(current->letterRight, soFar, used, HOR);
		lettersFormingWordRec(current->letterLeft, soFar, used, HOR);
	}
	if (direction == VERT) {
		lettersFormingWordRec(current->letterUp, soFar, used, VERT);
		lettersFormingWordRec(current->letterDown, soFar, used, VERT);
	}
}

/* This method returns to the caller the amount of points the user earned last turn.
 * It also resets pointsEarnedTurn back to 0.
 */
int ScrabbleChecker::getPointsEarnedLastTurn() {
	int returnValue = pointsEarnedTurn;
	pointsEarnedTurn = 0;
	
	return returnValue;
}

/* Returns the valid letters on board */
Grid<ScrabbleLetter *> ScrabbleChecker::getLettersOnBoard() {
	return placedLetters;
}

/* This method converts every letter to have black text color */
void ScrabbleChecker::lettersToBlack() {
	for (int i = 0; i < ScrabbleBoard::NUM_ROWS_COLS; i++) {
		for (int j = 0; j < ScrabbleBoard::NUM_ROWS_COLS; j++) {
			if (placedLetters[i][j] != NULL)
				ScrabbleGraphics::sharedObject(NULL)->changeTextColorOfImage(placedLetters[i][j]->getRef(), (char *)"black");
		}
	}	
}