#pragma once

//
// All difinitions of plugin interface
//
#include "PluginInterface.h"
#include "resource.h"
#include "Configuration.h"

/*
Used for:
Functions used:
	std::ifstream
*/
#include <fstream>

#define PLUGIN_DEFAULT_MESSAGEBOX_TITLE "BigFiles Plugin"

class FileTracker {

	wchar_t filename[500];
	int page_num_current;
	int page_num_max;
	int page_size_bytes;
	size_t file_size_bytes;
	size_t file_size_left;
	bool is_Binary;
	std::streampos sp;	//This is the stream position at the end of the page_size_bytes
	HWND sci_ptr;
	int bufferID;
	wchar_t filetype_name[20];
	char binarySignature[5];

	int bigfile_length;

private:
	bool get_file_stats();

public:
	
	//Constructor
	FileTracker();

	// Open a Bigfile
	void openBigFile(wchar_t[], HWND npp_handle, HWND scintilla_handle, Configuration &);

	// Move backward in the file (page--)
	void move_backward();
	// Move forward in the file (page++)
	void move_forward();
	// Move to the end of the file
	void move_to_end();
	// Move to the start of the file
	void move_to_start();

	// Delete buffer record from list. Triggered by Scintilla tab close
	void closeBufferID(int buffer_ID);

};