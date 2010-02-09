#include "../../xStdPch.h"
#include "../xOperationSys.h"
#include "xWin32DllLoader.h"
#include "xWin32Thread.h"
#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#include <Userenv.h>
#pragma  comment(lib,"Userenv.lib")
BEGIN_NAMESPACE_XEVOL3D
std::ds_wstring xOperationSys::getUserProfileFolder()
{
	HANDLE hToken=0;
	BOOL bres1 = OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken);
	DWORD size = 1024;
	wchar_t _strUsrDir[1024] = {0}; 
	//SHGetFolderLocation(GetActiveWindow() , CLSI_APP_DATA , hToken ,)
	BOOL bres = GetUserProfileDirectory(hToken, _strUsrDir, &size);  //得到当前进程用户的用户目录
	return _strUsrDir;
}

bool xOperationSys::getModuleFileName(HMODULE hModule , wchar_t* fileName , int len)
{
    wchar_t shortAppName[512] = {0};
    GetModuleFileName( (HMODULE)hModule , shortAppName , 512);    
    memset(fileName , 0 , sizeof(wchar_t) * len);
    GetLongPathName(shortAppName , fileName , len);
    return true;
}

void xOperationSys::enableConsole()
{
	BOOL bResult;

	// Could try attaching to the current console but there might not
	// be one and the output format is somewhat ugly.
	//AttachConsole(ATTACH_PARENT_PROCESS);

	// allocate a console for this app
	bResult = AllocConsole();

	if (bResult)
	{
		int hConHandle;
		HANDLE StdHandle;
		FILE *fp;
		CONSOLE_SCREEN_BUFFER_INFO coninfo;

		// set the screen buffer to be big enough to let us scroll text
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
		coninfo.dwSize.Y = 500;
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

		// redirect unbuffered STDOUT to the console
		StdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		hConHandle = _open_osfhandle((intptr_t)StdHandle, _O_TEXT);
		fp = _fdopen( hConHandle, "w" );
		*stdout = *fp;
		setvbuf( stdout, NULL, _IONBF, 0 );

		// redirect unbuffered STDIN to the console
		StdHandle = GetStdHandle(STD_INPUT_HANDLE);
		hConHandle = _open_osfhandle((intptr_t)StdHandle, _O_TEXT);
		fp = _fdopen( hConHandle, "r" );
		*stdin = *fp;
		setvbuf( stdin, NULL, _IONBF, 0 );
	}
}


unsigned long xGetTickCount()
{
    return timeGetTime();
}

void xSleep(unsigned long mSecond)
{
	Sleep(mSecond);
}


xDLLModule*    xOperationSys::createDLL(const wchar_t* dllName)
{
	xWin32Dll* pDLL = new xWin32Dll;
	if(true == pDLL->load(dllName))
		return pDLL;
	delete pDLL;
	return NULL;
}
xThreadLocker* xOperationSys::createThreadLocker()
{
	return new xWin32Locker;
}
xThreadSignal* xOperationSys::createThreadSignal()
{
	return new xWin32Signal;
}
xThreadRunner* xOperationSys::createThreadRunner()
{
	return new xWin32Thread;
}

int  xOperationSys::warnningBox(const wchar_t* message , const wchar_t* title)
{
	return ::MessageBoxW(::GetActiveWindow() , message , title , MB_ICONERROR|MB_OK);
}

int  xOperationSys::messageBox(const wchar_t* message , const wchar_t* title)
{
	return ::MessageBoxW(::GetActiveWindow() , message , title , MB_OK);
}

void  xOperationSys::convertSystemFileName(std::ds_wstring& _fileName)
{
	for(size_t i = 0 ; i < _fileName.length() ; i ++)
	{
		if(_fileName[i] == '/')
		{
			_fileName[i] = '\\';
		}
		else if(_fileName[i] >= 'A' && _fileName[i] <= 'Z')
		{
			_fileName[i] -= ('A'-'a');
		}
	}
}

bool  xOperationSys::clearClipboard()
{
	OpenClipboard(GetActiveWindow());
	EmptyClipboard();
	CloseClipboard(); 
	return true;
}

bool  xOperationSys::copyToClipboard(const wchar_t* clipBox)
{
	//CF_TEXT;
	OpenClipboard(GetActiveWindow());
	EmptyClipboard();
	HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (wcslen(clipBox) + 1) * sizeof(wchar_t)); 
	if (hglbCopy == NULL) 
	{ 
		CloseClipboard(); 
		return false; 
	} 

	wchar_t*   lptstrCopy = (wchar_t*)GlobalLock(hglbCopy); 
	memcpy(lptstrCopy, clipBox,	wcslen(clipBox) * sizeof(wchar_t) ); 
	lptstrCopy[ wcslen(clipBox) ] = (wchar_t) 0;    // null character 
	GlobalUnlock(hglbCopy); 

	SetClipboardData(CF_UNICODETEXT, hglbCopy);
	CloseClipboard();
    return true;
}

std::ds_wstring  xOperationSys::getClipboardString()
{
	if (!IsClipboardFormatAvailable(CF_UNICODETEXT)) 
		return L""; 
	if (!OpenClipboard(GetActiveWindow())) 
		return L""; 

	HGLOBAL hglb = GetClipboardData(CF_UNICODETEXT); 
	if (hglb != NULL) 
	{ 
		wchar_t* lptstr = (wchar_t*)GlobalLock(hglb); 
		if (lptstr != NULL) 
		{ 
			GlobalUnlock(hglb); 
			CloseClipboard(); 
			return lptstr; 
		} 
	} 
	CloseClipboard(); 

	return L"";
}

#define CNT_CURSORSTYLE 16
static HCURSOR gc_hDefCursor[CNT_CURSORSTYLE] = 
{
	::LoadCursor(0, IDC_ARROW),
	::LoadCursor(0, IDC_CROSS),
	::LoadCursor(0, IDC_NO),
	::LoadCursor(0, IDC_SIZEALL),
	::LoadCursor(0, IDC_SIZENESW),
	::LoadCursor(0, IDC_SIZENS),
	::LoadCursor(0, IDC_SIZENWSE),
	::LoadCursor(0, IDC_SIZEWE),
	::LoadCursor(0, IDC_UPARROW),
	::LoadCursor(0, MAKEINTRESOURCE(32649)/*IDC_HAND*/),
	::LoadCursor(0, IDC_IBEAM),
	::LoadCursor(0, IDC_WAIT),
	::LoadCursor(0, IDC_ICON),
	::LoadCursor(0, IDC_APPSTARTING),
	::LoadCursor(0, MAKEINTRESOURCE(32651)/*IDC_HELP*/),
	::LoadCursor(0, IDC_IBEAM),//::LoadCursor(0, MAKEINTRESOURCE(1027)/*IDC_SPLITER*/),
};

void xOperationSys::setCursorStyle(int nStyle)
{
	if ( nStyle < 0 || nStyle >= CNT_CURSORSTYLE )
		nStyle = 0;
	::SetCursor(gc_hDefCursor[nStyle]);
}

END_NAMESPACE_XEVOL3D
