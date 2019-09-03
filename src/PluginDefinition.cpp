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

#include "PluginDefinition.h"
#include "menuCmdID.h"

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

// Define the Icon Resources for the Toolbar
toolbarIcons * left_icon = new toolbarIcons;
toolbarIcons * right_icon = new toolbarIcons;

// Define the global variables for this plugin
struct bigfile_struct {
	wchar_t filename[500];
	int page_num_current;
	int page_num_max;
	int page_size_bytes;
	size_t file_size_bytes;
	bool is_Binary;
	std::streampos sp;
	HWND sci_ptr;
	int bufferID;
} bigfile[10];

int bigfile_length;
ShortcutKey * AltLeftKey = new ShortcutKey;
ShortcutKey * AltRightKey = new ShortcutKey;
wchar_t filename_temp[500];
wchar_t strMessage[1000];
char binaryBuffer[5];

struct file_type_structure {
	std::wstring name;
	std::string str_pattern;
	int pattern_length;
	bool is_Binary;
} file_types[2];

int file_types_length = 2;

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE global_npp_handle /*hModule*/)
{
	left_icon->hToolbarBmp = (HBITMAP)::LoadImage((HINSTANCE)global_npp_handle, MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, (LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS));;
	right_icon->hToolbarBmp = (HBITMAP)::LoadImage((HINSTANCE)global_npp_handle,MAKEINTRESOURCE(IDB_BITMAP2),IMAGE_BITMAP, 0, 0, (LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS));
	
	AltLeftKey->_isAlt = false;
	AltLeftKey->_isCtrl = true;
	AltLeftKey->_isShift = true;
	AltRightKey->_isAlt = false;
	AltRightKey->_isCtrl = true;
	AltRightKey->_isShift = true;
	AltLeftKey->_key = VK_LEFT;
	AltRightKey->_key = VK_RIGHT;

	bigfile_length = -1;

	// TODO: Add more file types
	//Load File Types
	char temp_pattern[] = { 0x50,0x4B,0x03,0x04 };
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

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
	// This happens when Notepad++ close the main window
	delete AltLeftKey, AltRightKey;
	delete [] &bigfile;
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );

	setCommand(0, TEXT("Open BigFile"), openBigFile, NULL, false);
	setCommand(1, TEXT("Backward"), move_backward, AltLeftKey, false);
	setCommand(2, TEXT("Forward"), move_forward, AltRightKey, false);

}

void commandRegToolbarIcons() {
	//Add icons in the toolbar
	::SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[1]._cmdID, (LPARAM)(left_icon));
	::SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[2]._cmdID, (LPARAM)(right_icon));
};

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// This cleanup happens before PlugInCleanUp, but still only happens when we close Notepad++

	// Don't forget to deallocate your shortcut here
	delete AltLeftKey, AltRightKey;
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

int getFileName()
{
	int ret = 0;	//Cancelled

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;
		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pFileOpen->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						// Initialize bigfile structure
						wcsncpy(filename_temp, pszFilePath, 500);

						//std::strcpy(my_filename, &pszFilePath);
						CoTaskMemFree(pszFilePath);
						ret = 1;
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	return ret;
}

