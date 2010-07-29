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
	typedef ds_vector(xuiHotArea*)      vHotAreas;
	typedef ds_vector(xuiWindow*)       vWindows;
	typedef ds_vector(xuiDialog*)       vDialogs;
	typedef ds_map(int , xXmlNode*)     mapCtrlLib;
protected:
    int                      m_thisTime;
    xuiWindow*               m_pCaptureWindow;
    vHotAreas      m_vHotAreas;
    vWindows       m_modalWindows;
    vDialogs       m_Dialogs;
    vDialogs       m_visibleDialog;
    vDialogs       m_blendingDialogs;
    mapCtrlLib     m_ctrlLib;

    //с╣сп
    xuiDialog*               m_pActiveWindow;
    xuiWindow*               m_pMouseFocusWindow;

    xuiWindowOpenCloseEffect m_pOpenCloseEffect;
    xXMLDocMgr               m_XMLManager;
    IFrameUpdateObject*      m_AutoUpdater;
    IRenderApi*              m_pRenderApi;
    xBaseTextureMgr*         m_pTextureMgr;
    x2DRenderer*             m_p2DRenderer;
    xRefCount                m_RefCount;
    DECL_REFCOUNT_OBJECT_INTERFACE(xuiWindowManager);
protected:
    xuiWindowManager(IRenderApi* pRenderApi , xBaseTextureMgr* pTexMgr);
public:

    static xuiWindowManager* createManager(IRenderApi* pRenderApi , xBaseTextureMgr* pTexMgr);
    ~xuiWindowManager();
    x2DRenderer*             get2DRenderer(){ return m_p2DRenderer ; }
    IRenderApi*              renderApi() { return m_pRenderApi ; }
    xBaseTextureMgr*         textureManager() { return m_pTextureMgr ; };
    bool                     drawRectf(int nTextureLayer , IBaseTexture* pTexture[], float vDestRect[4] , const xColor_4f& color);
    bool                     drawRectf(int nTextureLayer , IBaseTexture* pTexture[], float vDestRect[4] , float vSrcRect[4]  ,const  xColor_4f& color);
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

