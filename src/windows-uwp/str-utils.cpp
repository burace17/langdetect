#include "pch.h"
#include "str-utils.h"
using namespace Platform;

// Converts a CX String to a UTF8 C string
// NOTE: This function allocates memory which must be freed
char* wstr_to_utf8(String^ str) {
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, str->Data(), str->Length(), NULL, 0, NULL, NULL);
	char* utf8 = new char[size_needed + 1];
	WideCharToMultiByte(CP_UTF8, 0, str->Data(), wcslen(str->Data()), utf8, size_needed + 1, NULL, NULL);
	utf8[size_needed] = '\0';
	return utf8;
}

// Array size needed to convert this CX string to UTF8 C string
int wstr_size_needed(String^ str) {
	return WideCharToMultiByte(CP_UTF8, 0, str->Data(), str->Length(), NULL, 0, NULL, NULL) + 1;
}

// Converts a CX string to an already allocated UTF8 C string
void wstr_to_utf8(String^ src, char* dest) {
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, src->Data(), src->Length(), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, src->Data(), wcslen(src->Data()), dest, size_needed + 1, NULL, NULL);
	dest[size_needed] = '\0';
}

// Converts a UTF8 C string to a CX String
String^ utf8_to_wstr(char* utf8) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8, strlen(utf8), NULL, 0);
	wchar_t* wstr = new wchar_t[size_needed + 1];
	MultiByteToWideChar(CP_UTF8, 0, utf8, strlen(utf8), wstr, size_needed);
	wstr[size_needed] = '\0';
	String^ str = ref new String(wstr);
	delete wstr;
	return str;
}