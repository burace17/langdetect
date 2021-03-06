#ifndef INCLUDE_GUI
#define INCLUDE_GUI
#ifdef GTK_UI
#include "gtk/gtk_gui.h"
#endif

#if defined(_WIN32) && !defined(UWP)
#include "windows/windows_gui.h"
#endif

#if defined(__APPLE__) && !defined(CARBON_UI)
#include "osx/osx_init.h"
#include "osx/osx_gui.h"
#endif

#ifdef CARBON_UI
#include "osx-carbon/carbon_gui.h"
#endif

#ifdef HAIKU_OS
#include "beos/beos_init.h"
#endif

#ifdef QT_UI
#include "qt/qt_init.h"
#endif

#ifdef UWP
using namespace Windows::Storage;
typedef StorageFile^ STOP_FILE;
#else
#include <string.h>
typedef char* STOP_FILE;
#endif
#include "langdetect.h"

void create_window(int argc, char* argv[]);
void display_dialog(char* dialog);
char* get_language_name(STOP_FILE name);
#endif
