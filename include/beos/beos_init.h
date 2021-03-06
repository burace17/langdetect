#ifndef INCLUDE_BEOS_INIT
#define INCLUDE_BEOS_INIT
#include <Application.h>
#include <Alert.h>
#include <Path.h>
#include "beos/beos_gui.h"
class App : public BApplication {
	public:
		App(void);
		void RefsReceived(BMessage* msg);
	private:
		void OpenStopFilesDir(BMessage* msg);
};

void beos_init();
void beos_display_dialog(char* output);
#endif
