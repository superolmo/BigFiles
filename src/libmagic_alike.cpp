
#include "libmagic_alike.h"

#define FILE_TYPES_LENGTH 2

file_type_structure file_types[FILE_TYPES_LENGTH];
bool libmagic_initialized = FALSE;

/*
Function used to initialize the file type library
*/
void libmagic_initialize() {
	// TODO: Add more file types

	//Load File Types
	char temp_pattern[5] = { 0x50,0x4B,0x03,0x04 };
	file_types[0].name = TEXT("Zip file");
	file_types[0].pattern_length = 4;
	file_types[0].str_pattern = temp_pattern;
	file_types[0].is_Binary = true;

	sprintf(temp_pattern, "%c%c%c%c", 0x25, 0x50, 0x44, 0x46);
	file_types[1].name = TEXT("PDF file");
	file_types[1].pattern_length = 4;
	file_types[1].str_pattern = temp_pattern;
	file_types[1].is_Binary = true;

}

/*
 Function match the 4 character bytes patterns to a list of file types patterns. If found, it returns the index.
 Inputs : array of character of length 4
 Returns: pointer to file structure of the corresponding file type
			-1 if not found
*/
file_type_structure* libmagic_alike(char binBuf[4])
{
	char pattern[5];

	if (libmagic_initialized == FALSE) {
		libmagic_initialize();
	}

	// TODO: Change integer return value to enum

	// Go througt the whole file type array
	for (int j = 0; j < FILE_TYPES_LENGTH; j++) {

#ifdef BIGFILES_DEBUG
		//Copy string pattern to local char array
		memcpy(pattern, file_types[j].str_pattern.c_str(), file_types[j].pattern_length);
		wsprintf(strMessage, TEXT("%d - Type: %ls - Pattern: %02X-%02X-%02X-%02X | =? %02X-%02X-%02X-%02X"),
			j,
			file_types[j].name.c_str(),
			pattern[0], pattern[1], pattern[2], pattern[3],
			binBuf[0], binBuf[1], binBuf[2], binBuf[3]
		);
		::MessageBox(NULL, strMessage, TEXT("BigFiles Plugin - Debug"), MB_OK);
#endif
		// Compare patterns
		if (memcmp(binBuf, file_types[j].str_pattern.c_str(), file_types[j].pattern_length) == 0)
		{
			// If they match stop and return index
			return &file_types[j];
		}
	}
	return NULL;
}