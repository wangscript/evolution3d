#include <BaseLib/xEvol3DBaseInc.h>
#include <Application/xPluginMgr.h>
#include <BaseLib/xStringHash.h>
#include "xKidRenderCreator.h"
using namespace XEvol3D;

xKidRenderCreator * g_RendererCreator = NULL;

class xKidRenderCreatorPlugin : public IPluginObject
{
	bool            start(const wchar_t* pluginName , const wchar_t* pluginPath)
	{
		g_RendererCreator = new xKidRenderCreator;
		return true;
	}
	void            info(xPluginInfo* info)
	{
		wcsncpy(info->m_Description ,  L"KidsRenderer Plugin name=[KidsRenderer]" , 256);
		info->m_ID = xStringHash(L"KidsRenderer Plugin : a simple renderer");
		wcsncpy(info->m_Name , L"KidsRenderer Plugin" , 32);
		info->m_Type =ePT_Renderer;
	}
	bool            stop()
	{
		delete g_RendererCreator;
		g_RendererCreator = NULL;
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
	static xKidRenderCreatorPlugin gPluginObject;
	return &gPluginObject;
}
#else
 __declspec( dllexport ) bool InitKidRenderCreatorPlugin()
{
    g_RendererCreator = new xKidRenderCreator;
    return true;
}
 __declspec( dllexport ) bool UninitKidRenderCreatorPlugin()
{
    delete g_RendererCreator;
    g_RendererCreator = NULL;
    return true;
}
#endif