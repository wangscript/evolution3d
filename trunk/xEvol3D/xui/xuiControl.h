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

#ifndef __XUI_CONTROL_H__
#define __XUI_CONTROL_H__
#include "xuiWindow.h"
BEGIN_NAMESPACE_XUI

#define DECL_XUI_CONTROLCREATOR(CLASSNAME)\
class CLASSNAME##Creator : public xuiWindowCreator{\
public:\
    CLASSNAME##Creator(bool bRegiste){if(bRegiste) {xuiWindowCreator::registeWindowCreate(this);}; }\
    xuiWindow*      createInstance(xuiWindow* parent , xuiWindowManager* pMgr){return new CLASSNAME(parent , pMgr);}\
    const wchar_t*  scriptFile()    {return NULL;}\
    const wchar_t*  type()          {return _WIDETEXT_(#CLASSNAME);}\
    eXUIWinType     winType()       {return eXUIWT_Control;}\
};\


#define REG_XUI_CONTROLCREATOR(CLASSNAME,bReg)\
    static CLASSNAME##Creator gs_##CLASSNAME##Creator(bReg);\

class _XEVOL_BASE_API_ xuiDialog;
class _XEVOL_BASE_API_ xuiCtrlContainer;
class _XEVOL_BASE_API_ xuiControl : public xuiWindow
{  
    friend class xuiCtrlContainer; 
    friend class xuiCtrlContainer;
    friend class xuiDialog;
public:
    xuiControl(xuiWindow* parent , xuiWindowManager* pMgr);
    ~xuiControl();

    const int tabIndex() const ;

protected:
    //virtual function . ovride for use.
    virtual bool            onLMouseDown(xMouseMsg& mEvent);
    virtual bool            onLMouseRelease(xMouseMsg& mEvent);
    virtual bool            onRMouseDown(xMouseMsg& mEvent);
    virtual bool            onRMouseRelease(xMouseMsg& mEvent);
    virtual bool            onMMouseDown(xMouseMsg& mEvent);
    virtual bool            onMMouseRelease(xMouseMsg& mEvent);
    virtual bool            onMouseMove(xMouseMsg& mEvent);
    virtual bool            onMouseWhell(xMouseMsg& mEvent);
    virtual bool            onKeyDown(xKeyboardMsg& kEvent);
    virtual bool            onKeyUp(xKeyboardMsg& kEvent);

    virtual bool            onShow();
    virtual bool            onHide();
    virtual void            onLostActive();
    virtual void            onActive();
    virtual void            onMouseFocus();
    virtual void            onLostMouseFocus();


    virtual void            onKbInputFocus();
    virtual void            onLostKbInputFocus(bool bLostActive);

    virtual bool            onNotify(eXUIWinNotifyEvent _event, xuiWindow* pWndSend , int param1 , int param2);

    bool                    notify(eXUIWinNotifyEvent _event, xuiWindow* pWndSend , int param1 , int param2);
    bool                    command(eXUIWinCommand  _command, xuiWindow* pCtrl, int param1 , int param2);
protected:
    int                     m_tabIndex;
    bool                    m_bHasKbInputer;
};
END_NAMESPACE_XUI

#endif
