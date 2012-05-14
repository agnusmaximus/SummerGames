/*
 *  ScrabbleSize.cpp
 *  Scrabble
 *
 *  Created by Max Lam on 7/22/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 */
#include "ScrabbleSize.h"

/* This function is the function called whenever a new size
 * class is instantiated
 */
ScrabbleSize::ScrabbleSize() {
	//Default size is 0,0
	this->width = 0;
	this->height = 0;
}

/* Initializes size to the passed parameters
 *
 * @param width is the width
 * @param height is the height
 *
 * @return void
 */
ScrabbleSize::ScrabbleSize(double width, double height) {
	this->width = width;
	this->height = height;
}	

//Setters & getters
double ScrabbleSize::getWidth() {
	return this->width;
}

void ScrabbleSize::setWidth(double width) {
	this->width = width;
}

double ScrabbleSize::getHeight() {
	return this->height;
}

void ScrabbleSize::setHeight(double height) {
	this->height = height;
}
