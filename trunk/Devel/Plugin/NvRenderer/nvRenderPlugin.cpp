#include <BaseLib/xEvol3DBaseInc.h>
#include <Application/xPluginMgr.h>
#include <BaseLib/xStringHash.h>

#include "./src/nvRenderer.h"
using namespace XEvol3D;



class xNvRenderer10Plugin : public IPluginObject
{
	xNvRenderer* m_pRenderer;
public:
	xNvRenderer10Plugin()
	{
		m_pRenderer = NULL;
	}

	bool start(const wchar_t* pluginName , const wchar_t* pluginPath)
	{
		if(m_pRenderer == NULL)
		{
			m_pRenderer  = new xNvRenderer;
		}
		return true;
	}
	void info(xPluginInfo* info)
	{
		wcsncpy(info->m_Description ,  L"NvRenderer Plugin name=[NvRenderer]" , 256);
		info->m_ID = xStringHash(L"NvRenderer Plugin");
		wcsncpy(info->m_Name , L"vRenderer Plugin" , 32);
		info->m_Type =ePT_Renderer;
	}
	bool stop()
	{
		if(m_pRenderer != NULL)
		{
			delete m_pRenderer;
			m_pRenderer = NULL;
		}
		return true;
	}
	unsigned int nObject()
	{
		return 0;
	}
	void* createObject(const wchar_t* objName, const wchar_t * arg)
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
	static xNvRenderer10Plugin gPluginObject;
	return &gPluginObject;
}