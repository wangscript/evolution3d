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

#ifndef __XUI_CTRL_CONTAINER_H__
#define __XUI_CTRL_CONTAINER_H__
#include "xuiWindow.h"
#include "xuiControl.h"
BEGIN_NAMESPACE_XUI
class _XEVOL_BASE_API_ xuiControl;
class _XEVOL_BASE_API_ xuiDialog;
class _XEVOL_BASE_API_ xuiCtrlContainer : public xuiControl
{
public:
    void          addControl(xuiControl* pCtrl);
    xuiControl*   getControl(int ctrlID);
    xuiControl*   getControl(const wchar_t* _name);
    bool          moveKBFocus(int _dir);
    virtual bool  keyDown(xKeyboardMsg& kEvent);
    virtual bool  keyUp(xKeyboardMsg& kEvent);
    virtual bool  leftMouseDown(xMouseMsg& mEvent);
    virtual bool  leftMouseRelease(xMouseMsg& mEvent);
    virtual bool  leftMouseDbClick(xMouseMsg& mEvent);
    virtual bool  rightMouseDown(xMouseMsg& mEvent);
    virtual bool  rightMouseRelease(xMouseMsg& mEvent);
    virtual bool  midleMouseDown(xMouseMsg& mEvent);
    virtual bool  midleMouseRelease(xMouseMsg& mEvent);
    virtual bool  mouseMove(xMouseMsg& mEvent);
    virtual bool  mouseWhell(xMouseMsg& mEvent);

    virtual bool  setKBInputFocus(xuiControl* pCtrl);


    typedef xuiControl _TBase;

public:
    virtual void  onControlLoad(xXmlNode* pControlNode , xuiWindow* pWindow);
protected:
    bool          loadAllControl(xXmlNode* pCfgNode);
    void          onLostActive();
    void          onActive();
protected:
    xuiCtrlContainer(xuiWindow* parent , xuiWindowManager* pMgr) ;
    ~xuiCtrlContainer();
private:
    bool          __loadControl(xXmlNode::XmlNodes& _vCtrlNodes);
    bool          __loadControl(xXmlNode* pCfgNode);
protected:
    typedef std::vector<xuiControl* , dllsafe_alloc<xuiControl*> > xuiControlVec;
    xuiControlVec m_vControls;
    xuiControlVec m_vCtrlTabIndex;
    xuiControl*   m_pKeyFocusCtrl;
};


END_NAMESPACE_XUI
#endif

