#pragma once
using namespace Platform;

char* wstr_to_utf8(String^ str);
void wstr_to_utf8(String^ src, char* dest);
String^ utf8_to_wstr(char* utf8);
