//
//  GraphicsCore.m
//  Scrabble
//
//  Created by Max Lam on 7/22/11.
//  Copyright 2011 Max Lam. All rights reserved.
//

#import "GraphicsCore.h"
#import "Popup.h"
#import "GraphicsBlock.h"
#import "GraphicsContent.h"
#import "SpeechSynthesizer.h"
#import "Sounds.h"

@implementation GraphicsCore

/* This method allows the caller to create a new window.
 *
 * @param frame contains the location and the size of the to-be-created
 *        window
 *
 * @return a reference to the newly created window
 */
- (id)initWithFrame:(NSRect)frame andTitle:(NSString *)title {
	self = [super init];
	
	//Create window
	window  = [[NSWindow alloc] initWithContentRect:frame
										  styleMask:NSTitledWindowMask
											backing:NSBackingStoreBuffered
											  defer:NO];
	
	//Set window attributes
	[window setBackgroundColor:[NSColor whiteColor]];
	[window makeKeyAndOrderFront:NSApp];
	[window setTitle:title];
		
	//Set background color
	[window setBackgroundColor:[NSColor colorWithCalibratedRed:.3 green:.4 blue:.5 alpha:1]];
	
	//Create the content view which all images will be subviews of
	contentView = [[GraphicsContent alloc]initWithFrame:frame];
		
	//Enable contentView's communication with self
	[contentView setDelegate:self];
	
	//From the content view, derive a main and secondary view
	mainView = [[NSView alloc]initWithFrame:NSMakeRect(0, 0, frame.size.width, frame.size.height)];
	secondaryView = [[NSView alloc]initWithFrame:NSMakeRect(0, 0, frame.size.width, frame.size.height)];
	
	//Initialize objects array
	graphicalObjects = [[NSMutableArray alloc]initWithCapacity:0];
		
	//Init index
	index = 0;
	
	//No mouse presses yet
	mousePressed = NO;
	
	//No image was clicked yet..
	refImagePressed = -1;
	
	//Create the popup window to notify users
	popup = [[Popup alloc] initWithContentRect: NSMakeRect(window.frame.origin.x + window.frame.size.width/4, 
															  window.frame.origin.y + window.frame.size.height/4,
															  window.frame.size.width / 2, window.frame.size.height/2)
													styleMask: NSBorderlessWindowMask 
													backing:NSBackingStoreBuffered
													defer:NO];
	
	//Set bg color
	[popup setBackgroundColor:[NSColor colorWithCalibratedRed:.3 green:.4 blue:.5  alpha:.8]];
	
	//Set popup delegate
	[popup setDelegate:self];
	
	//Window has shadow
	[popup setHasShadow:YES];
	[popup setOpaque:NO];
	
	//Window is transparent
	[popup setAlphaValue:0];
	
	//Create speech synthesizer
	talker = [[SpeechSynthesizer alloc]init];
		
	//Sounds
	sound = [[Sounds alloc]init];
		
	menuView = nil;
	newAlert = nil;
	
    return self;
}

/* This method draws an image block at the specific positions. Note that the positions and
 * the dimensions of the block will be specified in pixels 'p for pixels.
 *
 * @param pWidth and pHeight are the block image dimensions in pixels
 * @param pX and pY is the location of the block in pixels
 * @param text is the optional text to draw
 * @param textColor will be the textColor of the text (if there is any)
 *
 * @return an index that references this newly created image in the mutablearray
 */
-(int)drawImageBlockWithWidth:(double)pWidth andHeight: (double)pHeight andLocationX: (double)pX locationY: (double)pY
					  andImageName: (NSString *)imageName andOptionText: (NSString *)text andTextColor: (NSString *)textColor {
	
	//Create the new block
	GraphicsBlock *newBlock = [[GraphicsBlock alloc]initWithFrame:NSMakeRect(pX, pY, pWidth, pHeight)
														 andLabel:text 
														withColor:textColor 
														 andImage:imageName];
	
	
	[secondaryView addSubview:newBlock];
	
	[graphicalObjects addObject:newBlock];
	
	[newBlock release];
	
	//Return reference to newly created image
	return index++;
}

