#ifndef INCLUDE_BEOS_GUI
#define INCLUDE_BEOS_GUI
#include <Window.h>
#include <Button.h>
#include <View.h>
#include <Alert.h>
#include <TextView.h>
#include "langdetect.h"
class MainWindow : public BWindow {
	public:
		MainWindow(void);
		void MessageReceived(BMessage* msg);
	private:
		BTextView* text;
};
#endif
