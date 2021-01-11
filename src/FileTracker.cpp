#include "FileTracker.h"

FileTracker::FileTracker(HWND npp_handle, HWND scintilla_handle) {
	//Initialize
	this->scintilla_handle = scintilla_handle;
	this->npp_handle = npp_handle;
}

/*
Function open the file in read-only and update the statistics in the file structure
Returns
*/
bool FileTracker::get_file_stats(bool first_time_call = false) {

	// Holding the first 4 character in the file for libmagic
	char binaryBuffer[5];
	long double page_num_calc = 0.0;

	// - Get the file size to calculate the number of pages
	// Open stream in binary
	std::ifstream mybigfile_size(this->filename, std::ios::binary);
	// First position stream pointer at the end
	mybigfile_size.seekg(0, mybigfile_size.end);
	// and get its position, this is the total file size
	this->file_size_bytes = mybigfile_size.tellg();
	// NEXT STEP IS NOT NECESSARY
	// Now position stream pointer at the beginning 
	mybigfile_size.seekg(0, mybigfile_size.beg);
	//Compute number of pages based on page size
	page_num_calc = (long double)this->file_size_bytes / (long double)this->page_size_bytes;
	this->page_num_max = (int)std::floorl(page_num_calc) + 1;

	if (first_time_call) {

		this->file_size_left = this->file_size_bytes;

		// Get first MAX_SIGNATURE_BYTES bytes of file
		mybigfile_size.read(this->binarySignature, MAX_SIGNATURE_BYTES);

		// find if the name of the file signature and get its name
		file_type_structure* temp_file;
		temp_file = libmagic_alike(this->binarySignature, MAX_SIGNATURE_BYTES);
		
		if (temp_file != NULL) {
			this->binarySignatureName = &temp_file->name;
			this->is_Binary = true;
		}
		else {
			this->binarySignatureName = NULL;
			this->is_Binary = false;
		}

	}

	// Close binary stream
	mybigfile_size.close();

	return TRUE;
}

unsigned int FileTracker::find_EndOfLine(const std::string data) {
	unsigned int charNum;
	char newLine[] = {0x0A};
	charNum = data.find_last_of(newLine);
	
	return charNum;
}

// Function gets a new page_size_bytes of data and updates the current Scintilla tab
// Inputs : integer with system of records index to update
// Returns: VOID
void FileTracker::updateBuffer(bool first_time_call)
{
	wchar_t strMessage[1000];
	size_t file_position;
	size_t cbDest = 1000 * sizeof(wchar_t);

	if (first_time_call == false) {
		this->get_file_stats(false);
	}

	// Open stream in Read-Only Mode and copy new data to string buffer
	std::ifstream myfile(this->filename, std::ios::in);
	std::string string_buffer(this->page_size_bytes, '\0');
	LPSTR pst = &string_buffer[0];
	
	// Calculate new position
	file_position = (this->page_num_current - 1) * (size_t)this->page_size_bytes;
	std::streamoff new_position = file_position;
	myfile.seekg(new_position, std::ios::beg);

	myfile.read(&string_buffer[0], this->page_size_bytes);

	//Save current stream position (streampos)
	// This is the end of the page. So when I go forward, I don't need to update sp. When I go back, I need to remove page size from sp.
	this->sp = myfile.tellg();

	// Close stream
	myfile.close();

	// TODO: Trunkate to last line feed and/or carriage return (\n and/or \r)

	// TODO: Change Scintilla Tab Name

	// TODO: Calculate maximum file size before it break this below calculation
	// - April 27, 2020 update: Should be fixed
	// Calculate the number of bytes left to read in the file
	size_t max_read = ((size_t)this->page_size_bytes * (size_t)this->page_num_current);
	if (this->file_size_bytes > max_read) {
		this->file_size_left = this->file_size_bytes - max_read;
	}
	else
		this->file_size_left = 0;

	// Scintilla control has no Unicode mode
	// Copy string buffer to the new Scintilla buffer
	// If the file is binary, use the APPENDTEXT function to show NULL characters too
	if (this->is_Binary)
	{
		// Clear Buffer
		::SendMessage(this->scintilla_handle, SCI_CLEARALL, 0, 0);

		// If the number of bytes left to read in the file are larger than the page size, append the whole page
		if (this->file_size_left > this->page_size_bytes)
			// Copy the specific length of bytes using the APPENDTEXT function
			::SendMessage(this->scintilla_handle, SCI_APPENDTEXT, this->page_size_bytes, (LPARAM)pst);
		// Otherwise write only what is left
		else
			// Copy the specific length of bytes using the APPENDTEXT function
			::SendMessage(this->scintilla_handle, SCI_APPENDTEXT, this->file_size_left, (LPARAM)pst);
	}
	else
	{
		wchar_t strMessage[1000];
		unsigned int charNum = find_EndOfLine(string_buffer);
		//unsigned int leftOverChars = 0;
		//StringCbPrintfW(strMessage, 1000, TEXT("Last Line Feed found at character number:%d"), charNum);
		//::MessageBox(NULL, strMessage, TEXT("Debug"), MB_OK);
		//string_buffer.assign(string_buffer, charNum);
		//leftOverChars = string_buffer.length - charNum;
		//this->file_size_left += leftOverChars;
		//string_buffer[charNum] = { 0x00 };
		::SendMessage(this->scintilla_handle, SCI_SETTEXT, 0, (LPARAM)pst);
	}
}

