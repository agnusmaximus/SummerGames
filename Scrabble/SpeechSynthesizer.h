//
//  SpeechSynthesizer.h
//  Scrabble
//
//  Created by Max Lam on 7/30/11.
//  Copyright 2011 Max Lam. All rights reserved.
//
//	This class defines the computerized voice that will
//  announce score-updates, or valid words.
//
//

#import <Cocoa/Cocoa.h>


@interface SpeechSynthesizer : NSObject {
	NSSpeechSynthesizer *speechSynth;
}

-(void)speakString:(NSString *)line;

@end
