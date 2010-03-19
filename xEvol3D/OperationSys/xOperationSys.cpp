#include "../xStdPch.h"
#include "xOperationSys.h"
#if (defined(_WIN32) || defined(_WIN64) )
#include <windows.h>
extern HANDLE Global_hModuleHandle;
#endif
BEGIN_NAMESPACE_XEVOL3D

void xThreadProc::run(xThreadRunner* pRunner)
{
	m_pRunner = pRunner;
	m_pRunner->run(this);

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

void  xThreadProc::exitThread(bool bWaited)
{
	m_bExitThread = true;
	if(bWaited)
		m_pRunner->wait_exit();
	else
		m_pRunner->exit_Thread(0);
};

static xOperationSys* g_OS = NULL;
xOperationSys* xOperationSys::singleton()
{
	if(g_OS == NULL) g_OS = new xOperationSys;
	return g_OS;
}

std::ds_wstring xOperationSys::getRelatePath(const wchar_t* _filename)
{
	return getRelatePath( _filename , getAppPath() );
}

std::ds_wstring xOperationSys::getRelatePath(const wchar_t* _filename , const wchar_t* _basePath)
{
	std::ds_wstring fileName = _filename;
	std::ds_wstring basePath = _basePath;
	convertSystemFileName(fileName);
	convertSystemFileName(basePath);
	if(basePath[basePath.length() - 1] != PATH_SPLITTER )
	{
		basePath.push_back(PATH_SPLITTER);
	}

	std::ds_wstring::size_type pos = fileName.find(basePath);
	if(pos != 0)
		return L"";
	std::ds_wstring ret = fileName.c_str() + basePath.length();;
	return ret;
}

static std::wstring _GetFilePath(const wchar_t* appName , bool bWithLastSlash = false)
{
	std::wstring _filePath = L"";
	wchar_t appPath[512]={0};
	wcsncpy(appPath , appName , 512);
	int i  = 0;
    bool bLastSlash = true;
	for(i = (int)wcslen(appPath) - 1 ; i >= 0 ; i --  )
	{
		if(appPath[i] == '/' || appPath[i] == '\\') 
		{
			if(bLastSlash && bWithLastSlash) //如果是最后一个Slash，并且允许存在最后的Slash，就继续
            {
                continue;
            }
            //否则终止循环
            appPath[i] = 0;
            break;		
		}
        bLastSlash = false;
	}
	if( i == 0)
	{
		_filePath = PATH_DOTSLASH;
	}
	else
	{
		_filePath = std::wstring(appPath) + PATH_SPLITTER_STRING;
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
	m_AppPath = PATH_DOTSLASH;
    m_ResPath = PATH_DOTSLASH;
#ifdef _WIN32
	wchar_t appName[512] = {0};
    getModuleFileName((HMODULE)Global_hModuleHandle , appName , 512);
	m_AppPath = _GetFilePath(appName);
    m_ResPath = m_AppPath;
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
    if(m_ResPath == L"./" || m_ResPath == L".\\")
        m_ResPath = m_AppPath ;

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
		_strAppPath =  PATH_DOTSLASH;
	else
		_strAppPath =  std::wstring(appPath) + PATH_SPLITTER_STRING;
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
	return getAbsPath(_relPath , getAppPath());
}

bool bIsUpLevelDir( const wchar_t* _relPath )
{
    return (_relPath[0] == '.' &&  _relPath[1] == '.' &&  (_relPath[2] == '/' || _relPath[2] == '\\') );
}

std::ds_wstring  GetUpLevelAbsPath(const wchar_t* _relPath , const wchar_t* pRootPath)
{
     _relPath += 3;
     std::ds_wstring rootPath = _GetFilePath(pRootPath , true).c_str();
     size_t nLen = wcslen(_relPath);
     if(nLen >= 2 && bIsUpLevelDir(_relPath) )
     {
         return GetUpLevelAbsPath(_relPath , rootPath.c_str() );
     }
     
     std::ds_wstring ret  = rootPath + _relPath;
     return ret;
}

std::ds_wstring  xOperationSys::getAbsPath(const wchar_t* _relPath , const wchar_t* pRootPath)
{
    size_t nLen = wcslen(_relPath);
    if(nLen >= 2)
    {
        if(_relPath[1] == '\\' && _relPath[0] == '\\')
        {
            m_LastRet = _relPath;
            convertToSystem(m_LastRet);
            return m_LastRet.c_str();
        }
        if(_relPath[1] == ':' || _relPath[0] == '/'  || _relPath[0] == '\\') //[C:\....]  [/mnt/....]  [\program files\]
        {
            m_LastRet = _relPath;
            convertToSystem(m_LastRet);
            return m_LastRet.c_str();
        }
        else if(_relPath[0] == '.' &&( _relPath[1] == '/' ||  _relPath[1] == '\\' ) ) // ["./xxxxx"]
        {
            m_LastRet = std::ds_wstring( pRootPath ) + &_relPath[2];
            convertToSystem(m_LastRet);
            return m_LastRet.c_str();
        }
        else if( bIsUpLevelDir(_relPath)  ) // ["../xxxxx"]
        {
            m_LastRet = GetUpLevelAbsPath(_relPath , pRootPath);
            convertToSystem(m_LastRet);
            return m_LastRet.c_str();
        }
        else
        {
            m_LastRet = std::ds_wstring( pRootPath ) + _relPath;
            convertToSystem(m_LastRet);
            return m_LastRet.c_str();
        }
    }
    if(nLen == 0 || _relPath[0] == '.')
    {
        m_LastRet = pRootPath;
        convertToSystem(m_LastRet);
        return m_LastRet.c_str();
    }
    m_LastRet = std::ds_wstring( pRootPath ) + _relPath;
    convertToSystem(m_LastRet);
    return m_LastRet.c_str();
}

const wchar_t*  xOperationSys::getWorkPath()
{
	return NULL;
}

const wchar_t*  xOperationSys::getAppPath()
{
	return m_AppPath.c_str();
}
void xOperationSys::setResourcePath(const wchar_t* resourePath)
{
    m_ResPath = resourePath;
}
const wchar_t* xOperationSys::getResourcePath()
{
    return m_ResPath.c_str();
}

std::ds_wstring xOperationSys::getAbsResourcePath(const wchar_t* relPath)
{
    return getAbsPath(relPath , m_ResPath.c_str() );
}

bool xOperationSys::isFolder(const wchar_t* folderName)
{
#ifdef _WIN32
    DWORD   dw=GetFileAttributesW( folderName );   
    if   (dw!=0xffffffff   &&   dw   &   FILE_ATTRIBUTE_DIRECTORY)   
    {   
        return true;
    }   
    else   
    {   
        return false;   
    }  
#else
    int8 mbsFileName[512]={0};
    _xcd_wcsTombs(folderName,mbsFileName,512);
    DIR* dirp = opendir(mbsFileName);
    bool bIsDir = dirp != NULL;
    closedir(dirp);
    return bIsDir;
#endif
}

END_NAMESPACE_XEVOL3D
