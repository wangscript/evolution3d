#include <BaseLib/xEvol3DBaseInc.h>
#include <Application/xPluginMgr.h>
#include <BaseLib/xStringHash.h>

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

class xEvolHelperObjPlugin : public IPluginObject
{
	bool            start(const wchar_t* pluginName , const wchar_t* pluginPath)
	{
		return true;
	}
	void            info(xPluginInfo* info)
	{
		wcsncpy(info->m_Description ,  L"Evol3D Helper Object" , 256);
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

	void*           createObject(const wchar_t* objName, const wchar_t * arg)
	{
		if( std::ds_wstring(objName) == g_ObjectList[0] )
			return new xCubeDrawable;
		if( std::ds_wstring(objName) == g_ObjectList[1] )
			return new xPlaneGridDrawable;
		if( std::ds_wstring(objName) == g_ObjectList[2] )
			return new xArcBallDrawable;
		if( std::ds_wstring(objName) == g_ObjectList[3] )
			return new xBoundBoxHelper;
		return NULL;
	}

	const wchar_t** objectList()
	{

		return (const wchar_t**)g_ObjectList;
	}
};

extern "C" _declspec(dllexport) IPluginObject* PLUGIN_ENTRYPOINT()
{
	static xEvolHelperObjPlugin gPluginObject;
	return &gPluginObject;
}