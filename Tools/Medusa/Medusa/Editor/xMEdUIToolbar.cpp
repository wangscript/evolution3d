#include "stdafx.h"
#include "xMedusaEditorImpl.h"
#include "../EditorFramework/MainFrm.h"
#include "../EditorUIPlugin/ToolBarElement.h"
#include "../EditorFramework/MainFrm.h"
#include "BaseLib/xXmlDocument.h"
#include <atlxml.h>
USING_NS_XEVOL3D;
using namespace ATL;

bool CMEdUiToolBarInfo::IsToolbarCmd(int cmdId)
{
	if(cmdId >= m_startID + m_nButton)
		return false;
	if(cmdId < m_startID )
		return false;
	return true;
}

int CMEdUiToolBarInfo::GetOriginID(int cmdID)
{
	int idx = cmdID - m_startID;
	return m_OriginBtnIDs[idx];
}

int CMEdUiToolBarInfo::MapOriginID2CtrlID(int originID)
{
   for(int i = 0 ; i < m_nButton ; i ++)
   {
       if(m_OriginBtnIDs[i] == originID)
           return m_startID + i;
   }
   return -1;
}

CMEdUiToolBarInfo::CMEdUiToolBarInfo()
{
	m_funcCallback = NULL;
	m_OriginBtnIDs = NULL;
    m_Type = eToolBarTypeNone;
    m_funcCallback;
    m_name[0] = 0;
    m_title[0] = 0;
    m_hDll  = NULL;
    m_ResID = 0;
    m_hToolbar = NULL;
}

CMEdUiToolBarInfo::~CMEdUiToolBarInfo()
{
	if(m_OriginBtnIDs)
	{
		delete [] m_OriginBtnIDs;
		m_OriginBtnIDs = NULL;
	}

}

 CMEdUiToolBarInfo::ToolBarType   CMEdUiToolBarInfo::GetType()
 {
     if(m_Type == eToolBarTypeNone)
     {
         LPCTSTR lpszResourceName = MAKEINTRESOURCE( m_ResID );
         if(lpszResourceName == NULL)
         {
             return m_Type;
         }
         //自动检测类型
         {
             HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_TOOLBAR);
             HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_TOOLBAR);
             if(hRsrc) m_Type = eToolBar; return m_Type ;
         }

         {
             HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_RIBBON);
             HRSRC hRsrc = ::FindResource(hInst, lpszResourceName    , RT_RIBBON);
             if(hRsrc) m_Type = eRibbonBar; return m_Type ;
         }

         {
             HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_MENU);
             HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_MENU);
             if(hRsrc) m_Type = eMenuBar; return m_Type ;
         }
     }
     return m_Type;
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

void  CMEdUiToolBarInfo::FreeData(void* pData)
{
    __int8* pUtf8Data = (__int8*)pData;
    delete [] pUtf8Data;
}

typedef std::vector< IXMLDOMNode* > vXMLDOMNodes;

void  findRibbonIDs(CXMLDocument pRootNode , vXMLDOMNodes& _ret)
{
    CXMLNode<IXMLDOMNode> childNode;
    CXMLNode<IXMLDOMElement> element ;
    pRootNode.GetDocumentElement(element);

    IXMLDOMNodeList* NodeList = NULL;
    CXMLNode<IXMLDOMNode> pElement = (IXMLDOMElement*)element;
    //pElement.SelectNodes(L"ID" , &NodeList );
    ((IXMLDOMElement*)element)->getElementsByTagName(L"ID" , &NodeList);
    long _NumNode = 0 ;
    NodeList->get_length( &_NumNode );
    for(int i = 0 ; i < _NumNode ; i ++)
    {
         IXMLDOMNode* pNode = NULL;
         NodeList->get_item(i , &pNode);
         
         CXMLNode<IXMLDOMNode> Node = pNode;
         
         IXMLDOMNode* pParentNode = NULL;
         pNode->get_parentNode( &pParentNode ) ; 

         CXMLNode<IXMLDOMNode> ParentNode = pParentNode;

         CString _ParentName = _T("");
         ParentNode.GetName( _ParentName );

         if(_ParentName == L"IMAGE" || _ParentName == L"IMAGE_SMALL" || _ParentName == L"IMAGE_LARGE"  )
         {
             pParentNode->Release();
             pNode->Release();
             continue;
         }

         IXMLDOMNodeList* ValueList = NULL;
         pNode->get_childNodes(&ValueList);
         long nValue = 0;
         ValueList->get_length(&nValue);
         for(int k = 0 ; k < nValue ; k ++)
         {
             IXMLDOMNode* pValueNode  = NULL;
             ValueList->get_item(k , &pValueNode );
             CXMLNode<IXMLDOMNode> ValueNode = pValueNode;
             CString _Name = _T("");
             ValueNode.GetName(_Name);
             if(_Name == L"VALUE")
             {
                _ret.push_back( pValueNode );
             }
         }
         ValueList->Release();
    }
    

    NodeList->Release();
}


