#ifndef INCLUDE_BEOS_INIT
#define INCLUDE_BEOS_INIT
#include <Application.h>

class App : public BApplication {
	public:
		App(void);
};

void beos_init();
void beos_displayDialog(char output[]);
#endif
