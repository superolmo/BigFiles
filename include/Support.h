#pragma once

#include "PluginInterface.h"

/*
Used for:
	StringCchCatW
	StringCchPrntW
*/
#include <strsafe.h>

void msgBox(wchar_t*);
void msgBox_int(wchar_t* str, int v);
void msgBox_hex(wchar_t* str, int v);


