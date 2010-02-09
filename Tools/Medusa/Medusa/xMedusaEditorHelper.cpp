#include "stdafx.h"
#include "xMedusaEditorHelper.h"
USING_NS_MDED
CMEdUIElConatiner::CMEdUIElConatiner()
{
}

CMEdUIElConatiner::~CMEdUIElConatiner()
{
	m_vElements.clear();
}

bool CMEdUIElConatiner::fireMEdUIEvent(eMEUIEvent _event , int param, int param2)
{
	bool bRet = false;
	for(size_t i= 0 ; i < m_vElements.size() ; i ++)
	{
		bool bRet1 = m_vElements[i]->onMEdUIEvent(_event , param , param2);
		bRet = bRet1 || bRet;
	}
	return bRet;
}

bool  CMEdUIElConatiner::AttachUIElement(nsMedusaEditor::IMEdUIElement* pPane)
{
	if(pPane == NULL)
		return false;
	pPane->AddRef();
	pPane->createMEdUI( GetParentWnd() );
	pPane->onAttach();
	m_vElements.push_back(pPane);
	return true;
} 
bool  CMEdUIElConatiner::DetachUIElement()
{
	vMEdUIElements::iterator pos = m_vElements.begin();
	for(; pos != m_vElements.end() ; pos ++)
	{
            nsMedusaEditor::IMEdUIElement* pPane = *pos;
			pPane->destroyMEdUI();
			pPane->ReleaseObject();
			return true;
	
	}
	m_vElements.clear();
	return true;
}
bool  CMEdUIElConatiner::DetachUIElement(nsMedusaEditor::IMEdUIElement* pPane)
{
	if(pPane == NULL)
		return false;

	vMEdUIElements::iterator pos = m_vElements.begin();
	for(; pos != m_vElements.end() ; pos ++)
	{
		if( *pos == pPane)
		{
			m_vElements.erase(pos);
			pPane->destroyMEdUI();
			pPane->ReleaseObject();
			return true;
		}
	}
	return false;
}
