#import "osx/gui_osx.h"

@implementation AppDelegate
@synthesize input;

-(IBAction)objc_detectLanguage:(id)sender {
	// Jump back to C code
	const char* text = [[input stringValue] UTF8String];
	detect_language(text);
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
