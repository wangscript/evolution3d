// ResuorcePlugin.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "xMedusaEditor.h"
#include "xMedusaApp.h"
USING_NS_MDED
#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int WINAPI WinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance,  LPSTR lpCmdLine,   int nCmdShow   )
{
	IMedusaApplication* pApp = GetMedusaApp();
	pApp->SetAppName(L"WorldComposer");	
	pApp->SetWorkDir(L"WorldComposer");
	pApp->SetUILayout(L"Configure/uidef.xml",L"Default" , true);
	pApp->Start();
    pApp->messageLoop();
	pApp->Exit();
	return 0;
}

