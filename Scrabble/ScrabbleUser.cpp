/*
 *  ScrabbleUser.cpp
 *  Scrabble
 *
 *  Created by Max Lam on 7/23/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 */

#include "ScrabbleUser.h"

//Constants
const float ScrabbleUser::SIDE_SHELF_WIDTH = 1.8;		//Inches

/* What this constructor method does is it initializes an array
 * of letters to be stored within the vector.
 *
 * @return the object of itself
 */
ScrabbleUser::ScrabbleUser() {
	//X location of the letters
	xPos = ScrabbleGraphics::sharedObject(NULL)->getWindowWidth() -
	ScrabbleLetter::SCRABBLE_TILE_WIDTH/2 - SIDE_SHELF_WIDTH/2;
	
	//Start y location of the letters
	yStartPos = 2;
	
	//Create 7 letters and add to the screen and vector
	addLettersToInventory();
	
	//Create the user's buttons
	createUserButtons();
	
	//User goes first
	userEndedTurn = false;
	
	//Create score keeper
	scoreRef = ScrabbleGraphics::sharedObject(NULL)->createLabelAt(ScrabblePoint(ScrabbleGraphics::sharedObject(NULL)->getWindowWidth() * 72
																	  - 115,
																	  ScrabbleGraphics::sharedObject(NULL)->getWindowHeight() * 72
																	  - 68), 
														"Score: \n0");
	score = 0;
	numConsecutivePasses = 0;
}

/* Spawns a scrabble letter at the specified positions
 *
 * @param x is the x location of the letter
 * @param y is the y location of the letter
 *
 * @return a new instance of scrabbleLetter
 */
ScrabbleLetter * ScrabbleUser::spawnScrabbleLetter(double x, double y) {
	//Letter character
	char letterChar = ScrabbleRandom::sharedObject()->getScrabbleCharacter();
	
	//Out of letters return NULL
	if (letterChar == 0)
		return NULL;
	
	//Scrabble randomness	
	ScrabbleLetter *newLetter = new ScrabbleLetter(x, y, letterChar);
	
	//No more letters left
	if (newLetter == NULL)
		return NULL;
	
	double interval = ScrabbleLetter::SCRABBLE_TILE_HEIGHT*1.5;
	
	//Check intersection and move down
	while (ScrabbleGraphics::sharedObject(NULL)->imageIntersectsAnother(newLetter->getRef())) {
		ScrabbleGraphics::sharedObject(NULL)->translateObject(ScrabblePoint(x, y += interval), 
															ScrabbleSize(ScrabbleLetter::SCRABBLE_TILE_WIDTH,
																		 ScrabbleLetter::SCRABBLE_TILE_HEIGHT),
															newLetter->getRef());
	}
	return newLetter;
}

/* This method destroys and releases memory */
ScrabbleUser::~ScrabbleUser() {
	//Destroy letters in vector
	for (int i = 0; i < inventory.size(); i++) {
		ScrabbleLetter * letter = inventory[i];
		inventory.removeAt(i);
		
		ScrabbleGraphics::sharedObject(NULL)->destroyImageWithRef(letter->getRef());
		
		delete letter;
		i--;
	}
	
	onBoard.clear();
}

/* Checks for any mouse click and drags on letters */
void ScrabbleUser::userMove() {
	
	//Loop through all letters and check if they are clicked
	for (int i = 0; i < inventory.size(); i++) {
		ScrabblePoint *ptTouch;
		
		//Check click on letter frame
		if ((ptTouch = ScrabbleGraphics::sharedObject(NULL)->clickedOrDragged(inventory[i]->getRef())) != NULL) {
			
			//There was a click -- move tile to mouse position
			inventory[i]->translateLetterToPoint(ScrabblePoint(ptTouch->getX()/ScrabbleGraphics::PPI - 
															   ScrabbleLetter::SCRABBLE_TILE_WIDTH/2, 
															   ptTouch->getY()/ScrabbleGraphics::PPI - 
															   ScrabbleLetter::SCRABBLE_TILE_HEIGHT/2),
												 ScrabbleSize(ScrabbleLetter::SCRABBLE_TILE_WIDTH,
															  ScrabbleLetter::SCRABBLE_TILE_HEIGHT));
			//free memory
			delete ptTouch;
		}
	}
}

