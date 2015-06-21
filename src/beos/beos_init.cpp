#include "beos/beos_init.h"
App::App(void) : BApplication("application/x-vnc.dw-langdetect")
{
	MainWindow* window = new MainWindow();
	window->Show();
}
void beos_init() {
	App* app = new App();
	app->Run();
	delete app;
}

void beos_displayDialog(char output[]) {
	BAlert* alert = new BAlert("Results", output, "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
	alert->Go();
	delete alert;
}