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
toolbarIcons* left_icon = new toolbarIcons;
toolbarIcons* right_icon = new toolbarIcons;
toolbarIcons* open_icon = new toolbarIcons;
toolbarIcons* start_icon = new toolbarIcons;
toolbarIcons* end_icon = new toolbarIcons;

// Define the global variables for this plugin
bigfile_struct bigfile[10];

int bigfile_length;
ShortcutKey* AltLeftKey = new ShortcutKey;
ShortcutKey* AltRightKey = new ShortcutKey;
ShortcutKey* AltUpKey = new ShortcutKey;
ShortcutKey* AltDownKey = new ShortcutKey;
wchar_t filename_temp[500];

Configuration* bigfiles_config;

extern void updateStatusBar3(FileTracker*);
extern void closeBufferID3(int buffer_ID);
extern int getBigFileRecordIndex3(int buffer_id);
extern void move_to_start3();
extern void move_to_end3();
extern void move_forward3();
extern void move_backward3();
extern void openBigFile3(const wchar_t filename[]);
extern void openBigFileDlg3();
extern void cleanupv3();

extern int ft_length;

extern std::vector<FileTracker> ftv;

HANDLE global_npp_handle;

//--------- START LOADING AND UNLOADING FUNCTIONS ----------------

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE g_npp_handle /*hModule*/)
{
	global_npp_handle = g_npp_handle;

	left_icon->hToolbarBmp = (HBITMAP)::LoadImage((HINSTANCE)g_npp_handle, MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, (LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS));
	right_icon->hToolbarBmp = (HBITMAP)::LoadImage((HINSTANCE)g_npp_handle,MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 0, 0, (LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS));
	open_icon->hToolbarBmp = (HBITMAP)::LoadImage((HINSTANCE)g_npp_handle, MAKEINTRESOURCE(IDB_BITMAP3), IMAGE_BITMAP, 0, 0, (LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS));
	start_icon->hToolbarBmp = (HBITMAP)::LoadImage((HINSTANCE)g_npp_handle, MAKEINTRESOURCE(IDB_BITMAP5), IMAGE_BITMAP, 0, 0, (LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS));
	end_icon->hToolbarBmp = (HBITMAP)::LoadImage((HINSTANCE)g_npp_handle, MAKEINTRESOURCE(IDB_BITMAP4), IMAGE_BITMAP, 0, 0, (LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS));

	//This section below can go to command menu init and get modified by configuration file
	AltLeftKey->_isAlt = true;
	AltLeftKey->_isCtrl = false;
	AltLeftKey->_isShift = false;
	AltLeftKey->_key = VK_LEFT; //VK_PRIOR;

	AltRightKey->_isAlt = true;
	AltRightKey->_isCtrl = false;
	AltRightKey->_isShift = false;
	AltRightKey->_key = VK_RIGHT; //VK_NEXT;

	AltUpKey->_isAlt = true;
	AltUpKey->_isCtrl = false;
	AltUpKey->_isShift = false;
	AltUpKey->_key = VK_UP;

	AltDownKey->_isAlt = true;
	AltDownKey->_isCtrl = false;
	AltDownKey->_isShift = false;
	AltDownKey->_key = VK_DOWN;

	bigfile_length = -1;
 
}


//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
	// This happens when Notepad++ close the main window
	delete bigfiles_config;
	cleanupv3();
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

	setCommand(0, TEXT("Open BigFile"), openBigFileDlg3, NULL, false);
	setCommand(1, TEXT("Backward"), move_backward3, AltLeftKey, false);
	setCommand(2, TEXT("Forward"), move_forward3, AltRightKey, false);

	setCommand(3, TEXT("Move to Start"), move_to_start3, AltUpKey, false);
	setCommand(4, TEXT("Move to End"), move_to_end3, AltDownKey, false);

	// Separator
	setCommand(5, TEXT("---"), NULL, NULL, false);
	
	setCommand(6, TEXT("Change Config"), openConfigFile, NULL, false);
	// TODO: New Features
	/*
	// Search for a string in a big file, move page automatically
	setCommand(7, TEXT("Search"), search_BigFile, NULL, false);
	// Open a sequence of files as if it is a single big file, this is used for searching
	setCommand(8, TEXT("Open BigFile Sequence"), open_BigFile_sequence, NULL, false);
	*/

	//Get configuration
	bigfiles_config = new Configuration(nppData);
	
}

