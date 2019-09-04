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
#include <iostream>
#include <fstream>
#include <shobjidl.h> 
#include <strsafe.h>

//#define BIGFILES_DEBUG
#define PLUGIN_NAME "BigFiles"
#define PLUGIN_NUMBER_OF_FUNCTIONS 3
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

// Opens the file dialog window to select the file name to open
int getFileName();
// Open a Bigfile
void openBigFile();
// Get the handle of Scintilla editor
HWND getCurrentHScintilla();
// Copy page_size_bytes to SCintilla tab
void updateBuffer(int record_index);
// Move backward in the file (page--)
void move_backward();
// Move forward in the file (page++)
void move_forward();
// Retrieve current visible buffer record index
int getBigFileRecordIndex(int buffer_id);
// Delete buffer record from list. Triggered by Scintilla tab close
void closeBufferID(int buffer_ID);
// Open the Debug window
void showDebug();

// Check the type of file based on the first 4 binary char
// TODO: Is it worth to import the libmagic library or better to keep it simple and just add simple checking?
int libmagic_alike(char binBuf[4]);

// Check if the current user has admin rights
BOOL IsUserAdmin(VOID);

// Function updates the DOC_TYPE status bar field
void updateStatusBar(int record_index);
// --------- END PLUGIN FUNCTIONS -----------

#endif //PLUGINDEFINITION_H
