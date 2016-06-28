#ifndef INCLUDE_GUI
#define INCLUDE_GUI
#ifdef GTK_UI
#include "gtk/gtk_gui.h"
#endif

#if defined(_WIN32) && !defined(UWP)
#include "windows/windows_gui.h"
#endif

#ifdef __APPLE__
#include "osx/osx_init.h"
#include "osx/osx_gui.h"
#endif

#ifdef HAIKU_OS
#include "beos/beos_init.h"
#endif
#include "langdetect.h"

void create_window(int argc, char* argv[]);
void display_dialog(char* dialog);
#endif