INT_PTR CALLBACK ConfigurationDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_INITDIALOG:

		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hwnd, IDOK);
			break;
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

// Function tells NOTEPAD++ to open the configuration file
void openConfigFile()
{
	// TODO: Add a save function hook to know when Notepad++ saves the file?
	// or just implement my own dialog box.
	::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, (LPARAM) bigfiles_config->confFileNameFull.c_str());
	Sleep(500);
	bigfiles_config->ConfFileBufferID = (int)::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// This cleanup happens before PlugInCleanUp, but still only happens when we close Notepad++

	// Don't forget to deallocate your shortcut here
	delete AltLeftKey, AltRightKey, AltDownKey, AltUpKey;
	delete left_icon, right_icon, open_icon, start_icon, end_icon;
}

// Register Toolbar Icons
void commandRegToolbarIcons() {
	//Add icons in the toolbar

	// Start
	::SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[3]._cmdID, (LPARAM)(start_icon));

	// Left
	::SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[1]._cmdID, (LPARAM)(left_icon));

	// Open
	::SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[0]._cmdID, (LPARAM)(open_icon));

	// Right
	::SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[2]._cmdID, (LPARAM)(right_icon));

	//End
	::SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[4]._cmdID, (LPARAM)(end_icon));
};

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

//---------- END LOADING AND UNLOADING FUNCTIONS ----------------

// --------- START PLUGIN FUNCTIONS -----------
// Function opens the file dialog window to select the file name to open.
// Filename is copied to wchar_t filename_temp[500].
// Inputs : VOID
// Returns: 1 for successfully gathered the filename
//			0 if operation cancelled
int getFileName()
{
	int ret = 0;	//Cancelled

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

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
						// Copy filename frompszFilePath to filename_temp, up to 500 chars
						wcsncpy(filename_temp, pszFilePath, 500);

						// Clear memory
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

// Function check if the current user is an admin.
// Inputs : VOID
// Returns: True is current user is an admin
//			False if current user is not an admin
BOOL IsUserAdmin(VOID)
/*++
Routine Description: This routine returns TRUE if the caller's
process is a member of the Administrators local group. Caller is NOT
expected to be impersonating anyone and is expected to be able to
open its own process and process token.
Arguments: None.
Return Value:
   TRUE - Caller has Administrators local group.
   FALSE - Caller does not have Administrators local group. --
*/
{
	BOOL b;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorsGroup;
	b = AllocateAndInitializeSid(
		&NtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&AdministratorsGroup);
	if (b)
	{
		if (!CheckTokenMembership(NULL, AdministratorsGroup, &b))
		{
			b = FALSE;
		}
		FreeSid(AdministratorsGroup);
	}

	return(b);
}

// Function shows a MessageBox with debug information
// Inputs : VOID
// Returns: VOID
void showDebug()
{
	// TODO: Should change MessageBox to integrated dialog box in Notepad++
	wchar_t str_buf[1000];
	wchar_t strMessage[1000];
	wchar_t bf_str[500];
	size_t cbDest = 1000 * sizeof(wchar_t);
	int i = 0;
	StringCchPrintfW(strMessage, cbDest, TEXT("Length = %d\nCurrent BufferID=%d\n\n"), bigfile_length, ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0));

	cbDest = 500 * sizeof(wchar_t);
	StringCchPrintfW(bf_str, cbDest,
		TEXT("FileName = %s\nBufferID = %d\nPage Number Current = %d\nPage Number Max = %d\nFileSizeByte = %llu\nFileSizeLeft = %llu\nFileNameType = %s\n"),
		bigfile[i].filename,
		bigfile[i].bufferID,
		bigfile[i].page_num_current,
		bigfile[i].page_num_max,
		bigfile[i].file_size_bytes,
		bigfile[i].file_size_left,
		bigfile[i].filetype_name
	);
	StringCchCatW(strMessage, cbDest, bf_str);
	//wcscat(strMessage, bf_str);

	::MessageBox(NULL, strMessage, TEXT("BigFiles Plugin - Debug"), MB_OK);
}


