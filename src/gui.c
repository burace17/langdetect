#include "gui.h"

void display_dialog(char* output) {
#ifdef GTK_UI
	_gtk_display_dialog(output);
#endif

#if defined(_WIN32) && !defined(UWP) && !defined(QT_UI)
	MessageBox(NULL, output, "Results", MB_ICONINFORMATION);
#endif

#ifdef UWP
	uwp_display_dialog(output);
#endif

#if defined(__APPLE__) && !defined(CARBON_UI)
	objc_display_dialog(output);
#endif

#ifdef CARBON_UI
	carbon_display_dialog(output);
#endif

#ifdef HAIKU_OS
	beos_display_dialog(output);
#endif

#ifdef QT_UI
	qt_display_dialog(output);
#endif

}


void create_window(int argc, char** argv) {
#if defined(_WIN32) && !defined(UWP) && !defined(QT_UI)
	win32_create_window();
#endif

#ifdef GTK_UI
	_gtk_create_window(argc, argv);
#endif

#if defined(__APPLE__) && !defined(CARBON_UI)
	// Cocoa must be initialized with the objective c compiler, so we jump to objc code here.
	// This gets rid of a warning from clang
	const char* cargv = (const char*)argv;
	objc_main(argc, &cargv);
#endif

#ifdef CARBON_UI
	carbon_create_window(argc, argv);
#endif

#ifdef HAIKU_OS
	beos_init();
#endif

#ifdef QT_UI
	qt_create_window(argc, argv);
#endif
}

// This function will either take in a char* containing a full file name (with extension) for a stop
// words file and return only the file name (minus the extension) or it will take in a StorageFile^ 
// and return its DisplayName as a UTF-8 string. This returns memory which must be freed.  
char* get_language_name(STOP_FILE name) {
#ifdef UWP
	// If we are on UWP, we are working with a StorageFile^ 
	// Allocate enough space for a new string that only contains the file name
	char* str = new char[wstr_size_needed(name->DisplayName)];

	// Convert the UTF-16 display name to a UTF-8 string. 
	wstr_to_utf8(name->DisplayName, str);
#else
	// Not on UWP, so name is a char*
	// Allocate enough space for the new string
	char* str = (char*)calloc(strlen(name)+1, sizeof(char));

	// Temporary string used with sscanf
	char lang[256];

	// Remove the file extension and place the new string in lang
	sscanf(name, "%[^.]", lang);

	// Copy it to our heap allocated string and return it.
	strncpy(str, lang, strlen(name)+1);
#endif

	return str;
}

