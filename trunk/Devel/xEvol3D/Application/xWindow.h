/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
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

#ifndef  __XWINDOW_H__
#define  __XWINDOW_H__
#include "xWindowMsg.h"
#include "../BaseLib/xEvol3DAPI.h"
#include "../BaseLib/xRect.h"
#include "../Image/Color.h"
#pragma warning(disable : 4996)

#include <vector>
using namespace std;

BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_  IWindow;
struct xWindowMsg : public xBaseWindowMsg
{
	IWindow*      m_pWndReciver;
};

typedef bool (*PFNWINDOWPROC)(IWindow* pWnd,xWindowMsg msg);
class IMsgListener;
struct _XEVOL_BASE_API_  WIN_CREATE_STRUCT
{
public:
	WIN_CREATE_STRUCT()
	{
		wcscpy(m_Caption,L"Evolution3D Engine Application");
		wcscpy(m_WinName,L"Evolution3D");
		m_isFullScreen = false;
		m_Style = WIN_STYLE_COMMON;
		m_WinRect = x2DRect(0,0,640,480);
		m_isExclusive = false;
		m_BackGrdColor= xColor_4ub(128,128,128);
	}

	x2DRect       m_WinRect;
	int           m_Style;
	wchar_t       m_Caption[64];
	wchar_t       m_WinName[64];
	bool          m_isFullScreen;
	bool          m_isExclusive;
	xColor_4ub    m_BackGrdColor;
};

class _XEVOL_BASE_API_  IWindow
{
public:
	IWindow();
	virtual ~IWindow();


	//virtual  bool createWindow(WIN_CREATE_STRUCT createStruct);

	//--------------------------------------------------
	//onMessage 函数为消息处理函数．
	//如果onMessage 函数返回了true．表示，消息已经被处理．
	//否则消息是没有被处理．则需要进一步进行处理
	//--------------------------------------------------
	virtual  bool dispatchMsg(xWindowMsg& msg);
	virtual  bool onMessage(xWindowMsg& msg);
	void          addMsgListener(IMsgListener* msgListenter);
	bool          removeMsgListener(IMsgListener* msgListenter);
    virtual void  setWindowPosition(int x , int y)=0;
	virtual void  setWindowSize(int w , int h) = 0;
	virtual void  getWindRect(x2DRect& rect) = 0;
	virtual void  getClientRect(x2DRect& rect) = 0;
	virtual long  getHandle() = 0;

	virtual void  setWindowCaption( const wchar_t* text) = 0;
	virtual int   getWindowCaption(wchar_t*buff,int len) = 0;
	virtual bool  close() = 0;
	virtual bool  show(bool bShow) = 0;
	virtual void  getMousePosition(x2DPoint& point) =0;
    virtual bool  resetWindowMsgHandler() = 0; //Add for Mozilla
	virtual bool  activeWindow() = 0; //Add for Mozilla
	virtual bool  deactiveWindow() = 0; //Add for Mozilla

	virtual const wchar_t* name() = 0;
	virtual int   RefCount() = 0;
	virtual int   AddRef() = 0;
	virtual int   ReleaseObject() = 0;
	virtual int   KillObject() = 0;


private:

	vector<IMsgListener*> m_MsgListeners;

};

END_NAMESPACE_XEVOL3D

#endif
