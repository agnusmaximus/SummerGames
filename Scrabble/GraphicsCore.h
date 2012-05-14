//
//  GraphicsCore.h
//  Scrabble
//
//  Created by Max Lam on 7/22/11.
//  Copyright 2011 Max Lam. All rights reserved.
//
//
//	This objective C class handles the low level creation of graphics.
//	Because this class can only be referenced by .mm files,
//  a specific mediator file will be used to communicate with
//  the graphics portion of the Scrabble game.
//
//  The way the views word is that there is a main view and a secondary view.
//  The secondary view usually contains backgrounds and whatnot.
//  The main view contains the more important things, such as scrabble letters.

#define IMG_MOVE_DURATION 1
#define POPUP_FONT_SZ 25
#define POPUP_BRK_LNGTH 40

#import <Cocoa/Cocoa.h>
#include <QuartzCore/QuartzCore.h>
#include "ScrabbleGameStates.h"
@class GraphicsBlock;
@class GraphicsContent;
@class SpeechSynthesizer;
@class Sounds;
@class Popup;

@interface GraphicsCore : NSObject {
	//Window
	NSWindow *window;
	
	//Window's content view
	GraphicsContent *contentView;
	
	//Primary and secondary views
	NSView *secondaryView, *mainView;
	
	//Menu view
	NSView *menuView;
	
	//Vector containing all graphical objects
	NSMutableArray *graphicalObjects;
	
	//Alert views
	NSAlert *newAlert;
	
	//Current index of the array
	int index;
	
	//Mouse point position
	NSPoint mousePosition;
	
	//Is the mouse down?
	BOOL mousePressed;
	
	//Is what image was pressed
	int refImagePressed;
	
	//Popup window
	Popup *popup;
	
	//Speech synthesizer
	SpeechSynthesizer *talker;
	
	//Sounds
	Sounds *sound;
	
	//which button pressed
	int buttonPress;
}

//Instantiation
- (id)initWithFrame:(NSRect)frame andTitle:(NSString *)title;

//Drawing
-(int)drawImageBlockWithWidth:(double)pWidth andHeight: (double)pHeight andLocationX: (double)pX locationY: (double)pY
					  andImageName: (NSString *)imageName andOptionText: (NSString *)text andTextColor: (NSString *)textColor;

-(int)drawColoredBlockWithWidth:(double)pWidth andHeight: (double)pHeight andLocaitonX: (double)pX locationY: (double)pY
			 andBackgroundColor: (NSString *)bgColor andOptionText: (NSString *)text andTextColor: (NSString *)textColor;

-(int)drawButtonWithWidth:(double)pWidth andHeight: (double)pHeight andLocationX: (double)px locationY:(double)py
				  andText: (NSString *)text;

//View management
-(void)bringToFrontObjectWithIndex:(int)ind;
-(void)snapToGrid:(int)ref;
-(void)displayGameArena;
-(void)displayMenu:(NSString *)logo andButtons:(NSMutableArray *)buttons;

//Animations
-(void)animate:(NSRect)newFrame andReference:(int)ref;
-(void)transitionContentViewsFade:(NSView *)newView;
-(void)transitionContentViewsAppear;

//Mouse events
-(NSPoint)mouseClickedOrDragOnImage:(int)ref;
-(void)updateMousePosition: (NSEvent *)theEvent;
-(BOOL)mouseClickedOnButton:(int)ref;

//Rect comparison
-(BOOL)rectLargerThanRect:(NSRect)rect andRect2:(NSRect)rect2;
-(NSRect)rectIntersectsRect:(int)ref1 andRef2:(int)ref2;
-(BOOL)rectIntersectsAnother:(int)ref;

//Image management
-(NSSize)sizeOfImageWithReference:(int)ref;
-(NSPoint)positionOfImageWithReference:(int)ref;
-(void)changeImageTextColor:(int)ref andColor:(NSString *)color;
-(void)destroyImageWithRef:(int)ref;
-(void)setSubTextForImageWithRef:(int)ref subText:(NSString *)text;
-(int)createBlockLabel:(NSRect)blockFrame andLabel:(NSString *)label;
-(void)setAlphaValueOfImage:(int)ref ofAlpha:(float)alpha;

//Window notifications
-(void)fadeInWindow:(NSWindow*)win;
-(void)fadeOutWindow:(NSWindow*)win;
-(void)createPopupWindowNotification:(NSString *)text;
-(BOOL)alertUserYesOrNo:(NSString *)action;

//Label management
-(void)changeTextOfLabel:(int)ref withString:(NSString *)str;
-(void)hyperLinkStringBetweenQuotes:(NSTextField *)textField;
-(void)addDefinition:(NSTextField *)textField;
-(NSString *)getUrl:(NSTextField *)textField;

//Setters & getters
-(NSRect)size;

//Button events
-(void)buttonPressed:(id)sender;

-(void)clearEverything;

@property (assign) int buttonPress;
@end