void SetToUtf16Encoding(ATL::CXMLDocument& doc)
{
    CXMLNode<IXMLDOMNode> oldInsctructionNode;
    doc.GetFirstChild(oldInsctructionNode);

    IXMLDOMNode* pOldInsNode = (IXMLDOMNode*)oldInsctructionNode;

    IXMLDOMDocument* pDoc = doc;
    //pDoc->removeChild(pChildNode , NULL);
    IXMLDOMProcessingInstruction* pInstruction = NULL;
    pDoc->createProcessingInstruction( _T("xml"), _T("version='1.0' encoding='UTF-16'")  , &pInstruction );

    HRESULT hr = pDoc->replaceChild( pInstruction , pOldInsNode , NULL);

    //pOldInsNode->Release();
    return ;
}
void* CMEdUiToolBarInfo::ProcessRibbonBarData(int startID)
{
    ATL::CXMLDocument doc;
    HINSTANCE hOldDll             = NULL;
    LPCTSTR      lpszResourceName = NULL;
    HINSTANCE    hInst            = NULL;
    HRSRC        hRsrc            = NULL;
    HGLOBAL      hGlobal          = NULL;
    int          _len             = 0;
    char*        pData            =  NULL;
    char*        newBuffer        = NULL;
    UINT         newBufLen        = 0;
    LPBYTE       newXmlBuffer     = NULL;
    vXMLDOMNodes _IdNodes;

	m_startID                     = startID;
    

	hOldDll = AfxGetResourceHandle();
	AfxSetResourceHandle(m_hDll);

	lpszResourceName = MAKEINTRESOURCE( m_ResID );
	ENSURE(lpszResourceName != NULL);

	// determine location of the bitmap in resource fork:
	hInst = AfxFindResourceHandle(lpszResourceName, RT_RIBBON);
	hRsrc = ::FindResource(hInst, lpszResourceName, RT_RIBBON);
	if (hRsrc == NULL)
		goto EXIT_SET;

	hGlobal = LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		goto EXIT_SET;

    _len  =  SizeofResource(hInst , hRsrc);
    pData =  (char*)LockResource(hGlobal);

	if (pData == NULL)
		goto EXIT_SET;

    if( S_OK != doc.Load( (LPBYTE)pData , _len ) )
    {
        goto EXIT_SET;
    }
    
    findRibbonIDs(doc , _IdNodes);
    
    //开始分配Toolbar新的ID;
    if(m_startID == -1)
    {
        m_startID = GetMedusaEditor()->GetUI()->AllocUIID( m_nButton );
    }

    m_nButton = _IdNodes.size();
    m_OriginBtnIDs = new int[m_nButton];

    for(int i = 0 ;i < m_nButton ; i ++)
	{
		//保存原始ID
        CXMLNode<IXMLDOMNode> Node = _IdNodes[i];
        CString _text;
        Node.GetText(_text);
        int v = 0;
        swscanf(_text.GetBuffer() , L"%d" , &v);
        m_OriginBtnIDs[i] = v;

        int newV = i + m_startID;
        _text.Format(L"%d" , newV );
        BSTR pNewValue = _text.AllocSysString();
        IXMLDOMNode * pNode = _IdNodes[i];
		pNode->put_text( pNewValue  );
        SysFreeString(pNewValue);
	}


    newBufLen = 0;

    SetToUtf16Encoding(doc);
    doc.Save(&newXmlBuffer , newBufLen);

    
 
    //newBufLen = (int)(_len * 1.3f);
    newBuffer = new char[newBufLen + 2];
    ZeroMemory(newBuffer , newBufLen + 2 );
    memcpy(newBuffer , newXmlBuffer , newBufLen );


	UnlockResource(hGlobal);
EXIT_SET:	
	FreeResource(hGlobal);
    AfxSetResourceHandle(hOldDll);
	return newBuffer;

}


//http://hi.baidu.com/dining/blog/item/004a9fee61a74d2b2df534bd.html