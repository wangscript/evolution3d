/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

// XEvol3DMaxExporter.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "XEvolMaxExporter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma comment(lib,"../Morpher.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"bmm.lib") 
#pragma comment(lib,"core.lib") 
#pragma comment(lib,"geom.lib") 
#pragma comment(lib,"gfx.lib") 
#pragma comment(lib,"mesh.lib") 
#pragma comment(lib,"maxutil.lib") 
#pragma comment(lib,"maxscrpt.lib") 
#pragma comment(lib,"gup.lib") 
#pragma comment(lib,"paramblk2.lib")
#pragma comment(lib,"IGame.lib")

BEGIN_MESSAGE_MAP(CXEvolMaxExporterApp, CWinApp)
END_MESSAGE_MAP()


extern ClassDesc2* GetXEvolMaxExporterUtilDesc();

HINSTANCE g_hInstance;
// CXEvolMaxExporterApp construction

CXEvolMaxExporterApp::CXEvolMaxExporterApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CXEvolMaxExporterApp object

CXEvolMaxExporterApp theApp;


// CXEvolMaxExporterApp initialization

BOOL CXEvolMaxExporterApp::InitInstance()
{
	CWinApp::InitInstance();

    g_hInstance = m_hInstance;
	return TRUE;
}

extern "C"
{

    __declspec( dllexport ) const TCHAR* LibDescription()
    {
        return GetString(IDS_LIBDESCRIPTION);
    }

    __declspec( dllexport ) int LibNumberClasses()
    {
        return 1;
    }

    __declspec( dllexport ) ClassDesc* LibClassDesc(int i)
    {
        switch(i) {
        case 0: return GetXEvolMaxExporterUtilDesc();
        default: return 0;
        }
    }

    __declspec( dllexport ) ULONG LibVersion()
    {
        return VERSION_3DSMAX;
    }
}

TCHAR *GetString(int id)
{
    static TCHAR buf[256];

    if (g_hInstance)
        return LoadString(g_hInstance, id, buf, sizeof(buf)) ? buf : NULL;
    return NULL;
}
