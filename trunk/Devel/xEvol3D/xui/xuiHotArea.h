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

#include "xuiWindow.h"
BEGIN_NAMESPACE_XUI

class _XEVOL_BASE_API_ xuiHotArea
{
public:
    enum eHotAreaActive
    {
        eHOTAREA_ACTIVE_SHOW , //热区在窗口显示的时候有效。
        eHOTAREA_ACTIVE_HIDE , //热区在窗口隐藏的时候有效。
        eHOTAREA_ACTIVE_ANY  , //任何时候都有效。
    };

    enum eHotAreaAction
    {
        eHOTAREA_ACTION_HIDE = 0, //热区在触发的时候隐藏窗口
        eHOTAREA_ACTION_SHOW = 1, //热区在触发的时候显示窗口
        eHOTAREA_ACTION_USER = 2, //热区在出发的时候执行用户回调。
    };
protected:
    eHotAreaAction    m_action;
    eHotAreaActive    m_active;
    bool              m_bEnable;
    xuiRegion         m_Region;
    wchar_t           m_name[32];
    xuiWindow*        m_pOwnder;
    xuiWindowManager* m_pWindowMgr;
    wstring           m_event;
    bool              __checkEvent(eMsgID msgID);
    bool              __checkActive();
public:
    xuiHotArea(xuiWindow* pWindow, xuiWindowManager* pWindowMgr);
    ~xuiHotArea();
    bool           hitTest(int x , int y);
    void           enable();//启用该热区。
    void           disable();//关闭该热区.
    bool           isEnable();

    eHotAreaAction getAction();
    eHotAreaActive getActiveType();
    void           fire(eMsgID msgID);//触发
    xuiRegion&     getRegion();
    bool           load(xXmlNode* pCfgNode);

};

END_NAMESPACE_XUI
