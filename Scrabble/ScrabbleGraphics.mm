/*
 *  ScrabbleGraphics.cpp
 *  Scrabble
 *
 *  Created by Max Lam on 7/21/11.
 *  Copyright 2011 Max Lam. All rights reserved.
 *
 */

#import "sound.h"
#include "GraphicsCore.h"
#include "ScrabbleGraphics.h"

//The cocoa window
GraphicsCore *window;

/* The constructor of the scrabble graphisc class should not be called directly.
 * instead, a wrapper method should call this behind the scenes to create a window instance
 *
 * What this method does is it instantiates the class and creates two windows -- one created
 * from the cs106 libraries and one created with objective c.
 * 
 * @param size specifies dimensions of the window
 *
 * @return instance
 */
ScrabbleGraphics::ScrabbleGraphics(ScrabbleSize size) {
	//Initialize window width and height
	windowWidth = size.getWidth();
	windowHeight = size.getHeight();
		
	//Create a cocoa window 
	window = [[GraphicsCore alloc]initWithFrame:NSMakeRect(windowWidth/2 * PPI, windowHeight/2 * PPI, 
														   windowWidth * PPI, windowHeight * PPI) andTitle:@"Scrabble"];
}

ScrabbleGraphics::~ScrabbleGraphics() {
	//Release window
	[window release];
}	

/* This method is a static method that returns a shared instance of the ScrabbleGraphics
 * object.
 *
 * Note that on its first call, a new graphics object is created and continually referenced to.
 *
 * On its first call, this method needs to be supplied a valid pointer
 * to a scrabbleSize object (for the objects' instantiation).
 *
 * After the first call, others may call again, but should pass NULL for
 * a pointer to sz.
 *
 * @return a shared instance of scrabbleGraphics
 */ 
ScrabbleGraphics * ScrabbleGraphics::sharedObject(ScrabbleSize *sz) {
	static ScrabbleGraphics *graphicsObject = NULL;
	
	if (graphicsObject == NULL) {		
		//Instantiate it
		graphicsObject = new ScrabbleGraphics(*sz);
	}
		
	//Delete sz if it is not null
	if (sz != NULL)
		delete sz;

	
	return graphicsObject;
}

/* This function creates a block that may or may not have text within it.
 *
 * @param location specifies where to draw the block
 * @param dimension is the size of the block
 * @param text is the string that will be pasted onto the block
 * @param textColor is what color the text-string is
 * @param backgroundColor is the color of the block
 *
 * @return a reference to the newly created block
 */
int ScrabbleGraphics::drawTextBlock(ScrabblePoint &location, ScrabbleSize &dimension, 
									string text, string textColor, string backgroundColor) {
	NSString *bgColor = [[NSString alloc]initWithUTF8String:backgroundColor.c_str()];
	NSString *txt = [[NSString alloc]initWithUTF8String:text.c_str()];
	NSString *txtColor = [[NSString alloc]initWithUTF8String:textColor.c_str()];
	
	//Draw the colored block and return an integer referencing it
	int ref = [window drawColoredBlockWithWidth:dimension.getWidth() * PPI
								   andHeight:dimension.getHeight() * PPI
								andLocaitonX:location.getX() * PPI
								   locationY:location.getY() * PPI
						  andBackgroundColor:bgColor
							   andOptionText:txt
								andTextColor:txtColor];
	[bgColor release];
	[txtColor release];
	[txt release];
	
	return ref;
}

/* This function is practically the same as above, except it uses an image instead of a block 
 *
 * @param location is where to set the image
 * @param imgName is the file containing the image
 * @param text is an option text string to center in the image
 * @param textColor is the text's color
 *
 * @return a reference to the newly created block
 */
int ScrabbleGraphics::drawImageBlock(ScrabblePoint &location, ScrabbleSize &dimension,
									 string imgName, string text, string textColor) {		
	NSString * picName = [[NSString alloc]initWithUTF8String:imgName.c_str()];
	NSString * txt = [[NSString alloc]initWithUTF8String:text.c_str()];
	NSString * txtCol = [[NSString alloc]initWithUTF8String:textColor.c_str()];
	
	//Draw the block in the cocoa window and return an integer that references this object
	int ref = [window drawImageBlockWithWidth:dimension.getWidth()* PPI 
								 andHeight:dimension.getHeight() * PPI 
							  andLocationX:location.getX() * PPI
								 locationY:location.getY() * PPI
							  andImageName:picName
							 andOptionText:txt
							  andTextColor:txtCol];
	[picName release];
	[txt release];
	[txtCol release];
	return ref;
}

