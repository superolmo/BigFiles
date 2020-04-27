#include "FileTracker.h"

FileTracker::FileTracker() {
	//Initialize to no files
	this->bigfile_length = -1;
}

/*
Function open the file in read-only and update the statistics in the file structure
Returns
*/
bool FileTracker::get_file_stats() {
	// Holding the first 4 character in the file for libmagic
	char binaryBuffer[5];

	// - Get the file size to calculate the number of pages
	// Open stream in binary
	std::ifstream mybigfile_size(this->filename, std::ios::binary);
	// Position stream pointer at the end
	mybigfile_size.seekg(0, mybigfile_size.end);
	// Get current position value
	this->file_size_bytes = mybigfile_size.tellg();
	this->file_size_left = this->file_size_bytes;
	// Position stream pointer at the beginning
	mybigfile_size.seekg(0, mybigfile_size.beg);
	//Compute number of pages based on page size
	// TODO: Fix calculation of maximum number of pages
	this->page_num_max = (int)(this->file_size_bytes / (size_t)this->page_size_bytes);

	// Get first 4 bytes of file
	mybigfile_size.read(this->binarySignature, 4);

	// Close binary stream
	mybigfile_size.close();

	return TRUE;
}

// Function open the filename specified by wchar_t filename_temp[500]
// Inputs : VOID
// Returns: VOID
void FileTracker::openBigFile(wchar_t filename_temp[], HWND npp_handle, HWND scintilla_handle, Configuration &bigfiles_config)
{
	//If system of records holds 9 records, do not open another one
	// TODO: Is the limit set to 9 records meaningless?
	if (bigfile_length == 9) {
		::MessageBox(NULL, TEXT("Too Many BigFiles Open"), TEXT(PLUGIN_DEFAULT_MESSAGEBOX_TITLE), MB_OK);
		return;
	}

	//Copy the filename from filename_temp to system of records new index
	wcsncpy(this->filename, filename_temp, 500);

	this->page_num_current = 0;
	this->page_num_max = 0;
	// TODO: Make page_size_bytes an externally changable setting
	this->page_size_bytes = bigfiles_config.get_default_page_size_bytes();
	this->file_size_bytes = 0;
	this->file_size_left = 0;
	this->is_Binary = false;

	// Get the Handle of the new Scintilla tab
	// TODO: May not be used in the future
	this->sci_ptr = scintilla_handle;
	// Get the Buffer ID of the new Scintilla tab
	this->bufferID = (int)::SendMessage(npp_handle, NPPM_GETCURRENTBUFFERID, 0, 0);
	// Set stream position to beginning 0
	this->sp = 0;

	// Get file statistics
    this->get_file_stats();
}

// Function use the system of records to move the stream pointer to the previous page
// Inputs : VOID
// Returns: VOID
// Shortcut: ALT+LEFT_ARROW
void FileTracker::move_backward()
{
	// If we still have more pages to go, read data, otherwise move Scintilla to first character
	if (this->page_num_current > 0)

		// We are moving backword, so decrease current page number
		this->page_num_current--;
}

// Function use the system of records to move the stream pointer to the next page
// Inputs : VOID
// Returns: VOID
// Shortcut: ALT+RIGHT_ARROW
void FileTracker::move_forward()
{
	// If we still have more pages to go, read data, otherwise move Scintilla to last character
	if (this->page_num_current < this->page_num_max) {
		// We are moving forward, so increase current page number. Stream pointer is already pointing to the last char in the page.
		// so running again updateBuffer, will automatically advance stream pointer
		this->page_num_current++;
	}
}

/*
Function move the cursor to the beginning of the file.
Inputs:
Returns:	VOID
Shortcut:	ALT+UP
*/
void FileTracker::move_to_start() {

	// If we still have more pages to go, read data, otherwise move Scintilla to last character
	if (this->page_num_current < this->page_num_max) {
		// We are moving to the end, so set page number to max. Stream pointer is updated automatically upon running updateBuffer
		this->page_num_current = 0;
		//bigfile[current_bigfile_index].sp = 0;
	}
};

/*
Function move the cursor to the end of the file.
Inputs:
Returns:	VOID
Shortcut:	ALT+DOWN
*/
void FileTracker::move_to_end() {
	// If we still have more pages to go, read data, otherwise move Scintilla to last character
	if (this->page_num_current < this->page_num_max) {
		// We are moving to the end, so set page number to max. Stream pointer is updated automatically upon running updateBuffer
		this->page_num_current = this->page_num_max;
		//bigfile[current_bigfile_index].sp.operator+(bigfile[current_bigfile_index].page_num_current * bigfile[current_bigfile_index].page_size_bytes);
	}
};

