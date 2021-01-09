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

extern FuncItem funcItem[nbFunc];
extern NppData nppData;
extern void updateStatusBar3(FileTracker*);
extern void closeBufferID3(int buffer_ID);
extern int getBigFileRecordIndex3(int buffer_id);
extern std::vector<FileTracker> ftv;
extern Configuration* bigfiles_config;

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  reasonForCall, LPVOID /*lpReserved*/)
{

	switch (reasonForCall)
	{
	case DLL_PROCESS_ATTACH:
		pluginInit(hModule);
	
		break;

	case DLL_PROCESS_DETACH:
		pluginCleanUp();

		break;

	case DLL_THREAD_ATTACH:

		break;

	case DLL_THREAD_DETACH:

		break;
	}

	return TRUE;
}


extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{
	nppData = notpadPlusData;
	commandMenuInit();
}

extern "C" __declspec(dllexport) const TCHAR * getName()
{
	return NPP_PLUGIN_NAME;
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int* nbF)
{
	*nbF = nbFunc;
	return funcItem;
}


extern "C" __declspec(dllexport) void beNotified(SCNotification * notifyCode)
{
	int sci_bufferId = 0;
	int record_index = 0;
	sci_bufferId = notifyCode->nmhdr.idFrom;

	switch (notifyCode->nmhdr.code)
	{
		case NPPN_FILEOPENED:
		{
			if (sci_bufferId == bigfiles_config->ConfFileBufferID) {
				bigfiles_config->ConfFileBufferID = 0;
			}
		}
		break;

		case NPPN_SHUTDOWN:
		{
			commandMenuCleanUp();
		}
		break;

		case NPPN_TBMODIFICATION:
		{
			commandRegToolbarIcons();
		}
		break;

		case NPPN_FILECLOSED:
		{
			//When the FileClosed event happens, the Scintilla Buffer gets closed.
			// BigFiles needs to remove the reference to the buffer ID.
			closeBufferID3(sci_bufferId);
		}
		break;
		
		case NPPN_FILESAVED:
		{
			if (sci_bufferId == bigfiles_config->ConfFileBufferID) {
				msgBox_int(TEXT("Reloading Configuration"), sci_bufferId);
				bigfiles_config->loadConfFile();
			}
		}
		break;

		case NPPN_BUFFERACTIVATED:
		{
			// This happens when a Scintilla Buffer tab has been activated
			// Update the current buffer reference in the BigFiles structure
			record_index = getBigFileRecordIndex3(sci_bufferId);
			if (record_index >= 0)
				updateStatusBar3(&ftv[record_index]);
			
		}
		break;
		
		default:
			return;
	}
}


// Here you can process the Npp Messages 
// I will make the messages accessible little by little, according to the need of plugin development.
// Please let me know if you need to access to some messages :
// http://sourceforge.net/forum/forum.php?forum_id=482781
//
extern "C" __declspec(dllexport) LRESULT messageProc(UINT /*Message*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{/*
	if (Message == WM_MOVE)
	{
		::MessageBox(NULL, "move", "", MB_OK);
	}
*/
	return TRUE;
}

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode()
{
    return TRUE;
}
#endif //UNICODE
