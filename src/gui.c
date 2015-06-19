/*
 * GUI Abstraction Layer
 */
#include "gui.h"

#ifdef GTK_UI
GtkWidget* input;
GtkWidget* window;
#endif


void display_dialog(char output[]) {
#ifdef GTK_UI
	GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_INFO, GTK_BUTTONS_OK, output, "Results");
	gtk_window_set_title(GTK_WINDOW(dialog), "Results");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
#endif

#ifdef _WIN32
	MessageBox(NULL, output, "Results", MB_ICONINFORMATION);
#endif

#ifdef __APPLE__
	objc_displayDialog(output);
#endif
}

/*
 * GTK USER INTERFACE CODE
 *
 */
#ifdef GTK_UI
void _gtk_detect_click(GtkWidget* widget, gpointer data) {
	GtkTextIter start;
	GtkTextIter end;
	char* text;
	GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(input));
	
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);

	text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
	
	detect_language(text);
	g_free(text);
}
void _gtk_create_window(int argc, char* argv[]) {
	GtkWidget* button;
	GtkWidget* frame;
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Language Detection");
	gtk_window_set_default_size(GTK_WINDOW(window), 480, 325);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	frame = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), frame);


	button = gtk_button_new_with_label("Detect Language");
	gtk_widget_set_size_request(button, 139, 27);
	gtk_fixed_put(GTK_FIXED(frame), button, 167, 290);

	input = gtk_text_view_new();
	gtk_widget_set_size_request(input, 475, 275);
	gtk_fixed_put(GTK_FIXED(frame), input, 5, 0);

	gtk_widget_show_all(window);

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(button, "clicked", G_CALLBACK(_gtk_detect_click), NULL);

	gtk_main();

}
#endif

/*
 * WINDOWS USER INTERFACE CODE
 *
 */

#ifdef _WIN32
#define ID_EDIT 140
#define ID_BUTTON 141
HWND hWndEdit, hWndBtn;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, len;
	char* buffer;
	switch (msg)
	{
	case WM_CREATE:
	{
		// Create edit control for user input.
		hWndEdit = CreateWindow(
			"EDIT",
			NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
			5, 0, 457, 250,
			hwnd,
			(HMENU)ID_EDIT,
			(HINSTANCE)GetWindowLong(hwnd, -6),
			NULL);

		// Create button
		hWndBtn = CreateWindow(
			"BUTTON",
			NULL,
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			171, 260, 139, 27,
			hwnd,
			(HMENU)ID_BUTTON,
			(HINSTANCE)GetWindowLong(hwnd, -6),
			NULL);
		SetDlgItemText(hwnd, ID_BUTTON, "Detect Language");
	}
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case ID_BUTTON:
		{
			len = SendMessage(hWndEdit, WM_GETTEXTLENGTH, 0, 0);
			buffer = malloc(len+1);
			GetDlgItemText(hwnd, ID_EDIT, buffer, len + 1);
			/* jump to platform independent code */
			detect_language(buffer);
			free(buffer);
		}
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}


void win32_create_window() {
	HINSTANCE hInstance = GetModuleHandle(NULL);
		WNDCLASS wc;
	HWND hwnd;
	MSG msg;

	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "LANG";

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, "couldn't register window", "Error", MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	hwnd = CreateWindow("LANG", "Language Detector", WS_OVERLAPPEDWINDOW&~WS_MAXIMIZEBOX&~WS_THICKFRAME,
		CW_USEDEFAULT, 0, 480, 335, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "couldn't create window", "Error", MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}
#endif

void create_window(int argc, char* argv[]) {
#ifdef _WIN32
	win32_create_window();
#endif

#ifdef GTK_UI
	_gtk_create_window(argc, argv);
#endif

#ifdef __APPLE__
	// Cocoa must be initialized with the objective c compiler, so we jump to objc code here.
	// This gets rid of a warning from clang
	const char* cargv = (const char*)argv;
	objc_main(argc, &cargv);
#endif

#ifdef QT_UI
	qt_create_window();
#endif
}
