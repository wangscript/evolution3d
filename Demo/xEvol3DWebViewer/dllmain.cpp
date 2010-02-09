// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "xEvol3DWebViewer_i.h"
#include "dllmain.h"

CxEvol3DWebViewerModule _AtlModule;
HINSTANCE Global_hModuleHandle ;
// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	Global_hModuleHandle = hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