//
//// -------- V2 -------- //
//
///*
//Function open the file in read-only and update the statistics in the file structure
//This runs only once when you open the file for the first time
//Returns True or False
//*/
//bool get_file_stats(bigfile_struct* bigfile_entry) {
//	// Holding the first 4 character in the file for libmagic
//	char binaryBuffer[5];
//	wchar_t strMessage[1000];
//	long double page_num_calc = 0.0;
//
//	// - Get the file size to calculate the number of pages
//	// Open stream in binary
//	std::ifstream mybigfile_size(bigfile_entry->filename, std::ios::binary);
//	// Position stream pointer at the end
//	mybigfile_size.seekg(0, mybigfile_size.end);
//	// Get current position value
//	bigfile_entry->file_size_bytes = mybigfile_size.tellg();
//	bigfile_entry->file_size_left = bigfile_entry->file_size_bytes;
//	// Position stream pointer at the beginning
//	mybigfile_size.seekg(0, mybigfile_size.beg);
//	//Compute number of pages based on page size
//	// TODO: Fix calculation of maximum number of pages
//	// - April 4th, 2020 update: It should be fixed!
//	page_num_calc = (long double)bigfile_entry->file_size_bytes / (long double)bigfile_entry->page_size_bytes;
//	bigfile_entry->page_num_max = (int)std::floorl(page_num_calc)+1;
//
//	// Get first 4 bytes of file
//	mybigfile_size.read(binaryBuffer, 4);
//	std::string binaryBuf(binaryBuffer);
//	// Check file type
//	file_type_structure* temp_file;
//	temp_file = libmagic_alike(binaryBuffer);
//	if (temp_file != NULL) {
//		bigfile_entry->is_Binary = true;
//		::MessageBox(NULL, temp_file->name.c_str(), TEXT(PLUGIN_DEFAULT_MESSAGEBOX_TITLE), MB_OK);
//		wcscpy(bigfile_entry->filetype_name, temp_file->name.c_str());
//	}
//	else {
//		// Not recognize, so maybe ASCII
//		bigfile_entry->is_Binary = false;
//		wcscpy(bigfile_entry->filetype_name, TEXT("Not Recognized"));
//	}
//
//	// Close binary stream
//	mybigfile_size.close();
//
//	return TRUE;
//}
//
//// Function open the filename specified by wchar_t filename_temp[500]
//// Inputs : VOID
//// Returns: VOID
//void openBigFile()
//{
//	//If system of records holds 9 records, do not open another one
//	// TODO: Is the limit set to 9 records meaningless?
//	if (bigfile_length == 9) {
//		::MessageBox(NULL, TEXT("Too Many BigFiles Open"), TEXT(PLUGIN_DEFAULT_MESSAGEBOX_TITLE), MB_OK);
//		return;
//	}
//	
//	// Warn the user if they opened Notepad++ with administartor privilages
//	if (bigfiles_config->get_default_isAdmin_warnings()) {
//		if (IsUserAdmin()) {
//			::MessageBox(NULL, TEXT("Be carefull, you are an admin!"), TEXT(PLUGIN_DEFAULT_MESSAGEBOX_TITLE), MB_OK);
//		}
//	}
//
//	// Get the file name from the Windows Dialog Box
//	if (getFileName())
//	{
//		// - Initialize system of records new index
//
//		//Increase system of record index
//		bigfile_length++;
//		//Copy the filename from filename_temp to system of records new index
//		wcsncpy(bigfile[bigfile_length].filename, filename_temp, 500);
//		
//		bigfile[bigfile_length].page_num_current = 1;
//		bigfile[bigfile_length].page_num_max = 0;
//		// TODO: Make page_size_bytes an externally changable setting
//		//- April 27, 2020 update: It should now be updatable via bigfile.conf
//		bigfile[bigfile_length].page_size_bytes = bigfiles_config->get_default_page_size_bytes();
//		bigfile[bigfile_length].file_size_bytes = 0;
//		bigfile[bigfile_length].file_size_left = 0;
//		bigfile[bigfile_length].is_Binary = false;
//
//		// - Open a new document in Scintilla
//		::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);
//		// Get the Handle of the new Scintilla tab
//		// TODO: May not be used in the future
//		bigfile[bigfile_length].sci_ptr = getCurrentHScintilla();
//		// Get the Buffer ID of the new Scintilla tab
//		bigfile[bigfile_length].bufferID = (int)::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0);
//		// Set stream position to beginning 0
//		bigfile[bigfile_length].sp = 0;
//
//		// Get file statistics
//		get_file_stats(&bigfile[bigfile_length]);
//
//		// - Get the file size to calculate the number of pages
//		// Open stream in binary
//		//std::ifstream mybigfile_size(bigfile[bigfile_length].filename, std::ios::binary);
//		// Position stream pointer at the end
//		//mybigfile_size.seekg(0, mybigfile_size.end);
//		// Get current position value
//		//bigfile[bigfile_length].file_size_bytes = mybigfile_size.tellg();
//		//bigfile[bigfile_length].file_size_left = bigfile[bigfile_length].file_size_bytes;
//		// Position stream pointer at the beginning
//		//mybigfile_size.seekg(0, mybigfile_size.beg);
//		//Compute number of pages based on page size
//		// TODO: Fix calculation of maximum number of pages
//		//bigfile[bigfile_length].page_num_max = (int)(bigfile[bigfile_length].file_size_bytes / (size_t)bigfile[bigfile_length].page_size_bytes);
//
//		//TODO: Check that file is not less than 10 bytes
//
//		// Get first 4 bytes of file
//		//mybigfile_size.read(binaryBuffer, 4);
//		//std::string binaryBuf(binaryBuffer);
//		// Check file type
//		//file_type_structure* temp_file;
//		//temp_file = libmagic_alike(binaryBuffer);
//		//if (temp_file != NULL) {
//		//	bigfile[bigfile_length].is_Binary = true;
//		//	::MessageBox(NULL, temp_file->name.c_str(), TEXT(PLUGIN_DEFAULT_MESSAGEBOX_TITLE), MB_OK);
//		//}
//		//else {
//			// I don't recognise the file
//		//}
//
//		// Close binary stream
//		//mybigfile_size.close();
//
//		// For debug maybe show the structure
//		// TODO: Add Debug option in Settings
//		//StringCbPrintf(strBuffer, 500, TEXT("%llu / %d = %d"), bigfile.file_size_bytes, bigfile.page_size_bytes, bigfile.page_num_max);
//		//::MessageBox(NULL, strBuffer, TEXT(PLUGIN_DEFAULT_MESSAGEBOX_TITLE), MB_OK);
//
//		// TODO: Add PageUp and PageDown button association to read different page of file
//
//		updateBuffer(bigfile_length);
//
//
//#ifdef BIGFILES_DEBUG
//		showDebug();
//#endif	
//	}
//	//else
//		// Message users that operation has been cancelled at the dialog box
//		//::MessageBox(NULL, TEXT("Cancelled"), TEXT(PLUGIN_DEFAULT_MESSAGEBOX_TITLE), MB_OK);
//}
//
//// Function gets Scintilla handle
//// Inputs : VOID
//// Returns: Handle
//HWND getCurrentHScintilla()
//{
//	int which;
//	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, reinterpret_cast<LPARAM>(&which));
//	return (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
//}
//
//// Function gets a new page_size_bytes of data and updates the current Scintilla tab
//// Inputs : integer with system of records index to update
//// Returns: VOID
//void updateBuffer(int record_index) 
//{
//	wchar_t strMessage[1000];
//	size_t file_position;
//	size_t cbDest = 1000 * sizeof(wchar_t);
//
//	// Open stream in Read-Only Mode and copy new data to string buffer
//	std::ifstream myfile(bigfile[record_index].filename, std::ios::in);
//	std::string string_buffer(bigfile[record_index].page_size_bytes, '\0');
//	LPSTR pst = &string_buffer[0];
//
//	// Calculate new position
//	file_position = (bigfile[record_index].page_num_current - 1) * (size_t)bigfile[record_index].page_size_bytes;
//	std::streamoff new_position = file_position;
//	myfile.seekg(new_position, std::ios::beg);
//
//	myfile.read(&string_buffer[0], bigfile[record_index].page_size_bytes);
//
//	//Save current stream position (streampos)
//	// This is the end of the page. So when I go forward, I don't need to update sp. When I go back, I need to remove page size from sp.
//	bigfile[record_index].sp = myfile.tellg();
//
//	// Close stream
//	myfile.close();
//
//	// TODO: Trunkate to last line feed and/or carriage return (\n and/or \r)
//
//	// TODO: Change Scintilla Tab Name
//
//	// TODO: Calculate maximum file size before it break this below calculation
//	// - April 27, 2020 update: Should be fixed
//	// Calculate the number of bytes left to read in the file
//	size_t max_read = ((size_t)bigfile[record_index].page_size_bytes * (size_t)bigfile[record_index].page_num_current);
//	if (bigfile[record_index].file_size_bytes > max_read) {
//		bigfile[record_index].file_size_left = bigfile[record_index].file_size_bytes - max_read;
//	} else
//		bigfile[record_index].file_size_left = 0;
//
//	// Scintilla control has no Unicode mode
//	// Copy string buffer to the new Scintilla buffer
//	// If the file is binary, use the APPENDTEXT function to show NULL characters too
//	if (bigfile[record_index].is_Binary)
//	{
//		// Clear Buffer
//		::SendMessage(bigfile[record_index].sci_ptr, SCI_CLEARALL, 0, 0);
//
//		// If the number of bytes left to read in the file are larger than the page size, append the whole page
//		if (bigfile[record_index].file_size_left > bigfile[record_index].page_size_bytes)
//			// Copy the specific length of bytes using the APPENDTEXT function
//			::SendMessage(bigfile[record_index].sci_ptr, SCI_APPENDTEXT, bigfile[record_index].page_size_bytes, (LPARAM)pst);
//		// Otherwise write only what is left
//		else
//			// Copy the specific length of bytes using the APPENDTEXT function
//			::SendMessage(bigfile[record_index].sci_ptr, SCI_APPENDTEXT, bigfile[record_index].file_size_left, (LPARAM)pst);
//	}
//	else
//	{
//		::SendMessage(bigfile[record_index].sci_ptr, SCI_SETTEXT, 0, (LPARAM)pst);
//	}
//
//	//Update StatusBar
//	updateStatusBar(record_index);
//}
//
//// Function use the system of records to move the stream pointer to the previous page
//// Inputs : VOID
//// Returns: VOID
//// Shortcut: ALT+LEFT_ARROW
//void move_backward()
//{
//	// Retrieve current visible buffer record
//	int current_bigfile_index = getBigFileRecordIndex((int)::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0));
//
//	if (current_bigfile_index > -1) {
//		// If we still have more pages to go, read data, otherwise move Scintilla to first character
//		if (bigfile[current_bigfile_index].page_num_current > 1) {
//
//			// We are moving backword, so decrease current page number
//			bigfile[current_bigfile_index].page_num_current--;
//			// and update the stream pointer
//			//bigfile[current_bigfile_index].sp.operator-=(bigfile[current_bigfile_index].page_size_bytes);
//
//			// Update Scintilla buffer
//			updateBuffer(current_bigfile_index);
//
//#ifdef BIGFILES_DEBUG
//			showDebug();
//#endif	
//		}
//		else {
//			// Move Scintilla to first character
//			::SendMessage(bigfile[current_bigfile_index].sci_ptr, SCI_GOTOLINE, 0, 0);
//		}
//	}
//	else {
//		// Current Buffer ID not found or the user is using the shortcut in a scintilla tab not opened by BigFiles
//		//::MessageBox(NULL, TEXT("ERROR: I could not find the Scintilla Buffer ID"), TEXT(PLUGIN_DEFAULT_MESSAGEBOX_TITLE), MB_ICONERROR | MB_OK);
//	}
//}
//
//// Function use the system of records to move the stream pointer to the next page
//// Inputs : VOID
//// Returns: VOID
//// Shortcut: ALT+RIGHT_ARROW
//void move_forward()
//{
//	// Retrieve current visible buffer record
//	int current_bigfile_index = getBigFileRecordIndex((int)::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0));
//
//	if (current_bigfile_index > -1) {
//
//		// If we still have more pages to go, read data, otherwise move Scintilla to last character
//		if (bigfile[current_bigfile_index].page_num_current < bigfile[current_bigfile_index].page_num_max) {
//			// We are moving forward, so increase current page number. Stream pointer is already pointing to the last char in the page.
//			// so running again updateBuffer, will automatically advance stream pointer
//			bigfile[current_bigfile_index].page_num_current++;
//			// Update Scintilla buffer
//			updateBuffer(current_bigfile_index);
//
//#ifdef BIGFILES_DEBUG
//			showDebug();
//#endif	
//		}
//		else {
//			// Move Scintilla to last character
//			int lines = (int)::SendMessage(bigfile[current_bigfile_index].sci_ptr, SCI_GETLINECOUNT, 0, 0) - 1;
//			::SendMessage(bigfile[current_bigfile_index].sci_ptr, SCI_GOTOLINE, lines, 0);
//		}
//	}
//	else {
//		// Current Buffer ID not found or the user is using the shortcut in a scintilla tab not opened by BigFiles
//		//::MessageBox(NULL, TEXT("ERROR: I could not find the Scintilla Buffer ID"), TEXT(PLUGIN_DEFAULT_MESSAGEBOX_TITLE), MB_ICONERROR | MB_OK);
//	}
//}
//
///*
//Function move the cursor to the beginning of the file. 
//Inputs:		
//Returns:	VOID
//Shortcut:	ALT+UP
//*/
//void move_to_start() {
//	// Retrieve current visible buffer record
//	int current_bigfile_index = getBigFileRecordIndex((int)::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0));
//
//	if (current_bigfile_index > -1) {
//
//		// If we still have more pages to go, read data, otherwise move Scintilla to last character
//		if (bigfile[current_bigfile_index].page_num_current != 1) {
//			// We are moving to the end, so set page number to max. Stream pointer is updated automatically upon running updateBuffer
//			bigfile[current_bigfile_index].page_num_current = 1;
//			//bigfile[current_bigfile_index].sp = 0;
//			
//			// Update Scintilla buffer
//			updateBuffer(current_bigfile_index);
//
//#ifdef BIGFILES_DEBUG
//			showDebug();
//#endif	
//		}
//		// Move Scintilla to first character
//		::SendMessage(bigfile[current_bigfile_index].sci_ptr, SCI_GOTOLINE, 0, 0);
//	}
//	else {
//		// Current Buffer ID not found or the user is using the shortcut in a scintilla tab not opened by BigFiles
//		//::MessageBox(NULL, TEXT("ERROR: I could not find the Scintilla Buffer ID"), TEXT(PLUGIN_DEFAULT_MESSAGEBOX_TITLE), MB_ICONERROR | MB_OK);
//	}
//};
//
///*
//Function move the cursor to the end of the file.
//Inputs:
//Returns:	VOID
//Shortcut:	ALT+DOWN
//*/
//void move_to_end() {
//	// Retrieve current visible buffer record
//	int current_bigfile_index = getBigFileRecordIndex((int)::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0));
//
//	if (current_bigfile_index > -1) {
//		// If we still have more pages to go, read data, otherwise move Scintilla to last character
//		if (bigfile[current_bigfile_index].page_num_current < bigfile[current_bigfile_index].page_num_max) {
//			// We are moving to the end, so set page number to max. Stream pointer is updated automatically upon running updateBuffer
//			bigfile[current_bigfile_index].page_num_current = bigfile[current_bigfile_index].page_num_max;
//			//bigfile[current_bigfile_index].sp.operator+(bigfile[current_bigfile_index].page_num_current * bigfile[current_bigfile_index].page_size_bytes);
//			// Update Scintilla buffer
//			updateBuffer(current_bigfile_index);
//#ifdef BIGFILES_DEBUG
//			showDebug();
//#endif	
//		}
//		else {
//			// Move Scintilla to last character
//			int lines = (int)::SendMessage(bigfile[current_bigfile_index].sci_ptr, SCI_GETLINECOUNT, 0, 0) - 1;
//			::SendMessage(bigfile[current_bigfile_index].sci_ptr, SCI_GOTOLINE, lines, 0);
//		}
//	}
//	else {
//		// Current Buffer ID not found or the user is using the shortcut in a scintilla tab not opened by BigFiles
//
//		//::MessageBox(NULL, TEXT("ERROR: I could not find the Scintilla Buffer ID"), TEXT(PLUGIN_DEFAULT_MESSAGEBOX_TITLE), MB_ICONERROR | MB_OK);
//	}
//};
//
//// Function finds the system of records index that match the Buffer ID
//// Inputs : integer number of the current Scintilla tab Buffer ID
//// Returns: >= 0 system or records index
////			-1 not found
//int getBigFileRecordIndex(int buffer_id)
//{
//	for (int i = 0; i <= bigfile_length; i++) {
//		if (bigfile[i].bufferID == buffer_id)
//			// Buffer ID matches the current system of records, so return index
//			return i;
//	}
//	// Buffer ID in the System of Records was not found
//	return -1;
//}
//
//// Function finds the buffer ID that got closed and, if I have it in my list, delete it from the list.
////	This function is called from NppPluginDemo.cpp notification switch
//// Inputs : integer number of the current Scintilla tab Buffer ID
//// Returns: VOID
//void closeBufferID(int buffer_ID) {
//	size_t cbDest = 500 * sizeof(wchar_t);
//	// Go through the whole system of records
//	for (int i = 0; i <= bigfile_length; i++) {
//		// If the Buffer ID is a match
//		if (bigfile[i].bufferID == buffer_ID) {
//			// To delete a system of records item from the middle of the array,
//			// I just copy the last (valid and not deleted) system of records item to the index where it needs to be deleted
//			// This leaves garbage in the unused system of records items!
//			bigfile[i].bufferID = bigfile[bigfile_length].bufferID;
//			StringCchCopyW(bigfile[i].filename, cbDest, bigfile[bigfile_length].filename);
//			//wcsncpy(bigfile[i].filename, bigfile[bigfile_length].filename,500);
//			bigfile[i].file_size_bytes = bigfile[bigfile_length].file_size_bytes;
//			bigfile[i].is_Binary = bigfile[bigfile_length].is_Binary;
//			bigfile[i].page_num_current = bigfile[bigfile_length].page_num_current;
//			bigfile[i].page_num_max = bigfile[bigfile_length].page_num_max;
//			bigfile[i].page_size_bytes = bigfile[bigfile_length].page_size_bytes;
//			bigfile[i].sci_ptr = bigfile[bigfile_length].sci_ptr;
//			bigfile[i].sp = bigfile[bigfile_length].sp;
//
//			//Reduce the 
//			bigfile_length--;
//		}
//	}
//}
//
//
//// Function updates the DOC_TYPE status bar field
//// Inputs : system of records index
//// Returns: VOID
//void updateStatusBar(int record_index)
//{
//	long double file_size_left_kb = 0.0;
//	wchar_t strMessage[1000];
//	size_t cbDest = 1000 * sizeof(wchar_t);
//	if (bigfile[record_index].file_size_left > 0)
//		file_size_left_kb = (long double) bigfile[record_index].file_size_left / 1024;
//	HRESULT hr = StringCbPrintfW(strMessage, cbDest, TEXT("BigFile ID: #%d - Bytes Left: %.2lf kB - Page: %d/%d"), record_index, file_size_left_kb, bigfile[record_index].page_num_current, bigfile[record_index].page_num_max);
//	::SendMessage(nppData._nppHandle, NPPM_SETSTATUSBAR, STATUSBAR_DOC_TYPE, reinterpret_cast<LPARAM>(strMessage));
//}