/* Same as the above method except creates a colored block. Not one with an image */
-(int)drawColoredBlockWithWidth:(double)pWidth andHeight: (double)pHeight andLocaitonX: (double)pX locationY: (double)pY
			 andBackgroundColor: (NSString *)bgColor andOptionText: (NSString *)text andTextColor: (NSString *)textColor {
	//Create the new block
	GraphicsBlock *newBlock = [[GraphicsBlock alloc]initWithFrame:NSMakeRect(pX, pY, pWidth, pHeight)
														 andLabel:text 
														withColor:textColor
											   andBackgroundColor:bgColor];
	[secondaryView addSubview:newBlock];
	
	[graphicalObjects addObject:newBlock];
	
	[newBlock release];
	
	return index++;
}

/* Brings an object within the objects array with index ind
 * to the front of the view.
 *
 * @param ind is the index of the object to bring to the front
 *
 * @return void
 */
-(void)bringToFrontObjectWithIndex:(int)ind {
	GraphicsBlock *obj = [graphicalObjects objectAtIndex:ind];
	
	[obj removeFromSuperview];
	[mainView addSubview:obj];
}

/* This method adds the contentview and the two main and secondary views
 * to the window.
 * This allows the game board/buttons/tiles to be seen and allows the user
 * to start playing a game of scrabble
 */
-(void)displayGameArena {
	//Do a transition with the current content view
	[self transitionContentViewsFade:contentView];
	
	//Set the content view
	[window setContentView:contentView];
	
	//Add main and secondary view in order
	[contentView addSubview:mainView];
	
	[self transitionContentViewsAppear];
	
	//For aesthetic purposes secondary view will popup after content appears
	[contentView addSubview:secondaryView];
	
	[mainView removeFromSuperview];
	
	//To keep mainview on top of the secondary view it is added again
	[contentView addSubview:mainView];
}

//Fade content view
-(void)transitionContentViewsFade:(NSView *)newView {
	[newView setAlphaValue:0];
	NSView *curView = [window contentView];
	while ([curView alphaValue] > 0) {
		[curView setAlphaValue:[curView alphaValue]-.1];
		[NSThread sleepForTimeInterval:.01];
	}
}

//Make content view appear
-(void)transitionContentViewsAppear {
	NSView *curView = [window contentView];
	
	while ([curView alphaValue] <= 1) {
		[curView setAlphaValue:[curView alphaValue]+.1];
		[NSThread sleepForTimeInterval:.01];
	}
}

/* Shows a menu with buttons and background */
-(void)displayMenu:(NSString *)logo andButtons:(NSMutableArray *)buttons {
	//Menu has to be allocated
	if (menuView == nil) {
		menuView = [[NSView alloc]initWithFrame:NSMakeRect(0, 0, window.frame.size.width, 
																   window.frame.size.height)];
		//Add logo
		NSImageView *logoimg = [[NSImageView alloc] initWithFrame:NSMakeRect((double)1/4 * window.frame.size.width, 
																			 (double)2.5/4 * window.frame.size.height, 
																			 (double)1/2 * window.frame.size.width, 
																			 (double)1/5 * window.frame.size.height)];
		NSImage *scrabbleLogo = [NSImage imageNamed:logo];
		[scrabbleLogo setSize:NSMakeSize(window.frame.size.width/2, 
										 window.frame.size.height/5)];
	
		[logoimg setImage:scrabbleLogo];
	
		[menuView addSubview:logoimg];
		[logoimg release];
		[logo release];
	
		//Add buttons
		for (NSButton *button in buttons) {
			[menuView addSubview:button];
			[button setTarget:self];
			[button setAction:@selector(buttonPressed:)];
		}

		
		for (NSButton *button in buttons) {
			[button release];
			button = nil;
		}
	
		[buttons removeAllObjects];
		[window setContentView:menuView];
	}
	//Menu has to be displayed 
	else {
		[self transitionContentViewsFade:menuView];
		
		[window setContentView:menuView];
		
		[self transitionContentViewsAppear];
	}
}

/* This method takes the object referenced by ref and animates it
 * so that it's frame will become newFrame
 *
 * @param newFrame is the newFrame of the object
 * @param ref references the object to animate
 *
 * @return void
 */
-(void)animate:(NSRect)newFrame andReference:(int)ref {
	GraphicsBlock *obj = [graphicalObjects objectAtIndex:ref];
	
	[obj setFrame:newFrame];
}

/* Checks if the user clicked or dragged on the image
 *
 * @param ref specifies the image
 *
 * @return an NSPoint of where the mouse clicked otherwise, -1, -1
 */
