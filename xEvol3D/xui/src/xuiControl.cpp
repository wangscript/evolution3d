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
#include "../xuiControl.h"

BEGIN_NAMESPACE_XUI
xuiControl::xuiControl(xuiWindow* parent , xuiWindowManager* pMgr)
:xuiWindow(parent , pMgr)
{
    m_bHasKbInputer = false;
}
xuiControl::~xuiControl()
{

}
const int xuiControl::tabIndex() const
{
    return m_tabIndex;
}
//默认的消息响应
bool xuiControl::onLMouseDown(xMouseMsg& mEvent)
{
    return false;
}

bool xuiControl::onLMouseRelease(xMouseMsg& mEvent)
{
    return false;
}

bool xuiControl::onRMouseDown(xMouseMsg& mEvent)
{
    return false;
}

bool xuiControl::onRMouseRelease(xMouseMsg& mEvent)
{
    return false;
}

bool xuiControl::onMMouseDown(xMouseMsg& mEvent)
{
    return false;
}

bool xuiControl::onLMouseDbClick(xMouseMsg& mEvent)
{
    return false;
}

bool xuiControl::onMMouseRelease(xMouseMsg& mEvent)
{
    return false;
}

bool xuiControl::onMouseMove(xMouseMsg& mEvent)
{
    return false;
}

bool xuiControl::onMouseWhell(xMouseMsg& mEvent)
{
    return false;
}

bool xuiControl::onKeyDown(xKeyboardMsg& kEvent)
{
    return false;
}

bool xuiControl::onKeyUp(xKeyboardMsg& kEvent)
{
    return false;
}

bool xuiControl::onShow()
{
    return true;
}
bool xuiControl::onHide()
{
    return true;
}

bool xuiControl::onNotify(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2)
{
    return false;
}

bool xuiControl::notify(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2)
{
    bool ret = onNotify(_event, pWndSender , param1 , param2);
    return xuiWindow::notify(_event, pWndSender , param1 , param2);
}

bool xuiControl::command(eXUIWinCommand  _command, xuiWindow* pCtrl, int param1 , int param2)
{
    return __fireCommand(_command, pCtrl, param1 , param2);
}
void xuiControl::onLostActive()
{
    setState(eXUIWState_LostActive);
}

void xuiControl::onActive()
{
    setState(eXUIWState_Normal);
}

void xuiControl::onMouseFocus()
{
    return xuiWindow::onMouseFocus();
}

void xuiControl::onLostMouseFocus()
{
    if(m_bHasKbInputer == false)
    {
        setState(eXUIWState_Normal);
    }
    else
    {
        setState(eXUIWState_KeyboardFocus);
    }
}

void xuiControl::onKbInputFocus()
{
    m_bHasKbInputer = true;
    setState(eXUIWState_KeyboardFocus , true );
}

void xuiControl::onLostKbInputFocus(bool bLostActive)
{
    if(bLostActive == false) 
    {
        m_bHasKbInputer = false;
        if(hasMouseFocus() )
        {
            return onMouseFocus();
        }
    }
    setState(eXUIWState_Normal , true );
}

END_NAMESPACE_XUI
