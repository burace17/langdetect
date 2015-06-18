#ifdef GTK_UI
#include <gtk/gtk.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

void create_window(int argc, char* argv[]);
void display_dialog(char* dialog);
