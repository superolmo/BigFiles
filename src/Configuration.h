#pragma once

#include "PluginInterface.h"
#include "resource.h"

/*
Used for:
Functions used:
	std::ifstream
*/
#include <fstream>

/*
Used for:
	PathFindExists
*/
#include <shlwapi.h>

class Configuration
{

	// used for messages
	wchar_t strMessage[1000];

	// Default value of page size in bytes
	int default_page_size_bytes = 100000;

	int default_isAdmin_warnings = false;		//If it is 1, it warns the user when Notepad++ is open in Admin mode.

	int default_BinaryFileType_warnings = false;

private:
	// Function gets the directory path where the configuration folder is located
	// This function fills the global variable that is later used for loading and saving configurations
	void _getPluginDirectory(NppData nppData);

	void _getCmdsFromConf(const TCHAR*);

public:
	// Holds the fully qualified file name of the configuration file
	std::wstring confFileNameFull;
	std::wstring confFilePath;

	int ConfFileBufferID = 0;

	// Constructor
	Configuration(NppData);

	void loadConfFile();
	void editConf(NppData nppData);

	// Getters
	int get_default_page_size_bytes();
	bool get_default_isAdmin_warnings();
	bool get_default_BinaryFileType_warnings();
} ;