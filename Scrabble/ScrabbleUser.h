/*
 *  ScrabbleUser.h
 *  Scrabble
 *
 *  Created by Max Lam on 7/23/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 *
 *  This class handles user interactions such as letter dragging,
 *  letter committing, etc.
 *
 *	The user will have a vector of letters that represent his words.
 */

#pragma once

#include "ScrabbleLetter.h"
#include "ScrabbleRandom.h"
#include "ScrabbleChecker.h"
#include "vector.h"
#include "grid.h"
#include "set.h"

class ScrabbleUser {

public:
	//Constructor & destructor
	ScrabbleUser();
	~ScrabbleUser();
	
	//How big is the shelf to store the letters
	static const float SIDE_SHELF_WIDTH;
	
	//Allows the player to drag and drop letters
	void userMove();
	
	//Checks if the player commited his move
	void commitedMove();
	void passedMove(); 
	void flushedMove();

	//Updates the indices of every letter the user has. This is so they can be
	//used with grids like a board piece on a board
	void updateInventoryGridPositions(Grid<ScrabbleTile *>tiles);
	
	//Whether the user ended his turn
	bool userEndedTurn;
	
	static const int NUM_LETTERS_INVENTORY = 7;		//7 Letters
	
	int numConsecutivePasses;
	
	//Does user have any letters left
	bool lettersLeft();
	
	int getScore();
	
	void cleanUp();
	
	//Cheats
	void forceInventory(string newHand);
	void instawin();
	
private:
	
	//X location of the letters
	double xPos;
	
	//Start y location of the letters
	double yStartPos;
	
	//Keep track of score image
	int scoreRef;
	
	//The user's score
	int score;
	
	//Spawn scrabble letter
	ScrabbleLetter * spawnScrabbleLetter(double x, double y);
	
	//The vector of letters that the user has
	Vector<ScrabbleLetter *> inventory;
	
	//The vector of letters the the user has placed on board
	Set<ScrabbleLetter *> onBoard;
	
	//Create buttons
	void createUserButtons();
	
	//Button reference
	int commitRef, passRef, flushRef;
	
	//Add 7 letters to inventory
	void addLettersToInventory();
};