HWND getCurrentHScintilla()
{
	int which;
	SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, reinterpret_cast<LPARAM>(&which));
	return (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//


// Find the buffer ID that closed, if I have it in my list, delete it from the list.
// This function is called from NppPluginDemo.cpp notification switch
void closeBufferID(int buffer_ID) {
	for (int i = 0; i <= bigfile_length; i++) {
		if (bigfile[i].bufferID == buffer_ID) {
			//Copy the last bigfile record to the location where I found the closed buffer ID.
			bigfile[i].bufferID = bigfile[bigfile_length].bufferID;
			wcsncpy(bigfile[i].filename, bigfile[bigfile_length].filename,500);
			bigfile[i].file_size_bytes = bigfile[bigfile_length].file_size_bytes;
			bigfile[i].is_Binary = bigfile[bigfile_length].is_Binary;
			bigfile[i].page_num_current = bigfile[bigfile_length].page_num_current;
			bigfile[i].page_num_max = bigfile[bigfile_length].page_num_max;
			bigfile[i].page_size_bytes = bigfile[bigfile_length].page_size_bytes;
			bigfile[i].sci_ptr = bigfile[bigfile_length].sci_ptr;
			bigfile[i].sp = bigfile[bigfile_length].sp;

			//Reduce the 
			bigfile_length--;
		}
	}
}

void openBigFile()
{
	// TODO: Add a notification that the Buffer has been deleted
	if (bigfile_length == 9) {
		::MessageBox(NULL, TEXT("Too Many BigFiles Open"), TEXT("BigFiles Plugin"), MB_OK);
		return;
	}

	// Get the file name from the Windows Dialog Box
	if (getFileName())
	{
		bigfile_length++;
		wcsncpy(bigfile[bigfile_length].filename, filename_temp, 500);
		//wcstombs(bigfile[bigfile_index].filename, filename_temp, 500);
		bigfile[bigfile_length].page_num_current = 0;
		bigfile[bigfile_length].page_num_max = 0;
		// TODO: Make page_size_bytes a setting
		bigfile[bigfile_length].page_size_bytes = 100000;
		bigfile[bigfile_length].file_size_bytes = 0;
		bigfile[bigfile_length].is_Binary = false;

		// Open a new document
		::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);
		bigfile[bigfile_length].sci_ptr = getCurrentHScintilla();
		bigfile[bigfile_length].bufferID = (int)::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID,0,0);

		// Open Big File, get first 100k bytes, and keep track of the file pointer

		// Get the file size to calculate the number of pages
		std::ifstream mybigfile_size(bigfile[bigfile_length].filename, std::ios::binary);
		mybigfile_size.seekg(0, mybigfile_size.end);
		bigfile[bigfile_length].file_size_bytes = mybigfile_size.tellg();
		mybigfile_size.seekg(0, mybigfile_size.beg);
		
		//Compute number of pages based on page size
		// TODO: Fix calculation of maximum number of pages
		bigfile[bigfile_length].page_num_max = (int)(bigfile[bigfile_length].file_size_bytes / (size_t)bigfile[bigfile_length].page_size_bytes);
		// Find if there are any null characters in the first 1000 bytes. If yes, mark file as binary
		
		//TODO: Check that file is not less than 10 bytes
		// Get first 4 bytes of file
		mybigfile_size.read(binaryBuffer, 4);
		std::string binaryBuf(binaryBuffer);

		switch (libmagic_alike(binaryBuffer))
		{
		case 0:
			bigfile[bigfile_length].is_Binary = true;
			::MessageBox(NULL, file_types[0].name.c_str(), TEXT("BigFiles Plugin"), MB_OK);
			break;
		case 1:
			bigfile[bigfile_length].is_Binary = true;
			::MessageBox(NULL, file_types[1].name.c_str(), TEXT("BigFiles Plugin"), MB_OK);
			break;
		default:
			break;
		} 

		mybigfile_size.close();

		// For debug maybe show the structure
		// TODO: Add Debug option in Settings
		//StringCbPrintf(strBuffer, 500, TEXT("%llu / %d = %d"), bigfile.file_size_bytes, bigfile.page_size_bytes, bigfile.page_num_max);
		//::MessageBox(NULL, strBuffer, TEXT("BigFiles Plugin"), MB_OK);

		// TODO: Add PageUp and PageDown button association to read different page of file

		// Read Only Mode
		bigfile[bigfile_length].page_num_current = 0;
		std::ifstream myfile(bigfile[bigfile_length].filename, std::ios::in);
		std::string string_buffer(bigfile[bigfile_length].page_size_bytes, '\0');
		LPSTR pst = &string_buffer[0];
		myfile.read(&string_buffer[0], bigfile[bigfile_length].page_size_bytes);
		

		//Save current streampos
		//bigfile.stream_offset = myfile.tellg();
		bigfile[bigfile_length].sp = myfile.tellg();

		myfile.close();

		//TODO: Change Scintilla Tab Name

		// Scintilla control has no Unicode mode, so we use (char *) here
		::SendMessage(bigfile[bigfile_length].sci_ptr, SCI_SETTEXT, 0, (LPARAM)pst);
#ifdef BIGFILES_DEBUG
		showDebug();
#endif	
	}
	else
		// Message users that operation has been cancelled at the dialog box
		::MessageBox(NULL, TEXT("Cancelled"), TEXT("BigFiles Plugin"), MB_OK);
}

int getBigFileRecord(int buffer_id)
{
	for (int i = 0; i <= bigfile_length; i++) {
		if (bigfile[i].bufferID == buffer_id)
			return i;
	}
	return -1;
}

