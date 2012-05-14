/*
 *  ScrabbleTile.cpp
 *  Scrabble
 *
 *  Created by Max Lam on 7/22/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 */

#include "ScrabbleTile.h"

//Constants
const double ScrabbleTile::SCRABBLE_TILE_WIDTH = .8;
const double ScrabbleTile::SCRABBLE_TILE_HEIGHT = .8;

/* This constructor allows the caller to immediately set a tile
 * position. The tile dimensions will automatically be set to SCRABBLE_TILE_WIDTH
 * and SCRABBLE_TILE_HEIGHT
 *
 * @param x and y are the positions to immediately set the tile to
 * @param color is the block's color
 * @param scrabbleTileType describes what kind of tile it is
 *
 * @return a new ScrabbleTile instance
 */
ScrabbleTile::ScrabbleTile(double x, double y, string color, int scrabbleTileType) {
	//Create the location and dimensions of the tile
	tileLocation = new ScrabblePoint(x, y);
	tileDimensions = new ScrabbleSize(SCRABBLE_TILE_WIDTH, SCRABBLE_TILE_HEIGHT);

	//Set tile type
	this->scrabbleTileType = scrabbleTileType;
	
	//Display it graphically
	drawTile(color);
}

/* Constructor that just sets tile position */
ScrabbleTile::ScrabbleTile(double x, double y) {
}

/* Destroy the memory that the class has created on the heap */
ScrabbleTile::~ScrabbleTile() {
	delete tileLocation;
	delete tileDimensions;
}

/* This method draws the tile according to its basic information (location, size, etc)*/
void ScrabbleTile::drawTile(string color) {
	//Save the reference as well
	ref = ScrabbleGraphics::sharedObject(NULL)->drawTextBlock(*tileLocation, *tileDimensions, 
														descriptionForTileType(), "white", color);
}


/* This method returns a string that represents a color.
 * It will use the instance variable scrabbleTileType to determine this
 * factor.
 *
 * @return string that references the color
 */
string ScrabbleTile::descriptionForTileType() {
	//Basically check what the type is
	
	if (scrabbleTileType == NORMAL_TILE)
		return "";
	if (scrabbleTileType == DOUBLE_LETTER_TILE) 
		return "DL";
	if (scrabbleTileType == DOUBLE_WORD_TILE)
		return "DW";
	if (scrabbleTileType == TRIPLE_LETTER_TILE)
		return "TL";
	if (scrabbleTileType == TRIPLE_WORD_TILE)
		return "TW";
	return "";
}

/* Returns tile position */
ScrabblePoint ScrabbleTile::getTilePosition() {
	return *tileLocation;
}

/* Returns tile dimsnsion */
ScrabbleSize ScrabbleTile::getTileDimensions() {
	return *tileDimensions;
}

/* Returns reference to image */
int ScrabbleTile::getRef() {
	return ref;
}