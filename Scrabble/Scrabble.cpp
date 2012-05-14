/* File: Scrabble.cpp
 *
 * This file contains the main connective tissue of the Scrabble program.
 * It'll contain the main loop, and coordinate the interactions between
 * the different classes
 *
 */

#include <iostream>
#include <pthread.h>
#include "scanner.h"
#include "ScrabbleGameStates.h"
#include "ScrabbleGraphics.h"
#include "ScrabbleBoard.h"
#include "ScrabbleLetter.h"
#include "ScrabbleUser.h"
#include "ScrabbleOpponent.h"

/* Cheat package contains computer item and user item */
struct CheatPackage {
	ScrabbleOpponent *computer;
	ScrabbleUser *user;
};

/* Constants that reference game play */
const int nPassesUntilGOver = 2;

/* Constants that reference files */

/* For bonus positions:
 * 
 * static const int NORMAL_TILE = 0;
 * static const int DOUBLE_LETTER_TILE = 1;
 * static const int DOUBLE_WORD_TILE = 2;
 * static const int TRIPLE_LETTER_TILE = 3;
 * static const int TRIPLE_WORD_TILE = 4;
 *
 */
const string fBonusPositions = "ScrabbleResources/ScrabbleBonusPositions.txt";	

/* For position colors:
 *
 * static const int BLACK = 0;
 * static const int WHITE = 1;
 * static const int GREEN = 2;
 * static const int BLUE = 3;
 * static const int YELLOW = 4;
 * static const int RED = 5;
 * static const int BROWN = 6;
 * static const int GREY = 7;
 * static const int ORANGE = 8;
 *
 */
const string fPositionColors = "ScrabbleResources/ScrabbleColorPositions.txt";

void initScrabbleGame(ScrabbleBoard *&board, ScrabbleUser *&user, ScrabbleOpponent *&computer);
void playGame();

int main() {
	//Create the graphics shared object (also instantiate the scrabble game's graphics)
	ScrabbleGraphics::sharedObject(new ScrabbleSize(ScrabbleBoard::NUM_ROWS_COLS * ScrabbleTile::SCRABBLE_TILE_WIDTH + ScrabbleUser::SIDE_SHELF_WIDTH * 2,
													ScrabbleBoard::NUM_ROWS_COLS * ScrabbleTile::SCRABBLE_TILE_HEIGHT));
	
	
	ScrabbleGraphics::sharedObject(NULL)->createMenu();
	
	//Main game loop
	while (true) {
		if (ScrabbleGraphics::sharedObject(NULL)->whichButtonPressed() == BEGIN_MENU_MODE) {
			ScrabbleGraphics::sharedObject(NULL)->setButtonMode(MENU_MODE);
			
			//Display menu
			ScrabbleGraphics::sharedObject(NULL)->showMenu();
		}
		
		if (ScrabbleGraphics::sharedObject(NULL)->whichButtonPressed() == BEGIN_PLAY_MODE) {
			ScrabbleGraphics::sharedObject(NULL)->setButtonMode(PLAY_MODE);
			
			//Play game
			playGame();
			
			//Back to menu
			ScrabbleGraphics::sharedObject(NULL)->setButtonMode(BEGIN_MENU_MODE);
		}
	}
	
	return 0;
}

/* This function is one of the major function that is called when the user starts a game
 * against the computer.
 * It comprises of the main game loop that loops from the user's turn to the 
 * computer's turn.
 * 
 * The game ends when there are no more scrabble tiles left, or if each player (computer 
 * and user) passes twice.
 *
 * @return void
 */
void playGame() {
	//Show the game
	ScrabbleGraphics::sharedObject(NULL)->showGame();
	
	//Essential scrabble entities
	ScrabbleBoard *board;
	ScrabbleUser *user;
	ScrabbleOpponent *computer;
	
	//Initialize everything required for the Scrabble game
	initScrabbleGame(board, user, computer);
	
	//Cheat package
	CheatPackage myCheatPackage;
	myCheatPackage.computer = computer;
	myCheatPackage.user = user;
	
	while (true) {
		
		//Allow user to move the pieces
		user->userMove();
		
		//Once the user moved the piece, update the piece position
		//on the grid
		user->updateInventoryGridPositions(board->board);
		
		//Check if the user commits his letterings or if he passed or if he
		//exchanged his letters
		user->commitedMove();
		user->passedMove();
		user->flushedMove();
		
		//If user ended turn, computers' turn
		if (user->userEndedTurn) {	
			
			//Computer move
			computer->computerMove();
			
			//User's turn again
			user->userEndedTurn = false;
		}
		
		//Check for end game here!
		//Checks if there were at least two passes made by both computer or user
		//If either the computer or the user has an empty inventory (and thus no more letters)
		if (user->numConsecutivePasses >= nPassesUntilGOver
			&& computer->numConsecutivePasses >= nPassesUntilGOver ||
			!user->lettersLeft() ||
			!computer->lettersLeft()) 
			break;
	}
	
	//Who won?
	if (computer->getScore() > user->getScore()) {
		//Computer won
		ScrabbleGraphics::sharedObject(NULL)->notifyUser("Game Over.\nComputer wins");
	}	
	else {
		ScrabbleGraphics::sharedObject(NULL)->notifyUser("Congratulations. You beat Computer");
	}
	
	//Clean up
	delete board;
	delete user;
	delete computer;
	
	ScrabbleGraphics::sharedObject(NULL)->clearEverything();
	ScrabbleRandom::sharedObject()->getTiles();
	ScrabbleChecker::sharedObject()->reset();
}

/* This function initializes the Scrabble game by
 * setting up the graphics display and instantiating
 * essential classes to the Scrabble program
 *
 * @return void
 */
void initScrabbleGame(ScrabbleBoard *&board, ScrabbleUser *&user, ScrabbleOpponent *&computer) {

	//Create the board and initialize with files that specify board layout
	board = new ScrabbleBoard(fBonusPositions, fPositionColors);
	
	//Create the user object
	user = new ScrabbleUser();

	//Create the computer
	computer = new ScrabbleOpponent();	
	
	//Set the computer's vision of the board
	computer->board = board->board;
}
