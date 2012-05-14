//
//  GraphicsContent.h
//  Scrabble
//
//  Created by Max Lam on 7/23/11.
//  Copyright 2011 Max Lam. All rights reserved.

//  This is a class to specifically handle mouse events.
//  The graphics window will be used as the content view of the
//  Cocoa window. 
//
//	A delegate will be set so that it notifies the GraphicsCore
//	Whenever there is a mouseDown or drag.


#import <Cocoa/Cocoa.h>


@interface GraphicsContent : NSView {
	id delegate;		//How it communicates with its implementer
}

@property (nonatomic, retain) id delegate;

@end
