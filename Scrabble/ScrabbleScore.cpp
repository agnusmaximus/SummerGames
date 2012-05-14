/*
 *  ScrabbleScore.cpp
 *  Scrabble
 *
 *  Created by Max Lam on 7/24/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 */

#include "ScrabbleScore.h"

//Constructor which shouldn't be called directly
ScrabbleScore::ScrabbleScore() {
	//initialization things
}

//Returns a shared class instance
ScrabbleScore * ScrabbleScore::sharedObject() {
	static ScrabbleScore *instance = NULL;
	
	if (instance == NULL) {
		//Create instance
		instance = new ScrabbleScore();
	}
	return instance;
}

/* Given a letters of words, computes the score for the word */
int ScrabbleScore::pointsForWord(Set<ScrabbleLetter *> letters) {
	int score = 0;
	
	//Stack of word score multipliers
	Stack<int> multipliers;
	
	//Compute each letter score adding to total score
	foreach(ScrabbleLetter *letter in letters) {
		int letter_value = letter->value;
		
		//Check null
		if (letter->tileOn == NULL)
			return 0;
		
		if (letter->tileOn->scrabbleTileType != ScrabbleTile::NORMAL_TILE) {
			switch(letter->tileOn->scrabbleTileType) {
				case ScrabbleTile::DOUBLE_LETTER_TILE:
					letter_value *= 2;
					break;
				case ScrabbleTile::TRIPLE_LETTER_TILE:
					letter_value *= 3;
					break;
				case ScrabbleTile::DOUBLE_WORD_TILE:
					multipliers.push(2);
					break;
				case ScrabbleTile::TRIPLE_WORD_TILE:
					multipliers.push(3);
					break;
			}
		}
		
		//Add to total score
		score += letter_value;
	}
	
	//Final score
	while (!multipliers.isEmpty()) {
		score *= multipliers.pop();
	}
	
	return score;
}