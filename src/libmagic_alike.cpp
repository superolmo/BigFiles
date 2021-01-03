
#include "libmagic_alike.h"

file_type_structure file_types[10];

std::vector<file_type_structure> fts;

unsigned int file_types_length;

bool libmagic_initialized = FALSE;

//void libmagic_init2() {
//	//Add new file type in the vector of file types
//	file_type_structure* file_type = new file_type_structure;
//	file_type->str_pattern = { 0x50, 0x4B, 0x03, 0x04 };
//	file_type->name = TEXT("ZIP");
//	file_type->pattern_length = 4;
//	fts.push_back(*file_type);
//	//delete file_type;
//
//	//Add new file type in the vector of file types
//	//file_type_structure* file_type = new file_type_structure;
//	file_type->str_pattern = { 0x25, 0x50, 0x44, 0x46, 0x2D };
//	file_type->name = TEXT("PDF");
//	file_type->pattern_length = 5;
//	fts.push_back(*file_type);
//	//delete file_type;
//
//	//Add new file type in the vector of file types
//	//file_type_structure* file_type = new file_type_structure;
//	file_type->str_pattern = { 0x4D, 0x5A };
//	file_type->name = TEXT("Executable");
//	file_type->pattern_length = 2;
//	fts.push_back(*file_type);
//	//delete file_type;
//
//	//Add new file type in the vector of file types
//	//file_type_structure* file_type = new file_type_structure;
//	file_type->str_pattern = { 0x00, 0x00, 0x00, 0x18 };
//	file_type->name = TEXT("Video");
//	file_type->pattern_length = 4;
//	fts.push_back(*file_type);
//	delete file_type;
//
//	//Add new file type in the vector of file types
//	//file_type_structure* file_type = new file_type_structure;
//	//file_type->str_pattern = { 0xFF, 0xD8, 0xFF };
//	//file_type->name = TEXT("JPG Image");
//	//file_type->pattern_length = 3;
//	//fts.push_back(*file_type);
//	//delete file_type;
//}
//
//file_type_structure* libmagic_alike2(char binBuf[], unsigned int binBuf_length)
//{
//	char pattern[25];
//
//	if (libmagic_initialized == FALSE) {
//		libmagic_initialize();
//	}
//
//	// TODO: Change integer return value to enum
//
//	// Go througt the whole file type array
//	for (file_type_structure ft_obj : fts) {
//
//#ifdef BIGFILES_DEBUG
//		//Copy string pattern to local char array
//		memcpy(pattern, ft_obj.str_pattern.c_str(), ft_obj.pattern_length);
//		wsprintf(strMessage, TEXT("Type: %ls - Pattern: %02X-%02X-%02X-%02X | =? %02X-%02X-%02X-%02X"),
//			ft_obj.name.c_str(),
//			pattern[0], pattern[1], pattern[2], pattern[3],
//			binBuf[0], binBuf[1], binBuf[2], binBuf[3]
//		);
//		::MessageBox(NULL, strMessage, TEXT("BigFiles Plugin - Debug"), MB_OK);
//#endif
//		// Compare patterns
//		if (memcmp(binBuf, ft_obj.str_pattern.c_str(), ft_obj.pattern_length) == 0)
//		{
//			// If they match stop and return index
//			return &ft_obj;
//		}
//	}
//	return NULL;
//}

/*
Function used to initialize the file type library
Sources:
https://www.file-recovery.com/signatures.htm 
https://en.wikipedia.org/wiki/List_of_file_signatures
*/
void libmagic_initialize() {
	// TODO: Add more file types

	unsigned int i = 0;
	char temp_pattern[20];

	//Load File Types
	memset(temp_pattern, 0x00, 20);
	sprintf(temp_pattern, "%c%c%c%c", 0x50,0x4B,0x03,0x04 );
	file_types[i].name = TEXT("ZIP");
	file_types[i].pattern_length = 4;
	file_types[i].str_pattern = temp_pattern;

	i++;
	memset(temp_pattern, 0x00, 20);
	sprintf(temp_pattern, "%c%c%c%c%c", 0x25, 0x50, 0x44, 0x46, 0x2D);
	file_types[i].name = TEXT("PDF");
	file_types[i].pattern_length = 5;
	file_types[i].str_pattern = temp_pattern;

	i++;
	memset(temp_pattern, 0x00, 20);
	sprintf(temp_pattern, "%c%c", 0x4D, 0x5A);
	file_types[i].name = TEXT("Executable");
	file_types[i].pattern_length = 2;
	file_types[i].str_pattern = temp_pattern;

	i++;
	memset(temp_pattern, 0x00, 20);
	sprintf(temp_pattern, "%c%c%c%c", 0x00, 0x00, 0x00, 0x18);
	file_types[i].name = TEXT("Video");
	file_types[i].pattern_length = 4;
	file_types[i].str_pattern = temp_pattern;

	i++;
	memset(temp_pattern, 0x00, 20);
	sprintf(temp_pattern, "%c%c%c", 0xFF, 0xD8, 0xFF);
	file_types[i].name = TEXT("JPG Image");
	file_types[i].pattern_length = 3;
	file_types[i].str_pattern = temp_pattern;

	file_types_length = i+1;
}


/*
 Function match the 4 character bytes patterns to a list of file types patterns. If found, it returns the index.
 Inputs : array of character of length 4
 Returns: pointer to file structure of the corresponding file type
			-1 if not found
*/
file_type_structure* libmagic_alike(char binBuf[], unsigned int signature_max_size)
{
	char pattern[20];
	wchar_t strMessage[1500];
	std::wstring strM;

	if (libmagic_initialized == FALSE) {
		libmagic_initialize();
	}

	// TODO: Change integer return value to enum

	// Go througt the whole file type array
	for (int j = 0; j < file_types_length; j++) {
		
		//Copy string pattern to local char array
		memcpy(pattern, file_types[j].str_pattern.c_str(), file_types[j].pattern_length);
		
#ifdef DEBUG_LIBMAGIC

		// Write pattern against file header
		wsprintf(strMessage, TEXT("%d - Type: %ls\n"), j, file_types[j].name.c_str() );
		strM.append(strMessage);
		for (int i = 0; i < file_types[j].pattern_length; i++) {
			wsprintf(strMessage, TEXT("%02X -- %02X\n"), pattern[i], binBuf[i]);
			strM.append(strMessage);
		}
		
		//binBuf[0], binBuf[1], binBuf[2], binBuf[3], binBuf[4]
		::MessageBox(NULL, strM.c_str(), TEXT("BigFiles Plugin - Debug"), MB_OK);
		strM.clear();

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

