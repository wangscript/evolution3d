
// ShaderComposerDoc.cpp : implementation of the CShaderComposerDoc class
//

#include "stdafx.h"
#include "ShaderComposer.h"

#include "ShaderComposerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CShaderComposerDoc

IMPLEMENT_DYNCREATE(CShaderComposerDoc, CDocument)

BEGIN_MESSAGE_MAP(CShaderComposerDoc, CDocument)
END_MESSAGE_MAP()


// CShaderComposerDoc construction/destruction

CShaderComposerDoc::CShaderComposerDoc()
{
	// TODO: add one-time construction code here

}

CShaderComposerDoc::~CShaderComposerDoc()
{
}

BOOL CShaderComposerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CShaderComposerDoc serialization

void CShaderComposerDoc::Serialize(CArchive& ar)
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


// CShaderComposerDoc diagnostics

#ifdef _DEBUG
void CShaderComposerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CShaderComposerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CShaderComposerDoc commands
