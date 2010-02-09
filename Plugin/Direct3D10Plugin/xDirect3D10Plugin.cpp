#include "./src/stdafx.h"
#include <BaseLib/xEvol3DBaseInc.h>
#include <Application/xPluginMgr.h>
#include <BaseLib/xStringHash.h>
#include "./src/xDirect3D10APICreator.h"
using namespace XEvol3D;
xD3D10RenderApiCreator * g_D3D10RenderApiCreator = NULL;

class xDirect3D10Plugin : public IPluginObject
{
	bool            start(const wchar_t* pluginName , const wchar_t* pluginPath)
	{
		g_D3D10RenderApiCreator = new xD3D10RenderApiCreator;
		return true;
	}
	void            info(xPluginInfo* info)
	{
		wcsncpy(info->m_Description ,  L"Direct3D10 RenderApi name=[D3D10]" , 256);
		info->m_ID = xStringHash(L"Direct3D10 Plugin Plugin");
		wcsncpy(info->m_Name , L"Direct3D10 Plugin Plugin" , 32);
		info->m_Type =ePT_Platform;
	}
	bool            stop()
	{
		delete g_D3D10RenderApiCreator;
		g_D3D10RenderApiCreator = NULL;
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
	static xDirect3D10Plugin gPluginObject;
	return &gPluginObject;
}
#else
 __declspec( dllexport ) bool InitDirect3D10Plugin()
{
    g_D3D10RenderApiCreator = new xD3D10RenderApiCreator;
    return true;
}
 __declspec( dllexport ) bool UninitDirect3D10Plugin()
{
    delete g_D3D10RenderApiCreator;
    g_D3D10RenderApiCreator = NULL;
    return true;
}
#endif
