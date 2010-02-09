// DemoPlugin.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "DemoPlugin.h"
#include "Resource.h"
#include <BaseLib/xEvol3DBaseInc.h>
#include <Application/xPluginMgr.h>
#include <BaseLib/xStringHash.h>
#include "xMedusaEditor.h"
using namespace nsMedusaEditor;
#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CDemoPluginApp
BEGIN_MESSAGE_MAP(CDemoPluginApp, CWinApp)
END_MESSAGE_MAP()

CMEdUiToolBarInfo g_TestTollbar;
// CDemoPluginApp construction

CDemoPluginApp::CDemoPluginApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CDemoPluginApp object

CDemoPluginApp theApp;


// CDemoPluginApp initialization

BOOL CDemoPluginApp::InitInstance()
{
	CWinApp::InitInstance();

	g_TestTollbar.m_hDll = AfxGetResourceHandle();
	wcscpy(g_TestTollbar.m_name , L"Toolbar.Plugin");
	wcscpy(g_TestTollbar.m_title , L"没名字的Toolbar");
	g_TestTollbar.m_ResID = IDR_TOOLBAR1;

	return TRUE;
}

using namespace XEvol3D;

class xTestResourcePlugin : public IPluginObject
{

	class CMyCallback :   public CMEdUiToolBarInfo::MEdUIToolbarCallback
	{
	public:
		virtual CMEdUiToolBarInfo::CommandUIStatus OnUpdateCommandUI(int ctrlID , int ctrlIdx)
		{
			return CMEdUiToolBarInfo::CUS_ENABLE;
		}
		virtual bool            OnCommand(int ctrlID , int ctrlIdx)
		{
			switch(ctrlID)
			{
			case ID_BUTTON32771:
				MessageBox(GetActiveWindow() , TEXT("你知道我是谁么？我是第一个") , TEXT("嘿嘿") , MB_OK);
				break;
			case ID_BUTTON32772:
				MessageBox(GetActiveWindow() , TEXT("你知道我是谁么？我是第二个") , TEXT("嘿嘿") , MB_OK);
				break;
			case ID_BUTTON32773:
				MessageBox(GetActiveWindow() , TEXT("你知道我是谁么？我是第三个") , TEXT("嘿嘿") , MB_OK);
				break;
			}
			return true;
		}
	};
	CMyCallback     g_Cb;

	bool            start(const wchar_t* pluginName , const wchar_t* pluginPath)
	{
		g_TestTollbar.m_funcCallback = &g_Cb;
		g_TestTollbar.ProcessToolbarID(-1);
		GetMedusaEditor()->GetUI()->RegisteToolbar(&g_TestTollbar);
		return true;
	}

	void            info(xPluginInfo* info)
	{
		wcsncpy(info->m_Description ,  L"ResourcePlugin.Test name=[Test]" , 256);
		info->m_ID = xStringHash(L"ResourcePlugin.Test");
		wcsncpy(info->m_Name , L"ResourcePlugin.Test Plugin" , 32);
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
	static xTestResourcePlugin gPluginObject;
	return &gPluginObject;
}
#else

#endif
