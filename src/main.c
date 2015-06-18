/* Make main the default entry point on windows */
#ifdef _MSC_VER
	#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif
#include <string.h>
#include <math.h>
#include "gui.h"

int main(int argc, char* argv[]) {
	// Create window
	create_window(argc, argv);
}
