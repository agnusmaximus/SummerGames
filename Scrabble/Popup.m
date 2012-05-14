//
//  Popup.m
//  Scrabble
//
//  Created by Max Lam on 8/3/11.
//  Copyright 2011 Max Lam. All rights reserved.
//

#import "Popup.h"


@implementation Popup

-(id)initWithContentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag {
	self = [super initWithContentRect:contentRect styleMask:aStyle backing:bufferingType defer:flag];
	return self;
}

-(void)setDelegate:(id)del {
	delegate = del;
}

-(void)mouseDown:(NSEvent *)theEvent {
	//Send message to deleate
	if ([delegate respondsToSelector:@selector(mouseDown:)])
		[delegate mouseDown:theEvent];
}

-(void)mouseUp:(NSEvent *)theEvent {
	if ([delegate respondsToSelector:@selector(mouseUp:)])
		[delegate mouseUp:theEvent];
}

@end
