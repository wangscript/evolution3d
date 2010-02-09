#pragma once
#include "xMedusaEditor.h"

class MEDUSA_API CMEdUIElConatiner : public nsMedusaEditor::IMEdUIElement
{
	//变量定义
public:
	typedef std::vector<nsMedusaEditor::IMEdUIElement*> vMEdUIElements;
public:
	virtual bool             AttachUIElement(IMEdUIElement* pPane) ;
	virtual bool             DetachUIElement(IMEdUIElement* pPane) ;
	virtual bool             DetachUIElement();
	virtual HWND             GetParentWnd()  = 0;
	bool                     fireMEdUIEvent(nsMedusaEditor::eMEUIEvent _event , int param, int param2 = 0);
    CMEdUIElConatiner();
	~CMEdUIElConatiner();
public:
	vMEdUIElements           m_vElements;
};