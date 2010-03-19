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

#ifndef __XUI_WINDOW_H__
#define __XUI_WINDOW_H__
#include "xuibase.h"
#include "xuiDataType.h"
#include "xuiWindowState.h"
#include "../font/xFontManager.h"
BEGIN_NAMESPACE_XUI

class _XEVOL_BASE_API_ xuiWindow : public IRefCountObject
{
    DECL_REFCOUNT_OBJECT_INTERFACE(xuiWindow);
public:
    virtual   ~xuiWindow();
    xuiWindow(xuiWindow* parent , xuiWindowManager* pMgr);
    friend class xuiWindowManager;
    friend class xuiWindowStateBlender;
    friend class xuiDialog;

    bool                    addChild(xuiWindow* pWindow);
    bool                    removeChild(xuiWindow* pWindow);

    xuiWindowManager*       getWindowMgr(){return m_pWindowMgr;}
private://这里的函数只给 xuiWindowManager 调用
    bool                    drawWindow();
    virtual bool            updateFrame(int passedTime , unsigned int thisTime);

public:
    virtual bool            load(xXmlNode* pCfgNode) = 0; // virtual ;
public:
    //公用状态查询函数
    virtual const wchar_t*  name();
    bool                    nameEq(const wchar_t* _name);

    const xuiWindowZValue&  zValue() const;
    void                    setZValue(unsigned int zValue);

    void                    setPosition(xuiPoint& pos);
    xuiPoint                getAbsPosition(xuiPoint& pos);
    xuiRect                 getAbsRect(xuiRect&   rect);

    xuiPoint                getWndAbsPos();
    xuiRect                 getWndAbsRect();
    xuiRegion&              getRegion();

    xuiWindowStateBlender*  getStateBlender();
    xuiWindowTransBlender*  getTransBlender();

    virtual void            setState(const wchar_t* _stateName, bool bBlend = true);
    virtual void            setState(eXUIWinState _state, bool bBlend = true);
    virtual void            setState(xuiWindowState* pState, bool bBlend = true);
    xuiWindowState*         findState(const wchar_t* _stateName);


    virtual bool            hasKbInputFocus();
    virtual bool            hasMouseFocus();

public:
    //外部公用行为函数
    virtual bool            hide();
    virtual bool            isVisable();
    virtual bool            show();

    virtual eXUIWinHitState hitTest(int x , int y);
    virtual bool            hitTest(xMathLib::xvec4& rect , int x , int y);



protected:

    virtual bool            hasSeperatedKBFocus();
    //===========内部行为函数
    virtual bool            draw() = 0;

    virtual void            startHideBlend();
    virtual void            startShowBlend();

    virtual bool            setMouseFocus(bool bFocus);

    //==========响应处理函数
    virtual void            onLostActive();
    virtual void            onActive();

    virtual void            onMouseFocus();
    virtual void            onLostMouseFocus();

    virtual bool            onShow();
    virtual bool            onHide();

    virtual bool            onNotify(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2) = 0;

    virtual bool            command(eXUIWinCommand  _command, xuiWindow* pCtrl, int param1 , int param2);
    virtual bool            notify(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2);

public:
    bool                    isCurrentState(xuiWindowState* pState);
protected:
    bool                    __loadAllStates(xXmlNode* pNode);
    void                    __setState(xuiWindowState* pState);
    bool                    __parseStyle(const wchar_t* _style);

    bool                    __fireCommand(eXUIWinCommand  _command, xuiWindow* pCtrl, int param1 , int param2);  //给父窗口发命令

    bool                    __notifyParent(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2); //给父窗口发通知
    bool                    __notifyChildren(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2);//给子窗口发通知

protected:


protected:
    wchar_t                  m_name[32];
    HFontRender              m_hFont;
    bool                     m_bVisable;
    bool                     m_bMouseWindow;
    bool                     m_bKbWindow;
    xuiWindowZValue          m_zValue;
    xuiWindowManager*        m_pWindowMgr;
    vXuiWindowState          m_vStates;
    xuiWindowState           m_CurState;
    xuiWindowStateBlender*   m_pStateBlend;
    xuiWindowTransBlender*   m_transBlend;

protected:
    xuiWindow*          m_wndParent;
    xuiWindows          m_Children;
};


class _XEVOL_BASE_API_ xuiWindowCreator
{
public:
    static bool                 registeWindowCreate(xuiWindowCreator* pXUIWindowCreator);
    static xuiWindowCreator*    findWindowCreater(const wchar_t* _windowName);

public:
    virtual xuiWindow*          createInstance(xuiWindow* parent , xuiWindowManager* pMgr) = 0;
    virtual const wchar_t*      scriptFile() = 0;
    virtual const wchar_t*      type() = 0;
    virtual eXUIWinType         winType() = 0;
};

END_NAMESPACE_XUI

#endif

