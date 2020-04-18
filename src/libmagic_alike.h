#pragma once

#include "PluginInterface.h"
#include "resource.h"
#include <string>
#include <strsafe.h>

struct file_type_structure {
	std::wstring name;
	std::string str_pattern;
	int pattern_length;
	bool is_Binary;
};

/*
Function used to initialize the file type library
*/
void libmagic_initialize();

/*
Function check the type of a file based on the first 4 binary char, similar to what libmagic does.
TODO: Is it worth to import the libmagic library or better to keep it simple and just add simple checking?
*/
file_type_structure* libmagic_alike(char binBuf[4]);
