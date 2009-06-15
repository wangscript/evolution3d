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

#include "../xuiHotArea.h"
#include "../xuiWindowManager.h"
#include "../xuiStringParser.h"
BEGIN_NAMESPACE_XUI
xuiHotArea::xuiHotArea(xuiWindow* pWindow , xuiWindowManager* pWindowMgr)
{
    m_bEnable    = false;
    m_pOwnder    = pWindow;
    m_pWindowMgr = pWindowMgr;
    m_event = L"move";
}
xuiHotArea::~xuiHotArea()
{
    disable();
}
void xuiHotArea::enable()
{
    m_bEnable = true;
    m_pWindowMgr->addHotArea(this);
}

void xuiHotArea::disable()
{
    m_bEnable = false;
    m_pWindowMgr->removeHotArea(this);
}

bool xuiHotArea::isEnable()
{
    return m_bEnable;
}

xuiHotArea::eHotAreaAction xuiHotArea::getAction()
{
    return m_action;
}
xuiHotArea::eHotAreaActive xuiHotArea::getActiveType()
{
    return m_active;
}

bool xuiHotArea::__checkEvent(eMsgID msgID)
{
    switch(msgID)
    {
    case WIN_MOUSE_MOVE:
        if(m_event.find(L"move") != wstring::npos)
            return true;
        return false;

    case WIN_LMOUSE_RELEASE:
    case WIN_LMOUSE_DOWN:
        if(m_event.find(L"left") != wstring::npos)
            return true;
        return false;

    case WIN_RMOUSE_RELEASE:
    case WIN_RMOUSE_DOWN:
        if(m_event.find(L"right") != wstring::npos)
            return true;
        return false;

    case WIN_MMOUSE_RELEASE:
    case WIN_MMOUSE_DOWN:
        if(m_event.find(L"middle") != wstring::npos)
            return true;
        return false;
    }
    return false;
}

bool  xuiHotArea::__checkActive()
{
    switch(m_active)
    {
    case eHOTAREA_ACTIVE_HIDE:
        return !m_pOwnder->isVisable();
    case eHOTAREA_ACTIVE_SHOW:
        return m_pOwnder->isVisable();
    default:
        return true;
    }
}
void xuiHotArea::fire(eMsgID msgID)
{
    if(__checkEvent(msgID) == false) return ;
    if(__checkActive() == false) return ;
    switch(m_action)
    {
    case eHOTAREA_ACTION_HIDE:
        m_pOwnder->hide();
        break;
    case eHOTAREA_ACTION_SHOW:
        m_pOwnder->show();
        break;
    }
}
xuiRegion& xuiHotArea::getRegion()
{
    return m_Region;
}

bool xuiHotArea::hitTest(int x , int y)
{
    xMathLib::xvec4 _thisRect = m_Region.Rect();
    if( _thisRect.rect.PtInRect((float)x ,  (float)y) )
    {
        return true;
    }
    return false;
}
bool xuiHotArea::load(xXmlNode* pCfgNode)
{
    if(pCfgNode->findValue(L"name") == NULL)
    {
        XEVOL_LOG(eXL_DEBUG_HIGH,"hotArea must have a name\n");
        return false;
    }

    wcsncpy(m_name , pCfgNode->value(L"name") , 32);

    xXmlValue* pValue = pCfgNode->findValue(L"action");
    const wchar_t* pTypeValue =L"show";
    if(pValue)pTypeValue=pValue->value();
    if(pTypeValue == wstring(L"show")) m_action = eHOTAREA_ACTION_SHOW;
    else if(pTypeValue == wstring(L"hide")) m_action = eHOTAREA_ACTION_HIDE;
    else m_action = eHOTAREA_ACTION_USER;

    pValue = pCfgNode->findValue(L"active");
    const wchar_t* pActiveValue = L"show";
    if(pValue) pActiveValue=pValue->value();
    if(pActiveValue == wstring(L"show")) m_active = eHOTAREA_ACTIVE_SHOW;
    else if(pActiveValue == wstring(L"hide")) m_active = eHOTAREA_ACTIVE_HIDE;
    else m_active = eHOTAREA_ACTIVE_ANY;


    pValue = pCfgNode->findValue(L"event");
    if(pValue) m_event  = pValue->value();

    if(pCfgNode->findValue(L"region"))
    {
        string2Region(pCfgNode->findValue(L"region")->value() , m_Region);
    }
    return true;
}
END_NAMESPACE_XUI

