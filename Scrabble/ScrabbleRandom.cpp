/*
 *  ScrabbleRandom.cpp
 *  Scrabble
 *
 *  Created by Max Lam on 7/23/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 */

#include "ScrabbleRandom.h"
#include <iostream>

/* This method instantiates the vector full of scrabble letters
 * from a constant text file.
 */
ScrabbleRandom::ScrabbleRandom() {
	getTiles();
	
	//Seed random
	srand(time(NULL));
}

/* Get tiles from file */
void ScrabbleRandom::getTiles() {
	ifstream characterFile("ScrabbleLetters.txt");
	while (!characterFile.eof()) {
		char i = characterFile.get();
		if (i != '\n' && i != -1) 
			possibleScrabbleCharacters.add(i);
	}
	characterFile.close();
}

/* Returns a shared instance of this class.
 *
 * ScrabbleRandom has to be shared because both
 * the computer and the player will be drawing their
 * letters from the same pile.
 */
ScrabbleRandom * ScrabbleRandom::sharedObject() {
	static ScrabbleRandom *letters = NULL;
	
	if (letters == NULL) {
		letters = new ScrabbleRandom();
	}
	return letters;
}

char ScrabbleRandom::getScrabbleCharacter() {
	//No more letters
	if (possibleScrabbleCharacters.size() <= 1)
		return 0;
	
	int randNum = rand() % (possibleScrabbleCharacters.size()-1);
	char i = possibleScrabbleCharacters[randNum];
	possibleScrabbleCharacters.removeAt(randNum);
	return i;
}

void ScrabbleRandom::addCharacter(char character) {
	possibleScrabbleCharacters.add(character);
}