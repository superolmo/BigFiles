# BigFiles
Notepad++ Plugin for reading large files

# Compiling
Solution has been developed in Visual Stusio 2017 with Windows 10 SDK.
Output file bin64/BigFiles.dll ready to be imported by Notepad++

# Structure
The plugin is developed based on the Plugin template from Don Ho at https://github.com/npp-plugins/plugintemplate
Big files are read using a small 100kB per page. No files are kept open. An internal structure keeps track of page number for every file opened by the plugin.

# Scope of Work
1. Plugin must be able to preview large files in Notepad++
2. Plugin must have 3 buttons in the toolbar allowing the user to open a bigfile and moving pages backard and forward

# Future Work
- Fix Toolbar icons
- Add current page number and more statistics about the big file in the status bar
- Add settings dialog and bigfiles.ini for changing page size
- Add classification of file type [Text, Binary, ZIP, PDF, etc.] and activate Hex-Editor if it is installed
- (Maybe) Change behaviour when moving from page to page in a text file to truncate at \n\r.

## Helping URLs
- https://notepad-plus-plus.org/community/category/5/plugin-development
- https://github.com/npp-plugins/plugintemplate
