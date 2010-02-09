#pragma once 
#include "BaseLib/xEvol3DBaseInc.h"
#include "../xEvolEnviroment.h"
#include "../xMedusaEditor.h"

class MEDUSA_API CEditorEnv
{
public:
	CEditorEnv();
	~CEditorEnv();
	void             SetRenderWindow(HWND hRenderWindow) { m_hRenderWindow = hRenderWindow ; }
	void             AddDocument(CDocument* pDocument);
	bool             init();
	void             initRenderer(HWND hRenderWindow);
	bool             bInited();
	void             Uninit();
	void             OnIdle();
	CEvolEnviroment* evol3DEnv(){ return  m_Evol3DEnv ; }
	const wchar_t*   sysCfgfile();
	bool             onMessage(xWindowMsg& msg);

	xXmlNode*        sysConfig();
	int              sysValue(const wchar_t* sector , const wchar_t* _value, int defaultValue);
	const wchar_t*   sysValue(const wchar_t* sector , const wchar_t* _value, const wchar_t* defValue);
	float            sysValue(const wchar_t* sector , const wchar_t* _value, float defValue);     
	bool             sysValue(const wchar_t* sector , const wchar_t* _value, bool defValue);

	void             setSysValue(const wchar_t* sector , const wchar_t* _value, int defaultValue);
	void             setSysValue(const wchar_t* sector , const wchar_t* _value, const wchar_t* defValue);
	void             setSysValue(const wchar_t* sector , const wchar_t* _value, float defValue);     
	void             setSysValue(const wchar_t* sector , const wchar_t* _value, bool defValue);
	
	xXmlNode*        sysCfgNode(const wchar_t* sector , bool bCreated = false);
	bool             messageLoop();

public:
	typedef    ds_vector(CDocument*) vDocuments;
	vDocuments m_vDocuments;

protected:
	bool             m_bInited;
	HWND             m_hRenderWindow;
	CEvolEnviroment* m_Evol3DEnv;
	xXmlDocument     m_sysConfig;
};

MEDUSA_API CEditorEnv *     GetEditorEnv();
MEDUSA_API CEvolEnviroment* GetEvol3DEnv();
MEDUSA_API xSceneSelection* GetSceneSelection();