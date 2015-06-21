#import "osx/gui_osx.h"

@implementation AppDelegate
@synthesize input;

-(IBAction)objc_detect_language:(id)sender {
	// Jump back to C code
	const char* text = [[input stringValue] UTF8String];
	detect_language(text);
}

@end

void objc_display_dialog(char output[]) {
	NSString *newOutput = [NSString stringWithUTF8String:output];
	NSAlert *alert = [[NSAlert alloc] init];
	[alert setMessageText:@"Results"];
	[alert setInformativeText:newOutput];
	[alert runModal];
	[alert release];	
}
