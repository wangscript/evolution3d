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

#ifndef __XUI_DIALOG_H__
#define __XUI_DIALOG_H__
#include "xuiCtrlContainer.h"
BEGIN_NAMESPACE_XUI

#define DECL_XUI_DIALOGCREATOR(CLASSNAME , XMLFILE)\
class CLASSNAME##Creator : public xuiWindowCreator{\
public:\
    CLASSNAME##Creator(bool bRegiste){if(bRegiste) {xuiWindowCreator::registeWindowCreate(this);}; }\
    xuiWindow*      createInstance(xuiWindow* parent , xuiWindowManager* pMgr){return new CLASSNAME(parent ,pMgr);}\
    const wchar_t*  scriptFile()    {return XMLFILE;}\
    const wchar_t*  type()          {return _WIDETEXT_(#CLASSNAME);}\
    eXUIWinType     winType()       {return eXUIWT_Dialog;}\
};\


#define REG_XUI_DIALOGCREATOR(CLASSNAME,bReg)\
    static CLASSNAME##Creator gs_##CLASSNAME##Creator(bReg);\

//==============================
class _XEVOL_BASE_API_ xuiCaption;
class _XEVOL_BASE_API_ xuiHotArea;
class _XEVOL_BASE_API_ xuiDialog : public xuiCtrlContainer
{
    friend class xuiWindowManager;
    friend class xuiDialogCreator;
    typedef xuiCtrlContainer xuiDialogBase;
public:
    virtual bool        load(xXmlNode* pCfgNode); // virtual ;
    virtual bool        hide();
    virtual bool        show();
    eXUIWinHitState     hitTest(int x , int y);

    //Attribute
    void                setText(const wchar_t* _text);

    virtual bool        hasKbInputFocus();
    virtual bool        hasActive();
    virtual bool        setActive(bool bActive);
    //行为函数，内部使用
protected:
    virtual bool        hasSeperatedKBFocus();
    virtual bool        draw();
    //=================
    virtual bool        leftMouseDown(xMouseMsg& mEvent);
    virtual bool        leftMouseRelease(xMouseMsg& mEvent);
    virtual bool        rightMouseDown(xMouseMsg& mEvent) ;
    virtual bool        rightMouseRelease(xMouseMsg& mEvent);
    virtual bool        midleMouseDown(xMouseMsg& mEvent);
    virtual bool        midleMouseRelease(xMouseMsg& mEvent);
    virtual bool        mouseMove(xMouseMsg& mEvent);
    virtual bool        mouseWhell(xMouseMsg& mEvent);

    virtual bool        keyDown(xKeyboardMsg& kEvent);
    virtual bool        keyUp(xKeyboardMsg& kEvent);

    virtual bool        command(eXUIWinCommand  _command , xuiWindow* pCtrl, int param1 , int param2);
    virtual bool        notify(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2);

    //以下函数为重载，当发生onXXXX中的XXXX行为后，会自动调用该函数
protected:
    virtual bool        onLoad(xXmlNode* pCfgNode);
    virtual bool        onDraw();
    virtual bool        onShow();
    virtual bool        onHide();
    virtual void        onLostActive();
    virtual void        onActive();


protected:
    virtual bool        onCommand(eXUIWinCommand  _command, xuiWindow* pCtrl     , int param1 , int param2);
    virtual bool        onNotify(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2);

protected:
    virtual bool        _create(xXmlNode*){return true;}
    virtual bool        _draw(){return true;}
    //bool                __loadControl(xXmlDocument::XmlNodes& _vCtrlNodes);
    //bool                __loadControl(xXmlNode* pCfgNode);

    bool                __loadHotArea(xXmlDocument::XmlNodes& _vHotAreasNodes);
    bool                __loadHotArea(xXmlNode* pCfgNode);

protected:
    bool      _onMouseMsg(xMouseMsg& mEvent);
    bool      _onKeyboardMsg(xKeyboardMsg& kEvent);
protected:
    xuiDialog(xuiWindow* parent, xuiWindowManager* pMgr);
    ~xuiDialog();

protected:
    wchar_t             m_name[64];
    xuiCaption*         m_pCaption;
    vector<xuiHotArea*> m_vHotAreas;
    bool                m_bIconize;
    bool                m_bKBHasSeperatedFocus;//键盘和鼠标有独立的输入焦点
};

END_NAMESPACE_XUI

#endif
