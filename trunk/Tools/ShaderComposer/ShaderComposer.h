
// ShaderComposer.h : main header file for the ShaderComposer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "ShaderComposerImpl.h"
#include <vector>
// CShaderComposerApp:
// See ShaderComposer.cpp for the implementation of this class
//


class CShaderComposerApp : public CWinAppEx , public CShaderComposerUIImpl
{
public:
	CShaderComposerApp();


// Overrides
public:
	virtual BOOL InitInstance();
    int   ShowShaderComposer();
	int   HideShaderComposer();
// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
protected:
	typedef std::vector<CMultiDocTemplate*>  vDocTemplates;
	vDocTemplates m_Doctemplates;

public:
	afx_msg void OnNewShader();
};

extern CShaderComposerApp theApp;