/* Responds if the user committed his move */
void ScrabbleUser::commitedMove() {
	//User wants to commit move
	if (ScrabbleGraphics::sharedObject(NULL)->buttonWasPressed(commitRef)) {
		//Valid move
		if(ScrabbleChecker::sharedObject()->checkMove(onBoard, ScrabbleChecker::PLAYER)) {
			//Remove used letters from inventory
			for (int i = 0; i < inventory.size(); i++) {
				//For each removed letter will be spawned a new one
				if (onBoard.contains(inventory[i])) 
					inventory.removeAt(i--);
			}
			
			addLettersToInventory();
	
			//Made a valid move, remove blocks from on-board
			onBoard.clear();
			
			//Compute points earned
			score += ScrabbleChecker::sharedObject()->getPointsEarnedLastTurn();
			
			//update text of score label
			ScrabbleGraphics::sharedObject(NULL)->changeTextOfLabel(scoreRef, "Score:\n" + IntegerToString(score));

			//User ended turn
			userEndedTurn = true;
			
			//Broke consecutive passes
			numConsecutivePasses = 0;
		}
	}
	//Reset points earned
	ScrabbleChecker::sharedObject()->getPointsEarnedLastTurn();
}

/* Responds if the user passed */
void ScrabbleUser::passedMove() {
	//User wants to pass.
	if (ScrabbleGraphics::sharedObject(NULL)->buttonWasPressed(passRef)) {
		//First make sure...
		if (ScrabbleGraphics::sharedObject(NULL)->sureAboutAction("Do you really want to pass?")) {
			//Then user ended turn
			userEndedTurn = true;
			
			//passed
			numConsecutivePasses++;
		}
	}
}

/* Responds if the user wants to flush his tiles */
void ScrabbleUser::flushedMove() {
	//User wants to exchange all tiles
	if (ScrabbleGraphics::sharedObject(NULL)->buttonWasPressed(flushRef)) {
		//First make sure	
		if (ScrabbleGraphics::sharedObject(NULL)->sureAboutAction("Do you really want to flush all tiles?")) {
		
			//Then flush all his tiles and create new ones	
			while (!inventory.isEmpty()) {	
				//Add character back into letters
				ScrabbleRandom::sharedObject()->addCharacter(inventory[0]->randomCharacter);
				
				//Destroy each one of the tiles
				ScrabbleGraphics::sharedObject(NULL)->destroyImageWithRef(inventory[0]->getRef());

				//Release the block from memory
				delete inventory[0];
			
				///Remove from inventory
				inventory.removeAt(0);
			}
			
			//Flush tiles within the on-board set as well
			onBoard.clear();
		
			//Then repopulate his pieces
			//Create 7 letters and add to the screen and vector
			addLettersToInventory();
			
			userEndedTurn = true;
		}
	}
}

/* This method updates the grid positions of the letters in 
 * the inventory array
 *
 * @return void 
 */
void ScrabbleUser::updateInventoryGridPositions(Grid<ScrabbleTile *>tiles) {	
	//Loop through all letters updating grid indexes
	for (int i = 0; i < inventory.size(); i++) {
		ScrabbleLetter *letter = inventory[i];
		bool letterOnGrid = false;
		
		//Indices
		double largestArea = 0;
		int x, y;
		
		//Loop through all tiles
		for (int row = 0; row < tiles.numCols(); row++) {
			for (int col = 0; col < tiles.numRows(); col++) {
				double currentArea;
				//Check intersection between tile and letter
				if ((currentArea = ScrabbleGraphics::sharedObject(NULL)->imageIntersectsImage(letter->getRef(),
																			   tiles[row][col]->getRef())) >
					largestArea) {
					
					
					//Letter is on grid because there was an intersection
					letterOnGrid = true;
					x = row;
					y = col;
				}
			}
		}
		
		//If the letter was on the grid
		if (letterOnGrid) {
			
			//Found where the letter is in the grid
			letter->gridIndexX = x;
			letter->gridIndexY = y;
								
			//Add to the set of letters that are on the board.
			if (!onBoard.contains(letter) && !letter->isCommitted()) {
				onBoard.add(letter);
			}
			
			//Letter is now on a tile
			letter->tileOn = tiles[x][y];
			
			//Change letter text color to red
			ScrabbleGraphics::sharedObject(NULL)->changeTextColorOfImage(letter->getRef(), "cyan");
		}
		else {
			//Change ltter text color back to black
			ScrabbleGraphics::sharedObject(NULL)->changeTextColorOfImage(letter->getRef(), "black");
			
			//Not on grid
			letter->gridIndexX = -1;
			letter->gridIndexY = -1;
			
			//Remove from the set of letters that are on the board
			if (onBoard.contains(letter)) {
				onBoard.remove(letter);
			}
			
			//clean tile
			letter->cleanFromBoard();
		}
	}
}

