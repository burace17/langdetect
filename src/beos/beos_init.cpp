#include "beos/beos_init.h"
MainWindow* window;
App::App(void) : BApplication("application/x-vnc.dw-langdetect")
{
	window = new MainWindow();
	window->Show();
}

void App::RefsReceived(BMessage* msg) 
{
	OpenStopFilesDir(msg);
}

void App::OpenStopFilesDir(BMessage* msg)
{
	entry_ref ref;
	char name[256]; 
	const char* orig_path;
	BPath path;
	BEntry entry;
	status_t err = B_OK;

	msg->FindRef("refs", &ref);
	entry.SetTo(&ref);
	entry.GetPath(&path);

	orig_path = path.Path();

	memset(name, 0, sizeof(name));
	strncpy(name, orig_path, strlen(orig_path)+1);
	printf("name: %s\n", name);

	// initialize stop files directory
	initialize(name);
}

void beos_init() {
	App* app = new App();
	app->Run();
	delete app;
}

void beos_display_dialog(const char* output) {
	BAlert* alert = new BAlert("Results", output, "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
	alert->Go();
	delete alert;
}
