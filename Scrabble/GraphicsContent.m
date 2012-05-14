//
//  GraphicsContent.m
//  Scrabble
//
//  Created by Max Lam on 7/23/11.
//  Copyright 2011 Max Lam. All rights reserved.
//

#import "GraphicsContent.h"


@implementation GraphicsContent

/* Normal init */
- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
 
    return self;
}

/* Delegate methods that handle mouse events */
-(void)mouseDown:(NSEvent *)event {
	//Send message to delegate
	if ([delegate respondsToSelector:@selector(mouseDown:)]) {
		[delegate mouseDown:event];
	}
}

/* Called whenever user drags the mouse while pressing */
-(void)mouseDragged:(NSEvent *)event {
	//Send message to delegate
	if ([delegate respondsToSelector:@selector(mouseDragged:)]) {
		[delegate mouseDragged:event];
	}
}

/* Called whenever user releases left button */
-(void)mouseUp:(NSEvent *)event {
	//Send message to delegate
	if ([delegate respondsToSelector:@selector(mouseUp:)]) {
		[delegate mouseUp:event];
	}
}

-(void)dealloc {
	[super dealloc];
}

@synthesize delegate;

@end
