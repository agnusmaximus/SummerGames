/*
 *  ScrabbleOpponent.cpp
 *  Scrabble
 *
 *  Created by Max Lam on 7/25/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 */

#define ANY_CHAR 0

#define DIR_VERT 1
#define DIR_HOR 2

#include "ScrabbleOpponent.h"

/* Creates an instance of the scrabble opponent class */
ScrabbleOpponent::ScrabbleOpponent() {
	//Populate inventory with 7 scrabble letters
	addLettersToInventory();
	
	//Resize board
	lettersOnBoard.resize(ScrabbleBoard::NUM_ROWS_COLS, ScrabbleBoard::NUM_ROWS_COLS);
	
	//Dictionary
	dict.addWordsFromFile("ScrabbleResources/lexicon.dat");
	
	//Computers' score
	scoreRef = ScrabbleGraphics::sharedObject(NULL)->createLabelAt(ScrabblePoint(-5,ScrabbleGraphics::sharedObject(NULL)->getWindowHeight() * 72 - 72), 
																   "Computer: \n0");
	score = 0;
	
	letterFormation = DIR_VERT;
	
	numConsecutivePasses = 0;
}

ScrabbleOpponent::~ScrabbleOpponent() {
	//Release memory
	foreach (ScrabbleLetter *letter in inventory) {
		ScrabbleGraphics::sharedObject(NULL)->destroyImageWithRef(letter->getRef());
		delete letter;
	}	
	
}

/* The computer's move in scrabble */
void ScrabbleOpponent::computerMove() {
	//Make first move if have to
	makeFirstMove();	
	
	//Update the letters on board.
	lettersOnBoard = ScrabbleChecker::sharedObject()->getLettersOnBoard();
	
	//Best solution outcome
	int bestSolutionScore = 0;
	
	//Solution set
	solutionSet bestSolution;
	

	letterFormation = DIR_HOR;

	/* COMPUTE BEST OF THE HORIZONTALLY CONSTRUCTED WORDS */
	
	//Loop through each row and check out which letters need to be present
	//in the word to form
	for (int row = 0; row < lettersOnBoard.numRows(); row++) {
		
		//Indices of letters on board
		Vector<int> letterIndices;
		
		//Character on board
		Vector<char> boardCharacters;
		
		for (int col = 0; col < lettersOnBoard.numCols(); col++) {
			//Add to vector of indices and character is the letters on board is not null
			if (lettersOnBoard[col][row] != NULL) {
				letterIndices.add(col);
				boardCharacters.add(lettersOnBoard[col][row]->randomCharacter);
			}
		}
		
		//If there were any character in this column
		if (boardCharacters.size() != 0) {
			//current solution
			
			int cur_score = 0;
			solutionSet cur_solution;
			
			if ((cur_score = generateBestSolution(letterIndices, row, boardCharacters, 
												  inventory, cur_solution)) > bestSolutionScore) {
				bestSolutionScore = cur_score;
				bestSolution = cur_solution;
			}
		}
	}
	
	/* COMPUTE BEST OF THE VERTICALLY CONSTRUCTED WORDS */
	
	letterFormation = DIR_VERT;
	
	for (int col = 0; col < lettersOnBoard.numRows(); col++) {
		
		//Indices of letters on board
		Vector<int> letterIndices;
		
		//Character on board
		Vector<char> boardCharacters;
		
		for (int row = 0; row < lettersOnBoard.numCols(); row++) {
			//Add to vector of indices and character is the letters on board is not null
			if (lettersOnBoard[col][row] != NULL) {
				letterIndices.add(row);
				boardCharacters.add(lettersOnBoard[col][row]->randomCharacter);
			}
		}
		
		//If there were any character in this column
		if (boardCharacters.size() != 0) {
			//current solution
			
			int cur_score = 0;
			solutionSet cur_solution;
			
			if ((cur_score = generateBestSolution(letterIndices, col, boardCharacters, 
												  inventory, cur_solution)) > bestSolutionScore) {
				bestSolutionScore = cur_score;
				bestSolution = cur_solution;
			}
		}
	}
	
	
	restoreSolutionSet(bestSolution);
	commitSolution(bestSolution.set, true);
	
	foreach (ScrabbleLetter *opLet in bestSolution.set) {
		//Highlight move in red
		ScrabbleGraphics::sharedObject(NULL)->changeTextColorOfImage(opLet->getRef(), (char *)"red");
	}
	
	//Did pass?
	if (bestSolutionScore == 0)
		numConsecutivePasses++;
	else 
		numConsecutivePasses = 0;
}

