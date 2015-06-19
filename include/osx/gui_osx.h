#import <Cocoa/Cocoa.h>
#include "langdetect.h"
@interface AppDelegate : NSObject 
@property (nonatomic) IBOutlet NSTextField *input;
-(IBAction)objc_detectLanguage:(id)sender;
@end
void objc_displayDialog(char output[]);
