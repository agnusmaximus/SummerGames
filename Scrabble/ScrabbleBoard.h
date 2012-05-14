/*
 *  ScrabbleBoard.h
 *  Scrabble
 *
 *  Created by Max Lam on 7/22/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 *
 *  This class represents the scrabble board, which is constructed
 *  from Scrabble tiles. More specifically, the scrabble board is represented
 *  by a grid of tiles. 
 *
 *  Also, the ScrabbleBoard class defines constants that specify how many
 *  tiles there will be per row & column.
 */

#pragma once

#include "ScrabbleTile.h"
#include "ScrabbleUser.h"
#include "grid.h"

class ScrabbleBoard {
	
public:
	//Constructor & destructor
	ScrabbleBoard(string bonusPositions, string tileColors);
	~ScrabbleBoard();
	
	//Determines the number of tiles per row and column
	static const int NUM_ROWS_COLS = 15;
	
	//Memory representation of the scrabble board
	Grid<ScrabbleTile *> board;
private:
	
	//Reads a file for its data and returns a grid of bonus positions
	Grid<int> bonusPositionsFromFile(string file);
	
	//Reads a file for its data and returns a grid of colors (string)
	Grid<string> colorPositionsFromFile(string file);
	
	//Flags that reference a color
	static const int BLACK = 0;
	static const int WHITE = 1;
	static const int GREEN = 2;
	static const int BLUE = 3;
	static const int YELLOW = 4;
	static const int RED = 5;
	static const int BROWN = 6;
	static const int GREY = 7;
	static const int ORANGE = 8;
	
	//Returns a string given a color flag
	string colorStringFromColorFlag(int cFlag);
};