/* This function draws a clickable button
 *
 * @param location is the location of the button
 * @param dimension is the size of the button
 * @param text is what the button says
 *
 * @return a reference to the button
 */
int ScrabbleGraphics::drawButtonBlock(ScrabblePoint &location, ScrabbleSize &dimension,
					string text) {
	
	//Convert from inches to pixels
	double width = dimension.getWidth() * PPI;
	double height = dimension.getHeight() * PPI;
	double x = location.getX() * PPI;
	double y = location.getY() * PPI;
	
	NSString *txt = [[NSString alloc]initWithUTF8String:text.c_str()];
	
	//Draw it
	int ref = [window drawButtonWithWidth:width andHeight:height andLocationX:x locationY:y
						andText:txt];
	
	[txt release];
	
	return ref;
}

/* Get window width */
double ScrabbleGraphics::getWindowWidth() {
	return windowWidth;
}

/* Get window height */
double ScrabbleGraphics::getWindowHeight() {
	return windowHeight;
}

/* Brings the image specified by ref to the front of the
 * window.
 */
void ScrabbleGraphics::bringImageToFront(int ref) {
	[window bringToFrontObjectWithIndex:ref];
}

/* This method translates a given object, both in location and
 * in size, referenced by ref.
 *
 * @param position is where to move the object
 * @param size is the new size of the object
 * @param ref references the object
 *
 * @return void
 */
void ScrabbleGraphics::translateObject(ScrabblePoint position, ScrabbleSize size, int ref) {
	//Create NSRect and pass that to the graphics core
	NSRect newRect = NSMakeRect(position.getX() * PPI, 
								position.getY() * PPI, 
								size.getWidth() * PPI , 
								size.getHeight() * PPI);
	[window animate:newRect andReference:ref];
}

/* This method takes an image referenced by ref and translates it 
 * to ref2
 */
void ScrabbleGraphics::translateObjectToObject(int ref, int ref2) {
	NSPoint destPt = [window positionOfImageWithReference:ref2];
	NSSize curSize = [window sizeOfImageWithReference:ref];
	NSRect newRect = NSMakeRect(destPt.x, destPt.y, curSize.width, curSize.height);
	
	[window animate:newRect andReference:ref];
}

/* This method returns a point where the user clicked or dragged a specific
 * image referenced by ref
 *
 * @param ref is the object reference
 *
 * @return a scrabblePoint if there was a click/drag -- ortherwise null
 */
ScrabblePoint * ScrabbleGraphics::clickedOrDragged(int ref) {
	//Check with cocoa core graphics
	NSPoint mLoc = [window mouseClickedOrDragOnImage:ref];
	
	if (mLoc.y >= 0 && mLoc.x >= 0) {		
		//Return info
		return new ScrabblePoint(mLoc.x, mLoc.y);
	}
	return NULL;
}

/* This method returns the image position of the image with ref */
ScrabblePoint ScrabbleGraphics::imagePositionWithRef(int ref) {
	NSPoint pt = [window positionOfImageWithReference:ref];
	return ScrabblePoint(pt.x, pt.y);
}

/* This method returns the size of the image of the image referenced
 * by ref
 */
ScrabbleSize ScrabbleGraphics::imageSizeWithRef(int ref) {
	NSSize sz = [window sizeOfImageWithReference:ref];
	return ScrabbleSize(sz.width, sz.height);
}

/* This method determines if image ref1 intersected
 * with image ref2
 *
 * @return intersection size
 */
double ScrabbleGraphics::imageIntersectsImage(int ref1, int ref2) {
	NSRect intersection = [window rectIntersectsRect:ref1 andRef2:ref2];
	return intersection.size.width * intersection.size.height;
}

/* Returns true if a button with reference ref was pressed */
bool ScrabbleGraphics::buttonWasPressed(int ref) {
	return [window mouseClickedOnButton:ref];
}

/* Change an image's text color */
void ScrabbleGraphics::changeTextColorOfImage(int ref, string color) {
	NSString *newString = [[NSString alloc] initWithUTF8String:color.c_str()];
	[window changeImageTextColor:ref andColor:newString];
	[newString release];
}

/* Checks if an image intersects another image */
bool ScrabbleGraphics::imageIntersectsAnother(int ref1) {
	return [window rectIntersectsAnother:ref1];
}

