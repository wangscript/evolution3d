/*
XReal3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xreal3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "../../xStdPch.h"
#include "../xuiWindowManager.h"
#include "../xuiDialog.h"
#include "../xuiHotArea.h"
#include "../../RenderAPI/xShaderManager.h"
BEGIN_NAMESPACE_XUI

class xuiWMUpdateObject : public IFrameUpdateObject
{
    bool              m_bTurnOn;
    xuiWindowManager* m_pWindowManager;
public:
    xuiWMUpdateObject(xuiWindowManager* pWindowManager);
    ~xuiWMUpdateObject();
    const wchar_t* name();
    bool           updateFrame(int passedTime , unsigned int thisTime);
    void           turnOn(bool bTurnOn);
    bool           isTurnOn();
};

xuiWMUpdateObject::xuiWMUpdateObject(xuiWindowManager* pWindowManager)
{
    m_pWindowManager = pWindowManager;
    IPlatform::singleton()->getApplication()->addFrameUpdateObj(this);
    m_bTurnOn = true;
};

xuiWMUpdateObject::~xuiWMUpdateObject()
{
    if(IPlatform::singleton() && IPlatform::singleton()->getApplication())
    {
        IPlatform::singleton()->getApplication()->removeFrameUpdateObj(this);
    }

};
const wchar_t* xuiWMUpdateObject::name()
{
    return L"xuiWindowManager";
}

bool  xuiWMUpdateObject::updateFrame(int passedTime , unsigned int thisTime)
{
    return m_pWindowManager->updateFrame(passedTime , thisTime);
}

void  xuiWMUpdateObject::turnOn(bool bTurnOn)
{
    m_bTurnOn = bTurnOn;
}
bool  xuiWMUpdateObject::isTurnOn()
{
    return m_bTurnOn;
}

//=============================================
class xuiWindowLess
{
public:
    bool operator () (const xuiWindow* lhv , const xuiWindow*  rhv)
    {
        return lhv->zValue() < rhv->zValue();
    }
};

class xuiWindowGreator
{
public:
    bool operator () (const xuiWindow* lhv , const xuiWindow*  rhv)
    {
        return lhv->zValue() > rhv->zValue();
    }
};
class xuiWindowEqual
{
public:
    bool operator () (const xuiWindow* lhv , const xuiWindow*  rhv)
    {
        return lhv->zValue() == rhv->zValue();
    }
};

//=============================================

void xuiWindowManager::addHotArea(xuiHotArea* pHotArea)
{
    vHotAreas::iterator i = m_vHotAreas.begin();
    for(; i < m_vHotAreas.end() ; i ++)
    {
        xuiHotArea* pHotAreaInVec = *i;
        if(pHotAreaInVec == pHotArea) return ;
    }
    m_vHotAreas.push_back(pHotArea);
}

void xuiWindowManager::removeHotArea(xuiHotArea* pHotArea)
{
    vHotAreas::iterator i = m_vHotAreas.begin();
    for(; i < m_vHotAreas.end() ; i ++)
    {
        xuiHotArea* pHotAreaInVec = *i;
        if(pHotAreaInVec == pHotArea)
        {
            m_vHotAreas.erase(i);
            return ;
        }
    }
}

bool xuiWindowManager::onCommand(xuiDialog* pDialog , eXUIWinCommand  _command, xuiWindow* pCtrl, int param1 , int param2)
{
    if(m_commandProc == NULL)
    {
        XEVOL_LOG(eXL_DEBUG_NORMAL,"{warning:} xuiWindowManager recive a command , but no command handler installed\n");
        return false;
    }
    return (*m_commandProc)(pDialog , _command, pCtrl, param1 , param2);
}
void xuiWindowManager::setCommandProc(xuiCommandProc* proc)
{
    m_commandProc = proc;
}
void xuiWindowManager::hideDialog(xuiDialog* pWindow)
{
    vDialogs::iterator i = m_visibleDialog.begin();
    for(; i < m_visibleDialog.end() ; i ++)
    {
        xuiDialog* pDialog = *i;
        if(pDialog == pWindow)
        {
            pWindow->onHide();
            xuiWindowStateBlender* pBlend = pWindow->getStateBlender();
            if(pBlend &&  pBlend->isInBlending())
            {
                m_blendingDialogs.push_back(pWindow);
            }
            m_visibleDialog.erase(i);
            return ;
        }
    }
}
IMPL_REFCOUNT_OBJECT_FUNCTION(xuiWindowManager);

void xuiWindowManager::showDialog(xuiDialog* pWindow)
{
    vDialogs::iterator i = m_visibleDialog.begin();
    for(; i < m_visibleDialog.end() ; i ++)
    {
        xuiDialog* pDialog = *i;
        if(pDialog == pWindow)
        {
            return ;
        }
    }

    //Remove the Window from blendingDialog
    for(i = m_blendingDialogs.begin() ; i < m_blendingDialogs.end() ; i ++)
    {
        xuiDialog* pDialog = *i;
        if(pDialog == pWindow)
        {
            m_blendingDialogs.erase(i);
            return ;
        }
    }

    m_visibleDialog.push_back(pWindow);
    pWindow->onShow();
}
xuiWindowManager* xuiWindowManager::createManager(IRenderApi* pRenderApi , xBaseTextureMgr* pTexMgr)
{
    return new xuiWindowManager(pRenderApi , pTexMgr);
}

xuiWindowManager::xuiWindowManager(IRenderApi* pRenderApi , xBaseTextureMgr* pTexMgr)
:m_XMLManager(L"UIXMLManager" , 1)
{
    m_RefCount = 1;
    m_pRenderApi  = pRenderApi;
    m_pTextureMgr = pTexMgr;
    m_AutoUpdater = new xuiWMUpdateObject(this);
    m_AutoUpdater->turnOn(true);
    turnOn(true);
    m_thisTime = 0;
    m_pActiveWindow      = NULL;
    m_pMouseFocusWindow  = NULL;
    m_commandProc        = NULL;

    m_p2DRenderer   = new x2DRenderer(pRenderApi);

}

xuiWindowManager::~xuiWindowManager()
{
    delete m_AutoUpdater;
    m_AutoUpdater = NULL;
}


bool xuiWindowManager::onMouseMsg(xMouseMsg& mEvent)
{
	size_t _nVisibleDialog = m_visibleDialog.size() ;
    for(size_t i = 0 ; i < _nVisibleDialog ; ++i)
    {
        xuiDialog* pDlg = m_visibleDialog[i];
        if(pDlg->hitTest(mEvent.x , mEvent.y) > eXUIWHS_OutWindow)
        {
            return pDlg->_onMouseMsg(mEvent);
        }
    }
    if(mEvent.MsgID != WIN_MOUSE_MOVE &&  mEvent.MsgID != WIN_MOUSE_WHEEL)
    {
        setActiveDialog(NULL,false);
    }
    setMouseFocus(NULL,false);


    //处理所有注册的热区
    vHotAreas::iterator i = m_vHotAreas.begin();
    for(; i < m_vHotAreas.end() ; i ++)
    {
        xuiHotArea* pHotArea = *i;
        if(pHotArea->isEnable() && pHotArea->hitTest(mEvent.x , mEvent.y) )
        {
            pHotArea->fire(mEvent.MsgID);
        }

    }

    return false;
}


bool xuiWindowManager::onViewportResize(int w , int h)
{
	size_t _nDlg = m_Dialogs.size() ;
    for(size_t i = 0 ; i < _nDlg ; ++i)
    {
        xuiDialog* pDialog = m_Dialogs[i];
        pDialog->notify(eXUIWN_Sys_Resize,NULL,w , h);
    }
    return false;
}

bool xuiWindowManager::onKeyboardMsg(xKeyboardMsg& msg)
{
    if(m_pActiveWindow)
    {
        m_pActiveWindow->_onKeyboardMsg(msg);
    }
    return false;
}


bool xuiWindowManager::onMessage(xWindowMsg& msg)
{
    switch(msg.MsgID)
    {
    case WIN_LMOUSE_DOWN:
    case WIN_LMOUSE_RELEASE:
    case WIN_RMOUSE_DOWN:
    case WIN_RMOUSE_RELEASE:
    case WIN_MMOUSE_DOWN:
    case WIN_MMOUSE_RELEASE:
    case WIN_MOUSE_MOVE:
    case WIN_MOUSE_WHEEL:
    case WIN_LMOUSE_DB_CLICK:
    case WIN_RMOUSE_DB_CLICK:
    case WIN_MMOUSE_DB_CLICK:
        return onMouseMsg(msg.Mouse);
        break;

    case WIN_KEYUP:
    case WIN_KEYDOWN:
        return onKeyboardMsg(msg.Keyboard);
        break;
    case WIN_RESIZE:
        return onViewportResize(msg.Common.w , msg.Common.h );
        break;
    }
    return false;
}

xXMLDocMgr* xuiWindowManager::getXMLManager()
{
    return &m_XMLManager;
}


xuiWindow* xuiWindowManager::createControl(const wchar_t* _typeName,xuiWindow* parent)
{
    xuiWindowCreator* pWinCreator = xuiWindowCreator::findWindowCreater(_typeName);
    xuiWindow* pControl = NULL;
    if(pWinCreator == NULL) 
        return pControl;
    pControl =  pWinCreator->createInstance(parent , this);
    return pControl;
}
xuiDialog* xuiWindowManager::createDialog(const wchar_t* _typeName,xuiWindow* parent)
{
    xuiWindowCreator* pWinCreator = xuiWindowCreator::findWindowCreater(_typeName);
    if(pWinCreator == NULL) 
        return NULL;
    const wchar_t* pXMLFile = pWinCreator->scriptFile();
    if(pXMLFile)
    {
        return createDialogFromFile(pXMLFile , parent);
    }

    xuiDialog* pDialog = (xuiDialog*)pWinCreator->createInstance(parent , this);
    m_Dialogs.push_back(pDialog);
    int w = 0 ; int h = 0;
    m_pRenderApi->getWindowSize(w , h);
    pDialog->onNotify(eXUIWN_Sys_Resize,NULL, w ,  h);
    return pDialog;
}

xuiDialog* xuiWindowManager::createDialogFromFile(const wchar_t* _xuiScript,xuiWindow* parent)
{
    HXMLDoc hXML =  m_XMLManager.add(_xuiScript, true , true);
    if( hXML.isHandle() )
    {
        xXmlDocument* pDocument = hXML.getResource();
        if(pDocument == NULL)
        {
            return NULL;
        }
        else
        {
            xXmlNode* pNode = pDocument->findNode(L"dialog");
            if(pNode == NULL)
            {
                return NULL;
            }
            xXmlValue* pTypeNode = pNode->findValue(L"type");
            const wchar_t* typeName = L"dialog";
            if(pTypeNode) typeName = pTypeNode->value();
            xuiWindowCreator* pWinCreator = xuiWindowCreator::findWindowCreater(typeName);
            if(pWinCreator == NULL) 
                return NULL;

            xuiDialog* pDialog = pDialog =  (xuiDialog*)pWinCreator->createInstance(parent , this);
            pDialog->load(pNode);
            m_Dialogs.push_back(pDialog);
            int w = 0 ; int h = 0;
            m_pRenderApi->getWindowSize(w , h);
            pDialog->onNotify(eXUIWN_Sys_Resize,NULL, w ,  h);
            hXML.unload();
            return pDialog;
        }
        return NULL;
    }
    return NULL;
}

void xuiWindowManager::draw()
{
	for(int i = 0 ; i < (int)m_visibleDialog.size() ; i ++)
	{
		//if(m_visibleDialog[i]->getStateBlender() == NULL || m_visibleDialog[i]->getStateBlender()->isInBlending() == false)
		m_visibleDialog[i]->drawWindow();
	}

	for(int i = 0 ; i < (int)m_blendingDialogs.size() ; i ++)
	{
		m_blendingDialogs[i]->drawWindow();
	}
}

bool xuiWindowManager::drawRectf(int nTextureLayer , IBaseTexture* pTexture[], float vDestRect[4] , const xColor_4f& color)
{
    return m_p2DRenderer->drawRectf(nTextureLayer , pTexture , vDestRect , color);
}

bool xuiWindowManager::drawRectf(int nTextureLayer , IBaseTexture* pTexture[], float vDestRect[4] , float vSrcRect[4]  , const xColor_4f& color)
{
    return m_p2DRenderer->drawRectf(nTextureLayer , pTexture , vDestRect , vSrcRect , color);
}

bool xuiWindowManager::drawRectf(I2DRectObject*   p2DRectObject)
{
  return m_p2DRenderer->drawRectf(p2DRectObject);
}

xuiRect xuiWindowManager::clipRect(xuiRect& _baseRect , xuiRect& _rect)
{
    xuiRect _out = _rect;

    //裁减掉下半。
    if(_out.rect.right() > _baseRect.rect.right() )
    {
        _out.rect.w -= (_out.rect.right() - _baseRect.rect.right() );
    }

    if(_out.rect.bottom() > _baseRect.rect.bottom() )
    {
        _out.rect.h -= (_out.rect.bottom() - _baseRect.rect.bottom() );
    }


    if(_out.rect.left() < _baseRect.rect.left() )
    {
        _out.rect.x -= _baseRect.rect.right() ;
    }

    if(_out.rect.top() < _baseRect.rect.top() )
    {
        _out.rect.y -= _baseRect.rect.top() ;
    }

    if(_out.rect.left() > _baseRect.rect.right()) 
    {
        _out.rect.x = _baseRect.rect.right();
        _out.rect.w = 0;
    }

    if(_out.rect.top() > _baseRect.rect.bottom() ) 
    {
        _out.rect.y = _baseRect.rect.bottom() ;
        _out.rect.h = 0;
    }


    return _out;
}

bool xuiWindowManager::updateFrame(int passedTime , unsigned int thisTime)
{
    m_thisTime = thisTime;
    //对可见对话框进行排序
    sort(m_visibleDialog.begin(),m_visibleDialog.end(),xuiWindowGreator());

    //处理所有的对话框
	size_t _nDlg = m_Dialogs.size() ;
    for(size_t i = 0 ; i < _nDlg ;  i ++)
    {
        m_Dialogs[i]->updateFrame(passedTime , thisTime);
    }

    //处理blending状态为激活的对话框
    vDialogs::iterator i = m_blendingDialogs.begin();
    for(; i < m_blendingDialogs.end() ; )
    {
        xuiDialog* pDialog = *i;
        xuiWindowStateBlender* pBlend = pDialog->getStateBlender();
        if(pBlend &&  pBlend->isInBlending() == false )
        {
            i = m_blendingDialogs.erase(i);
            break;
        }
        else
        {
            ++ i;
        }
    }
    return true;
}
unsigned  int xuiWindowManager::getTime()
{
    return m_thisTime;
}

void xuiWindowManager::setMouseFocus(xuiWindow* pWindow,bool bFocus)
{
    xuiWindow* pOldFocusWindow = m_pMouseFocusWindow;
    if(bFocus == true)
    {
        if(m_pMouseFocusWindow != pWindow)
        {
            m_pMouseFocusWindow = pWindow;
            if(pOldFocusWindow) pOldFocusWindow->onLostMouseFocus();
            if(m_pMouseFocusWindow) m_pMouseFocusWindow->onMouseFocus();
        }
    }
    else
    {
        if(m_pMouseFocusWindow) m_pMouseFocusWindow->onLostMouseFocus();
        m_pMouseFocusWindow = NULL;
    }	
}

xuiWindow* xuiWindowManager::getMouseFocus()
{
    return m_pMouseFocusWindow;
}

void xuiWindowManager::setActiveDialog(xuiDialog* pWindow,bool bFocus)
{
    xuiWindow* pOldActiveWindow = m_pActiveWindow;
    if(bFocus == true)
    {
        if(m_pActiveWindow != pWindow)
        {
            m_pActiveWindow = pWindow;
            if(pOldActiveWindow) pOldActiveWindow->onLostActive();
            if(m_pActiveWindow) m_pActiveWindow->onActive();
        }
    }
    else
    {
        if(m_pActiveWindow) m_pActiveWindow->onLostActive();
        m_pActiveWindow = NULL;
    }	  
}

xuiDialog* xuiWindowManager::getActiveDialog()
{
    return m_pActiveWindow;
}

bool xuiWindowManager::loadPlugin(const wchar_t* plugin)
{
    return NULL != xPluginMgr::singleton()->loadPlugin(plugin);
}

bool xuiWindowManager::addCommCtrlLib(const wchar_t* _file)
{
    HXMLDoc hXML =  m_XMLManager.add(_file, true , true);
    xXmlNode* pLibNode =  NULL;
    if( hXML.isHandle() )
    {
        pLibNode = hXML->root();
    }
    if(pLibNode == NULL)
        return false;

    xXmlNode::XmlNodes ctrlNodes;
    pLibNode->findNode(L"control",ctrlNodes);
	size_t _nCtrlNodes = ctrlNodes.size() ;
    for(size_t i = 0 ; i < _nCtrlNodes ; i ++)
    {
        xXmlNode* pNode = ctrlNodes[i];
        const wchar_t* _name =  pNode->value(L"control");
        if(_name)
        {
            int hId = xStringHash(_name );
            m_ctrlLib.insert( mapCtrlLib::value_type(hId , pNode) );
        }
    }
    return true;
}

xXmlNode* xuiWindowManager::findCommCtrl(const wchar_t* _name)
{
    if(_name == NULL) return NULL;
    int hId = xStringHash(_name);
    mapCtrlLib::iterator pos = m_ctrlLib.find(hId);
    if(pos == m_ctrlLib.end() )
        return NULL;
    return pos->second;
}
END_NAMESPACE_XUI
