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
#include "../xuiDialog.h"
#include "../xuiControl.h"
#include "../xuiCaption.h"
#include "../xuiHotArea.h"
#include "../xuiWindowManager.h"

BEGIN_NAMESPACE_XUI
DECL_XUI_DIALOGCREATOR(xuiDialog , NULL);

REG_XUI_DIALOGCREATOR(xuiDialog,true); 
xuiDialog::xuiDialog(xuiWindow* parent , xuiWindowManager* pMgr)
:xuiCtrlContainer(parent , pMgr)
{
    m_pCaption = NULL;
    m_bIconize = false;
    m_bKBHasSeperatedFocus = false;
}

xuiDialog::~xuiDialog()
{
    if(m_pCaption) m_pCaption->ReleaseObject();
	size_t _nHotAreas = m_vHotAreas.size() ;
    for(size_t i = 0 ; i < _nHotAreas ; i ++)
    {
        m_vHotAreas[i]->disable();
        delete m_vHotAreas[i];
    }
    m_vHotAreas.clear();
}


bool xuiDialog::onCommand(eXUIWinCommand  _command, xuiWindow* pCtrl, int param1 , int param2)
{
    switch(_command)
    {
    case eXUIWCMD_Close:
        hide();
        return  true;
        break;
    case eXUIWCMD_Icon:
        m_bIconize =  !m_bIconize;
        return  true;
        break;
    default:
        break;
    }
    return m_pWindowMgr->onCommand(this , _command, pCtrl, param1 , param2);
}

bool xuiDialog::onNotify(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2)
{
    return false;
}

bool  xuiDialog::leftMouseDown(xMouseMsg& mEvent)
{
    setActive(true);
    return xuiDialogBase::leftMouseDown(mEvent);
}

bool  xuiDialog::leftMouseRelease(xMouseMsg& mEvent)
{
    setActive(true);   
    if(m_pCaption && m_pCaption->hitTest(mEvent.x ,mEvent.y))
    {
        m_pCaption->onLMouseRelease(mEvent);
        return true;
    } 
    return xuiDialogBase::leftMouseRelease(mEvent);
}

bool  xuiDialog::rightMouseDown(xMouseMsg& mEvent)
{
    setActive(true);
    return xuiDialogBase::rightMouseDown(mEvent);
}

bool  xuiDialog::rightMouseRelease(xMouseMsg& mEvent)
{
    setActive(true);
    return xuiDialogBase::rightMouseRelease(mEvent);
}

bool  xuiDialog::midleMouseDown(xMouseMsg& mEvent)
{
    setActive(true);
    return xuiDialogBase::midleMouseDown(mEvent);
}

bool  xuiDialog::midleMouseRelease(xMouseMsg& mEvent)
{
    setActive(true);
    return xuiDialogBase::midleMouseRelease(mEvent);
}

bool  xuiDialog::mouseMove(xMouseMsg& mEvent)
{
    return xuiDialogBase::mouseMove(mEvent);
}

bool  xuiDialog::mouseWhell(xMouseMsg& mEvent)
{
    return xuiDialogBase::mouseWhell(mEvent);
}

bool xuiDialog::hasKbInputFocus()
{
    return hasActive();       
}

bool xuiDialog::hasActive()
{
    return m_pWindowMgr->getActiveDialog() == this;
}

bool xuiDialog::setActive(bool bActive)
{
    m_pWindowMgr->setActiveDialog(this,bActive);
    return hasActive();
}

bool xuiDialog::command(eXUIWinCommand  _command, xuiWindow* pCtrl, int param1 , int param2)
{
    //首先处理派生类的。
    bool _ret = onCommand(_command , pCtrl,param1,param2);
    if(_ret == true) return true;
    //然后用默认的。
    return  xuiDialog::onCommand(_command,pCtrl,param1,param2);	
}

bool xuiDialog::hasSeperatedKBFocus()
{
    return   m_bKBHasSeperatedFocus;
}

bool xuiDialog::notify(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2)
{
    bool _ret = onNotify(_event, pWndSender , param1 , param2);
    if(_ret == true) return true;

    return false;
}


bool xuiDialog::hide()
{
    xuiWindow::hide();
    m_pWindowMgr->hideDialog(this);
    onLostActive();
    return true;
}

bool xuiDialog::show()
{
    xuiWindow::show();
    m_pWindowMgr->showDialog(this);
    onActive();
    return true;
}

void xuiDialog::setText(const wchar_t* _text)
{
    if(m_pCaption) m_pCaption->setText(_text);
}

bool xuiDialog::__loadHotArea(xXmlDocument::XmlNodes& _vHotAreasNodes)
{
    int nNode = (int)_vHotAreasNodes.size();
    for(int i = 0 ; i < nNode ; ++i)
    {
        __loadHotArea(_vHotAreasNodes[i]);
    }
    return true;
}