/* Get best solution given a vector of board characters, their indices, a reference
 * to set of scrabble letters, and inventory of available words.
 *
 * @modify the best solution score 
 */
int ScrabbleOpponent::generateBestSolution(Vector<int> &letterIndices, int rowOrCol, Vector<char> &boardCharacters,
								  Set<ScrabbleLetter *> inventory, solutionSet &bestSet) {
	//Best score
	int bestScore = 0;
	
	//Create vector of chars that represent a simplified version of inventory
	Vector<char> simplifiedInventory;
	
	foreach(ScrabbleLetter * letter in inventory) {
		simplifiedInventory.add(letter->randomCharacter);
	}
	
	//Foreach board character, and the modified inventory,
	//Generate a list of all the words that can be formed.
	for (int charLoop = 0; charLoop < boardCharacters.size(); charLoop++) {
		//Current character
		char character = boardCharacters[charLoop];
		
		//Create modified inventory which has the character
		//in boardCharacters added
		Vector<char> modifiedInventory = simplifiedInventory;
		modifiedInventory.add(character);
		
		//Generate list of strings with this modified inventory
		Vector<string> validWordsWithCharacters;
		
		possibleWordsWithInventory(validWordsWithCharacters, modifiedInventory, "", character);
		
		//Create a partial solution for every string
		foreach (string word in validWordsWithCharacters) {
			
			Set<ScrabbleLetter *> partialSolution;
			partialSolution = partialScrabbleLettersSolution(inventory, word, letterIndices[charLoop], character, rowOrCol);
			
			int cur_score = 0;
			
			//Check bounds of the word
			if (checkBoundsOfLetters(partialSolution)) {
			
				//Try this partial solution out
				if ((cur_score = tryPartialSolution(partialSolution)) > bestScore) {
					saveSolutionSet(bestSet, partialSolution);
					bestScore = cur_score;
				}
			}
		}	
	}
	return bestScore;
}

/* This function takes in by reference a vector of valid words to be
 * populated with words made from letters from a vector
 *
 * @return void
 */
void ScrabbleOpponent::possibleWordsWithInventory(Vector<string> &validWords, Vector<char> &inventory,
												  string soFar, char mandatory) {
	//Check if soFar is a word
	if (dict.containsWord(soFar) && soFar.find(mandatory) != string::npos) {
		//Add the word to the vector
		validWords.add(soFar);
	}
	if (dict.containsPrefix(soFar)) {
		//Recurse down
		for (int i = 0; i < inventory.size(); i++) {
			char character = inventory[i];
			
			string newString = soFar + character;
			Vector<char> newInventory = inventory;
			newInventory.removeAt(i);
			
			possibleWordsWithInventory(validWords, newInventory, newString, mandatory);
		}
	}
}

/* This function creates a partial solution given a string, the index of the
 * character on board.
 *
 * A partial solution consists of a scrabbleLetters with specified indicies on the 
 * real board.
 *
 * @return a set of scrabble letters so that their grid indices correspond to their
 *           positions on the board
 */
