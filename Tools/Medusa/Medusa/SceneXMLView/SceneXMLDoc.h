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

// MedusaDoc.h : interface of the CMedusaDoc class
//


#pragma once


class CSceneXMLDoc : public CDocument
{
protected: // create from serialization only
	CSceneXMLDoc();
	DECLARE_DYNCREATE(CSceneXMLDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CSceneXMLDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace)
	{
		return TRUE;
	}
	virtual BOOL DoFileSave()
	{
		return TRUE;
	}

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