bool xuiDialog::__loadHotArea(xXmlNode* pCfgNode)
{
    xuiHotArea* pHotArea = new xuiHotArea(this ,m_pWindowMgr);
    pHotArea->load(pCfgNode);
    m_vHotAreas.push_back(pHotArea);
    pHotArea->enable();
    return true;
}


bool xuiDialog::load(xXmlNode* pCfgNode)
{
    //first window creator
    xuiWindow::load(pCfgNode);

    //Load the caption
    xXmlNode* pCaptionNode = pCfgNode->findNode(L"caption");
    if(pCaptionNode)
    {
        m_pCaption = (xuiCaption*)m_pWindowMgr->createControl(L"xuiCaption",this);
        m_pCaption->load(pCaptionNode);
    }

    xXmlNode* pAttrNode = pCfgNode->findNode(L"attribute");
    if(pAttrNode == NULL) pAttrNode = pCfgNode;
    assert(pAttrNode);
    if(pAttrNode->value(L"HasSepKbFocus"))
    {
        m_bKBHasSeperatedFocus = pAttrNode->bool_value(L"HasSepKbFocus");
    }

    //Now load all window
    loadAllControl(pCfgNode);

    xXmlDocument::XmlNodes hotAreaNodes;
    pCfgNode->findNode(L"hotarea",hotAreaNodes);
    __loadHotArea(hotAreaNodes);
    onLoad(pCfgNode);

    return true;
} 

eXUIWinHitState xuiDialog::hitTest( int x , int y)
{
    if(!m_bIconize)
    {
        return xuiWindow::hitTest(x,y);
    }
    else if(m_pCaption)
    {
        xuiRect _thisRect = m_pCaption->getWndAbsRect();
        if( _thisRect.rect.PtInRect( (float)x ,  (float)y) )
        {
            return eXUIWHS_InWindow;
        }
        return eXUIWHS_OutWindow;
    }
    return eXUIWHS_OutWindow;

}

bool xuiDialog::draw()
{
    if(m_bIconize == false)
    {
        m_pWindowMgr->drawRectf(0,NULL,m_state.m_Region.Rect().v, m_state.m_color);
        //Draw userdef content
        onDraw();

        int nCtrl  = (int)m_vControls.size();
        for(int i = 0 ; i < nCtrl ; ++i)
        {
            xuiControl* pControl = m_vControls[i];
            pControl->draw();
        }

        //Draw Caption
        if(m_pCaption) m_pCaption->draw();

        xuiWindow::draw();
    }
    else
    {
        if(m_pCaption) m_pCaption->draw();
        if(m_state.m_border.m_size > 0)
        {
            xuiRect _rect = m_pCaption->getWndAbsRect();
            this->_drawRect(_rect , m_state.m_border.m_color, m_state.m_border.m_bevel);
        }
    }

    return true;
}



//==================================
bool xuiDialog::onLoad(xXmlNode* pCfgNode)
{
    return true;
}

bool xuiDialog::onDraw()
{
    return true;    
}
//默认的消息派送
bool xuiDialog::_onMouseMsg(xMouseMsg& mEvent)
{
    switch(mEvent.MsgID)
    {
    case WIN_LMOUSE_DOWN:
        return leftMouseDown(mEvent);
    case WIN_LMOUSE_RELEASE:
        return leftMouseRelease(mEvent);
    case WIN_RMOUSE_DOWN:
        return rightMouseDown(mEvent);
    case WIN_RMOUSE_RELEASE:
        return rightMouseRelease(mEvent);
    case WIN_MMOUSE_DOWN:
        return midleMouseDown(mEvent);
    case WIN_MMOUSE_RELEASE:
        return midleMouseRelease(mEvent);
    case WIN_MOUSE_MOVE:
        return mouseMove(mEvent);
    case WIN_MOUSE_WHEEL:
        return mouseWhell(mEvent);
    }
    return false;
}

bool xuiDialog::_onKeyboardMsg(xKeyboardMsg& kEvent)
{
    switch(kEvent.MsgID)
    {
    case WIN_KEYDOWN:
        return keyDown(kEvent);
    case WIN_KEYUP:
        return keyUp(kEvent);
    }
    return false;
}

bool xuiDialog::keyUp(xKeyboardMsg& kEvent)
{
    return xuiDialogBase::keyUp(kEvent);
}

bool xuiDialog::keyDown(xKeyboardMsg& kEvent)
{
    return xuiDialogBase::keyDown(kEvent);
}

bool xuiDialog::onShow()
{
    return true;
}

bool xuiDialog::onHide()
{
    return true;
}

void xuiDialog::onLostActive()
{
    xuiDialogBase::onLostActive();
}

void xuiDialog::onActive()
{
    setZValue( m_pWindowMgr->getTime() ); 
    xuiDialogBase::onActive();
}
END_NAMESPACE_XUI
