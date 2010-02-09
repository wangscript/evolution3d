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

#include "SceneXMLDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMedusaDoc

IMPLEMENT_DYNCREATE(CSceneXMLDoc, CDocument)

BEGIN_MESSAGE_MAP(CSceneXMLDoc, CDocument)
END_MESSAGE_MAP()


// CMedusaDoc construction/destruction

CSceneXMLDoc::CSceneXMLDoc()
{
	// TODO: add one-time construction code here

}

CSceneXMLDoc::~CSceneXMLDoc()
{
}

BOOL CSceneXMLDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMedusaDoc serialization

void CSceneXMLDoc::Serialize(CArchive& ar)
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
void CSceneXMLDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSceneXMLDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMedusaDoc commands
