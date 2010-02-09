#include "stdafx.h"
#include "xMedusaEditorImpl.h"
#include "../EditorFramework/MainFrm.h"
#include "../EditorUIPlugin/ToolBarElement.h"


CMedusaMainUI::CMedusaMainUI()
{
	m_pMainFrame = NULL;
	m_IDManager.setBase(ID_PLUGIN_ID_0);
	for(int i = 0 ; i < MAX_UNDO ; i ++)
	{
		m_CommandQueue[i] = UNDO_QUEUE_MASK;
	}
}

CMedusaMainUI::~CMedusaMainUI()
{

}

bool CMedusaMainUI::PushMessageListenner(IMEdUIMessageListenner* pListener)
{
	if(m_Listeners.size() > 0 )
	{
		m_Listeners[m_Listeners.size() - 1]->onActive(false);
	}
	m_Listeners.push_back(pListener);
	pListener->onActive(true);
	return true;
}

void CMedusaMainUI::PopMessageListenner()
{
    m_Listeners.pop_back();
}

IMEdUIMessageListenner*  CMedusaMainUI::GetMessageListenner()
{
	if(m_Listeners.size() == 0)
		return NULL;
	return m_Listeners[ m_Listeners.size() - 1] ;
}

bool CMedusaMainUI::FireWindowMsg(NS_XEVOL3D::xWindowMsg& msg)
{
    if(m_Listeners.size() == 0)
		return false;
	return m_Listeners[ m_Listeners.size() - 1]->onMessage(msg);
}

void CMedusaMainUI::_DetachUIElementImpl( nsMedusaEditor::IMEdUIElement* pPane ) 
{
	vMEdUIElements::iterator pos = m_vElements.begin();
	for(; pos != m_vElements.end() ; pos ++)
	{
		if( *pos == pPane)
		{
			m_vElements.erase(pos);
			pPane->destroyMEdUI();
			pPane->ReleaseObject();
			return ;
		}
	}
}
bool CMedusaMainUI::DetachUIElement()
{
	vMEdUIElements::iterator pos = m_vElements.begin();
	for(; pos != m_vElements.end() ; pos ++)
	{
		nsMedusaEditor::IMEdUIElement* pPane = *pos;
		pPane->DetachUIElement();
		pPane->destroyMEdUI();
		pPane->ReleaseObject();
	}
	m_vElements.clear();
	return true;
}

void  CMedusaMainUI::AttachUIElement(nsMedusaEditor::IMEdUIElement* pPane)
{
	if(pPane == NULL)
		return ;
    IMEdDockPane* pDockPane = dynamic_cast<IMEdDockPane*>(pPane);
	if(pDockPane)
	{
         return AttachDockPane(pDockPane);
	}

	//
	pPane->AddRef();
	pPane->createMEdUI(m_pMainFrame->GetSafeHwnd() );
	pPane->onAttach();
	m_vElements.push_back(pPane);
	return ;
} 

void  CMedusaMainUI::DetachUIElement(nsMedusaEditor::IMEdUIElement* pPane)
{
	if(pPane == NULL)
		return ;

	IMEdDockPane* pDockPane = dynamic_cast<IMEdDockPane*>(pPane);
    if(pDockPane)
	{
		return DetachDockPane(pDockPane);
	}

	_DetachUIElementImpl(pPane);
	return ;
}

void CMedusaMainUI::AttachDockPane(nsMedusaEditor::IMEdDockPane* pPane)
{
	if(pPane == NULL)
		return ;

	if(m_pMainFrame == NULL)
		return ;

	pPane->AddRef();
	pPane->createMEdUI(m_pMainFrame->GetSafeHwnd() );
	m_pMainFrame->AttachDockPane(pPane);
	pPane->onAttach();
	m_vElements.push_back(pPane);
}

void CMedusaMainUI::DetachDockPane(nsMedusaEditor::IMEdDockPane* pPane)
{
	if(pPane == NULL)
		return ;

	if(m_pMainFrame == NULL) return ;
	m_pMainFrame->DetachDockPane(pPane);
	_DetachUIElementImpl(pPane);
}

CMEdUiToolBarInfo* CMedusaMainUI::FindToolbar(const wchar_t* _tbName)
{
	vMEUIToolbars::iterator pos = m_vToolbars.find(_tbName);
	if(pos != m_vToolbars.end() )
	{
		return pos->second;
	}
	return NULL;
}

CMEdUiToolBarInfo* CMedusaMainUI::FindToolbarByCmdID(int cmdID)
{
	vMEUIToolbars::iterator pos = m_vToolbars.begin();
	for(;pos != m_vToolbars.end() ; pos ++)
	{
		CMEdUiToolBarInfo* pToolbar = pos->second;
		if(pToolbar->IsToolbarCmd(cmdID))
			return pToolbar;
	}
	return NULL;
}

void CMedusaMainUI::RegisteToolbar(CMEdUiToolBarInfo* _toolbar)
{
	this->m_vToolbars.insert( vMEUIToolbars::value_type( std::wstring(_toolbar->m_name) , _toolbar) );
}

void CMedusaMainUI::UnregisteToolbar(const wchar_t* _tbName)
{
	DestroyToolbar(_tbName);
    this->m_vToolbars.erase(_tbName);
}

