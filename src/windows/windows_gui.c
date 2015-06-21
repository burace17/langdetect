#include "windows/windows_gui.h"
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

