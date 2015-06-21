#import <Cocoa/Cocoa.h>
#include "langdetect.h"
@interface AppDelegate : NSObject {
	NSTextField *input;
}
@property (nonatomic, retain) IBOutlet NSTextField *input;
-(IBAction)objc_detect_language:(id)sender;
@end
void objc_display_dialog(char output[]);
