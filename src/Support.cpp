#include "Support.h"

void msgBox_int(wchar_t* str, int v) {
	wchar_t strMessage[500];
	size_t cbDest = 500 * sizeof(wchar_t);
	StringCchPrintfW(strMessage, cbDest, TEXT("%s: %d"), str, v);
	//wsprintf(strMessage, TEXT("%s: %d"), str, v);
	::MessageBox(NULL, strMessage, TEXT("BigFiles Plugin - Debug"), MB_OK);
}

void msgBox_hex(wchar_t* str, char v) {
	wchar_t strMessage[500];
	size_t cbDest = 500 * sizeof(wchar_t);
	StringCchPrintfW(strMessage, cbDest, TEXT("%s: %02X"), str, v);
	//wsprintf(strMessage, TEXT("%s: %d"), str, v);
	::MessageBox(NULL, strMessage, TEXT("BigFiles Plugin - Debug"), MB_OK);
}

void msgBox(wchar_t* str) {
	wchar_t strMessage[500];
	size_t cbDest = 500 * sizeof(wchar_t);
	StringCchPrintfW(strMessage, cbDest, TEXT("%s"), str);
	//wsprintf(strMessage, TEXT("%s"), str);
	::MessageBox(NULL, strMessage, TEXT("BigFiles Plugin - Debug"), MB_OK);
}

