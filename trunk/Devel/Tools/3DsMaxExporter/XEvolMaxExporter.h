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

// XEvolMaxExporter.h : main header file for the XEvolMaxExporter DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <Max.h>
#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <utilapi.h>
#include <modstack.h>
#include <stdmat.h>
#include <iskin.h>
//Charactor Studio SDK
#include <BIPEXP.H>
#include <Phyexp.h>
//#include "wm3.h"
#include <IGame/IGame.h>
#include <IGame/IGameModifier.h>



extern TCHAR *GetString(int id);
extern HINSTANCE g_hInstance;

class CXEvolMaxExporterApp : public CWinApp
{
public:
	CXEvolMaxExporterApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
