//
//  GraphicsBlock.m
//  Scrabble
//
//  Created by Max Lam on 7/22/11.
//  Copyright 2011 Max Lam. All rights reserved.
//

#import "GraphicsBlock.h"


@implementation GraphicsBlock

/* The two methods below instantiate an object of type graphics block in different ways.
 *
 * The first instantiates the graphics block with an optional string and a mandatory imageFile
 * (This may be to create the letters for scrabble)
 *
 * The second instantiates the graphics block with an optional string and a mandatory background color.
 * The second method only creates a colored rect. (This may be to create tiles for the game)
 */

- (id)initWithFrame:(NSRect)frame andLabel:(NSString *)label withColor:(NSString *)color andImage:(NSString *)imgFile {
	//Init parent
	self = [super initWithFrame:frame];
	
	//Initialize and add image to subview
	imageView = [[NSImageView alloc]initWithFrame:NSMakeRect(0, 0, frame.size.width, frame.size.height)];
	[imageView setImage:[NSImage imageNamed:imgFile]];
		
	 if ([imageView image] == nil) {
		 //Image loading failed
		 NSLog(@"Image loading failed with image: %@",imgFile);
		 return nil;
	 }
	 
	 //Add to image to subview
	[self addSubview:imageView];
	
	
	//If caller wants to set a label..
	if (![label isEqualToString:@""]) 		
		[self createBlockLabel:frame andLabel:label andColor:color];
	
	//Image here
	imagePresent = YES;
	
	return self;
}

/* Same as above except creates only a colored rectangle */
- (id)initWithFrame:(NSRect)frame andLabel:(NSString *)label withColor:(NSString *)color andBackgroundColor:(NSString *)bgColor {
	self = [super initWithFrame:frame];
	
	//No image here
	imagePresent = NO;
	
	//Just set background color
	//First create an NSImage with bg color of bgColor
	NSImage *bgImage = [[NSImage alloc]initWithSize:frame.size];
	NSRect imageBounds = NSMakeRect(0, 0, frame.size.width, frame.size.height);
	
	//Prepare to draw on image
	[bgImage lockFocus];
	
	//Set the drawing color
	[[self colorFromString:bgColor] set];
	
	//Fill the rectangle of the image
	NSRectFill (imageBounds);
	
	//end drawing
	[bgImage unlockFocus];
	
	//Set NSImage to be part of the image view
	imageView = [[NSImageView alloc]initWithFrame:NSMakeRect(0, 0, frame.size.width, frame.size.height)];
	
	[imageView setImage:bgImage];
	
	//Release bgImage
	[bgImage release];
	
	//Add image view to self
	[self addSubview:imageView];
	
	//If caller wants to set a label..
	if (![label isEqualToString:@""])
		[self createBlockLabel:frame andLabel:label andColor:color];

	return self;
}

/* This method creates a label at the specified position (Note that this edits the blockLabel variable)
 *
 * @param blockFrame is the dimensions and origin of the whole block to which the label will be pasted on
 * @param label is the label string
 * @param color is the color of the label
 *
 * @return void
 */
- (void)createBlockLabel:(NSRect)blockFrame andLabel:(NSString *)label andColor:(NSString *)color {
	//Create the label first
	blockLabel = [[NSTextField alloc]initWithFrame:NSMakeRect(0, 0, 0, 0)];
	[blockLabel setEditable:NO];
	[blockLabel setStringValue:label];
	[blockLabel setTextColor:[self colorFromString:color]];
	
	//Set some attributes
	[blockLabel setAlignment:NSCenterTextAlignment];
	[blockLabel setBordered:NO];
	[blockLabel setFont:[NSFont fontWithName:@"Verdana Bold" size:PTSZ]];
	
	//Little tune-up for size
	[blockLabel sizeToFit];
	
	//Set background to clear
	[blockLabel setDrawsBackground:NO];
	
	//Place the label in the center of the block
	NSRect labelRect = NSMakeRect(blockFrame.size.width/2 - [blockLabel frame].size.width/2, 
								  blockFrame.size.height/2 - [blockLabel frame].size.height/3, 
								  [blockLabel frame].size.width, [blockLabel frame].size.height);

	//Set label
	[blockLabel setFrame:labelRect];
	
	//Add to the view
	[self addSubview:blockLabel];
}