Set<ScrabbleLetter *> ScrabbleOpponent::partialScrabbleLettersSolution(Set<ScrabbleLetter *> inventory, string wordToForm,
									int indexOfCharOnBoard, int charOnBoard, int rowOrCol) {
	//Return set
	Set<ScrabbleLetter *> returnSet;
	
	//Find the index of charOnBoard in wordToForm
	int charIndexInWord = wordToForm.find(charOnBoard);
	
	int diverge = charIndexInWord + 1, converge = charIndexInWord-1;
	
	while (converge >= 0 || diverge < wordToForm.length()) {
		int convergeIndex, divergeIndex;
		
		//Indices
		if (letterFormation == DIR_HOR) {
			convergeIndex =  indexOfCharOnBoard + converge - charIndexInWord;
			divergeIndex =  indexOfCharOnBoard + diverge - charIndexInWord;
		}
		else {
			convergeIndex =  ScrabbleBoard::NUM_ROWS_COLS - 1 -indexOfCharOnBoard + converge - charIndexInWord;
			divergeIndex =  ScrabbleBoard::NUM_ROWS_COLS - 1 -indexOfCharOnBoard + diverge - charIndexInWord;
		}
		
		//Assign indices
		if (converge >= 0) {
			//First find in inventory a letter with corresponding character
			ScrabbleLetter *chosen = scrabbleLetterWithCharacter(wordToForm[converge], inventory);
			
			if (chosen != NULL) {
				
				//Letter formation is horizontal
				if (letterFormation == DIR_HOR) {
					chosen->gridIndexX = convergeIndex;
					chosen->gridIndexY = rowOrCol;
				}
				//Letter formation is vertical
				else {
					chosen->gridIndexX = rowOrCol;
					chosen->gridIndexY = ScrabbleBoard::NUM_ROWS_COLS - 1 - convergeIndex;
				}
				
				returnSet.add(chosen);
			
				//Make sure a letter isn't assigned to a grid twice, which is
				//redundant
				inventory.remove(chosen);
			}
		}
		if (diverge < wordToForm.length()) {
			ScrabbleLetter *chosen = scrabbleLetterWithCharacter(wordToForm[diverge], inventory);
			
			if (chosen != NULL) {
				
				//If letter formation is horizontal
				if (letterFormation == DIR_HOR) {
					chosen->gridIndexX = divergeIndex;
					chosen->gridIndexY = rowOrCol;
				}
				//Letter formation is vertical
				else {
					chosen->gridIndexX = rowOrCol;
					chosen->gridIndexY = ScrabbleBoard::NUM_ROWS_COLS - 1 - divergeIndex;
				}
					
				returnSet.add(chosen);
				
				//Make sure a letter isn't assigned to a grid twice, which is
				//redundant
				inventory.remove(chosen);
			}
		}
		converge--;
		diverge++;
	}
	return returnSet;
}

/* This function given a character returns a pointer to the scrabbleLetter in that
 * set which contains that character
 */
ScrabbleLetter * ScrabbleOpponent::scrabbleLetterWithCharacter(char search, Set<ScrabbleLetter *> inventory) {
	foreach (ScrabbleLetter *letter in inventory) {
		if (letter->randomCharacter == search &&
			letter->tileOn == NULL) 
			return letter;
	}
	return NULL;
}

/* This method tries the partial solution out by placing them on the board.
 * @return scoring of the partial solution if it was successful, else -1.
 */
int ScrabbleOpponent::tryPartialSolution(Set<ScrabbleLetter *> partial) {
	int score = -1;
	
	bool wordvalid = true;
	
	//Put pieces on the board
	foreach(ScrabbleLetter *letter in partial) {
		//If there was a collision, break
		if (letterWithPosition(letter->gridIndexX, letter->gridIndexY, lettersOnBoard) || letter == NULL) {
			wordvalid = false;
			break;
		}
						
		//Simulate the placings...
		ScrabbleGraphics::sharedObject(NULL)->translateObjectToObject(letter->getRef(), 
																	  board[letter->gridIndexX][letter->gridIndexY]->getRef());
		
		//Pause for a few seconds
		usleep(COMPUTER_VISUALIZATION_TIMING);
		
		letter->tileOn = board[letter->gridIndexX][letter->gridIndexY];
	}
	
	if (wordvalid) {
		//Check valid move
		if (ScrabbleChecker::sharedObject()->checkMove(partial, ScrabbleChecker::SILENT)) {
			score = ScrabbleChecker::sharedObject()->getPointsEarnedLastTurn();
		}
	}
	
	//Rescind moves
	foreach(ScrabbleLetter *letter in partial) {
		ScrabbleGraphics::sharedObject(NULL)->translateObject(ScrabblePoint(-ScrabbleLetter::SCRABBLE_TILE_WIDTH, 
																			-ScrabbleLetter::SCRABBLE_TILE_HEIGHT),
															  ScrabbleSize(ScrabbleLetter::SCRABBLE_TILE_WIDTH,
																		   ScrabbleLetter::SCRABBLE_TILE_HEIGHT),
															  letter->getRef());
		letter->cleanFromBoard();
	}
	
	return score;
}

