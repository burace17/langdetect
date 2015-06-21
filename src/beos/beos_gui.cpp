#include "beos/beos_gui.h"

#include <Button.h>
#include <View.h>
#include <String.h>
#include <Alert.h>
#include <TextView.h>
enum {
	M_BUTTON_CLICKED = 'btcl'
};

MainWindow::MainWindow(void) : BWindow(BRect(100, 100, 480, 325), "Language Detector", B_TITLED_WINDOW,
																B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{
	BButton* button = new BButton(BRect(125, 185, 11, 11), "button", "Detect Language", new 
BMessage(M_BUTTON_CLICKED));
	button->ResizeToPreferred();
	AddChild(button);
	
	this->text = new BTextView(BRect(10, 10, 350, 150), "input", BRect(0, 0, 345, 150), B_FOLLOW_LEFT_RIGHT |
				B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
	AddChild(text); 
}

void MainWindow::MessageReceived(BMessage* msg) {
	const char* buffer;
	switch (msg->what) {
		case M_BUTTON_CLICKED:
			{
				//buffer = new char[text->TextLength()];
				buffer = text->Text();
				//text->GetText(0, sizeof(buffer), buffer);
				detect_language(buffer);
				text->SetText("");
				break;
			}
		default:
			{
				BWindow::MessageReceived(msg);
				break;
			}
	}
}
