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
USING_NS_XEVOL3D

class xDsVideoTexturePlugin : public IPluginObject
{
	xVideoTextureLoader*        m_VideoTextureLoader;
	xVideoCaptureTextureLoader* m_CapTextureLoader;
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
		wcsncpy(info->m_Description ,  L"DirectShow Video Texture Plugi" , 256);
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
	void*           createObject(const wchar_t* objName, const wchar_t * arg)
	{
		return NULL;
	}
	const wchar_t** objectList()
	{
		static wchar_t* objectList[1] = {NULL};
		return (const wchar_t**)objectList;
	}
};

extern "C" _declspec(dllexport) IPluginObject* PLUGIN_ENTRYPOINT()
{
	static xDsVideoTexturePlugin gPluginObject;
	return &gPluginObject;
}