/* Creates a popup window that notifies the user of something */
void ScrabbleGraphics::notifyUser(string text) {
	int count = 0;
	const int amountTillNewline = 10;
	for (int i = 0; i < text.length(); i++) {
		count++;
		if (count >= amountTillNewline && text[i] == ' ') {
			text.insert(i++, 1, '\n');
			count = 0;
		}
	}
	NSString *message = [[NSString alloc] initWithUTF8String:text.c_str()];
	[window createPopupWindowNotification:message];
	[message release];
}

/* Destroys image referenced by parameter ref */
void ScrabbleGraphics::destroyImageWithRef(int ref) {
	[window destroyImageWithRef:ref];
}

/* Prompts user to see if he sure about action */
bool ScrabbleGraphics::sureAboutAction(string action) {
	NSString *actionString = [[NSString alloc]initWithUTF8String:action.c_str()];
	bool returnValue = [window alertUserYesOrNo:actionString];
	[actionString release];
	return returnValue;
}

/* Sets the image's subtext */
void ScrabbleGraphics::setSubtext(int ref, string text) {
	NSString *sbtext = [[NSString alloc]initWithUTF8String:text.c_str()];

	[window setSubTextForImageWithRef:ref subText:sbtext];
	[sbtext release];
}

/* Creates a label at the specified point and msg */
int ScrabbleGraphics::createLabelAt(ScrabblePoint point, string msg) {
	NSString *label = [[NSString alloc]initWithUTF8String:msg.c_str()];
	return [window createBlockLabel:NSMakeRect(point.getX(), point.getY(), 0, 0) 
						   andLabel:label];
	[label release];
}

/* Changes the text of a label referenced by integer ref with string msg */
void ScrabbleGraphics::changeTextOfLabel(int ref, string msg) {
	NSString *msgStr = [[NSString alloc]initWithUTF8String:msg.c_str()];
	[window changeTextOfLabel:ref withString:msgStr];
	[msgStr release];
}

/* This method loads a screen */
void ScrabbleGraphics::loadScreen(string fileName) {
	NSString *screenName = [[NSString alloc]initWithUTF8String:fileName.c_str()];
	[NSBundle loadNibNamed:screenName owner:window];
	[screenName release];
}

/* Set alpha value of image */
void ScrabbleGraphics::setAlphaOfImage(int ref, float value) {
	[window setAlphaValueOfImage:ref ofAlpha:value];
}

/* Show menu */
void ScrabbleGraphics::createMenu() {
	//Buttons
	NSMutableArray *buttons = [[NSMutableArray alloc]init];
	
	//Button attributes
	NSMutableDictionary *atributes = [[NSMutableDictionary alloc]init];
	[atributes setObject:[NSColor whiteColor] forKey:NSForegroundColorAttributeName];
	[atributes setObject:[NSFont fontWithName:@"Verdana" size:30] forKey:NSFontAttributeName];
	
	NSButton *playButton = [[NSButton alloc] initWithFrame:NSMakeRect([window size].size.width / 2 - BUTTON_WIDTH / 2, 
																	  [window size].size.width / 4 - BUTTON_HEIGHT / 2, 
																	  BUTTON_WIDTH, BUTTON_HEIGHT)];
	[playButton setBordered:NSThickSquareBezelStyle];
	[playButton setImage:[NSImage imageNamed:@"Button.png"]];
	
	//Title of the button (Spaces because no attribute for alignment...)
	NSMutableAttributedString *title = [[NSMutableAttributedString alloc]initWithString:@"    Play" 
																			 attributes:atributes];
	
	[playButton setAlignment:NSRightTextAlignment];
	[playButton setAttributedTitle:title];
	
	[buttons addObject:playButton];
	
	[window displayMenu:@"ScrabbleLogo.png" andButtons:buttons];
	
	//Release memory
	[buttons release];
	[atributes release];
	[title release];
}

/* Allows the user to start playing */
void ScrabbleGraphics::showGame() {
	//Show game board/buttons
	[window displayGameArena];
}

void ScrabbleGraphics::setButtonMode(int mode) {
	window.buttonPress = mode;
}

int ScrabbleGraphics::whichButtonPressed() {
	return window.buttonPress;
}

void ScrabbleGraphics::showMenu() {
	[window displayMenu:nil andButtons:nil];
}

void ScrabbleGraphics::clearEverything() {
	[window clearEverything];
}