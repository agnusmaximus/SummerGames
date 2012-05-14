//
//  SpeechSynthesizer.m
//  Scrabble
//
//  Created by Max Lam on 7/30/11.
//  Copyright 2011 Max Lam. All rights reserved.
//

#import "SpeechSynthesizer.h"


@implementation SpeechSynthesizer

-(id)init {
	return self;
}

-(void)speakString:(NSString *)line {
	speechSynth = [[NSSpeechSynthesizer alloc]init];
	[speechSynth setRate:260];
	[speechSynth setDelegate:self];
	[speechSynth startSpeakingString:line];
}

-(void)speechSynthesizer:(NSSpeechSynthesizer *)sender didFinishSpeaking:(BOOL)finished {
	[sender release];
	sender = nil;
}

-(void)dealloc {
	[super dealloc];
}

@end
