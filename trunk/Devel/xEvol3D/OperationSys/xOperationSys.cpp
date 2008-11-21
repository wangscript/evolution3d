#include "xOperationSys.h"
#ifdef _WIN32
#include <windows.h>
#endif
BEGIN_NAMESPACE_XEVOL3D

void xThreadProc::run(xThreadRunner* pRunner)
{
   pRunner->run(this);
}

xThreadProc::xThreadProc()
{
	m_bExitThread = false; 
	m_pRunner=NULL;
}

xThreadProc::~xThreadProc()
{
	m_bExitThread = false;
}

bool  xThreadProc::bExitThread()
{
	return m_bExitThread ; 
}

void  xThreadProc::exitThread()
{
	m_bExitThread = true;
};
static xOperationSys* g_OS = NULL;
xOperationSys* xOperationSys::singleton()
{
	if(g_OS == NULL) g_OS = new xOperationSys;
	return g_OS;
}

static std::wstring _GetFilePath(const wchar_t* appName)
{
	std::wstring _filePath = L"";
	wchar_t appPath[512]={0};
	wcsncpy(appPath , appName , 512);
	int i  = 0;
	for(i = (int)wcslen(appPath) - 1 ; i >= 0 ; i --  )
	{
		if(appPath[i] == '/' || appPath[i] == '\\') 
		{
			appPath[i] = 0;
			break;
		}
	}
	if( i == 0)
	{
		_filePath = L"./";
	}
	else
	{
		_filePath = std::wstring(appPath) + L"/";
	}
	return _filePath;
}
const wchar_t*  xOperationSys::name()
{
	static wchar_t* _osName = L"Microsoft Windows";
	return _osName;
}

xOperationSys::xOperationSys()
{
	m_AppPath = L"./";
#ifdef _WIN32
	wchar_t appName[512] = {0};
	GetModuleFileName(NULL , appName , 512);
	m_AppPath = _GetFilePath(appName);
#endif

	XEVOL_LOG(eXL_DEBUG_HIGH,L"============XEvol Operation System=================\n");
	XEVOL_LOG(eXL_DEBUG_HIGH,L"XEvol3D is running in %s\n\n" , name() );

	XEVOL_LOG(eXL_DEBUG_HIGH,L"<Importance:>    Don't forget to call the xOperationSys::init()\n");
	XEVOL_LOG(eXL_DEBUG_HIGH,L"             If you doesn't do this , you can heard a program is crashing.....\n");
	XEVOL_LOG(eXL_DEBUG_HIGH,L"                 Also this call should better be the first line\n");
	XEVOL_LOG(eXL_DEBUG_HIGH,L"             in the mian/WinMain function\n");
	XEVOL_LOG(eXL_DEBUG_HIGH,L"\n\n=====================================================\n\n");
	m_pImeInput = NULL;
}

xOperationSys::~xOperationSys()
{

}

bool  xOperationSys::init(const wchar_t* appName)
{
	//Init the AppPath;
	m_AppPath = _GetFilePath(appName);
	return true;	
}

std::ds_wstring    xOperationSys::getPath(const wchar_t* _filename)
{
	static std::wstring _strAppPath;
	wchar_t appPath [256];
	
	wcsncpy(appPath,_filename,256);
	size_t i  = 0;
	for(i = wcslen(appPath) - 1 ; i >= 0 ; i --  )
	{
		if(appPath[i] == '/' || appPath[i] == '\\') 
		{
			appPath[i] = 0;
			break;
		}
	}
	if( i == 0)
		_strAppPath =  L"./";
	else
		_strAppPath =  std::wstring(appPath) + L"/";
	return _strAppPath.c_str();
}



void convertToSystem(std::ds_wstring& str)
{
	for(size_t i = 0 ; i < str.length() ; i ++)
	{
		if(str[i] == '\\' || str[i] == '/')
		{
			str[i] = PATH_SPLITTER;
		}
	}
}

std::ds_wstring  xOperationSys::getAbsPath(const wchar_t* _relPath)
{
	size_t nLen = wcslen(_relPath);
	if(nLen >= 2)
	{
		if(_relPath[1] == '\\' && _relPath[0] == '\\')
		{
			std::ds_wstring str = _relPath;
			convertToSystem(str);
			return str;
		}
		if(_relPath[1] == ':' || _relPath[0] == '/' ) //[C:\....]  [/mnt/....]
		{
			std::ds_wstring str = _relPath;
			convertToSystem(str);
			return str;
		}
		else if(_relPath[0] == '.' &&( _relPath[1] == '/' ||  _relPath[1] == '\\' ) ) // ["./xxxxx"]
		{
			std::ds_wstring str = std::ds_wstring( getAppPath() ) + &_relPath[2];
			convertToSystem(str);
			return str;
		}
		else
		{
			std::ds_wstring str = std::ds_wstring( getAppPath() ) + _relPath;
			convertToSystem(str);
			return str;
		}
	}
	if(nLen == 0 || _relPath[0] == '.')
	{
		std::ds_wstring str = getAppPath();
		convertToSystem(str);
		return str;
	}
	std::ds_wstring str = std::ds_wstring( getAppPath() ) + _relPath;
	convertToSystem(str);
	return str;
}

const wchar_t*  xOperationSys::getWorkPath()
{
     return NULL;
}

const wchar_t*  xOperationSys::getAppPath()
{
     return m_AppPath.c_str();
}



END_NAMESPACE_XEVOL3D
