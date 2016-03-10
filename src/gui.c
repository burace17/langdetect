#include "gui.h"

void display_dialog(char* output) {
#ifdef GTK_UI
	_gtk_display_dialog(output);
#endif

#ifdef _WIN32
	MessageBox(NULL, output, "Results", MB_ICONINFORMATION);
#endif

#ifdef __APPLE__
	objc_display_dialog(output);
#endif
#ifdef HAIKU_OS
	beos_display_dialog(output);
#endif

}


void create_window(int argc, char** argv) {
#ifdef _WIN32
	win32_create_window();
#endif

#ifdef GTK_UI
	_gtk_create_window(argc, argv);
#endif

#ifdef __APPLE__
	// Cocoa must be initialized with the objective c compiler, so we jump to objc code here.
	// This gets rid of a warning from clang
	const char* cargv = (const char*)argv;
	objc_main(argc, &cargv);
#endif
#ifdef HAIKU_OS
	beos_init();
#endif

#ifdef QT_UI
	qt_create_window();
#endif
}



