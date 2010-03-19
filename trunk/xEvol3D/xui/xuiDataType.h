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

#ifndef __XUI_DATA_TYPE_H__
#define __XUI_DATA_TYPE_H__
#include "xuibase.h"

BEGIN_NAMESPACE_XUI

class _XEVOL_BASE_API_ xuiWindowManager;
class _XEVOL_BASE_API_ xuiWindow;
struct xuiEllipse_t;
struct xuiBevel;
struct xuiRegion;

struct xuiEllipse_t
{
    float x , y;
    float a , b;
};

struct xuiBevel
{
    float tl, tr,bl,br;
    xuiBevel(float _tl = 0 ,float _tr = 0,float _br = 0,float _bl = 0)
        :tl(_tl), tr(_tr),bl(_bl),br(_br)
    {
    }
    bool isBevel()
    {
        return (tl>0 || tr>0 || bl>0 || br>0);
    }
};

struct xuiRegion
{
    enum eRegionType
    {
        eRT_Rect,
        eRT_Ellipse
    };
    eRegionType        _type;
    xuiRect2D&          Rect2D(){ return _rect.rect ; }
    xuiRect&           Rect(){  return _rect ; };
protected:
    xMathLib::xvec4    _rect;
public:
    xuiRegion()
    {
        _type = eRT_Rect;
    }
};


enum eXUIWinType
{
    eXUIWT_Control         ,
    eXUIWT_Dialog          ,
    eXUIWT_ControlContainer,

};
enum eXUIWinHitState
{
    eXUIWHS_OutWindow = 0,
    eXUIWHS_InBorder  = 1,
    eXUIWHS_InWindow  = 2,
    eXUIWHS_InNoClient= 3,

};
enum eXUIWinZOrder
{
    eXUIWZOder_BottomMost = 0,
    eXUIWZOder_Normal     = 100000,
    eXUIWZOder_TopMost    = 200000,
};

enum eXUIWinNotifyEvent
{
    eXUIWN_Close  =  0, //Dialog [close] box click
    eXUIWN_Resize ,     //Dialog is resize.
    eXUIWN_Icon   ,     //Dialog is icon.

    eXUIWN_Hide   ,     //父窗口被Hide掉会收到这个消息
    eXUIWN_Show   ,     //父窗口Show的时候会收到这个消息

    eXUIWN_MouseLeave,  //Mouse leave a control
    eXUIWN_LostFocus,    //Lost Mouse focus
    eXUIWN_LostKeyboard, //Lost Keyboard
    eXUIWN_SetFocus,
    eXUIWN_SetKeyboard,  //

    eXUIWN_ParentLostFocus,
    eXUIWN_ParentLostKeyboard,
    eXUIWN_ParentSetFocus,
    eXUIWN_ParentSetKeyboard,

    //System Notify
    eXUIWN_Sys_Resize = 20000,  //Windows is resize. [Renderer window , not the  xuiWindow]
};

enum eXUIWinCommand
{
    eXUIWCMD_Close  =  0, //Dialog [close] box click
    eXUIWCMD_Resize ,     //Dialog is resize.
    eXUIWCMD_Icon   ,     //Dialog is icon.

    eXUIWCMD_Hide   ,     //父窗口被Hide掉会收到这个消息,

    //----------------------------
    eXUIWCMD_Click  ,     //单击
    eXUIWCMD_SelChange ,
};
enum eXUIWinState
{
    eXUIWState_Hide,         //"hide"
    eXUIWState_Normal,       //"normal"
    eXUIWState_LostActive,   //"lostactive"
    eXUIWState_Gray,         //"gray"
    eXUIWState_Press,      //"press";
    eXUIWState_Focus,        //"focus"
    eXUIWState_KeyboardFocus,//"keyboardfocus"
};
#pragma pack(push , 1)

struct xuiWindowZValue
{
    eXUIWinZOrder m_zOrder;
    unsigned int  m_zValue;
    enum {MAX_ZVALUE = 0xffffffff};

    xuiWindowZValue()
    {
        m_zOrder = eXUIWZOder_Normal;
        m_zValue = MAX_ZVALUE;
    }
};
#pragma pack(pop)
bool operator >  (const xuiWindowZValue& lhv , const xuiWindowZValue& rhv);
bool operator == (const xuiWindowZValue& lhv , const xuiWindowZValue& rhv);
bool operator <  (const xuiWindowZValue& lhv , const xuiWindowZValue& rhv);

typedef std::vector<xuiWindow* , dllsafe_alloc<xuiWindow*> >       xuiWindows;

END_NAMESPACE_XUI


#endif