-(NSPoint)mouseClickedOrDragOnImage:(int)ref {
	GraphicsBlock *img = [graphicalObjects objectAtIndex:ref];
	
	//Set reference to image if reference is not set 
	if (mousePressed) {
		
		//Check if the image being pressed is the reference
		if (refImagePressed == ref) 
			return mousePosition;
	
		if (refImagePressed < 0) {
			
			//Check if clicked the object specified by ref
			if (NSPointInRect(mousePosition, [img frame])) {
				
				//Save the reference to the image being pressed
				refImagePressed = ref;
			}
		}
	}
	
	//Otherwise return -1s
	return NSMakePoint(-1, -1);
}

//NOTE: THESE MESSAGES ARE SENT BY THE GRAPHICSCONTENT

/* Delegate methods that handle mouse events */
-(void)mouseDown:(NSEvent *)event {
	
	//Mouse pressed
	mousePressed = YES;
	
	//Update mouse position
	[self updateMousePosition:event];
}

/* Called whenever user drags the mouse while pressing */
-(void)mouseDragged:(NSEvent *)event {
	
	//Update mouse point position
	[self updateMousePosition:event];
}

/* Called whenever user releases left button */
-(void)mouseUp:(NSEvent *)event {
	mousePressed = NO;
		
	[self updateMousePosition:event];
	
	//Every time user let's go of left click, snap the letter
	//that he was dragging to a grid
	if (refImagePressed >= 0) {
		[self snapToGrid:refImagePressed];
	}
	
	//No images being pressed
	refImagePressed = -1;
}

/* Just updates mouse position */
-(void)updateMousePosition: (NSEvent *)theEvent {
	//Update mouse position
	mousePosition = [theEvent locationInWindow];
}

/* Checks if there was a click on the button with reference ref */
-(BOOL)mouseClickedOnButton:(int)ref {
	if ([graphicalObjects count] < ref)
		return NO;
	NSButton *button = [graphicalObjects objectAtIndex:ref];
	if([button state] == NSOnState) {
		[button setState:NSOffState];
		return YES;
	}
	return NO;
}

/* Compares rectangle sizes. Used for the comparison during snapping
 * image to grid
 *
 * @returns true if rect is greater than rect2
 */
-(BOOL)rectLargerThanRect:(NSRect)rect andRect2:(NSRect)rect2 {
	NSSize size1 = rect.size;
	NSSize size2 = rect2.size;
	
	return (size1.width * size1.height) > (size2.width * size2.height);
}

/* This method takes a reference and snaps that image to the grid
 * (specified by images in the secondary view)
 *
 * @return void
 */
-(void)snapToGrid:(int)ref {
	
	NSView *img = [graphicalObjects objectAtIndex:ref];
	
	//Largets intersect
	NSRect biggestIntersect = NSMakeRect(0, 0, 0, 0);
	NSRect tileOfBiggestIntersect = NSMakeRect(0, 0, 0, 0);
	BOOL didFindLargest = false;
	
	//Snap to a grid in the secondary view
	for (NSView *grids in secondaryView.subviews) {
		NSRect temp = NSIntersectionRect(grids.frame, img.frame);
		if ([self rectLargerThanRect:temp andRect2:biggestIntersect]) {
			//Update biggestIntersect
			biggestIntersect = temp;
			
			//Update the tile of the biggest intersect
			tileOfBiggestIntersect = grids.frame;
			
			//Found largest
			didFindLargest = true;
		}
	}
	
	//Snap to grid
	if (didFindLargest) {
		//Play click sound
		[sound playDefaultClick];
		img.frame = NSMakeRect(tileOfBiggestIntersect.origin.x-1, tileOfBiggestIntersect.origin.y-1, 
							   img.frame.size.width, img.frame.size.height);
	}
}

/* Returns nssize of the image referenced by integer ref */
-(NSSize)sizeOfImageWithReference:(int)ref {
	GraphicsBlock *block = [graphicalObjects objectAtIndex:ref];
	return [block frame].size;
}

/* Returns NSpoint position of the image referenced by integer ref */
-(NSPoint)positionOfImageWithReference:(int)ref {
	GraphicsBlock *block = [graphicalObjects objectAtIndex:ref];
	return [block frame].origin;
}

/* Returns the intersect rect */
-(NSRect)rectIntersectsRect:(int)ref1 andRef2:(int)ref2 {
	GraphicsBlock *block1 = [graphicalObjects objectAtIndex:ref1];
	GraphicsBlock *block2 = [graphicalObjects objectAtIndex:ref2];
	
	NSRect rect = [contentView convertRectToBase:block1.frame];
	NSRect rect2 = [contentView convertRectToBase:block2.frame];
	
	return NSIntersectionRect(rect, rect2);
}

