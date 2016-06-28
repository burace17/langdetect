#include "gui.h"

void display_dialog(char* output) {
#ifdef GTK_UI
	_gtk_display_dialog(output);
#endif

#if defined(_WIN32) && !defined(UWP)
	MessageBox(NULL, output, "Results", MB_ICONINFORMATION);
#endif

#ifdef UWP
	uwp_display_dialog(output);
#endif

#ifdef __APPLE__
	objc_display_dialog(output);
#endif
#ifdef HAIKU_OS
	beos_display_dialog(output);
#endif

}


void create_window(int argc, char** argv) {
#if defined(_WIN32) && !defined(UWP)
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

char* get_language_name(STOP_FILE name) {
#ifdef UWP
	char* str = new char[wstr_size_needed(name->DisplayName)];
	wstr_to_utf8(name->DisplayName, str);
#else
	char* str = (char*)calloc(strlen(name)+1, sizeof(char));
	char lang[256];
	sscanf(name, "%[^.]", lang);
	strncpy(str, lang, sizeof(str));
#endif

	return str;
}

