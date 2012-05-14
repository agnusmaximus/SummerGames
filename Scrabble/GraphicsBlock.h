//
//  GraphicsBlock.h
//  Scrabble
//
//  Created by Max Lam on 7/22/11.
//  Copyright 2011 Max Lam. All rights reserved.
//
//
//  The graphics block is a child of the NSView class,
//  and it represents a block. The graphics block may have
//  and image set, or it may just be colored.
//  An option string label may be set as well.
//
//

#define PTSZ 23		//font size

#import <AppKit/AppKit.h>
#import <Cocoa/Cocoa.h>
#include <Quartz/Quartz.h>

@interface GraphicsBlock : NSView {
	//The graphics block will contain
	//an imageView, and a label.
	NSImageView *imageView;
	NSTextField *blockLabel;		//This will not be editable
	BOOL imagePresent;
}

//Instantiators
- (id)initWithFrame:(NSRect)frame andLabel:(NSString *)label withColor:(NSString *)color andImage:(NSString *)imgFile;
- (id)initWithFrame:(NSRect)frame andLabel:(NSString *)label withColor:(NSString *)color andBackgroundColor:(NSString *)bgColor;

//Create label
- (void)createBlockLabel:(NSRect)blockFrame andLabel:(NSString *)label andColor:(NSString *)color;

//Return a color
- (NSColor *)colorFromString:(NSString *)color;

//Set label color
-(void)setColorForLabel:(NSString *)color;

//Set sub text for block
-(void)setSubTextForBlock:(NSString *)text;

@property (nonatomic, retain) NSTextField * blockLabel;

@end
