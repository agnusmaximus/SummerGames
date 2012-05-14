/*
 *  ScrabbleScore.h
 *  Scrabble
 *
 *  Created by Max Lam on 7/24/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 *  This class scores scrabble letters.
 *
 *  Because both the user and the computer will need to compute their scores,
 *  there will be another shared object that will be called by both the 
 *  computer portion of the program and the user.
 *
 *  This class also manages how the score is displayed -- at the bottom of the window.
 *
 */

#pragma once

#include "ScrabbleGraphics.h"
#include "ScrabbleLetter.h"
#include "set.h"
#include "stack.h"

class ScrabbleScore {
	
public:
	//Constructor which shouldn't be called directly
	ScrabbleScore();
	
	//Returns a shared class instance
	static ScrabbleScore * sharedObject();
	
	//Computes the score of a word given the set of letters
	//that form it
	int pointsForWord(Set<ScrabbleLetter *> letters);
	
private:
	
};

