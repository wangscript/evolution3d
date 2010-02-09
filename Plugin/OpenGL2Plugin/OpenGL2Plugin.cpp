#include <BaseLib/xEvol3DBaseInc.h>
#include <Application/xPluginMgr.h>
#include <BaseLib/xStringHash.h>
#include "./src/xOpenGL2APICreator.h"
using namespace XEvol3D;
xGL2RenderApiCreator * g_GL2RenderApiCreator = NULL;

class xOpenGL2Plugin : public IPluginObject
{
	bool            start(const wchar_t* pluginName , const wchar_t* pluginPath)
	{
		g_GL2RenderApiCreator = new xGL2RenderApiCreator;
		return true;
	}
	void            info(xPluginInfo* info)
	{
		wcsncpy(info->m_Description ,  L"OpenGL2 RenderApi name=[OpenGL2RenderApi]" , 256);
		info->m_ID = xStringHash(L"OpenGL2 Plugin Plugin");
		wcsncpy(info->m_Name , L"OpenGL2 Plugin Plugin" , 32);
		info->m_Type =ePT_Platform;
	}
	bool            stop()
	{
		delete g_GL2RenderApiCreator;
		g_GL2RenderApiCreator = NULL;
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


#ifdef _XEVOL_BUILD_STATIC_PLUGIN_
extern "C" _declspec(dllexport) IPluginObject* PLUGIN_ENTRYPOINT()
{
	static xOpenGL2Plugin gPluginObject;
	return &gPluginObject;
}
#else

#endif
