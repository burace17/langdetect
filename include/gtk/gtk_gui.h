#ifndef INCLUDE_GTK_GUI
#define INCLUDE_GTK_GUI
#include <gtk/gtk.h>
#include "langdetect.h"
void _gtk_detect_click(GtkWidget* widget, gpointer data);
void _gtk_create_window(int argc, char* argv[]);
void _gtk_display_dialog(char output[]);
#endif
