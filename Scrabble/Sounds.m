//
//  Sounds.m
//  Scrabble
//
//  Created by Max Lam on 7/30/11.
//  Copyright 2011 Max Lam. All rights reserved.
//

#import "Sounds.h"


@implementation Sounds

-(id)init {
	[super init];
	track = 0;
	soundObjects = [[NSMutableArray alloc]init];
	
	defaultClickSound = [self createSoundWithPath:@"ScrabbleResources/click.wav"];
	return self;
}

/* Creates and stores a sound object with pathname soundPath into the array.
 *
 * @returns a reference to the sound object for easy playback
 */
-(int)createSoundWithPath:(NSString *)soundPath {
	NSSound *newSound = [[NSSound alloc] initWithContentsOfFile:soundPath byReference:YES];
	
	[soundObjects addObject:newSound];
	
	//Release mem
	[newSound release];
	
	return track++;
}

-(void)playSoundWithRef:(int)ref {
	NSSound *playSound = [soundObjects objectAtIndex:ref];
	[playSound play];
}

-(void)playDefaultClick {
	[self playSoundWithRef:defaultClickSound];
}

-(void)dealloc {
	[soundObjects release];
	[super dealloc];
}

@end