bool CMedusaMainUI::CreateToolbar(const wchar_t* _tbName)
{
	if(_tbName == NULL)
		return false;
    CMEdUiToolBarInfo* pToolbar = FindToolbar(_tbName);
	if(pToolbar == NULL)
		return false;

	CMEdUiToolBar* pMFCToolBar = new CMEdUiToolBar(pToolbar);
    AttachUIElement(pMFCToolBar);
	pToolbar->m_hToolbar = (void*)pMFCToolBar;
	return true;
}
bool CMedusaMainUI::DestroyToolbar()
{
	vMEUIToolbars::iterator pos = m_vToolbars.begin();
	for(;pos != m_vToolbars.end() ; pos ++ )
	{
		CMEdUiToolBarInfo* pToolbar = pos->second;
		if(pToolbar == NULL || pToolbar->m_hToolbar == NULL)
			continue;
		CMEdUiToolBar* pMFCToolBar = (CMEdUiToolBar*)(pToolbar->m_hToolbar );
		DetachUIElement(pMFCToolBar);
		pMFCToolBar->ReleaseObject();
		pToolbar->m_hToolbar = NULL;
	}
	return NULL;
}

bool CMedusaMainUI::DestroyToolbar(const wchar_t* _tbName)
{
	CMEdUiToolBarInfo* pToolbar = FindToolbar(_tbName);
	if(pToolbar == NULL)
		return false;

	CMEdUiToolBar* pMFCToolBar = (CMEdUiToolBar*)(pToolbar->m_hToolbar );
	if(pMFCToolBar == NULL)
		return false;
	DetachUIElement(pMFCToolBar);
	pMFCToolBar->ReleaseObject();
	pToolbar->m_hToolbar = NULL;
	return true;
}
bool CMedusaMainUI::fireMEdUIEvent(eMEUIEvent _event , int param, int param2)
{
	bool bRet = false;
	for(size_t i= 0 ; i < m_vElements.size() ; i ++)
	{
		bool bRet1 = m_vElements[i]->onMEdUIEvent(_event , param , param2);
		bRet = bRet || bRet1;
	}
	return bRet;
}
bool CMedusaMainUI::HideToolbar(const wchar_t* _tbName)
{
	CMEdUiToolBarInfo* pToolbar = FindToolbar(_tbName);
	if(pToolbar == NULL)
		return false;

	CMEdUiToolBar* pMFCToolBar = (CMEdUiToolBar*)pToolbar->m_hToolbar;
    if(pMFCToolBar)
	{
		pMFCToolBar->HideMEdUI();
	}
	return true;
}

bool CMedusaMainUI::ShowToolbar(const wchar_t* _tbName)
{
	CMEdUiToolBarInfo* pToolbar = FindToolbar(_tbName);
	if(pToolbar == NULL)
		return false;

	CMEdUiToolBar* pMFCToolBar = (CMEdUiToolBar*)pToolbar->m_hToolbar;
	if(pMFCToolBar)
	{
		pMFCToolBar->ShowMEdUI();
	}
	return true;
}

int CMedusaMainUI::AllocUIID(int nId)
{
	return  m_IDManager.getID(nId);
}

IMEdUIStatusBar* CMedusaMainUI::GetStatusBar()
{
    return m_pMainFrame->GetStatusBarImpl();
}

//编辑器对象
void CMedusaEditor::SetMainFrame(CMainFrame*   pMainFrame)
{
	m_MainUI.m_pMainFrame = pMainFrame;
}

bool CMedusaMainUI::CanRedo()
{
	if(m_CommandQueue[m_CmdQueueLast] == UNDO_QUEUE_MASK)
		return false;
	if(m_CommandQueue[m_CmdQueueLast] == NULL)
		return false;
	return true;
}

bool CMedusaMainUI::Redo()
{
	if(CanRedo() == false)
		return false;

	m_CommandQueue[m_CmdQueueLast]->Redo();
	int idxNext = (m_CmdQueueLast + 1) % MAX_UNDO;
	m_CmdQueueLast = idxNext ;
	return true;
}

bool CMedusaMainUI::CanUndo()
{
     int idxPrev = (m_CmdQueueLast - 1)%MAX_UNDO;
	 if(idxPrev < 0 ) idxPrev += MAX_UNDO;
	 if(m_CommandQueue[idxPrev] == UNDO_QUEUE_MASK)
		 return false;
	 if(m_CommandQueue[idxPrev] == NULL)
		 return false;
	 return true;
}

bool CMedusaMainUI::Undo()
{
	if(CanUndo() == false)
		return false;

	int idxPrev = (m_CmdQueueLast - 1)%MAX_UNDO;
	if(idxPrev < 0 ) idxPrev += MAX_UNDO;
	m_CommandQueue[idxPrev]->Undo();
	m_CmdQueueLast = idxPrev;
	return true;
}

bool CMedusaMainUI::InsertCommand(IMEdUICommand* pCommand)
{
	//插入进来.需要干掉后面所有存在的Undo. 一直干到NULL.标记的结束位置
	for(int i = 0 ; i < MAX_UNDO ; i ++)
	{
		int idx = i + m_CmdQueueLast ;
		idx = idx % MAX_UNDO;
		if(m_CommandQueue[idx] == UNDO_QUEUE_MASK)
			break;
		XSAFE_RELEASEOBJECT(m_CommandQueue[idx]);
		m_CommandQueue[idx] = UNDO_QUEUE_MASK;
	}

	//检查下一个
    int idxNext = (m_CmdQueueLast + 1) % MAX_UNDO;
	if(m_CommandQueue[idxNext] != UNDO_QUEUE_MASK ) //有效指针
	{
		XSAFE_RELEASEOBJECT(m_CommandQueue[idxNext]);
	}

	m_CommandQueue[m_CmdQueueLast] = pCommand;
	m_CmdQueueLast = idxNext ;
	m_CommandQueue[idxNext] = UNDO_QUEUE_MASK;  //下一个就变成有用的了。
	return true;
}