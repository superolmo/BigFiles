//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef PLUGINDEFINITION_H
#define PLUGINDEFINITION_H

//
// All difinitions of plugin interface
//
#include "PluginInterface.h"
#include "resource.h"

/*
Used for:
	libmagic_alike
*/

#include "libmagic_alike.h"

/*
Used for:
Functions used:
	std::ifstream
*/
#include <fstream>

/*
Used for opening the Open File Dialog
Functions used:
	IFileOpenDialog
	IID_IFileOpenDialog
	CLSID_FileOpenDialog
	IShellItem
	SIGDN_FILESYSPATH
*/
#include <shobjidl.h> 

/*
Used for:
	wsprint
	StringCchCatW
	StringCchPrntW
*/
#include <strsafe.h>

/*
Used for Message Boxes, Debug Window
Functions used:
	msgBox
	msgBox_int
	showDebug
*/
#include "Support.h"

/*
Used for reading and writing to configuration file
Function used:
	loadConfFile
	editConf
*/
#include "Configuration.h"

#include "FileTracker.h"
#include <vector>


//#define BIGFILES_DEBUG
#define DEBUG_LIBMAGIC

#define PLUGIN_NAME "BigFiles"
#define PLUGIN_NUMBER_OF_FUNCTIONS 7
#define PLUGIN_DEFAULT_MESSAGEBOX_TITLE "BigFiles Plugin"

//-------- START SETTINGS --------
//
// Here define your plugin name
//
const TCHAR NPP_PLUGIN_NAME[] = TEXT(PLUGIN_NAME);

//
// Here define the number of your plugin commands
//
const int nbFunc = PLUGIN_NUMBER_OF_FUNCTIONS;

//-------- END SETTINGS --------

INT_PTR CALLBACK ConfigurationDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

//--------- START LOADING AND UNLOADING FUNCTIONS ----------------
//
// Initialization of your plugin data
// It will be called while plugin loading
//
void pluginInit(HANDLE hModule);

//
// Cleaning of your plugin
// It will be called while plugin unloading
//
void pluginCleanUp();

//
//Initialization of your plugin commands
//
void commandMenuInit();

//
//Clean up your plugin commands allocation (if any)
//
void commandMenuCleanUp();

// Register toolbar icons
void commandRegToolbarIcons();

//
// Function which sets your command 
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk = NULL, bool check0nInit = false);

//---------- END LOADING AND UNLOADING FUNCTIONS ----------------


// --------- START PLUGIN FUNCTIONS -----------
//
// Your plugin command functions
//

struct bigfile_struct {
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
};

void openConfigFile();

// Opens the file dialog window to select the file name to open
int getFileName();
//bool get_file_stats(bigfile_struct*);
// Open a Bigfile
//void openBigFile();
// Get the handle of Scintilla editor
//HWND getCurrentHScintilla();
// Copy page_size_bytes to SCintilla tab
//void updateBuffer(int record_index);
// Move backward in the file (page--)
//void move_backward();
// Move forward in the file (page++)
//void move_forward();
// Move to the end of the file
//void move_to_end();
// Move to the start of the file
//void move_to_start();

// Retrieve current visible buffer record index
//int getBigFileRecordIndex(int buffer_id);
// Delete buffer record from list. Triggered by Scintilla tab close
//void closeBufferID(int buffer_ID);

// Function updates the DOC_TYPE status bar field
//void updateStatusBar(int record_index);


// Check if the current user has admin rights
BOOL IsUserAdmin(VOID);

// Open the Debug window
void showDebug();

// --------- END PLUGIN FUNCTIONS -----------

#endif //PLUGINDEFINITION_H