/* This method creates the user's buttons */
void ScrabbleUser::createUserButtons() {

	//Create the commit button
	ScrabblePoint *commitbuttonloc = new ScrabblePoint(ScrabbleGraphics::sharedObject(NULL)->getWindowWidth() - 
												 ScrabbleLetter::SCRABBLE_TILE_WIDTH * 1.5 - .2, 
												 .2);
	ScrabbleSize *commitbuttonsz = new ScrabbleSize(ScrabbleLetter::SCRABBLE_TILE_WIDTH * 1.5,
											  ScrabbleLetter::SCRABBLE_TILE_HEIGHT/1.5);
	
	commitRef = ScrabbleGraphics::sharedObject(NULL)->drawButtonBlock(*commitbuttonloc, 
																	  *commitbuttonsz, 
																	  "Commit");
	
	ScrabblePoint *passbuttonloc = new ScrabblePoint(ScrabbleGraphics::sharedObject(NULL)->getWindowWidth() - 
													 ScrabbleLetter::SCRABBLE_TILE_WIDTH * 1.5 - .2, 
													 .2 + commitbuttonsz->getHeight());
	
	ScrabbleSize *passbuttonsz = new ScrabbleSize(ScrabbleLetter::SCRABBLE_TILE_WIDTH * 1.5,
													ScrabbleLetter::SCRABBLE_TILE_HEIGHT/1.5);
	//Create the pass button
	passRef = ScrabbleGraphics::sharedObject(NULL)->drawButtonBlock(*passbuttonloc, 
																	*passbuttonsz, 
																	"Pass");
	
	//Create the flush button
	ScrabblePoint *flushbuttonloc = new ScrabblePoint(ScrabbleGraphics::sharedObject(NULL)->getWindowWidth() - 
													 ScrabbleLetter::SCRABBLE_TILE_WIDTH * 1.5 - .2, 
													 .2 + passbuttonsz->getHeight() + commitbuttonsz->getHeight());
	
	ScrabbleSize *flushbuttonsz = new ScrabbleSize(ScrabbleLetter::SCRABBLE_TILE_WIDTH * 1.5,
												  ScrabbleLetter::SCRABBLE_TILE_HEIGHT/1.5);
	
	flushRef = ScrabbleGraphics::sharedObject(NULL)->drawButtonBlock(*flushbuttonloc, 
																	*flushbuttonsz, 
																		"Flush");
	
	delete commitbuttonloc;
	delete commitbuttonsz;
	delete passbuttonloc;
	delete passbuttonsz;
	delete flushbuttonloc;
	delete flushbuttonsz;
}

/* Populate user inventory until 7 letters */
void ScrabbleUser::addLettersToInventory() {
	while (inventory.size() < NUM_LETTERS_INVENTORY) {
		//Create new letter
		ScrabbleLetter *newLetter = spawnScrabbleLetter(xPos, yStartPos);
		
		//No more letters left
		if (newLetter == NULL)
			break;
		
		ScrabbleGraphics::sharedObject(NULL)->changeTextColorOfImage(newLetter->getRef(),"cyan");
		
		//Otherwise add the letter
		inventory.add(newLetter);
		inventory[inventory.size()-1]->bringToFront();
	}
}

bool ScrabbleUser::lettersLeft() {
	return inventory.size() != 0;
}

int ScrabbleUser::getScore() {
	return score;
}

/* Forces the computer to have a specific hand */
void ScrabbleUser::forceInventory(string newLetters) {
	
	Vector<ScrabbleLetter *> newSet;
	
	for (int i = 0; i < newLetters.length(); i++) {		
		//Create new letters
		ScrabbleLetter *add = new ScrabbleLetter(-ScrabbleLetter::SCRABBLE_TILE_WIDTH, 
												 -ScrabbleLetter::SCRABBLE_TILE_HEIGHT,
												 newLetters[i]);
		
		
		//Add the new letters to inventory
		newSet.add(add);
	}
	
	//remove old letters
	foreach (ScrabbleLetter *letter in inventory) {
		//Clean graphics
		ScrabbleGraphics::sharedObject(NULL)->destroyImageWithRef(letter->getRef());
		delete letter;
		letter = NULL;
	}
	inventory.clear();
	inventory = newSet;
	
	int x = xPos;
	int y = yStartPos;
	double interval = ScrabbleLetter::SCRABBLE_TILE_HEIGHT*1.5;
	
	//Place new letters in users' sighs
	foreach (ScrabbleLetter *newLetters in inventory) {
		ScrabbleGraphics::sharedObject(NULL)->translateObject(ScrabblePoint(x, y += interval), 
															  ScrabbleSize(ScrabbleLetter::SCRABBLE_TILE_WIDTH,
																		   ScrabbleLetter::SCRABBLE_TILE_HEIGHT),
															  newLetters->getRef());
		newLetters->bringToFront();
	}
}

/* Computer automatically wins */
void ScrabbleUser::instawin() {
	//increase score to a million
	score += 1000000;
	
	//update text of score label
	ScrabbleGraphics::sharedObject(NULL)->changeTextOfLabel(scoreRef, "Score:\n" + IntegerToString(score));
	
	//Clear inventory
	//Release memory
	foreach (ScrabbleLetter *letter in inventory) {
		ScrabbleGraphics::sharedObject(NULL)->destroyImageWithRef(letter->getRef());
		delete letter;
	}	
	inventory.clear();
}
