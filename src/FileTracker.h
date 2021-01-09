#pragma once

//
// All difinitions of plugin interface
//
#include "PluginInterface.h"
#include "resource.h"
#include "Configuration.h"
#include "menuCmdID.h"
/*
Used for:
Functions used:
	std::ifstream
*/
#include <fstream>

/*
Used for:
	wsprint
	StringCchCatW
	StringCchPrntW
*/
#include <strsafe.h>

#define MAX_SIGNATURE_BYTES 10

#include "libmagic_alike.h"

class FileTracker {

public:
	bool get_file_stats(bool first_time_call);

	// File properties
	wchar_t filename[500];
	size_t filename_size = 500 * sizeof(wchar_t);

	int page_num_current;
	int page_num_max;
	int page_size_bytes;
	size_t file_size_bytes;
	size_t file_size_left;
	bool is_Binary;
	std::streampos sp;	//This is the stream position at the end of the page_size_bytes

	int bufferID;
	wchar_t filetype_name[20];

	// Binary Signature
	char binarySignature[MAX_SIGNATURE_BYTES];
	std::wstring *binarySignatureName;

	// Handles to communicate with Scintilla and NPP
	HWND scintilla_handle;
	HWND npp_handle;

	//Constructor
	FileTracker(HWND npp_handle, HWND scintilla_handle);

	// Open a Bigfile
	void openBigFile(const wchar_t[], Configuration&);

	// Move backward in the file (page--)
	bool move_backward();
	// Move forward in the file (page++)
	bool move_forward();
	// Move to the end of the file
	bool move_to_end();
	// Move to the start of the file
	bool move_to_start();

	void updateBuffer(bool first_time_call);

	unsigned int find_EndOfLine(const std::string data);
};