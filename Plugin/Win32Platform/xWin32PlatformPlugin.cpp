#include "xStdPch.h"
#include <BaseLib/xEvol3DBaseInc.h>
#include <Application/xPluginMgr.h>
#include <BaseLib/xStringHash.h>
#include "xWin32Platform.h"
#pragma warning (disable:4996)

using namespace XEvol3D;
class xWin32PlatformPlugin : public IPluginObject
{
	bool            start(const wchar_t* pluginName , const wchar_t* pluginPath)
	{
        new PlatformWin32Factory;
		return true;
	}
	void            info(xPluginInfo* info)
	{
		wcsncpy(info->m_Description ,  L"Win32 Platform name=[Win32]" , 256);
		info->m_ID = xStringHash(L"Win32 Platform Plugin");
		wcsncpy(info->m_Name , L"Win32 Platform Plugin" , 32);
		info->m_Type =ePT_Platform;
	}
	bool            stop()
	{
		return true;
	}
	unsigned int    nObject()
	{
		return 0;
	}
	void*           createObject(const wchar_t* objName, const void * arg)
	{
		return NULL;
	}
	const wchar_t** objectList()
	{
		static wchar_t* objectList[1] = {NULL};
		return (const wchar_t**)objectList;
	}
};

#ifndef _XEVOL_BUILD_STATIC_PLUGIN_
extern "C" _declspec(dllexport) IPluginObject* PLUGIN_ENTRYPOINT()
{
	static xWin32PlatformPlugin gPluginObject;
	return &gPluginObject;
}

HANDLE Global_hModuleHandle = NULL;
BOOL WINAPI DllMain(HANDLE  hDllHandle,   DWORD   dwReason,    LPVOID  lpreserved     )
{
        if (dwReason == DLL_PROCESS_ATTACH)
        {
        /*
         * The /GS security cookie must be initialized before any exception
         * handling targetting the current image is registered.  No function
         * using exception handling can be called in the current image until
         * after __security_init_cookie has been called.
         */
            Global_hModuleHandle = hDllHandle;
        }
        return TRUE;
}

#else
 __declspec( dllexport ) bool InitWin32PlatformPlugin()
{
    new PlatformWin32Factory;
    return true;
}
 __declspec( dllexport ) bool UninitWin32PlatformPlugin()
{
    return true;
}
#endif