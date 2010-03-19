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
#include "../xuiButton.h"
#include "../xuiWindowManager.h"
#include "../xuiStringParser.h"
BEGIN_NAMESPACE_XUI

DECL_XUI_CONTROLCREATOR(xuiButton)
REG_XUI_CONTROLCREATOR(xuiButton, true)

bool xuiButton::onLMouseRelease(xMouseMsg& mEvent)
{
    setState(eXUIWState_Focus , true);
    __fireCommand(eXUIWCMD_Click, this,0,0);
    return true;
}

bool xuiButton::onMouseMove(xMouseMsg& mEvent)
{
    return true;
}

bool xuiButton::onLMouseDown(xMouseMsg& mEvent)
{
    setState(eXUIWState_Press , true);
    return true;
}

bool  xuiButton::onKeyDown(xKeyboardMsg& kEvent)
{
    if(kEvent.nKey == VKEY_RETURN)
    {
        setState(eXUIWState_Press , true);
        return true;
    }
    return false;
}
bool  xuiButton::onKeyUp(xKeyboardMsg& kEvent)
{
    if(kEvent.nKey == VKEY_RETURN)
    {

        if(this->hasMouseFocus())
        {
            setState(eXUIWState_Focus , true);
            __fireCommand(eXUIWCMD_Click, this,0,0);
        }
        else
        {
            onKbInputFocus();
            __fireCommand(eXUIWCMD_Click, this,0,0);
        }
        return true;
    }
    return false;

}

xuiButton::xuiButton(xuiWindow* parent , xuiWindowManager* pMgr):xuiTextLabel(parent , pMgr)
{

}

xuiButton::~xuiButton()
{

}

bool xuiButton::load(xXmlNode* pCfgNode )
{
    xuiTextLabel::load(pCfgNode);
    return true;
}


bool xuiButton::draw()
{
    xuiRect _rect = m_wndParent->getAbsRect( m_CurState.m_Region.Rect() ) ;// getWndAbsRect();
    m_pWindowMgr->drawRectf(0 , NULL, _rect.v , m_CurState.m_color);
    m_hFont->drawTextOneLine(m_text.c_str(),_rect, m_CurState.m_textcolor,xFontRender::eAlign_Center,xFontRender::eAlign_Center);
    xuiWindow::draw();
    return true;
}

void  xuiButton::onKbInputFocus()
{
    return xuiControl::onKbInputFocus();
}

void  xuiButton::onLostKbInputFocus(bool bLostActive)
{
    return xuiControl::onLostKbInputFocus(bLostActive);
}

END_NAMESPACE_XUI
