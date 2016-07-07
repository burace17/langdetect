#ifndef INCLUDE_BEOS_GUI
#define INCLUDE_BEOS_GUI
#include <Window.h>
#include <Button.h>
#include <View.h>
#include <Alert.h>
#include <TextView.h>
#include <FilePanel.h>
#include <Path.h>
#include "langdetect.h"
class MainWindow : public BWindow {
	public:
		MainWindow(void);
		~MainWindow();
		void MessageReceived(BMessage* msg);
		void RefsReceived(BMessage* msg);
		status_t OpenStopFilesDir(BMessage* msg);
	private:
		BTextView* text;
		BFilePanel* open;
};
#endif
