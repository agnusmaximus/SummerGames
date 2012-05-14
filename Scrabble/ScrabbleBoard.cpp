/*
 *  ScrabbleBoard.cpp
 *  Scrabble
 *
 *  Created by Max Lam on 7/22/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 */

#include "ScrabbleBoard.h"

/* This function instantiates an object of type scrabble board.
 *
 * @param bonus positions is a string that references the file that
 *        specify what positions the bonus places will be
 * @param tileColors is a string that references a file that contains
 *        what each tile's color should be
 *
 * @return an instance of the class
 */
ScrabbleBoard::ScrabbleBoard(string bonusPositions, string tileColors) {
	//First read the files
	Grid<int> bonusFlags = bonusPositionsFromFile(bonusPositions);
	Grid<string> positionColors = colorPositionsFromFile(tileColors);
	
	//Create the board
	board.resize(NUM_ROWS_COLS, NUM_ROWS_COLS);
	
	//Create the board
	for (int x = 0; x < NUM_ROWS_COLS; x++) {
		for (int y = 0; y < NUM_ROWS_COLS; y++) {
			//Create each individual tile and center them...
			board[x][y] = new ScrabbleTile(x * ScrabbleTile::SCRABBLE_TILE_WIDTH + ScrabbleUser::SIDE_SHELF_WIDTH,
										   y * ScrabbleTile::SCRABBLE_TILE_HEIGHT,
										   positionColors[x][y], bonusFlags[x][y]);
		}
	}
}

/* This function destroys an object of type scrabble board */
ScrabbleBoard::~ScrabbleBoard() {
	for (int x = 0; x < NUM_ROWS_COLS; x++) {
		for (int y = 0; y < NUM_ROWS_COLS; y++) {
			delete board[x][y];
		}
	}
}

/* This method takes in a file that specifies bonus positions.
 * then it returns a grid of integers that act as flags for what
 * type the tile at the specific position will be.
 *
 * (Assumes that file x and y dimensions are NUM_ROWS_COL by NUM_ROWS_COL
 *
 * @param file is the file
 * @return a grid of flags
 */
Grid<int> ScrabbleBoard::bonusPositionsFromFile(string file) {
	ifstream bonusFile;
	bonusFile.open(file.c_str());
	
	Grid<int> bonusPositions(NUM_ROWS_COLS, NUM_ROWS_COLS);
	
	//Read until eof and scan data
	int x = 0, y = 0;
	while (!bonusFile.eof()) {
		//Get each character from file
		int i = bonusFile.get();
		
		//Check if its a newline -- skip it if it is
		if ((char)i == '\n')
			i = bonusFile.get();
		
		//Set it to the grid (the integer value)
		bonusPositions[x++][y] = i - '0';
		
		//Check when next row
		if (x >= NUM_ROWS_COLS) {
			x = 0;
			y++;
		}
		
		//Done scanning when all elements have
		//been assigned a tile type
		if (y >= NUM_ROWS_COLS)
			break;
	}
	
	bonusFile.close();
	return bonusPositions;
}

/* Takes a file that specifies color of each tile at specific position
 * (Assumes that file x and y dimensions are NUM_ROWS_COL by NUM_ROWS_COL
 *
 * @returns a grid of strings that represent these colors
 */
Grid<string> ScrabbleBoard::colorPositionsFromFile(string file) {
	ifstream colorFile;
	colorFile.open(file.c_str());
	
	Grid<string> positionColors(NUM_ROWS_COLS, NUM_ROWS_COLS);
	
	//Read until eof
	int x = 0, y = 0;
	while(!colorFile.eof()) {
		//Scan in the color flag
		int colorFlag;
		
		colorFlag = colorFile.get();
		
		//Skip newlines
		if ((char)colorFlag == '\n')
			colorFlag = colorFile.get();
		
		//Convert character to integer
		colorFlag -= '0';
		
		//Convert color flag to color
		positionColors[x++][y] = colorStringFromColorFlag(colorFlag);
		
		//Check when next row
		if (x >= NUM_ROWS_COLS) {
			x = 0;
			y++;
		}
		
		//Done scanning when all elements are a specific color
		if (y >= NUM_ROWS_COLS)
			break;
	}
	
	colorFile.close();
	
	return positionColors;
}

/* Returns a string given a color flag
 *
 * @param cFlag is the color flag
 * @return color flag
 */
string ScrabbleBoard::colorStringFromColorFlag(int cFlag) {
	switch (cFlag) {
		case BLACK:
			return "black";
		case WHITE:
			return "white";
		case BLUE:
			return "blue";
		case GREEN:
			return "green";
		case GREY:
			return "grey";
		case RED:
			return "red";
		case BROWN:
			return "brown";
		case YELLOW:
			return "yellow";
		case ORANGE:
			return "orange";
	}
	return "";
}