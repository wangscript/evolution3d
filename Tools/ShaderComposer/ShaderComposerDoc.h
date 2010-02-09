
// ShaderComposerDoc.h : interface of the CShaderComposerDoc class
//


#pragma once


class CShaderComposerDoc : public CDocument
{
protected: // create from serialization only
	CShaderComposerDoc();
	DECLARE_DYNCREATE(CShaderComposerDoc)

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
	virtual ~CShaderComposerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


