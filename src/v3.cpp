
#include "PluginDefinition.h"

void updateStatusBar3(FileTracker*);
void closeBufferID3(int buffer_ID);
int getBigFileRecordIndex3(int buffer_id);
void move_to_start3();
void move_to_end3();
void move_forward3();
void move_backward3();
void openBigFile3(const wchar_t filename[]);
void openBigFileDlg3();
void cleanupv3();

std::vector<FileTracker> ftv;

int ft_length;

extern NppData nppData;
extern Configuration* bigfiles_config;
extern wchar_t filename_temp[500];

void cleanupv3() {
	// Delete will be used in the future when I move from static to dynamic allocation of ftv vector
	//delete[] & ftv;
}

void openBigFileDlg3() {
	//If system of records holds 9 records, do not open another one
	// TODO: Is the limit set to 9 records meaningless?
	if (ft_length == 9) {
		::MessageBox(NULL, TEXT("Too Many BigFiles Open"), TEXT(PLUGIN_DEFAULT_MESSAGEBOX_TITLE), MB_OK);
		return;
	}

	// Warn the user if they opened Notepad++ with administartor privilages
	if (bigfiles_config->get_default_isAdmin_warnings()) {
		if (IsUserAdmin()) {
			::MessageBox(NULL, TEXT("Be carefull, you are an admin!"), TEXT(PLUGIN_DEFAULT_MESSAGEBOX_TITLE), MB_OK);
		}
	}

	// Get the file name from the Windows Dialog Box
	if (getFileName()) {

		openBigFile3(filename_temp);
	}
}

void openBigFile3(const wchar_t filename[]) {

	// Get the current scintilla
	int which = -1;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
	if (which == -1)
		return;
	HWND curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

	FileTracker* ft1 = new FileTracker(nppData._nppHandle, curScintilla);
	ft1->openBigFile(filename, *bigfiles_config);
	ft1->updateBuffer(true);
		
	updateStatusBar3(ft1);

	ftv.push_back(*ft1);
		
	// check file type
	if (ft1->binarySignatureName != NULL) {
		::MessageBox(NULL, ft1->binarySignatureName->c_str(), TEXT(PLUGIN_DEFAULT_MESSAGEBOX_TITLE), MB_OK);
	}
	else {
		// i don't recognise the file
	}
}

void move_backward3() {
	// Retrieve current visible buffer record
	int i = getBigFileRecordIndex3((int)::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0));
	if (i != -1) {
		if (ftv[i].move_backward()) {
			ftv[i].updateBuffer(false);
			updateStatusBar3(&ftv[i]);
		}
		else {
			// Move Scintilla to first character
			::SendMessage(ftv[i].scintilla_handle, SCI_GOTOLINE, 0, 0);
		}
	}
}

void move_forward3() {
	// Retrieve current visible buffer record
	int i = getBigFileRecordIndex3((int)::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0));
	if (i != -1) {
		if (ftv[i].move_forward()) {
			ftv[i].updateBuffer(false);
			updateStatusBar3(&ftv[i]);
		}
		else {
			// Move Scintilla to last character
			int lines = (int)::SendMessage(ftv[i].scintilla_handle, SCI_GETLINECOUNT, 0, 0) - 1;
			::SendMessage(ftv[i].scintilla_handle, SCI_GOTOLINE, lines, 0);
		}
	}
}

void move_to_start3() {
	// Retrieve current visible buffer record
	int i = getBigFileRecordIndex3((int)::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0));
	if (i != -1) {
		if (ftv[i].move_to_start()) {
			ftv[i].updateBuffer(false);
			updateStatusBar3(&ftv[i]);
		}
		// Move Scintilla to first character
		::SendMessage(ftv[i].scintilla_handle, SCI_GOTOLINE, 0, 0);
	}
}

void move_to_end3() {
	// Retrieve current visible buffer record
	int i = getBigFileRecordIndex3((int)::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0));
	if (i != -1) {
		if (ftv[i].move_to_end()) {
			ftv[i].updateBuffer(false);
			updateStatusBar3(&ftv[i]);
		}
		else {
			// Move Scintilla to last character
			int lines = (int)::SendMessage(ftv[i].scintilla_handle, SCI_GETLINECOUNT, 0, 0) - 1;
			::SendMessage(ftv[i].scintilla_handle, SCI_GOTOLINE, lines, 0);
		}
	}
}

int getBigFileRecordIndex3(int buffer_id)
{
	int i = 0;
	// Go through the vector and find the FileTracker with the right BufferID
	for (FileTracker ft_obj : ftv) {
		if (ft_obj.bufferID == buffer_id)
			// Buffer ID matches the current system of records, so return the object
			return i;
		i++;
	}
	return -1;
}

void closeBufferID3(int buffer_ID) {
	// Retrieve current visible buffer record
	int i = getBigFileRecordIndex3(buffer_ID);
	if (i != -1)
		ftv.erase(ftv.begin() + i);
}

void updateStatusBar3(FileTracker* ft_obj) {
	#define STR_MESSAGE_SIZE 1500
	long double file_size_left_kb = 0.0;
	wchar_t strMessage[STR_MESSAGE_SIZE];
	size_t cbDest = STR_MESSAGE_SIZE * sizeof(wchar_t);
	std::wstring default_file_type = TEXT("TEXT");

	if (ft_obj->file_size_left > 0)
		file_size_left_kb = (long double)ft_obj->file_size_left / 1024;
	if (ft_obj->binarySignatureName != NULL) {
		HRESULT hr = StringCbPrintfW(strMessage, cbDest,
			TEXT("BigFile Type: %s - Page: %d of %d - Bytes Left: %.2lf kB - Name: %s"),
			ft_obj->binarySignatureName->c_str(),
			ft_obj->page_num_current,
			ft_obj->page_num_max,
			file_size_left_kb,
			ft_obj->filename
		);
	}
	else {
		HRESULT hr = StringCbPrintfW(strMessage, cbDest,
			TEXT("BigFile Type: %s - Page: %d of %d - Bytes Left: %.2lf kB - Name: %s"),
			TEXT("TEXT"),
			ft_obj->page_num_current,
			ft_obj->page_num_max,
			file_size_left_kb,
			ft_obj->filename
		);
	}
	::SendMessage(nppData._nppHandle, NPPM_SETSTATUSBAR, STATUSBAR_DOC_TYPE, reinterpret_cast<LPARAM>(strMessage));
}