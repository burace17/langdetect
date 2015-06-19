#include "osx/init_osx.h"
int objc_main(int argc, char* const argv[]) {
	// OSX sets the default working directory to be / which screws up my fopen call
	// To make things easier, I just change the working directory here to be our bundle's
	// root. This allows fopen() to find the dictionaries 
	// TODO: Alow user to specify custom dictionary location
	NSString *appPath = [[NSBundle mainBundle] bundlePath];
	chdir([[appPath stringByAppendingString:@"/Contents/Resources"] UTF8String]);	

	return NSApplicationMain(argc, argv);
}
