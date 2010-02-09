#include <BaseLib/xEvol3DBaseInc.h>
#include <Application/xPluginMgr.h>
#include <BaseLib/xStringHash.h>

using namespace XEvol3D;

//=====================
static wchar_t* g_ObjectList[] = 
{
	NULL
};

class xEvolPythonObjPlugin : public IPluginObject
{
	bool            start(const wchar_t* pluginName , const wchar_t* pluginPath)
	{
		return true;
	}
	void            info(xPluginInfo* info)
	{
		wcsncpy(info->m_Description ,  L"Evol3D Python Object" , 256);
		info->m_ID = xStringHash(L"Evol3D Python Object Plugin");
		wcsncpy(info->m_Name , L"Evol3D Python Object Plugin" , 32);
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

	void*           createObject(const wchar_t* objName, const wchar_t * arg)
	{
		return NULL;
	}

	const wchar_t** objectList()
	{

		return (const wchar_t**)g_ObjectList;
	}
};

extern "C" _declspec(dllexport) IPluginObject* PLUGIN_ENTRYPOINT()
{
	static xEvolPythonObjPlugin gPluginObject;
	return &gPluginObject;
}