/* Draws a button */
-(int)drawButtonWithWidth:(double)pWidth andHeight: (double)pHeight andLocationX: (double)px locationY:(double)py
				  andText: (NSString *)text {
	NSButton *newButton = [[NSButton alloc]initWithFrame:NSMakeRect(px, py, pWidth, pHeight)];
	
	//Set button attributes
	[newButton setButtonType:NSMomentaryLight];
	[newButton setTitle:text];
	[newButton setBezelStyle:NSRegularSquareBezelStyle];
	
	//Add to array and graphics
	[contentView addSubview:newButton];
	
	[graphicalObjects addObject:newButton];
	
	//Release it
	[newButton release];
	
	//Return reference to newly created button
	return index++;
}

/* Change an image's text color */
-(void)changeImageTextColor:(int)ref andColor:(NSString *)color {
	GraphicsBlock *block = [graphicalObjects objectAtIndex:ref];
	[block setColorForLabel:color];
}

/* Destroy image referenced by ref */
-(void)destroyImageWithRef:(int)ref {
	GraphicsBlock *destroy = [graphicalObjects objectAtIndex:ref];
	
	//Remove block from view
	[destroy removeFromSuperview];
		
	destroy = NULL;
	
	//Replace object from array with string "dead"
	[graphicalObjects replaceObjectAtIndex:ref withObject:@"dead"];
}

-(BOOL)rectIntersectsAnother:(int)ref {
	GraphicsBlock *block = [graphicalObjects objectAtIndex:ref];
	
	int i;
	for (i = 0; i < [graphicalObjects count]; i++) {
		//First check if the item is type graphics block
		id graphicalItem = [graphicalObjects objectAtIndex:i];
		
		if ([graphicalItem class] == [GraphicsBlock class]) {
			
			//If it is, check if any intersect ref
			GraphicsBlock *another = [graphicalObjects objectAtIndex:i];
			
			//If so, return yes
			if (block != nil &&
				block != another && 
				NSIntersectsRect(block.frame, another.frame))
				return YES;
		}
	}
	//Otherwise return no
	return NO;
}

/* Creates a popup window that notifies the user of text */
-(void)createPopupWindowNotification:(NSString *)text {
	//Alert the user of text
	[talker speakString:text];
	
	//Set up notification
	NSTextField *note = [[NSTextField alloc]initWithFrame:NSMakeRect(0, 0, 0, 0)];
	[note setStringValue:text];
	[note setFont:[NSFont fontWithName:@"Helvetica" size:POPUP_FONT_SZ]];
	[note setTextColor:[NSColor whiteColor]];
	[note setEditable:NO];
	[note setSelectable:YES];
	[note setAlignment:NSCenterTextAlignment];
	[note setBordered:NO];
	[note setDrawsBackground:NO];
	
	//Add definition
	[self addDefinition:note];
	
	//Hyperlink word to def
	[self hyperLinkStringBetweenQuotes:note];
	[note sizeToFit];
	
	//Place textfield in center of content view
	NSRect notePos = NSMakeRect(popup.frame.size.width/2 - note.frame.size.width/2, 
								popup.frame.size.height/2 - note.frame.size.height/2, 
								note.frame.size.width, note.frame.size.height);
	
	[note setFrame:notePos];
	
	[popup.contentView addSubview:note];
	
	//Set popup position
	[popup setFrameOrigin:NSMakePoint(window.frame.origin.x + window.frame.size.width/4, 
									  window.frame.origin.y + window.frame.size.height/4)];
	
	//Fade in
	[self fadeInWindow:popup];
	
	//Setup Notification
	while (mousePressed == NO);
	
	//Fade out
	[self fadeOutWindow:popup];
	
	[note removeFromSuperview];
	
	//Release the notification
	[note release];
}

/* Fade in with a passed window */
-(void)fadeInWindow:(NSWindow*)win {
	[win makeKeyAndOrderFront:NSApp];
	double alpha = [win alphaValue];

	while (alpha < .8) {
		alpha += 0.1;
		[win setAlphaValue:alpha];
		[NSThread sleepForTimeInterval:.03];
	}
}

/* Fade out a passed window */
-(void)fadeOutWindow:(NSWindow*)win {
	[win makeKeyAndOrderFront:NSApp];
	double alpha = [win alphaValue];
	
	while (alpha > 0) {
		alpha -= 0.1;
		[win setAlphaValue:alpha];
		[NSThread sleepForTimeInterval:.03];
	}
}

