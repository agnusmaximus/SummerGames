//
//  Sounds.h
//  Scrabble
//
//  Created by Max Lam on 7/30/11.
//  Copyright 2011 Max Lam. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface Sounds : NSObject {
	NSMutableArray *soundObjects;
	int defaultClickSound;
	int track;
}

-(int)createSoundWithPath:(NSString *)soundPath;
-(void)playSoundWithRef:(int)ref;
-(void)playDefaultClick;

@end
