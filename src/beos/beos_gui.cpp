#include "beos/beos_gui.h"

enum {
	M_BUTTON_CLICKED = 'btcl',
	M_SELECT_CLICKED = 'btc2',
	M_OPENED = 'btc3'
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
				tmp = new char[sizeof(buffer)];
				strncpy(tmp, buffer, sizeof(buffer));
				detect_language(tmp);
				text->SetText("");
				break;
			}
		case M_SELECT_CLICKED:
			{
				BMessenger messenger(this);
				this->open = new BFilePanel(B_OPEN_PANEL, &messenger, NULL,
							B_DIRECTORY_NODE, false, new BMessage(M_OPENED),
							NULL, false, true);
				this->open->SetTarget(this);
				this->open->Show();
				break;
			}
		case M_OPENED:
			{
				printf("test");
				break;
			}
		default:
			{
				BWindow::MessageReceived(msg);
				break;
			}
	}
}

void MainWindow::RefsReceived(BMessage* msg)
{
	printf("ref received\n");
	switch (msg->what)
	{
		case B_REFS_RECEIVED:
			{
				OpenStopFilesDir(msg);
				break;
			}
	}
}

status_t MainWindow::OpenStopFilesDir(BMessage* msg)
{
	entry_ref ref;
	const char* name;
	BPath path;
	BEntry entry;
	status_t err = B_OK;

	printf("in method\n");
	
	if (err = msg->FindRef("directory", &ref) != B_OK)
		return err;

	if (err = msg->FindString("name", &name) != B_OK)
		return err;

	if (err = entry.SetTo(&ref) != B_OK) 
		return err;

	entry.GetPath(&path);
	path.Append(name);
	
	printf("%s\n", path.Path());

	return B_OK;
}
