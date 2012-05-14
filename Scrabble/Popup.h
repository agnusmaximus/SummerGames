//
//  Popup.h
//  Scrabble
//
//  Created by Max Lam on 8/3/11.
//  Copyright 2011 Max Lam. All rights reserved.
//
//
//  Implements a popup screen to which notifies the user

#import <Cocoa/Cocoa.h>


@interface Popup : NSWindow {
	id delegate;
}

-(void)setDelegate:(id)del;

@end
