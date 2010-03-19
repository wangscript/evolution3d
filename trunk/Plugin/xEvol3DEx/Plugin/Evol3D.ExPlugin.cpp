#include "../stdafx.h"

using namespace XEvol3D;

class xEvol3DExPlugin : public IPluginObject
{
public:
	xEvol3DExPlugin()
	{
	}

	bool start(const wchar_t* pluginName , const wchar_t* pluginPath)
	{
		return true;
	}
	void info(xPluginInfo* info)
	{
		wcsncpy(info->m_Description ,  L"xEvol3D Extension name=[xEvol3D.Ex]" , 256);
		info->m_ID = xStringHash(L"xEvol3DEx Plugin");
		wcsncpy(info->m_Name , L"xEvol3DEx Plugin" , 32);
		info->m_Type =ePT_ObjectCreator;
	}
	bool stop()
	{
		return true;
	}
	unsigned int nObject()
	{
		return 0;
	}
	void* createObject(const wchar_t* objName, const void * arg)
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
	static xEvol3DExPlugin gPluginObject;
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

#endif
