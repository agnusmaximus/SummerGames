/*
 *  ScrabbleLetter.h
 *  Scrabble
 *
 *  Created by Max Lam on 7/22/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 *
 *	This file implements the letters that the user will
 *  drag around on the map. 
 *
 *  It extends from ScrabbleTile because each letter
 *  also has similar attributes to the tile.
 *
 *  But, there are differences such as the state of the
 *  tile, and some other methods
 *
 *  ScrabbleLetters can be dragged around and therefore be moved from place
 *  to place.
 */

#pragma once

#include "ScrabbleTile.h"
#include "random.h"

/* Flag defines for the letter state */
#define LETTER_COMMITTED 1

class ScrabbleLetter : public ScrabbleTile {

public:
	//Instantiator method
	ScrabbleLetter(double x, double y, char letter);
	
	//Get a random character
	char randomCharacter;
	
	//Characters' value
	int value;
	
	//Bring the letter to the front
	void bringToFront();
	
	//Enlarge the letter to a size
	void translateLetterToPoint(ScrabblePoint pt, ScrabbleSize sz);
	
	//Index in grid
	int gridIndexX, gridIndexY;
	
	//What tile is this letter on
	ScrabbleTile *tileOn;
	
	
	//What are the letters that are beside this letter
	ScrabbleLetter *letterRight;
	ScrabbleLetter *letterLeft;
	ScrabbleLetter *letterUp;
	ScrabbleLetter *letterDown;
	
	//Set letter state
	void setCommitted();
	bool isCommitted();
	
	//Operator overloading
	bool operator == (ScrabbleLetter &i);
	
	//Disassasociate letter from board
	void cleanFromBoard(); 
	
	//Returns if this letter is permanently on the board
	bool isPermanent();
	
	//Sets permanent of the letter
	void setPermanent(bool yesOrNo);
	
private:
	//A different draw tile method
	virtual void drawTile(string color);

	//Calculate how much a letter costs
	int letterCosts(char character);
	
	//Flag specifying what state the letter is in
	char state;
	
	//Is the tile permanent
	bool permanent;
};