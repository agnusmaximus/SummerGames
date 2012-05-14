/*
 *  ScrabblePoint.h
 *  Scrabble
 *
 *  Created by Max Lam on 7/22/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 *
 *  This class implements the point class, which 
 *  specifies an x and a y coordinate
 */

#pragma once

#include <math.h>

class ScrabblePoint {
	
public:
	//Main constructor method
	ScrabblePoint();
	
	//Secondary constructor
	ScrabblePoint(double x, double y);
	
	//Setters and getters
	void setX(double x);
	double getX();
	void setY(double y);
	double getY();
private:
	//The point
	double x, y;
};

