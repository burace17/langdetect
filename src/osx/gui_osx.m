#import "osx/gui_osx.h"

@implementation AppDelegate
@synthesize input;

-(IBAction)objc_detectLanguage:(id)sender {
	// Jump back to C code
	NSString *text = [input stringValue];
	detect_language([text UTF8String]);
}

@end

void objc_displayDialog(char output[]) {
	NSString *newOutput = [NSString stringWithUTF8String:output];
	NSAlert *alert = [[NSAlert alloc] init];
	[alert setMessageText:@"Results"];
	[alert setInformativeText:newOutput];
	[alert runModal];
	[alert release];	
}
