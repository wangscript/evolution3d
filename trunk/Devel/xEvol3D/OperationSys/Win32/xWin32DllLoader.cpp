#include "xWin32DllLoader.h"
BEGIN_NAMESPACE_XEVOL3D

xWin32Dll::xWin32Dll()
{
	m_hDLL = NULL;
	m_RefCount = 1;
}
xWin32Dll::~xWin32Dll()
{
    unload();
}

bool xWin32Dll::load(const wchar_t* dllName)
{
	unload();
	m_hDLL = ::LoadLibraryW(dllName);
	return m_hDLL != NULL;  
}

bool xWin32Dll::unload()
{
	if(m_hDLL != NULL)
	{
		::FreeLibrary(m_hDLL);
		m_hDLL = NULL;
		return true;
	}
	return false;
}

void* xWin32Dll::getProcAddress(const wchar_t* funcName)
{
	if(NULL != m_hDLL)
	{
		char mbsFuncName[128]={0};
		BOOL bUsedDefChar = FALSE;
		WideCharToMultiByte(CP_ACP , 0 , funcName , (int)wcslen(funcName) , mbsFuncName , 128 , NULL, &bUsedDefChar);
#ifdef _WIN32_WCE
		return ::GetProcAddress(m_hDLL , funcName );
#else
		return ::GetProcAddress(m_hDLL , mbsFuncName );
#endif
	}
	return NULL;
}

size_t xWin32Dll::release()
{
	m_RefCount -- ;
   if(m_RefCount == 0)
   {
	   delete this;
	   return 0;
   }
   return m_RefCount;
}

size_t xWin32Dll::addRef()
{
	m_RefCount ++ ;
	return m_RefCount;
}

END_NAMESPACE_XEVOL3D
