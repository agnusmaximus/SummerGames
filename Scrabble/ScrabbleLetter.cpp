/*
 *  ScrabbleLetter.cpp
 *  Scrabble
 *
 *  Created by Max Lam on 7/22/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 */

#include "ScrabbleLetter.h"

/* This function overrides its parents' constructor 
 *
 * @param x and y are the letter's coordinates
 * @param letter is the letter to set the block to
 */
ScrabbleLetter::ScrabbleLetter(double x, double y, char letter):ScrabbleTile(x, y) {
	
	//Set position
	tileLocation = new ScrabblePoint(x, y);
	tileDimensions = new ScrabbleSize(SCRABBLE_TILE_WIDTH, SCRABBLE_TILE_HEIGHT);
	
	randomCharacter = letter;
	
	//Display the letter in text-color black, with a random character
	drawTile("black");
	
	//No grid index yet -- letter on shelf
	gridIndexX = -1;
	gridIndexY = -1;
	
	//Not on a tile
	tileOn = NULL;
	
	//State empty
	state = 0;
	
	//No pointers to any other letter
	letterRight = NULL;
	letterLeft = NULL;
	letterUp = NULL;
	letterDown = NULL;
	
	//set temporary
	setPermanent(false);
}

/* This function is like the one implemented in its parent class
 *
 * Except, instead of a background color, an image is set 
 *
 * @param color is the color for the text
 * @return void
 */
void ScrabbleLetter::drawTile(string color) {
	string storage = "";
	storage.append(1,randomCharacter);
	
	//Save reference as well
	ref = ScrabbleGraphics::sharedObject(NULL)->drawImageBlock(*tileLocation, *tileDimensions,
															   "ScrabbleTile.png", storage, color);
	
	//Set letter value
	value = letterCosts(randomCharacter);
	
	string letterVal = "";
	if (value < 10) {
		letterVal.append(1,value+'0');
	}
	else {
		letterVal = "10";
	}
	
	//Create subtext with that value
	ScrabbleGraphics::sharedObject(NULL)->setSubtext(ref, letterVal);
}

/* This method brings the letter block graphicaly above
 * all other images.
 */
void ScrabbleLetter::bringToFront() {
	ScrabbleGraphics::sharedObject(NULL)->bringImageToFront(ref);
}

/* This method moves the letter to a specific position : pt */
void ScrabbleLetter::translateLetterToPoint(ScrabblePoint pt, ScrabbleSize sz) {
	//Dont move the letter if it is committed
	if (isCommitted())
		return;
	
	ScrabbleGraphics::sharedObject(NULL)->translateObject(pt, sz, ref);
	
	//Update the position of the letter to the grid
	ScrabblePoint newPt = ScrabbleGraphics::sharedObject(NULL)->imagePositionWithRef(ref);
	this->tileLocation->setX(newPt.getX() / ScrabbleGraphics::PPI);
	this->tileLocation->setY(newPt.getY() / ScrabbleGraphics::PPI);
}

/* Operator overloads -- used to compare letters */
bool ScrabbleLetter::operator == (ScrabbleLetter &i) {
	//Check memory locations -- don't compare content
	return this == &i;
}

/* Set the state of this letter to be committed */
void ScrabbleLetter::setCommitted() {
	ScrabbleGraphics::sharedObject(NULL)->changeTextColorOfImage(ref, (char *)"black");
	state |= LETTER_COMMITTED;
	
	//Letter is permanent
	setPermanent(true);
}

/* Returns whether this letter is committed */
bool ScrabbleLetter::isCommitted() {
	return (state & LETTER_COMMITTED);
}

/* Returns how many points a character is */
int ScrabbleLetter::letterCosts(char character) {
	switch (character) {
		case 'e':
		case 'a':
		case 'i':
		case 'o':
		case 'n':
		case 'r':
		case 't':
		case 'l':
		case 's':
		case 'u':
			return 1;
		case 'd':
		case 'g':
			return 2;
		case 'b':
		case 'c':
		case 'm':
		case 'p':
			return 3;
		case 'f':
		case 'h':
		case 'v':
		case 'w':
		case 'y':
			return 4;
		case 'k':
			return 5;
		case 'j':
		case 'x':
			return 8;
		case 'q':
		case 'z':
			return 10;
			
	}
	return 0;
}

/* Cleans the letter from the board so that no other letters are confused with it */
void ScrabbleLetter::cleanFromBoard() {
	tileOn = NULL;
	letterRight = NULL;
	letterLeft = NULL;
	letterUp = NULL;
	letterDown = NULL;
}

//Returns if this letter is permanently on the board
bool ScrabbleLetter::isPermanent() {
	return permanent;
}
	
//Sets permanent of the letter
void ScrabbleLetter::setPermanent(bool yesOrNo) {
	permanent = yesOrNo;
	
	//If yes, make the letter transparent...
	if (!yesOrNo) {
		ScrabbleGraphics::sharedObject(NULL)->setAlphaOfImage(getRef(), .7);
	}
	//If no, make the letter opaque
	else {
		ScrabbleGraphics::sharedObject(NULL)->setAlphaOfImage(getRef(), 1);
	}
}