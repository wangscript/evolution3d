#include "xStdPch.h"


#include "xCubeDrawable.h"
#include "xPlaneGrid.h"
#include "xArcBall.h"
#include "xBoundBox.h"
using namespace XEvol3D;

//=====================
static wchar_t* g_ObjectList[] = 
{
	L"CubeDrawable",
	L"PlaneGrid",
	L"ArcBall",
	L"BoundBox",
	NULL
};
void __cdecl my_new_handler()
{

    return ;
}

class xEvolHelperObjPlugin : public IPluginObject
{
	bool            start(const wchar_t* pluginName , const wchar_t* pluginPath)
	{
        set_new_handler( my_new_handler );
		return true;
	}
	void            info(xPluginInfo* info)
	{
		wcsncpy(info->m_Description ,  L"xEvol3D Helper Object name=[xEvol3DHelperObject]" , 256);
		info->m_ID = xStringHash(L"Evol3D Helper Object Plugin");
		wcsncpy(info->m_Name , L"Evol3D Helper Object Plugin" , 32);
		info->m_Type = ePT_ObjectCreator;
	}
	bool            stop()
	{
		return true;
	}

	unsigned int    nObject()
	{
		return 4;
	}

	void*           createObject(const wchar_t* objName, const void * arg)
	{
		IBaseRenderer* pRenderer = (IBaseRenderer*)arg;
		if( std::ds_wstring(objName) == g_ObjectList[0] )
			return new xCubeDrawable(pRenderer , 0);
		if( std::ds_wstring(objName) == g_ObjectList[1] )
			return new xPlaneGridDrawable(pRenderer , 0 );
		if( std::ds_wstring(objName) == g_ObjectList[2] )
			return new xArcBallDrawable(pRenderer , 0);
		if( std::ds_wstring(objName) == g_ObjectList[3] )
			return new xBoundBoxHelper(pRenderer , 0);
		return NULL;
	}

	const wchar_t** objectList()
	{

		return (const wchar_t**)g_ObjectList;
	}
};

#ifndef _XEVOL_BUILD_STATIC_PLUGIN_
extern "C" _declspec(dllexport) IPluginObject* PLUGIN_ENTRYPOINT()
{
	static xEvolHelperObjPlugin gPluginObject;
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
bool InitEvolHelperObjPlugin()
{
    return true;
}
bool UninitEvolHelperObjPlugin()
{
    return true;
}
#endif