/* Alerts the user if he really wants to do an action */
-(BOOL)alertUserYesOrNo:(NSString *)action {
	if (newAlert == nil) {
		newAlert = [NSAlert alertWithMessageText:action
								   defaultButton:nil
								 alternateButton:@"Cancel"
									 otherButton:nil 
					   informativeTextWithFormat:@"Action can not be undone."];
	}
	[newAlert setMessageText:action];
		
	int returnValue = [newAlert runModal];
	
	
	if (returnValue == NSAlertDefaultReturn)
		return YES;
	return NO;
}

/* Set's the image's sub text */
-(void)setSubTextForImageWithRef:(int)ref subText:(NSString *)text {
	[[graphicalObjects objectAtIndex:ref] setSubTextForBlock:text];
}

/* Label creation */
- (int)createBlockLabel:(NSRect)blockFrame andLabel:(NSString *)label {
	//Create the textfield
	NSTextField *blockLabel = [[NSTextField alloc]initWithFrame:NSMakeRect(blockFrame.origin.x, blockFrame.origin.y, 
																		   0, 0)];
	[blockLabel setEditable:NO];
	[blockLabel setStringValue:label];
	[blockLabel setTextColor:[NSColor blackColor]];
	
	//Set some attributes
	[blockLabel setAlignment:NSCenterTextAlignment];
	[blockLabel setBordered:NO];
	[blockLabel setFont:[NSFont fontWithName:@"Verdana Bold" size:PTSZ]];
	
	//Little tune-up for size
	[blockLabel sizeToFit];
	
	//Set background to clear
	[blockLabel setDrawsBackground:NO];
	
	//Place the label in the center of the block
	NSRect labelRect = NSMakeRect(blockFrame.origin.x, blockFrame.origin.y, 
								  [blockLabel frame].size.width, [blockLabel frame].size.height);
	
	//Set label
	[blockLabel setFrame:labelRect];
	
	//Add to the view
	[mainView addSubview:blockLabel];
	
	//Insert the label into the graphical objects array
	[graphicalObjects addObject:blockLabel];
	
	[blockLabel release];
	
	return index++;
}

/* Changes a labels' text to str */
-(void)changeTextOfLabel:(int)ref withString:(NSString *)str {
	NSTextField *label = [graphicalObjects objectAtIndex:ref];
	
	[label setStringValue:str];
}

/* Set the alpha value of an image */
-(void)setAlphaValueOfImage:(int)ref ofAlpha:(float)alpha {
	NSView *image = [graphicalObjects objectAtIndex:ref];
	[image setAlphaValue:alpha];
}

/* Hyperlinks to dictionary webpage the string in between quotes */
-(void)hyperLinkStringBetweenQuotes:(NSTextField *)textField {
	
	//Check first if there is a word between quotes
	NSRange rangeOfQuotes;
	rangeOfQuotes = [[textField stringValue] rangeOfString:@"'"];
	if (rangeOfQuotes.location == NSNotFound)
		return;
	
	//Add hyperlink to popup
	[textField setAllowsEditingTextAttributes: YES];
	
	NSMutableAttributedString *link = [[textField attributedStringValue]
									mutableCopy];
	NSString *linkString = [link string];
	
	int firstQuote = -1, secondQuote = -1;
	
	//Get string in between quotes
	for (firstQuote = 0; firstQuote < [linkString length]; firstQuote++) {
		if ([linkString characterAtIndex:firstQuote] == '\'')
			break;
	}
	
	for (secondQuote = firstQuote + 1; secondQuote < [linkString length]; secondQuote++) {
		if ([linkString characterAtIndex:secondQuote] == '\'')
			break;
	}
	
	NSString *searchString = [linkString substringWithRange:NSMakeRange(firstQuote+1, secondQuote - firstQuote - 1)];
	
	//Create hyperlink
	NSString *url = @"http://oxforddictionaries.com/search?searchType=dictionary&isWritersAndEditors=true&searchUri=All&q=";
	url = [url stringByAppendingFormat:searchString];
	
	NSDictionary *dict = [NSDictionary dictionaryWithObject:url
													 forKey: NSLinkAttributeName];
	
	//Create hyperlink between quotes
	[link addAttributes: dict range: NSMakeRange(firstQuote + 1, secondQuote - firstQuote - 1)];
	
	[textField setAttributedStringValue: link];
	[link release];
}