/* This method commits a solution */
void ScrabbleOpponent::commitSolution(Set<ScrabbleLetter *> &final, bool shouldDrawTiles) {
	
	//Put pieces on the board
	foreach(ScrabbleLetter *letter in final) {
		//Simulate the placings...
		ScrabbleGraphics::sharedObject(NULL)->translateObjectToObject(letter->getRef(), 
																	  board[letter->gridIndexX][letter->gridIndexY]->getRef());
		
		//Pause for a few seconds
		usleep(COMPUTER_VISUALIZATION_TIMING);
		
		letter->tileOn = board[letter->gridIndexX][letter->gridIndexY];
	}
	
	//Check valid move
	if (ScrabbleChecker::sharedObject()->checkMove(final, ScrabbleChecker::COMPUTER)) {
			
		//subtract sets
		inventory.subtract(final);
	
		if (shouldDrawTiles) {
			//Draw letters
			addLettersToInventory();
		}
		
		//Update score
		updateScore(ScrabbleChecker::sharedObject()->getPointsEarnedLastTurn());
	}
	else {
		//Somehow an invalid move... Retract letters
		foreach(ScrabbleLetter *letter in final) {
			ScrabbleGraphics::sharedObject(NULL)->translateObject(ScrabblePoint(-ScrabbleLetter::SCRABBLE_TILE_WIDTH, 
																				-ScrabbleLetter::SCRABBLE_TILE_HEIGHT),
																  ScrabbleSize(ScrabbleLetter::SCRABBLE_TILE_WIDTH,
																			   ScrabbleLetter::SCRABBLE_TILE_HEIGHT),
																  letter->getRef());
			letter->cleanFromBoard();
		}
	}
}

/* This method takes in a set of scrabbleLetters and saves their state
 * in a vector of solutionSets.
 */
void ScrabbleOpponent::saveSolutionSet(solutionSet &solution, Set<ScrabbleLetter *> save) {
	solution.description.clear();
	
	foreach (ScrabbleLetter * letter in save) {
		//Single solution
		partialSolution singleSolution;
		singleSolution.character = letter->randomCharacter;
		singleSolution.indexX = letter->gridIndexX;
		singleSolution.indexY = letter->gridIndexY;

		
		//Add to the solution
		solution.description.add(singleSolution);
	}
	solution.set = save;
}

/* This method takes in a set of scrabbleLetters and restores the set
 * according to a vector of solutionSets
 */
void ScrabbleOpponent::restoreSolutionSet(solutionSet &restore) {
	foreach(partialSolution solution in restore.description) {
		//Restore set
		ScrabbleLetter *singleLetter = scrabbleLetterWithCharacter(solution.character, restore.set);
		
		//Set the tile on so that this letter isn't returned again
		singleLetter->tileOn = board[0][0];
		
		//Edit solution set
		if (singleLetter != NULL) {
			singleLetter->gridIndexX = solution.indexX;
			singleLetter->gridIndexY = solution.indexY;
		}
	}
}

/* Return true if bounds fits, false otherwise. */
bool ScrabbleOpponent::checkBoundsOfLetters(Set<ScrabbleLetter *> check) {
	foreach (ScrabbleLetter *letter in check) {
		if (letter->gridIndexX >= ScrabbleBoard::NUM_ROWS_COLS ||
			letter->gridIndexX < 0 ||
			letter->gridIndexY >= ScrabbleBoard::NUM_ROWS_COLS ||
			letter->gridIndexY < 0)
			return false;
	}
	return true;
}

