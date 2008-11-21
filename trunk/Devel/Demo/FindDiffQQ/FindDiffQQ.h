// FindDiffQQ.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CFindDiffQQApp:
// See FindDiffQQ.cpp for the implementation of this class
//

class CFindDiffQQApp : public CWinApp
{
public:
	CFindDiffQQApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CFindDiffQQApp theApp;