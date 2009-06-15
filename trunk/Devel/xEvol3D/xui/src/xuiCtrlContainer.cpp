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

#include "../xuiCtrlContainer.h"
#include "../xuiWindowManager.h"
BEGIN_NAMESPACE_XUI


xuiCtrlContainer::xuiCtrlContainer(xuiWindow* pParent , xuiWindowManager* pMgr)
:xuiControl(pParent , pMgr)
{
    m_pKeyFocusCtrl        = NULL;
}
xuiCtrlContainer::~xuiCtrlContainer()
{ 

}
void xuiCtrlContainer::addControl(xuiControl* pCtrl)
{
    m_vControls.push_back(pCtrl);
}

xuiControl* xuiCtrlContainer::getControl(int ctrlID)
{
    return NULL;
}

xuiControl* xuiCtrlContainer::getControl(const wchar_t* _name)
{
    if(_name == NULL) return NULL;
    for(size_t i = 0 ; i < m_vControls.size() ; i ++)
    {
        if(m_vControls[i]->nameEq(_name))
            return m_vControls[i];
    }
    return NULL;
}

bool xuiCtrlContainer::moveKBFocus(int _dir)
{
    if(m_vControls.size() == 0)
        return false;
    int index  = 0;
    if(m_pKeyFocusCtrl == NULL)
    {
        if(_dir > 0 )
        {
            index = 0;
        }
        else
        {
            index = (int)m_vControls.size()-1;
        }
    }
    else
    {
        for( int i = 0 ; i < (int)m_vControls.size() ; i ++)
        {
            if(m_vControls[i] == m_pKeyFocusCtrl)
            {
                index = i;
                break;
            }
        }
        index += _dir;

    }
    index %= m_vControls.size();

    this->setKBInputFocus(m_vControls[index]);
    if(!hasSeperatedKBFocus() && m_pKeyFocusCtrl) 
    {
        m_pKeyFocusCtrl->setMouseFocus(true);
    }
    return true;
}

bool xuiCtrlContainer::keyDown(xKeyboardMsg& kEvent)
{
    bool _ret =  false;
    if(m_pKeyFocusCtrl)
    {
        _ret =  m_pKeyFocusCtrl->onKeyDown(kEvent);
    }
    return false;
}

bool xuiCtrlContainer::keyUp(xKeyboardMsg& kEvent)
{
    bool _ret = false;
    if(m_pKeyFocusCtrl)
    {
        _ret =   m_pKeyFocusCtrl->onKeyUp(kEvent);
    }
    if(_ret == false)
    {
        if(kEvent.nKey == VKEY_LEFT || kEvent.nKey == VKEY_UP)
        {
            moveKBFocus(-1);
        }
        else if(kEvent.nKey == VKEY_DOWN || kEvent.nKey == VKEY_RIGHT)
        {
            moveKBFocus(1);
        }
    }
    return true;
}

bool  xuiCtrlContainer::leftMouseDown(xMouseMsg& mEvent)
{
    int nCtrl  = (int)m_vControls.size();
    for(int i = 0 ; i < nCtrl ; ++i)
    {
        xuiControl* pControl = m_vControls[i];
        if(pControl->hitTest(mEvent.x ,mEvent.y) && pControl->isVisable() )
        {
            pControl->setMouseFocus(true);
            setKBInputFocus(pControl);
            bool _ret = pControl->onLMouseDown(mEvent);
            if(_ret == true)
                return true;
        }
    }
    return false;
}

bool  xuiCtrlContainer::leftMouseRelease(xMouseMsg& mEvent)
{
    int nCtrl  = (int)m_vControls.size();
    for(int i = 0 ; i < nCtrl ; ++i)
    {
        xuiControl* pControl = m_vControls[i];
        if(pControl->hitTest(mEvent.x ,mEvent.y) && pControl->isVisable() )
        {
            pControl->setMouseFocus(true);
            setKBInputFocus(pControl);
            bool _ret = pControl->onLMouseRelease(mEvent);
            if(_ret == true)
                return true;
        }
    }
    return false;
}

