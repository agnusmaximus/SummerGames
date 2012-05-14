/*
 *  ScrabbleGraphics.h
 *  Scrabble
 *
 *  Created by Max Lam on 7/21/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 *
 *	This class contains all the methods that set up and create
 *  Scrabble graphics.
 *
 *  The caller will create a new static instance of the graphics object and
 *  reference this object throughout the program. This object will be 
 *  public to every class that calls the 'sharedObject()' method
 *
 *  In essence, this class acts as a mediator to the cocoa class that handles most
 *  graphics
 */
#pragma once

#define BUTTON_WIDTH  150
#define BUTTON_HEIGHT 75

#include "simpio.h"
#include "ScrabblePoint.h"
#include "ScrabbleSize.h"

class ScrabbleGraphics {
public:
	/* Constructor and destructor (these should not be called directly) */

	/* Does everything to initialize scrabble graphics,
	 * including the tiles, the game board, etc.
	 *
	 * @param size specifies window dimensions
	 */
	ScrabbleGraphics(ScrabbleSize size);
	~ScrabbleGraphics();
	
	/* This method is a wrapper function that allows the user to
	 * access a shared graphics object
	 */
	static ScrabbleGraphics * sharedObject(ScrabbleSize *sz);
	
	/* This method allows the user to create a colored rectangle with
	 * an option label in the center
	 */
	int drawTextBlock(ScrabblePoint &location, ScrabbleSize &dimension, string text, 
					  string textColor,string backgroundColor);
	
	/* This method allows the user to graphically create an image */
	int drawImageBlock(ScrabblePoint &location, ScrabbleSize &dimension,
					   string imgName, string text, string textColor);	
	
	/* This method allows the caller to create a button */
	int drawButtonBlock(ScrabblePoint &location, ScrabbleSize &dimension,
						string text);
	
	/* Get window width and height... */
	double getWindowWidth();
	double getWindowHeight();
	
	/* Bring image to front with an index */
	void bringImageToFront(int ref);
	
	/* Return the position of the image with ref */
	ScrabblePoint imagePositionWithRef(int ref);
	 
	/* Return the size of the image with ref */
	ScrabbleSize imageSizeWithRef(int ref);
	
	/* Animate a picture with a reference */
	void translateObject(ScrabblePoint position, ScrabbleSize size, int ref);
	
	/* Place a ref on top of ref */
	void translateObjectToObject(int ref, int ref2);
	
	/* This method returns a point where the user clicked/dragged -- null if no click/drag */
	ScrabblePoint * clickedOrDragged(int ref);
	
	/* This method returns intersection of ref1 and ref2 */
	double imageIntersectsImage(int ref1, int ref2);
	
	/* This method checks if an image intersects another image */
	bool imageIntersectsAnother(int ref1);
	
	/* This method checks if a button with reference was pressed */
	bool buttonWasPressed(int ref);
	
	/* Changes an images' text color */
	void changeTextColorOfImage(int ref, string color);
	
	/* Notifies the user with a certain text */
	void notifyUser(string text);
	
	/* Destroys an image with reference ref */
	void destroyImageWithRef(int ref);
	
	/* Prompts the user to see if he is sure about an action */
	bool sureAboutAction(string action);
	
	/* Sets images' subtext */
	void setSubtext(int ref, string text);
	
	/* Create label at x and y */
	int createLabelAt(ScrabblePoint point, string msg);
	
	/* Change text of label referenced by ref */
	void changeTextOfLabel(int ref, string msg);
	
	/* Load a screen referenced by nib file */
	void loadScreen(string fileName);
	
	/* Set alpha value of image */
	void setAlphaOfImage(int ref, float value);
	
	/* Menu */
	void createMenu();
	
	void showMenu();
	
	/* Show game */
	void showGame();
	
	/* What button was pressed */
	int whichButtonPressed();
	
	/* Set the button pressed state */
	void setButtonMode(int mode);
	
	/* Clears everything */
	void clearEverything();
	
	//Pixels per inch
	static const int PPI = 72;
	
private:
	//Size of font
	static const int ptSize = 20;
	
	//Window dimensions
	double windowWidth;
	double windowHeight;
};