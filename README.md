# BigFiles
Notepad++ Plugin for reading large files

# Compiling
Solution has been developed in Visual Stusio 2019 with Windows XP compatibity.
Output file x64/BigFiles.dll ready to be imported by Notepad++ 64bit, use Win32 for x86.

# How to install
0. Download the BigFiles.dll file for your specific platform and Notepad++ version
1. Open Notepad++ as Administrator
2. Select in the main window top menu Settings->Import->Import Plugin(s)
3. Select the file BigFiles.dll
At this point it should be installed in Notepad++ plugin subfolder.

# How to use it
1. Open Notepad++
2. Select in the main window top menu Plugins->BigFiles->Open BigFile
3. Select file you want to open. Make sure it is a text file
4. Use ALT+LEFT_ARROW and ALT+RIGHT_ARROW to change page backward and forward respectively

# Structure
The plugin is developed based on the Plugin template from Don Ho at https://github.com/npp-plugins/plugintemplate
Big files are read using a small 100kB per page. No files are kept open. An internal structure keeps track of page number for every file opened by the plugin.

# Scope of Work
1. Plugin must be able to preview very large files in Notepad++
2. Plugin must have 3 buttons in the toolbar allowing the user to open a bigfile and moving pages backard and forward

# Future Work
- Add settings dialog and bigfiles.ini for changing page size
- Improve classification of file type [Text, Binary, ZIP, PDF, etc.] and activate Hex-Editor if it is installed
- Open sequence of files. Ex: data1.csv + data2.csv + etc.
- Search for regex in the file by auto forwarding to next page
- (Maybe) Change behaviour when moving from page to page in a text file to truncate at \n\r.

## Helping URLs
- https://notepad-plus-plus.org/community/category/5/plugin-development
- https://github.com/npp-plugins/plugintemplate
