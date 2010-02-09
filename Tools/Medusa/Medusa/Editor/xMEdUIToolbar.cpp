#include "stdafx.h"
#include "xMedusaEditorImpl.h"
#include "../EditorFramework/MainFrm.h"
#include "../EditorUIPlugin/ToolBarElement.h"
#include "../EditorFramework/MainFrm.h"
bool CMEdUiToolBarInfo::IsToolbarCmd(int cmdId)
{
	if(cmdId >= m_startID + m_nButton)
		return false;
	if(cmdId < m_startID )
		return false;
	return true;
}

int CMEdUiToolBarInfo::GetCommandID(int cmdID)
{
	int idx = cmdID - m_startID;
	return m_OriginBtnIDs[idx];
}

CMEdUiToolBarInfo::CMEdUiToolBarInfo()
{
	m_funcCallback = NULL;
	m_OriginBtnIDs = NULL;
}

CMEdUiToolBarInfo::~CMEdUiToolBarInfo()
{
	if(m_OriginBtnIDs)
	{
		delete [] m_OriginBtnIDs;
		m_OriginBtnIDs = NULL;
	}

}

void CMEdUiToolBarInfo::ProcessToolbarID(int startID)
{

	m_startID = startID;

	struct CToolBarData
	{
		WORD wVersion;
		WORD wWidth;
		WORD wHeight;
		WORD wItemCount;

		WORD items[1];
	};


	HINSTANCE hOldDll = AfxGetResourceHandle();
	AfxSetResourceHandle(m_hDll);

	LPCTSTR lpszResourceName = MAKEINTRESOURCE( m_ResID );
	ENSURE(lpszResourceName != NULL);

	// determine location of the bitmap in resource fork:
	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_TOOLBAR);
	HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_TOOLBAR);
	if (hRsrc == NULL)
		goto EXIT_SET;

	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		goto EXIT_SET;

	CToolBarData* pData = (CToolBarData*)LockResource(hGlobal);
	if (pData == NULL)
		goto EXIT_SET;
	ASSERT(pData->wVersion == 1);

	//开始分配Toolbar新的ID;
	m_nButton = pData->wItemCount;
	if(m_startID == -1)
	{
		m_startID = GetMedusaEditor()->GetUI()->AllocUIID( m_nButton );
	}
	m_OriginBtnIDs = new int[m_nButton];
	for(int i = 0 ;i < pData->wItemCount ; i ++)
	{
		//保存原始ID
		m_OriginBtnIDs[i] = pData->items[i];
		pData->items[i] = i + m_startID;
	}
	UnlockResource(hGlobal);
EXIT_SET:	
	FreeResource(hGlobal);
	AfxSetResourceHandle(hOldDll);
	return ;

}

void CMEdUiToolBarInfo::ProcessMenuID(int startID)
{

    return ;
    m_startID = startID;

    struct CToolBarData
    {
        WORD wVersion;
        WORD wWidth;
        WORD wHeight;
        WORD wItemCount;

        WORD items[1];
    };


    HINSTANCE hOldDll = AfxGetResourceHandle();
    AfxSetResourceHandle(m_hDll);

    LPCTSTR lpszResourceName = MAKEINTRESOURCE( m_ResID );
    ENSURE(lpszResourceName != NULL);

    // determine location of the bitmap in resource fork:
    HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_MENU);
    HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_MENU);
    if (hRsrc == NULL)
        goto EXIT_SET;

    HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
    if (hGlobal == NULL)
        goto EXIT_SET;

    CToolBarData* pData = (CToolBarData*)LockResource(hGlobal);
    if (pData == NULL)
        goto EXIT_SET;
    ASSERT(pData->wVersion == 1);

    //开始分配Toolbar新的ID;
    m_nButton = pData->wItemCount;
    if(m_startID == -1)
    {
        m_startID = GetMedusaEditor()->GetUI()->AllocUIID( m_nButton );
    }
    m_OriginBtnIDs = new int[m_nButton];
    for(int i = 0 ;i < pData->wItemCount ; i ++)
    {
        //保存原始ID
        m_OriginBtnIDs[i] = pData->items[i];
        pData->items[i] = i + m_startID;
    }
    UnlockResource(hGlobal);
EXIT_SET:	
    FreeResource(hGlobal);
    AfxSetResourceHandle(hOldDll);
    return ;

}

//http://hi.baidu.com/dining/blog/item/004a9fee61a74d2b2df534bd.html