// Function get a new page before the current file position.
// CTRL+SHIFT+LEFT_ARROW
void move_backward()
{
	// Retrieve current visible buffer record
	int current_bigfile_index = getBigFileRecord((int)::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0));

	if (current_bigfile_index > -1) {
		// If we still have more pages to go, read data, otherwise move Scintilla to first character
		if (bigfile[current_bigfile_index].page_num_current > 0) {

			bigfile[current_bigfile_index].page_num_current--;

			// Read Only Mode
			std::ifstream myfile(bigfile[current_bigfile_index].filename, std::ios::in);
			std::string string_buffer(bigfile[current_bigfile_index].page_size_bytes, '\0');
			LPSTR pst = &string_buffer[0];
			myfile.seekg(bigfile[current_bigfile_index].page_num_current*bigfile[current_bigfile_index].page_size_bytes, std::ios::beg);
			myfile.read(&string_buffer[0], bigfile[current_bigfile_index].page_size_bytes);

			//Save current streampos
			bigfile[current_bigfile_index].sp = myfile.tellg();

			myfile.close();

			// Scintilla control has no Unicode mode, so we use (char *) here
			::SendMessage(bigfile[current_bigfile_index].sci_ptr, SCI_SETTEXT, 0, (LPARAM)pst);
#ifdef BIGFILES_DEBUG
			showDebug();
#endif	
		}
		else {
			// Move Scintilla to first character
			// MAYBE NOT NECESSARY
		}
	}
	else {
		// Current Buffer ID not found

	}
}

// Function get a new page ahead of the current file position.
// CTRL+SHIFT+RIGHT_ARROW
void move_forward()
{
	// Retrieve current visible buffer record
	int current_bigfile_index = getBigFileRecord((int)::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0));

	if (current_bigfile_index > -1) {

		// If we still have more pages to go, read data, otherwise move Scintilla to last character
		if (bigfile[current_bigfile_index].page_num_current < bigfile[current_bigfile_index].page_num_max) {
			// Read Only Mode
			std::ifstream myfile(bigfile[current_bigfile_index].filename, std::ios::in);
			std::string string_buffer(bigfile[current_bigfile_index].page_size_bytes, '\0');
			LPSTR pst = &string_buffer[0];
			myfile.seekg(bigfile[current_bigfile_index].sp);
			myfile.read(&string_buffer[0], bigfile[current_bigfile_index].page_size_bytes);

			bigfile[current_bigfile_index].page_num_current++;

			// TODO: Trunkate to last line feed and/or carriage return (\n and/or \r)
			//string_buffer.find_last_of('\n');

			//Save current streampos
			bigfile[current_bigfile_index].sp = myfile.tellg();

			myfile.close();

			// Scintilla control has no Unicode mode, so we use (char *) here
			::SendMessage(bigfile[current_bigfile_index].sci_ptr, SCI_SETTEXT, 0, (LPARAM)pst);
#ifdef BIGFILES_DEBUG
			showDebug();
#endif	
		}
		else {
			// Move Scintilla to last character
			int lines = (int)::SendMessage(bigfile[current_bigfile_index].sci_ptr, SCI_GETLINECOUNT, 0, 0) - 1;
			::SendMessage(bigfile[current_bigfile_index].sci_ptr, SCI_GOTOLINE, lines, 0);
		}
	}
	else {
		// Current Buffer ID not found

	}
}

int libmagic_alike(char binBuf[4])
{
	char pattern[5];

	for (int j = 0; j < file_types_length; j++) {

#ifdef BIGFILES_DEBUG
		//Copy string pattern to local char array
		memcpy(pattern, file_types[j].str_pattern.c_str(), file_types[j].pattern_length);
		wsprintf(strMessage, TEXT("%d - Type: %ls - Pattern: %02X-%02X-%02X-%02X | =? %02X-%02X-%02X-%02X"),
			j, 
			file_types[j].name.c_str(),
			pattern[0], pattern[1], pattern[2], pattern[3],
			binBuf[0], binBuf[1], binBuf[2], binBuf[3]
		);
		::MessageBox(NULL, strMessage, TEXT("BigFiles Debug"), MB_OK);
#endif

		if (memcmp(binBuf, file_types[j].str_pattern.c_str(), file_types[j].pattern_length) == 0)
		{
			return j;
		}
	}
	return -1;
}

void showDebug()
{
	wchar_t str_buf[1000];
	wchar_t bf_str[10][500];
	wsprintf(strMessage, TEXT("Length = %d\nCurrent BufferID=%d\n\n"), bigfile_length, ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0));
	for (int i = 0; i <= bigfile_length; i++) {
		wsprintf(bf_str[0], TEXT("FileName = %s\nBufferID = %d\nPage Number Current = %d\nPage Number Max = %d\n\n"), bigfile[i].filename, bigfile[i].bufferID, bigfile[i].page_num_current, bigfile[i].page_num_max);
		wcscat(strMessage, bf_str[0]);
	}

	::MessageBox(NULL, strMessage, TEXT("BigFiles Plugin - Debug"), MB_OK);
}

