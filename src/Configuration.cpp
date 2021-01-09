#include "Configuration.h"

Configuration::Configuration(NppData nppData) {
	//Get directory where the configuration file is located
	this->_getPluginDirectory(nppData);

	//Load config file
	this->loadConfFile();
}

void Configuration::_getPluginDirectory(NppData nppData) {
	//TODO replace confFolder with confDir
	//TCHAR confFolder[1000];
	TCHAR confDir[MAX_PATH];
	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)confDir);
	confFilePath = std::wstring(confDir);
	confFileNameFull = confFilePath + L"\\bigfile.conf";
}

int Configuration::get_default_page_size_bytes() {
	return this->default_page_size_bytes;
};
bool Configuration::get_default_isAdmin_warnings() {
	return this->default_isAdmin_warnings;
};
bool Configuration::get_default_BinaryFileType_warnings() {
	return this->default_BinaryFileType_warnings;
};

/***********************************************************************************
Function was taken from Notepad++ plugin called Converter at
https://github.com/npp-plugins/converter/blob/master/source/PluginDefinition.cpp
Modified to suite BigFiles needs
*/
// 
// if conf file does not exist, then create it and load it.

void Configuration::_getCmdsFromConf(const TCHAR* confPath)
{
	TCHAR cmdNames[MAX_PATH];
	::GetPrivateProfileSectionNames(cmdNames, MAX_PATH, confPath);
	TCHAR* pFn = cmdNames;

	if (*pFn && wcscmp(pFn, TEXT("BigFiles")) == 0)
	{
		int val = 0;

		// Get page_size_bytes
		val = GetPrivateProfileInt(pFn, TEXT("page_size_bytes"), 0, confPath);	//TODO: If not found, it will return 0 right now, is this right?
		if (val != 0)
			this->default_page_size_bytes = val;
		
		// Get isAdmin_warnings
		val = GetPrivateProfileInt(pFn, TEXT("isAdmin_warnings"), 0, confPath);	//TODO: If not found, it will return 0 right now, is this right?
		this->default_isAdmin_warnings = val != 0;

		// Get BinaryFileType_warnings
		val = GetPrivateProfileInt(pFn, TEXT("BinaryFileType_warnings"), 0, confPath);	//TODO: If not found, it will return 0 right now, is this right?
		this->default_BinaryFileType_warnings = val != 0;
	}
	else {
		::MessageBox(NULL, TEXT("Configuration not found"), TEXT("BigFiles Plugin - Configuration::_getCmdsFromConf"), MB_OK);
	}
}
void Configuration::loadConfFile()
{

	const char confContent[] = "\
[BigFiles]\n\
page_size_bytes=100000\n\
isAdmin_warnings=1\n\
BinaryFileType_warnings=1\n\
\n";

	//Get the fully qualified name of the config file, function saves it in confFileNameFull
	//_getPluginDirectory(nppData);

	// If the file does not exist, save default values
	if (!::PathFileExists(confFileNameFull.c_str()))
	{
		wsprintf(strMessage, TEXT("%s%s"), TEXT("First time usage. Writing new BigFile config file to "), confFileNameFull.c_str());
		::MessageBox(NULL, strMessage, TEXT("BigFiles Plugin - loadConfFile"), MB_OK);
		//Write default configuration file
		std::ofstream confFile(confFileNameFull);
		if (confFile)
			confFile << confContent;
		else
			::MessageBox(NULL, TEXT("Something went wrong1"), TEXT("BigFiles Plugin - Configuration::loadConfFile"), MB_OK);
		if (confFile.bad())
			::MessageBox(NULL, TEXT("Something went wrong2"), TEXT("BigFiles Plugin - Configuration::loadConfFile"), MB_OK);
		confFile.close();
	}
	//Get values from configuration file
	_getCmdsFromConf(confFileNameFull.c_str());
}

void Configuration::editConf(NppData nppData)
{
	//IDM_FILE_OPEN
	if (!::PathFileExists(confFileNameFull.c_str()))
	{
		loadConfFile();
	}
	::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, (LPARAM)confFileNameFull.c_str());
}
/*
End of copied section from
https://github.com/npp-plugins/converter/blob/master/source/PluginDefinition.cpp
*******************************************************************************************/