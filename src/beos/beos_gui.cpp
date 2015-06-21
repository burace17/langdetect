#include "beos/beos_gui.h"

#include <Button.h>
#include <View.h>
#include <String.h>

enum {
	M_BUTTON_CLICKED = 'btcl'
};

MainWindow::MainWindow(void) : BWindow(BRect(100, 100, 300, 200), "Language Detector", B_TITLED_WINDOW,
																B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{
	BButton* button = new BButton(BRect(10, 10, 11, 11), "button", "Detect Language", new BMessage(M_BUTTON_CLICKED));
	button->ResizeToPreferred();
	AddChild(button);
}

void MainWindow::MessageReceived(BMessage* msg) {
	switch (msg->what) {
		case M_BUTTON_CLICKED:
			{
				BAlert* alert = new BAlert("Results", "Testing", "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
				alert->Go();
				delete alert;
				break;
			}
		default:
			{
				BWindow::MessageReceived(msg);
				break;
			}
	}
}
