#import "osx/osx_gui.h"

@implementation AppDelegate
@synthesize input;

-(IBAction)objc_detect_language:(id)sender {
	// Jump back to C code
	const char* text = [[input stringValue] UTF8String];
	detect_language(text);
}

-(IBAction)selectStopFiles:(id)sender {
    NSOpenPanel* selectDlg = [NSOpenPanel openPanel];
    [selectDlg setCanChooseFiles:NO];
    [selectDlg setCanChooseDirectories:YES];
    
    if ([selectDlg runModalForDirectory:nil file:nil] == NSOKButton) {
        initialize([[[selectDlg filenames] objectAtIndex:0] UTF8String]);
    }
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