bool  xuiCtrlContainer::rightMouseDown(xMouseMsg& mEvent)
{
    return false;
}

bool  xuiCtrlContainer::rightMouseRelease(xMouseMsg& mEvent)
{
    return false;
}

bool  xuiCtrlContainer::midleMouseDown(xMouseMsg& mEvent)
{
    return false;
}

bool  xuiCtrlContainer::midleMouseRelease(xMouseMsg& mEvent)
{
    return false;
}

bool  xuiCtrlContainer::mouseMove(xMouseMsg& mEvent)
{
    int nCtrl  = (int)m_vControls.size();
    for(int i = 0 ; i < nCtrl ; ++i)
    {
        xuiControl* pControl = m_vControls[i];
        if(pControl->hitTest(mEvent.x ,mEvent.y) && pControl->isVisable())
        {
            if(!hasSeperatedKBFocus()) //没有分离焦点
            { 
                setKBInputFocus(pControl);
            }
            else
            {
                pControl->setMouseFocus(true);
            }
            bool _ret = pControl->onMouseMove(mEvent);
            if(_ret == true)
                return true;
        }
    }
    m_pWindowMgr->setMouseFocus(NULL,false);
    if(!hasSeperatedKBFocus())
    {
        setKBInputFocus(NULL);
    }
    return false;
}

bool  xuiCtrlContainer::mouseWhell(xMouseMsg& mEvent)
{
    return false;
}

bool xuiCtrlContainer::setKBInputFocus(xuiControl* pCtrl)
{
    if(pCtrl == m_pKeyFocusCtrl) return false;
    if(m_pKeyFocusCtrl)  m_pKeyFocusCtrl->onLostKbInputFocus(false);

    m_pKeyFocusCtrl = pCtrl;
    if(m_pKeyFocusCtrl)  pCtrl->onKbInputFocus();

    if(m_pKeyFocusCtrl && !hasSeperatedKBFocus())
    {
        m_pKeyFocusCtrl->setMouseFocus(true);
    }
    return true;
}


void  xuiCtrlContainer::onControlLoad(xXmlNode* pControlNode , xuiWindow* pWindow) 
{ 
}

bool xuiCtrlContainer::loadAllControl(xXmlNode* pCfgNode)
{
    xXmlDocument::XmlNodes ctrlNodes;
    pCfgNode->findNode(L"control",ctrlNodes);
    return __loadControl(ctrlNodes);        
}

void xuiCtrlContainer::onLostActive()
{
    _TBase::onLostActive();
    if(m_pKeyFocusCtrl)
    {
        m_pKeyFocusCtrl->onLostKbInputFocus(true);
    }
}

void xuiCtrlContainer::onActive()
{
    _TBase::onActive();
    if(m_pKeyFocusCtrl)
    {
        m_pKeyFocusCtrl->onKbInputFocus();
        if(! hasSeperatedKBFocus() )
        {
            m_pKeyFocusCtrl->setMouseFocus(true);
        }
    }
}

bool xuiCtrlContainer::__loadControl(xXmlDocument::XmlNodes& _vCtrlNodes)
{
    int nNode = (int)_vCtrlNodes.size();
    for(int i = 0 ; i < nNode ; ++i)
    {
        __loadControl(_vCtrlNodes[i]);
    }
    return true;

}

bool xuiCtrlContainer::__loadControl(xXmlNode* pCfgNode)
{
    const wchar_t* _typeName = pCfgNode->findValue(L"type")->value();
    xuiControl* pControl = (xuiControl*)m_pWindowMgr->createControl(_typeName,this);
    if(pControl == NULL)
        return NULL;

    pControl->load(pCfgNode);
    onControlLoad(pCfgNode , pControl);
    addControl(pControl);
    pControl->show();
    return true;
}

END_NAMESPACE_XUI