/*
Function open the filename
Inputs : filename specified by wchar_t filename_temp[500]
Returns: VOID
*/
void FileTracker::openBigFile(const wchar_t filename_temp[], Configuration& bigfiles_config)
{

	//Copy the filename from filename_temp to system of records new index
	StringCchCopyW(this->filename, this->filename_size, filename_temp);

	this->page_num_current = 1;
	this->page_num_max = 0;
	// TODO: Make page_size_bytes an externally changable setting
	this->page_size_bytes = bigfiles_config.get_default_page_size_bytes();
	this->file_size_bytes = 0;
	this->file_size_left = 0;
	this->is_Binary = false;

	// - Open a new document in Scintilla
	::SendMessage(this->npp_handle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);
	// Get the Buffer ID of the new Scintilla tab
	this->bufferID = (int)::SendMessage(this->npp_handle, NPPM_GETCURRENTBUFFERID, 0, 0);
	// Set stream position to beginning 0
	this->sp = 0;

	// Get file statistics - First Time Call
	this->get_file_stats(true);
	
}

/*
Function use the system of records to move the stream pointer to the previous page
Inputs:		None
Returns:	True if done
			False if we are at the start
Shortcut:	ALT+LEFT_ARROW
*/
bool FileTracker::move_backward()
{
	// If we still have more pages to go, read data, otherwise move Scintilla to first character
	if (this->page_num_current > 1) {
		// We are moving backword, so decrease current page number
		this->page_num_current--;
		return true;
	}
	else {
		return false;
	}
}

/*
Function use the system of records to move the stream pointer to the next page
Inputs:		None
Returns:	True if done
			False if we are at the start
Shortcut:	ALT+RIGHT_ARROW
*/
bool FileTracker::move_forward()
{
	// If we still have more pages to go, read data, otherwise move Scintilla to last character
	if (this->page_num_current < this->page_num_max) {
		// We are moving forward, so increase current page number. Stream pointer is already pointing to the last char in the page.
		// so running again updateBuffer, will automatically advance stream pointer
		this->page_num_current++;
		return true;
	}
	else {
		return false;
	}
}

/*
Function move the cursor to the beginning of the file.
Inputs:		None
Returns:	True if done
			False if we are at the start
Shortcut:	ALT+UP
*/
bool FileTracker::move_to_start() {

	// If we still have more pages to go, read data, otherwise move Scintilla to last character
	if (this->page_num_current > 1) {
		// We are moving to the end, so set page number to max. Stream pointer is updated automatically upon running updateBuffer
		this->page_num_current = 1;
		//bigfile[current_bigfile_index].sp = 0;
		return true;
	}
	else {
		return false;
	}
};

/*
Function move the cursor to the end of the file.
Inputs:		None
Returns:	True if done
			False if we are at the start
Shortcut:	ALT+DOWN
*/
bool FileTracker::move_to_end() {
	// If we still have more pages to go, read data, otherwise move Scintilla to last character
	if (this->page_num_current < this->page_num_max) {
		// We are moving to the end, so set page number to max. Stream pointer is updated automatically upon running updateBuffer
		this->page_num_current = this->page_num_max;
		//bigfile[current_bigfile_index].sp.operator+(bigfile[current_bigfile_index].page_num_current * bigfile[current_bigfile_index].page_size_bytes);
		return true;
	}
	else {
		return false;
	}
};

