#include "../../xStdPch.h"
#ifdef _UNIX
#include "xWin32DllLoader.h"

BEGIN_NAMESPACE_XEVOL3D

xLinuxDll::xLinuxDll()
{
	m_hDLL = NULL;
	m_RefCount = 1;
}
xLinuxDll::~xLinuxDll()
{
    unload();
}

bool xLinuxDll::load(const wchar_t* dllName)
{
	unload();

	char mbsdllName[XEVOL_MAX_PATH]={0};
    XEvol_UnicodeToFsEnc(dllName , mbsdllName , XEVOL_MAX_PATH);
	m_hDLL = dlopen( mbsdllName, RTLD_LAZY );
	return m_hDLL != NULL;  
}

bool xLinuxDll::unload()
{
	if(m_hDLL != NULL)
	{
		dlclose(m_hDLL );
		m_hDLL = NULL;
		return true;
	}
	return false;
}

void* xLinuxDll::getProcAddress(const wchar_t* funcName)
{
	if(NULL != m_hDLL)
	{
		char mbsFuncName[128]={0};
		XEvol_UnicodeToFsEnc(funcName,mbsFuncName,128);
        return dlsym(m_hDLL , mbsFuncName);
	}
	return NULL;
}

size_t xLinuxDll::Release()
{
	m_RefCount -- ;
   if(m_RefCount == 0)
   {
	   delete this;
	   return 0;
   }
   return m_RefCount;
}

size_t xLinuxDll::AddRef()
{
	m_RefCount ++ ;
	return m_RefCount;
}


END_NAMESPACE_XEVOL3D

#endif

