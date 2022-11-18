#include <windows.h>
#include <tchar.h>
#include <libloaderapi.h>
#include <wchar.h>

typedef DWORD(*msgBox)(wchar_t*);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
{

    HMODULE hDll = LoadLibrary(_T("BigFiles"));

    _tprintf(_T("library loaded at 0x%x\n"), hDll);

    // load DivFunc and casting to appropriate type
    msgBox mB = (msgBox)GetProcAddress(hDll, "msgBox");
    if (!mB)
        _tprintf(_T("unable to load msgBox\n"));
    else
        mB(TEXT("TEST ME"));


    FreeLibrary(hDll);

    return 0;
}