-(void)addDefinition:(NSTextField *)textField {
	NSString *url = [self getUrl:textField];
	
	if (url == nil)
		return;
	
	//Get web page and show definition!
	//NSString *webContents = [NSString stringWithContentsOfURL:[NSURL URLWithString:url] encoding:NSUTF8StringEncoding error:nil];
	NSMutableURLRequest *request = [[NSMutableURLRequest alloc]init];
	[request setURL:[NSURL URLWithString:[NSString stringWithFormat:[self getUrl:textField]]]];
	[request setHTTPMethod:@"GET"];
	
	NSHTTPURLResponse *authResponse;
	NSError *authError;
	NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:&authResponse error:&authError];
	NSString *webContents = [[NSString alloc]initWithData:data encoding:NSUTF8StringEncoding];
	NSString *contentArea;
	[request release];
		
	//Definition string
	NSString *def = @"\n\n\"";
	
	//Funnel contents in webContents
	NSRange contentRange = [webContents rangeOfString:@"<span class=\"definition\">"];
	if (contentRange.location != NSNotFound) {
		contentArea = [webContents substringFromIndex:contentRange.location];
			
		//Find def
		NSRange defRange = [contentArea rangeOfString:@"<span class=\"definition\">"];
	
		int defStartIndex = defRange.location + defRange.length;
		
		char storage;
		
		int charCount = 0;
		
		bool firstLetter = true;
		
		//Get definition
		while ((storage = [contentArea characterAtIndex:defStartIndex++]) != '<') {
			if (firstLetter && storage >= 'a' && storage <= 'z') {
				storage += 'A' - 'a';
				firstLetter = false;
			}
			
			//Don't concatenate colons
			if (storage == ':')
				continue;
			def = [def stringByAppendingFormat:@"%c",storage];
			if (charCount++ >= POPUP_BRK_LNGTH && [def characterAtIndex:[def length]-1] == ' ') {
				def = [def stringByAppendingFormat:@"\n"];
				charCount = 0;
			}
		}
		//end of quote
		def = [def stringByAppendingFormat:@".\""];
	}
	else {
		def = @"\n\nDefinition not found";
	}
	[webContents release];
	[textField setStringValue:[[textField stringValue] stringByAppendingFormat:def]];
}

-(NSString *)getUrl:(NSTextField *)textField {
	//Check first if there is a word between quotes
	NSRange rangeOfQuotes;
	rangeOfQuotes = [[textField stringValue] rangeOfString:@"'"];
	if (rangeOfQuotes.location == NSNotFound)
		return nil;
	
	int firstQuote = -1, secondQuote = -1;
	
	NSString *linkString = [textField stringValue];
	
	//Get string in between quotes
	for (firstQuote = 0; firstQuote < [linkString length]; firstQuote++) {
		if ([linkString characterAtIndex:firstQuote] == '\'')
			break;
	}
	
	for (secondQuote = firstQuote + 1; secondQuote < [linkString length]; secondQuote++) {
		if ([linkString characterAtIndex:secondQuote] == '\'')
			break;
	}
	
	NSString *searchString = [linkString substringWithRange:NSMakeRange(firstQuote+1, secondQuote - firstQuote - 1)];
	
	//Create hyperlink
	NSString *url = @"http://oxforddictionaries.com/search?searchType=dictionary&isWritersAndEditors=true&searchUri=All&q=";
	url = [url stringByAppendingFormat:searchString];
	
	return url;
}

/* Return window size */
-(NSRect)size {
	return window.frame;
}

/* Called when button pressed */
-(void)buttonPressed:(id)sender {
	NSRange playString = [[sender title] rangeOfString:@"Play"];
	//Playing mode
	if (playString.location != NSNotFound)
		buttonPress = BEGIN_PLAY_MODE;
}

/* Called when user wants to clear everything */
-(void)clearEverything {
	for (id object in graphicalObjects) {
		if (![object isKindOfClass:[NSString class]]) {
			[object removeFromSuperview];
			[object release];
		}
		object = nil;
	}
	[graphicalObjects removeAllObjects];
	index = 0;
}

-(void)dealloc {	
	//Remove all objects
	[graphicalObjects removeAllObjects];
	
	//Release window and content view
	[contentView release];
	[mainView release];
	[secondaryView release];
	[window release];
	[talker release];
	
	[popup release];
	[sound release];
	
	//Release self
	[super dealloc];
}

@synthesize buttonPress;
@end