/* Returns NSColor given string */
- (NSColor *)colorFromString:(NSString *)color {
	if ([color isEqualToString:@"black"])
		return [NSColor blackColor];
	if ([color isEqualToString:@"brown"])
		return [NSColor brownColor];
	if ([color isEqualToString:@"blue"])
		return [NSColor blueColor];
	if ([color isEqualToString:@"green"])
		return [NSColor greenColor];
	if ([color isEqualToString:@"yellow"])
		return [NSColor yellowColor];
	if ([color isEqualToString:@"red"])
		return [NSColor redColor];
	if ([color isEqualToString:@"white"])
		return [NSColor whiteColor];
	if ([color isEqualToString:@"grey"])
		return [NSColor colorWithCalibratedRed:0 green:0 blue:.2 alpha:.2];
	if ([color isEqualToString:@"orange"])
		return [NSColor orangeColor];
	if ([color isEqualToString:@"clear"])
		return [NSColor clearColor];
	if ([color isEqualToString:@"cyan"]) {
		return [NSColor cyanColor];
	}
	return nil;
}

/* This method is called whenever the View is updated
 *
 * This method produces a shadowy and curved-corner effect,
 *
 * @return void
 */
- (void)drawRect:(NSRect)dirtyRect {
	//Only do effects when there is no image
	if (!imagePresent) {	
		
		// set any NSColor for filling, say white:
		[[NSColor whiteColor] setFill];
		NSRectFill(dirtyRect);
		
		//Create a shadowy effect
		//Save the graphics state
		NSGraphicsContext *context = [NSGraphicsContext currentContext];
		[context saveGraphicsState];
		
		[context setCompositingOperation:NSCompositePlusDarker];
		
		//Create the outline of the shadow 
		NSBezierPath *path = [NSBezierPath
							  bezierPathWithRoundedRect:[self bounds]
							  xRadius:10.0f
							  yRadius:10.0f];
		
		
		[[NSColor whiteColor] setStroke];
		
		//Create the shadow
		NSShadow * shadow = [[NSShadow alloc] init];
		[shadow setShadowColor:[NSColor colorWithCalibratedRed:0 green:0 blue:0 alpha:1]];
		[shadow setShadowBlurRadius:12.0f];
		[shadow set];
		
		//Draw the shadow
		[path stroke];
		
		[context restoreGraphicsState];
		
		//Grid like appearance effect
		[NSBezierPath setDefaultLineWidth:5.0];
		[[NSColor highlightColor] set];
		[NSBezierPath strokeRect:self.bounds];
		
		//Release shadow
		[shadow release];
	}
}

/* Set's opaque to no */
-(BOOL)isOpaque {
	return NO;
}

//Sets color of the label of the block
-(void)setColorForLabel:(NSString *)color {
	NSColor *labelColor = [self colorFromString:color];
	
	[blockLabel setTextColor:labelColor];
}

/* Set the block's sub text */
-(void)setSubTextForBlock:(NSString *)text {
	
	//Create the label first
	NSTextField * subLabel = [[NSTextField alloc]initWithFrame:NSMakeRect(0, 0, 0, 0)];
	[subLabel setEditable:NO];
	[subLabel setStringValue:text];
	[subLabel setTextColor:[self colorFromString:@"black"]];
	
	//Set some attributes
	[subLabel setAlignment:NSCenterTextAlignment];
	[subLabel setBordered:NO];
	[subLabel setFont:[NSFont fontWithName:@"Verdana Bold" size:PTSZ/2.5]];
	
	//Little tune-up for size
	[subLabel sizeToFit];
	
	//Set background to clear
	[subLabel setDrawsBackground:NO];
	
	//Place the label in the center of the block
	NSRect labelRect = NSMakeRect(self.frame.size.width * 3 / 4 - [subLabel frame].size.width/2, 
								  [subLabel frame].size.height/2, 
								  [subLabel frame].size.width, [subLabel frame].size.height);
	
	//Set label
	[subLabel setFrame:labelRect];
	
	//Add to the view
	[self addSubview:subLabel];
	
	//Release sub label
	[subLabel release];
}

-(NSView *)hitTest:(NSPoint)aPoint {
	return [self superview];
}

/* Deallocation */
-(void)dealloc {
	if (imageView != nil)
		[imageView release];
	[blockLabel release];
	[super dealloc];
}

@synthesize blockLabel;

@end
