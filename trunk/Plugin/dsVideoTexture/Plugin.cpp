#include <BaseLib/xEvol3DBaseInc.h>
#include <Application/xPluginMgr.h>
#include <BaseLib/xStringHash.h>
#include "xRE_VideoTexture.h"
#include "videoTexLoader.h"
#ifdef _DEBUG
#pragma comment(lib,"VideoRenderD.lib")
#else
#pragma comment(lib,"VideoRender.lib")
#endif

#ifdef _WIN32
HANDLE Global_hPluginHandle = NULL;
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
            Global_hPluginHandle = hDllHandle;
        }
        return TRUE;
}
#endif

USING_NS_XEVOL3D
xVideoTextureLoader*        m_VideoTextureLoader;
xVideoCaptureTextureLoader* m_CapTextureLoader;
class xDsVideoTexturePlugin : public IPluginObject
{
	bool            start(const wchar_t* pluginName , const wchar_t* pluginPath)
	{
		m_VideoTextureLoader = new xVideoTextureLoader;
		m_CapTextureLoader   = new xVideoCaptureTextureLoader;
		xTextureLdrMgr::singleton()->addTextureLoader(m_CapTextureLoader);
		xTextureLdrMgr::singleton()->addTextureLoader(m_VideoTextureLoader);
		return true;
	}
	void            info(xPluginInfo* info)
	{
		wcsncpy(info->m_Description ,  L"DirectShow Video Texture name=[dsVideoTexture]" , 256);
		info->m_ID = xStringHash(L"dsVideoTexture Plugin");
		wcsncpy(info->m_Name , L"dsVideoTexture Plugin" , 32);
		info->m_Type =ePT_ObjectCreator;
	}
	bool            stop()
	{
		xTextureLdrMgr::singleton()->removeTextureLoader(m_CapTextureLoader);
		xTextureLdrMgr::singleton()->removeTextureLoader(m_VideoTextureLoader);
		XSAFE_RELEASEOBJECT(m_CapTextureLoader);
		XSAFE_RELEASEOBJECT(m_VideoTextureLoader);
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
	static xDsVideoTexturePlugin gPluginObject;
	return &gPluginObject;
}
#else
bool InitDsVideoTexturePlugin()
{
    m_VideoTextureLoader = new xVideoTextureLoader;
    m_CapTextureLoader   = new xVideoCaptureTextureLoader;
    xTextureLdrMgr::singleton()->addTextureLoader(m_CapTextureLoader);
    xTextureLdrMgr::singleton()->addTextureLoader(m_VideoTextureLoader);
    return true;
}
bool UninitDsVideoTexturePlugin()
{
    xTextureLdrMgr::singleton()->removeTextureLoader(m_CapTextureLoader);
    xTextureLdrMgr::singleton()->removeTextureLoader(m_VideoTextureLoader);
    XSAFE_RELEASEOBJECT(m_CapTextureLoader);
    XSAFE_RELEASEOBJECT(m_VideoTextureLoader);
    return true;
    return true;
}
#endif
