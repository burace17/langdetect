#ifndef INCLUDE_GUI
#define INCLUDE_GUI
#ifdef GTK_UI
#include <gtk/gtk.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <stdlib.h>
#endif

#ifdef __APPLE__
#include "osx/init_osx.h"
#include "osx/gui_osx.h"
#endif

#include "langdetect.h"

void create_window(int argc, char* argv[]);
void display_dialog(char* dialog);
#endif
