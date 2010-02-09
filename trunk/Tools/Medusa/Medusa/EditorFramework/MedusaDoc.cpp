// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MedusaDoc.cpp : implementation of the CMedusaDoc class
//

#include "stdafx.h"
#include "Medusa.h"

#include "MedusaDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMedusaDoc

IMPLEMENT_DYNCREATE(CMedusaDoc, CDocument)

BEGIN_MESSAGE_MAP(CMedusaDoc, CDocument)
END_MESSAGE_MAP()


// CMedusaDoc construction/destruction

CMedusaDoc::CMedusaDoc()
{
	// TODO: add one-time construction code here

}

CMedusaDoc::~CMedusaDoc()
{
}

BOOL  CMedusaDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	std::ds_wstring fileName = L"";
	if(lpszPathName == NULL)
	{
		const TCHAR szFilters[]=_T("场景xml文件 files (*.xml)|*.xml|编译过的场景文件 files (*.evs)|*.evs|All Files (*.*)|*.*||");
		CFileDialog dlg(FALSE, _T("xml"), _T("*.xml"), OFN_EXPLORER | OFN_HIDEREADONLY, szFilters);
		if(dlg.DoModal() != IDOK)
		{
			return FALSE;
		}
		fileName = dlg.m_ofn.lpstrFile;
	}
	else
	{
		fileName = lpszPathName;
	}

	CMedusaApp* pApp = &theApp;
	CEditorEnv* pEditorEnv = dynamic_cast<CEditorEnv*>(pApp);
	CEvolEnviroment* pEvol3DEnv = pEditorEnv->evol3DEnv();

	pEvol3DEnv->scene()->save( fileName.c_str() ) ;

	return TRUE;
}

BOOL  CMedusaDoc::DoFileSave()
{
	CMedusaApp* pApp = &theApp;
	CEditorEnv* pEditorEnv = dynamic_cast<CEditorEnv*>(pApp);
	CEvolEnviroment* pEvol3DEnv = pEditorEnv->evol3DEnv();
	const wchar_t* pFileName = pEvol3DEnv->sceneFile();
	return DoSave(pFileName,TRUE);
}

BOOL CMedusaDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMedusaDoc serialization

void CMedusaDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CMedusaDoc diagnostics

#ifdef _DEBUG
void CMedusaDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMedusaDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMedusaDoc commands
