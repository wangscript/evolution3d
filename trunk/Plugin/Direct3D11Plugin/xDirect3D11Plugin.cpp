#include "./src/stdafx.h"
#include <BaseLib/xEvol3DBaseInc.h>
#include <Application/xPluginMgr.h>
#include <BaseLib/xStringHash.h>
#include "./src/xDirect3D11APICreator.h"
typedef int (__cdecl *new_hand)(size_t);
new_hand __cdecl _set_new_handler(new_hand);

using namespace XEvol3D;
xD3D11RenderApiCreator * g_D3D11RenderApiCreator = NULL;
int __cdecl my_new_handler(size_t _size)
{
    wchar_t buf[1024] = {0};
    XEVOL_LOG(eXL_ERROR_FALT , "Falt error ,dx11 renderapi  out of memory size=%d" , _size);
    return 0;
}

class xDirect3D11Plugin : public IPluginObject
{
	bool            start(const wchar_t* pluginName , const wchar_t* pluginPath)
	{
        _set_new_handler( my_new_handler );
		g_D3D11RenderApiCreator = new xD3D11RenderApiCreator;
		return true;
	}
	void            info(xPluginInfo* info)
	{
		wcsncpy(info->m_Description ,  L"Direct3D11 RenderApi name=[D3D11]" , 256);
		info->m_ID = xStringHash(L"Direct3D11 Plugin Plugin");
		wcsncpy(info->m_Name , L"Direct3D11 Plugin Plugin" , 32);
		info->m_Type =ePT_Platform;
	}
	bool            stop()
	{
		delete g_D3D11RenderApiCreator;
		g_D3D11RenderApiCreator = NULL;
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
	static xDirect3D11Plugin gPluginObject;
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
 __declspec( dllexport ) bool InitDirect3D11Plugin()
{
    g_D3D11RenderApiCreator = new xD3D11RenderApiCreator;
    return true;
}
 __declspec( dllexport ) bool UninitDirect3D11Plugin()
{
    delete g_D3D11RenderApiCreator;
    g_D3D11RenderApiCreator = NULL;
    return true;
}
#endif
