#include <BaseLib/xEvol3DBaseInc.h>
#include <Application/xPluginMgr.h>
#include <BaseLib/xStringHash.h>

#pragma warning (disable:4996)

using namespace XEvol3D;
class xWin32PlatformPlugin : public IPluginObject
{
	bool            start(const wchar_t* pluginName , const wchar_t* pluginPath)
	{
		return true;
	}
	void            info(xPluginInfo* info)
	{
		wcsncpy(info->m_Description ,  L"Win32 Platform Plugin name=[Win32]" , 256);
		info->m_ID = xStringHash(L"Win32 Platform Plugin");
		wcsncpy(info->m_Name , L"Win32 Platform Plugin" , 32);
		info->m_Type =ePT_Platform;
	}
	bool            stop()
	{
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
	static xWin32PlatformPlugin gPluginObject;
	return &gPluginObject;
}