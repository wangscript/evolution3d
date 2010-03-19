// ExcelWriter.h : main header file for the ExcelWriter DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CExcelWriterApp
// See ExcelWriter.cpp for the implementation of this class
//

class CExcelWriterApp : public CWinApp
{
public:
	CExcelWriterApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
