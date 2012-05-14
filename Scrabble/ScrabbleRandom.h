/*
 *  ScrabbleRandom.h
 *  Scrabble
 *
 *  Created by Max Lam on 7/23/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 *  This class defines the randomness of scrabble
 *
 */

#pragma once

#include <stdlib.h>
#include <time.h>
#include "vector.h"

class ScrabbleRandom {
	
public:
	//Constructor
	ScrabbleRandom();
	
	//Shared object
	static ScrabbleRandom *sharedObject();
	
	char getScrabbleCharacter();
	
	void getTiles();
	
	void addCharacter(char character);
	
private:
	Vector<char> possibleScrabbleCharacters;
};

