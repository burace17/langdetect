#include "beos/beos_gui.h"

enum {
	M_BUTTON_CLICKED = 'btcl',
	M_SELECT_CLICKED = 'btc2',
};

MainWindow::MainWindow(void) : BWindow(BRect(100, 100, 480, 350), "Language Detector", B_TITLED_WINDOW,
																B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{
	BButton* button = new BButton(BRect(125, 185, 11, 11), "button", "Detect Language", new 
BMessage(M_BUTTON_CLICKED));
	button->ResizeToPreferred();
	AddChild(button);
	
	BButton* selectfolder = new BButton(BRect(100, 215, 11, 11), "selectfolder", "Select Stop Files 
Directory", new BMessage(M_SELECT_CLICKED));
	selectfolder->ResizeToPreferred();
	AddChild(selectfolder);

	this->text = new BTextView(BRect(10, 10, 350, 150), "input", BRect(0, 0, 345, 150), B_FOLLOW_LEFT_RIGHT |
				B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
	AddChild(text); 
}

MainWindow::~MainWindow() 
{
	if (this->open != NULL)
		delete this->open;
}
void MainWindow::MessageReceived(BMessage* msg) {
	const char* buffer;
	char* tmp;
	switch (msg->what) {
		case M_BUTTON_CLICKED:
			{
				buffer = text->Text();
				tmp = new char[strlen(buffer)+1];
				strncpy(tmp, buffer, strlen(buffer)+1);
				detect_language(tmp);
				delete tmp;
				break;
			}
		case M_SELECT_CLICKED:
			{
				if (this->open != NULL)
					delete this->open;
				this->open = new BFilePanel(B_OPEN_PANEL, NULL, NULL,
							B_DIRECTORY_NODE, false, NULL,
							NULL, false, true);
				this->open->Show();
				break;
			}
		default:
			{
				BWindow::MessageReceived(msg);
				break;
			}
	}
}
