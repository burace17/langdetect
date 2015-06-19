#import <Cocoa/Cocoa.h>
#include "langdetect.h"
@interface AppDelegate : NSObject {
	NSTextField *input;
}
@property (nonatomic, retain) IBOutlet NSTextField *input;
-(IBAction)objc_detectLanguage:(id)sender;
@end
void objc_displayDialog(char output[]);