/* This method checks whether a letter in the set inv is on the position of the grid x and y */
bool ScrabbleOpponent::letterWithPosition(int x, int y, Grid<ScrabbleLetter *> inv) {
	//Make sure the letters were played down before
	return inv[x][y] != NULL && inv[x][y]->isPermanent();
}

/* Given amount increase, updates' the computers' score */
void ScrabbleOpponent::updateScore(int increase) {
	score += increase;
	ScrabbleGraphics::sharedObject(NULL)->changeTextOfLabel(scoreRef,  "Computer: \n" + IntegerToString(score));
}

/* This method inserts into the inventory
 * 7 letters.
 */
void ScrabbleOpponent::addLettersToInventory() {
	while (inventory.size() < ScrabbleUser::NUM_LETTERS_INVENTORY) {
		//The character for letter
		char letterChar = ScrabbleRandom::sharedObject()->getScrabbleCharacter();
		
		//No more letters left
		if (letterChar == 0)
			break;
		
		ScrabbleLetter *add = new ScrabbleLetter(-ScrabbleLetter::SCRABBLE_TILE_WIDTH, 
												 -ScrabbleLetter::SCRABBLE_TILE_HEIGHT,
												 letterChar);
		
		
		//Add the new letters to inventory
		inventory.add(add);
	}
}

/* Makes the first move if there are no letters on the board */
void ScrabbleOpponent::makeFirstMove() {
	//First check to see if it's the first move 
	if (ScrabbleChecker::sharedObject()->isFirstMove()) {
		//Pick random letter place in center square
		Set<ScrabbleLetter *> randomLetter;
		int randomIndex = RandomInteger(0, inventory.size()-1);
		
		foreach(ScrabbleLetter *letter in inventory) {
			if (--randomIndex <= 0) {
				//Choose this letter
				randomLetter.add(*inventory.find(letter));
				inventory.remove(letter);
				
				//Set letter attributes
				letter->gridIndexX = 7;
				letter->gridIndexY = 7;
				
				letter->tileOn = board[letter->gridIndexX][letter->gridIndexY];
				
				break;
			}
		}
		
		//Commit it
		commitSolution(randomLetter, false);
	}
}

/* Returns true if computer has letters left. False otherewise */
bool ScrabbleOpponent::lettersLeft() {
	return inventory.size() != 0;
}

int ScrabbleOpponent::getScore() {
	return score;
}

/* Prints out computers' letters */
void ScrabbleOpponent::computerInventory() {
	foreach (ScrabbleLetter *letter in inventory) {
		cout << letter->randomCharacter << endl;
	}
}

/* Forces the computer to have a specific hand */
void ScrabbleOpponent::forceInventory(string newLetters) {
	
	Set<ScrabbleLetter *> newSet;
	
	for (int i = 0; i < newLetters.length(); i++) {		
		//Create new letters
		
		ScrabbleLetter *add = new ScrabbleLetter(-ScrabbleLetter::SCRABBLE_TILE_WIDTH, 
												 -ScrabbleLetter::SCRABBLE_TILE_HEIGHT,
												 newLetters[i]);
		
		
		//Add the new letters to inventory
		newSet.add(add);
	}
	
	//remove old letters
	foreach (ScrabbleLetter *letter in inventory) {
		//Clean up graphics
		ScrabbleGraphics::sharedObject(NULL)->destroyImageWithRef(letter->getRef());
		delete letter;
		letter = NULL;
	}
	inventory.clear();
	inventory = newSet;
}

/* Computer automatically wins */
void ScrabbleOpponent::instawin() {
	//increase score to a million
	updateScore(1000000);
	
	//Clear inventory
	//Release memory
	foreach (ScrabbleLetter *letter in inventory) {
		ScrabbleGraphics::sharedObject(NULL)->destroyImageWithRef(letter->getRef());
		delete letter;
	}	
	inventory.clear();
}