/*
 *  ScrabbleSize.h
 *  Scrabble
 *
 *  Created by Max Lam on 7/22/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 *  This class represents a size component which will be used in many
 *  parts of the Scrabble program; for instance, the size of each tile,
 *  Or the size of the whole screen...
 *
 */

#pragma once

class ScrabbleSize {
	
public:
	//Constructors method
	ScrabbleSize();
	
	//Secondary constructor
	ScrabbleSize(double width, double height);
	
	//Setters & getters
	double getWidth();
	void setWidth(double width);
	double getHeight();
	void setHeight(double height);
private:
	double width, height;
};

