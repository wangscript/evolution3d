#include "./src/stdafx.h"
#include <BaseLib/xEvol3DBaseInc.h>
#include <Application/xPluginMgr.h>
#include <BaseLib/xStringHash.h>
#include "./src/xDirect3D9APICreator.h"
using namespace XEvol3D;
xD3D9RenderApiCreator * g_D3D9RenderApiCreator = NULL;

class xDirect3D9Plugin : public IPluginObject
{
	bool            start(const wchar_t* pluginName , const wchar_t* pluginPath)
	{
		g_D3D9RenderApiCreator = new xD3D9RenderApiCreator;
		return true;
	}
	void            info(xPluginInfo* info)
	{
		wcsncpy(info->m_Description ,  L"Direct3D9 RenderApi name=[D3D9]" , 256);
		info->m_ID = xStringHash(L"Direct3D9 Plugin Plugin");
		wcsncpy(info->m_Name , L"Direct3D9 Plugin Plugin" , 32);
		info->m_Type =ePT_Platform;
	}
	bool            stop()
	{
		delete g_D3D9RenderApiCreator;
		g_D3D9RenderApiCreator = NULL;
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
	static xDirect3D9Plugin gPluginObject;
	return &gPluginObject;
}
#ifdef _WIN32
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
#endif
#else
 __declspec( dllexport ) bool InitDirect3D9Plugin()
{
    g_D3D9RenderApiCreator = new xD3D9RenderApiCreator;
    return true;
}
 __declspec( dllexport ) bool UninitDirect3D9Plugin()
{
    delete g_D3D9RenderApiCreator;
    g_D3D9RenderApiCreator = NULL;
    return true;
}
#endif
