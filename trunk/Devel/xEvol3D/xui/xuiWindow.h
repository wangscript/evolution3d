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
#include "../font/xFontManager.h"
BEGIN_NAMESPACE_XUI
class _XEVOL_BASE_API_ xuiWindowManager;
class _XEVOL_BASE_API_ xuiWindow;
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
//=====================================================
//  xuiWindow的一些基本属性
//几何外观属性：
//     1。Region. 颜色，
//     2。边框，[是否有边框，边框颜色，边框宽度]
//行为属性
//     1。是否是焦点，是否高亮，是否
//
//
//======================================================


/*
xuiWindow的一些基本相关状态:
1.  是不是高亮? 
2.  父窗口是不是有焦点
3.  是不是有键盘焦点
4.  该xuiWindow所在的Dialog是不是有键盘焦点
5.  是不是有标记状态(check)

xuiButton的一些相关状态:
1.  xuiWindow的状态
2.  鼠标是不是在窗口上面
3.  鼠标是不是按下?
4.  
*/
#pragma pack(push , 1)
struct xuiWindowBorder_t
{
    int       m_size;
    xuiBevel  m_bevel; //边框圆角的大小
    xColor_4f m_color;
};

struct _XEVOL_BASE_API_ xuiWindowTransform_t
{
    wchar_t                m_name[16];
    xMathLib::xvec4        m_rotCenter;
    float                  m_rotAngle;
    xMathLib::xvec4        m_scale;
    xMathLib::xvec4        m_translate;

public:
    void                   setName(const wchar_t* _name);
    void                   begin();
    void                   end();
    xuiWindowTransform_t();

    static xuiWindowTransform_t  XUITRANFORM_IDENTITY;//单位变换
};

class _XEVOL_BASE_API_  xuiWindowTransBlender
{
    xuiWindowTransform_t    m_Start;
    xuiWindowTransform_t    m_End;
    long                    m_time;
    long                    m_totalTime;
    xuiWindow*              m_pWindow;

public:
    xuiWindowTransBlender(xuiWindow* pWindow);
    virtual ~xuiWindowTransBlender();
    virtual bool update(long passedTime);
    virtual bool isInBlending() const;
    virtual bool stop();
    virtual bool setTrans(xuiWindowTransform_t* pStartState , xuiWindowTransform_t* pEndState , long _time);
    virtual bool setTrans(xuiWindowTransform_t* pEndState , long _time);
    virtual bool getTrans(xuiWindowTransform_t& tans);
};


struct _XEVOL_BASE_API_ xuiWindowState_t
{
    wchar_t                 m_Name[32];
    xColor_4f               m_color;
    xColor_4f               m_textcolor;
    xuiWindowBorder_t       m_border;
    xuiRegion               m_Region;
    void            setName(const wchar_t* _name);
    xuiWindowState_t(const wchar_t* name=L"");
    void operator = (const xuiWindowState_t& rhv);
    static xuiWindowState_t XUISTATE_HIDE;
};




class _XEVOL_BASE_API_ xuiWindowStateBlender
{
    xuiWindowState_t  m_Start;
    xuiWindowState_t  m_End;
    long              m_time;
    long              m_totalTime;
    xuiWindow*        m_pWindow;

public:
    xuiWindowStateBlender(xuiWindow* pWindow);
    virtual ~xuiWindowStateBlender();
    virtual bool update(long passedTime);
    virtual bool isInBlending() const;
    virtual bool stop();
    virtual bool setState(xuiWindowState_t* pStartState , xuiWindowState_t* pEndState , long _time);
    virtual bool setState(xuiWindowState_t* pEndState , long _time);
};
#pragma pack(pop)

typedef std::vector<xuiWindowState_t> xuiWindowStates_t;
typedef std::vector<xuiWindow*>       xuiWindows;
class _XEVOL_BASE_API_ xuiWindow : public IRefCountObject
{
    IMPL_REFCOUNT_OBJECT_INTERFACE(xuiWindow);
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
    virtual void            setState(xuiWindowState_t* pState, bool bBlend = true);
    xuiWindowState_t*       findState(const wchar_t* _stateName);


    virtual bool              hasKbInputFocus();
    virtual bool              hasMouseFocus();

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
    virtual bool            drawBorder();


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

protected:
    bool                    _drawRegion(xuiRegion& _region, xColor_4f& cl );
    bool                    _drawRect(xMathLib::xvec4& _rect , xColor_4f& cl);
    bool                    _drawRect(xMathLib::xvec4& _rect , xColor_4f& cl , xuiBevel& bevel);
    bool                    __loadBorder(xuiWindowBorder_t& border , xXmlNode* pNode);
    bool                    __loadAllStates(xXmlNode* pNode);
    bool                    __loadState(xuiWindowState_t& _state, xXmlNode* pNode);
    void                    __setState(xuiWindowState_t* pState);
    bool                    __parseStyle(const wchar_t* _style);

    bool                    __fireCommand(eXUIWinCommand  _command, xuiWindow* pCtrl, int param1 , int param2);  //给父窗口发命令

    bool                    __notifyParent(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2); //给父窗口发通知
    bool                    __notifyChildren(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2);//给子窗口发通知

protected:


protected:
    wchar_t              m_name[32];
    HFontRender          m_hFont;
    bool                 m_bVisable;
    bool                 m_bMouseWindow;
    bool                 m_bKbWindow;
    xuiWindowZValue      m_zValue;
    xuiWindowManager*    m_pWindowMgr;
    xuiWindowStates_t          m_vStates;
    xuiWindowState_t           m_state;
    xuiWindowStateBlender*     m_stateBlend;
    xuiWindowTransBlender*     m_transBlend;

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

