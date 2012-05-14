/*
 *  ScrabblePoint.cpp
 *  Scrabble
 *
 *  Created by Max Lam on 7/22/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 */

#include "ScrabblePoint.h"

/* The instantiator method, by default, sets x and y
 * to be 0
 */
ScrabblePoint::ScrabblePoint() {
	this->x = 0;
	this->y = 0;
}

/* A secondary instantiator method, which allows the caller to immediately
 * assign an x and y value to the objects' variables
 *
 * @param x and y are initial values to set the objects' variables: x and y
 * 
 * @return a new point
 */
ScrabblePoint::ScrabblePoint(double x, double y) {
	this->x = x;
	this->y = y;
}

/* The below 4 functions are setters and getters that update or return the dimensional
 * coordinates that the object stores
 */
void ScrabblePoint::setX(double x) {
	this->x = x;
}

double ScrabblePoint::getX() {
	return this->x;
}

void ScrabblePoint::setY(double y) {
	this->y = y;
}

double ScrabblePoint::getY() {
	return this->y;
}