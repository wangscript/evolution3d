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

#include "stdafx.h"
#include "MaxEnv.h"
#include "XEvolMaxExporter.h"
#define XEvolMaxExporterUtil_CLASS_ID	Class_ID(0x4e8a3647, 0x68ba4996)

class XEvolMaxExporterUtil : public UtilityObj 
{
public:

	HWND			hPanel;
	IUtil			*iu;
	Interface		*ip;

	void BeginEditParams(Interface *ip,IUtil *iu);
	void EndEditParams(Interface *ip,IUtil *iu);

	void Init(HWND hWnd);
	void Destroy(HWND hWnd);


	void DeleteThis() { }		
	//Constructor/Destructor

	XEvolMaxExporterUtil();
	~XEvolMaxExporterUtil();		

};

static XEvolMaxExporterUtil theXEvolMaxExporterUtil;

class XEvolMaxExporterUtilClassDesc:public ClassDesc2 
{
public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return &theXEvolMaxExporterUtil; }
	const TCHAR *	ClassName() { return _T("PiscesXEvol3D"); }
	SClass_ID		SuperClassID() { return UTILITY_CLASS_ID; }
	char *          GetRsrcString(long){return NULL;}
	Class_ID		ClassID() { return XEvolMaxExporterUtil_CLASS_ID; }
	const TCHAR* 	Category() { return _T("Pisces-XEvol3D"); }

	const TCHAR*	InternalName() { return _T("XEvol3DMaxUtil"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return g_hInstance; }				// returns owning module handle

};



static XEvolMaxExporterUtilClassDesc XEvolMaxExporterUtilDesc;
ClassDesc2* GetXEvolMaxExporterUtilDesc() { return &XEvolMaxExporterUtilDesc; }


bool PannelCommand(HWND hDlg,WPARAM wParam,LPARAM lParam);

static BOOL CALLBACK XEvolMaxExporterUtilDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( )); 
	switch (msg) 
	{
	case WM_INITDIALOG:
        {
            srand(GetTickCount());
            CMaxEnv& maxEnv = CMaxEnv::singleton();
            theXEvolMaxExporterUtil.Init(hWnd);
			maxEnv.start(theXEvolMaxExporterUtil.ip,theXEvolMaxExporterUtil.hPanel);
            CheckDlgButton(hWnd,IDC_BUSE_BEFORESKELETON_BIND , CMaxEnv::singleton().m_bUseBeforeSkeletonPose);
            CheckDlgButton(hWnd,IDC_CALCINITPOS_MYSEF , CMaxEnv::singleton().m_bCalcInitPosMySelf);
            CheckDlgButton(hWnd,IDC_EXPORT_VERTEX_SHELL,CMaxEnv::singleton().m_bExpVertexNormal);
        }
		break;

	case WM_DESTROY:
		{
			CMaxEnv& maxEnv = CMaxEnv::singleton();
		    theXEvolMaxExporterUtil.Destroy(hWnd);
			maxEnv.shutdown();
		}

		break;

	case WM_COMMAND:
		PannelCommand(hWnd,wParam,lParam);
		break;


	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		theXEvolMaxExporterUtil.ip->RollupMouseMessage(hWnd,msg,wParam,lParam); 
		break;

	default:
		return FALSE;
	}
	return TRUE;
}



//--- XEvolMaxExporterUtil -------------------------------------------------------
XEvolMaxExporterUtil::XEvolMaxExporterUtil()
{
	iu = NULL;
	ip = NULL;	
	hPanel = NULL;
}

XEvolMaxExporterUtil::~XEvolMaxExporterUtil()
{

}

void XEvolMaxExporterUtil::BeginEditParams(Interface *ip,IUtil *iu) 
{
#ifdef __SET_LOCALE
	std::locale::global(std::locale(""));
#endif
	this->iu = iu;
	this->ip = ip;
	hPanel = ip->AddRollupPage(
		g_hInstance,
		MAKEINTRESOURCE(IDD_PANEL),
		XEvolMaxExporterUtilDlgProc,
		GetString(IDS_PARAMS),
		0);
}

void XEvolMaxExporterUtil::EndEditParams(Interface *ip,IUtil *iu) 
{
	this->iu = NULL;
	this->ip = NULL;
	ip->DeleteRollupPage(hPanel);
	hPanel = NULL;
}

void XEvolMaxExporterUtil::Init(HWND hWnd)
{

}

void XEvolMaxExporterUtil::Destroy(HWND hWnd)
{

}