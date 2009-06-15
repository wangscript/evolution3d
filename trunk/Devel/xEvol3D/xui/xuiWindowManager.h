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

#ifndef __XUI_WINDOW_MANAGER_H__
#define __XUI_WINDOW_MANAGER_H__
#include "xuibase.h"

BEGIN_NAMESPACE_XUI

class _XEVOL_BASE_API_ xuiDialog;
class _XEVOL_BASE_API_ xuiControl;
class _XEVOL_BASE_API_ xuiWindow;
class _XEVOL_BASE_API_ xuiWindowManager;
class _XEVOL_BASE_API_ xuiWindowCreator;
class _XEVOL_BASE_API_ xuiHotArea;
enum eXUIWinCommand;

class _XEVOL_BASE_API_ xuiWindowOpenCloseEffect
{
protected:
    xuiDialog* m_pCloseWindow;
    xuiDialog* m_pOpenWindow;
};

class xuiInputGather
{
public:

};

class _XEVOL_BASE_API_ xuiCommandProc
{
public:
    virtual bool operator()(xuiDialog* pDialog , eXUIWinCommand  _command, xuiWindow* pCtrl, int param1 , int param2) = 0;
};

class _XEVOL_BASE_API_ xuiWindowManager : public IMsgListener
{
    xuiCommandProc*          m_commandProc;
protected:
    int                      m_thisTime;
    xuiWindow*               m_pCaptureWindow;
    vector<xuiHotArea*>      m_vHotAreas;
    vector<xuiWindow*>       m_modalWindows;
    vector<xuiDialog*>       m_Dialogs;
    vector<xuiDialog*>       m_visibleDialog;
    vector<xuiDialog*>       m_blendingDialogs;
    map<int , xXmlNode*>     m_ctrlLib;

    //с╣сп
    xuiDialog*               m_pActiveWindow;
    xuiWindow*               m_pMouseFocusWindow;

    xuiWindowOpenCloseEffect m_pOpenCloseEffect;
    xXMLDocMgr               m_XMLManager;
    IFrameUpdateObject*      m_AutoUpdater;
    IRenderApi*              m_pRenderApi;
    xBaseTextureMgr*         m_pTextureMgr;
    I2DRectObject*           m_p2DRectObject;
    IGpuProgram*             m_GpuProgram[2];
 
    
DECL_REFCOUNT_OBJECT_INTERFACE(xuiWindowManager);
protected:
    xuiWindowManager(IRenderApi* pRenderApi , xBaseTextureMgr* pTexMgr);
public:

    static xuiWindowManager* createManager(IRenderApi* pRenderApi , xBaseTextureMgr* pTexMgr);
    ~xuiWindowManager();
    IRenderApi*              renderApi() { return m_pRenderApi ; }
    xBaseTextureMgr*         textureManager() { return m_pTextureMgr ; };
    bool                     drawRectf(int nTextureLayer , IBaseTexture* pTexture[], float vDestRect[4] , xColor_4f& color);
    bool                     drawRectf(int nTextureLayer , IBaseTexture* pTexture[], float vDestRect[4] , float vSrcRect[4]  , xColor_4f& color);
    bool                     drawRectf(I2DRectObject*   p2DRectObject = NULL);
    unsigned  int            getTime();
    static xuiRect           clipRect(xuiRect& _baseRect , xuiRect& _rect);
    xXMLDocMgr*              getXMLManager();
    xuiDialog*               createDialogFromFile(const wchar_t* _xuiScript,xuiWindow* parent = NULL);
    xuiDialog*               createDialog(const wchar_t* _typeName , xuiWindow* parent = NULL);
    xuiWindow*               createControl(const wchar_t* _typeName, xuiWindow* parent);
    void                     hideDialog(xuiDialog*    pWindow);
    void                     showDialog(xuiDialog*    pWindow);
    bool                     loadPlugin(const wchar_t* dir);
    I2DRectObject*           get2DRectObject(){return m_p2DRectObject ; }
    void                     setMouseFocus(xuiWindow* pWindow,bool bFocus);
    xuiWindow*               getMouseFocus();

    void                     setActiveDialog(xuiDialog* pWindow,bool bFocus);
    xuiDialog*               getActiveDialog();

    bool                     onMessage(xWindowMsg& msg);
    void                     draw();

    void                     setCommandProc(xuiCommandProc* proc);
    bool                     onCommand(xuiDialog* pDialog , eXUIWinCommand  _command, xuiWindow* pCtrl, int param1 , int param2);
    bool                     updateFrame(int passedTime , unsigned int thisTime);

    void                     addHotArea(xuiHotArea* pHotArea);
    void                     removeHotArea(xuiHotArea* pHotArea);
    bool                     localToUTF8(const wchar_t* _strIn, wchar_t* _strOut,int len);

    bool                     addCommCtrlLib(const wchar_t* _file);
    xXmlNode*                findCommCtrl(const wchar_t* _name);
protected:
    bool                     onMouseMsg(xMouseMsg& msg);
    bool                     onKeyboardMsg(xKeyboardMsg& msg);
    bool                     onViewportResize(int w , int h);
    //
    //мо╥е
    //bool  onDragBegin();
    //bool  onDragOver();
    //bool  onDragEnd();
};

#define XUI_CREATE_DIALOG(_windowClass , _winMgr) (  (_winMgr== NULL) ? NULL :  (nsXui::_windowClass*) _winMgr->createDialog( _WIDETEXT_(#_windowClass) ) )

END_NAMESPACE_XUI
#endif

