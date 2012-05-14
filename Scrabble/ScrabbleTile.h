/*
 *  ScrabbleTile.h
 *  Scrabble
 *
 *  Created by Max Lam on 7/22/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 *
 *  The scrabble tile is a class that represents not only pieces of
 *  the map, but also the parent class of the letters that the user
 *  will drag around.
 *
 *  All tiles will have a position and a size.
 *
 *  Some constants will be defined in this class, such as how big each
 *  scrabble tile will be.
 */

#pragma once

#include "ScrabbleGraphics.h"
#include "ScrabblePoint.h"
#include "ScrabbleSize.h"
#include <iostream>

class ScrabbleTile {
public:
	//Constructor & Destructor
	ScrabbleTile(double x, double y, string color, int scrabbleTileType);
	ScrabbleTile(double x, double y);
	virtual ~ScrabbleTile();
	
	//Scrable tile type
	int scrabbleTileType;
	
	//Scrabble tile type flags
	static const int NORMAL_TILE = 0;
	static const int DOUBLE_LETTER_TILE = 1;
	static const int DOUBLE_WORD_TILE = 2;
	static const int TRIPLE_LETTER_TILE = 3;
	static const int TRIPLE_WORD_TILE = 4;
	
	//Scrable tile dimension
	static const double SCRABBLE_TILE_WIDTH;
	static const double SCRABBLE_TILE_HEIGHT;
	
	ScrabblePoint getTilePosition();
	ScrabbleSize getTileDimensions();
	
	//Get reference
	int getRef();
	
private:
	//Some drawing methods that will be done in the background
	//(This is virtual because subclasses of ScrabbleTile may implement
	// different draw functions)
	virtual void drawTile(string color);
	
	//Based on the type of scrabble tile this object is, returns a string
	//that notifies the user of what type the tile is
	string descriptionForTileType();
	
protected:
	//Tile position and size
	ScrabblePoint *tileLocation;
	ScrabbleSize *tileDimensions;
	
	//The scrabble reference in graphics
	int ref;
};

