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

#ifndef __WIN32__WINDOW_IMPLEMT_H__
#define __WIN32__WINDOW_IMPLEMT_H__

#include <Application/xWindow.h>
#include <BaseLib/xCfgParser.h>
#include <windows.h>

namespace XEvol3D
{
	class  PlatformWin32;//forward declare
	class  Win32WindowImp :public IWindow
	{
		friend class PlatformWin32;
		wchar_t             m_name[64];
	public:
		IMPL_REFCOUNT_OBJECT_INTERFACE(Win32WindowImp);
		Win32WindowImp();
		const wchar_t*    name(){return m_name;}
		virtual void      getWindRect(x2DRect& rect);
		virtual void      getClientRect(x2DRect& rect);
		virtual void      setWindowPosition(int x , int y);
		virtual void      setWindowSize(int w , int h);
		//------------------------------------
		//´°¿Ú±êÌâ
		//------------------------------------
		virtual void      setWindowCaption(const wchar_t* text);
		virtual int       getWindowCaption(wchar_t*buff,int len);
		virtual bool      close();
		virtual long      getHandle();
		virtual bool      show(bool bShow);
		virtual void      getMousePosition(x2DPoint& point);

		virtual bool      resetWindowMsgHandler();
		virtual bool      activeWindow() ; //Add for Mozilla
		virtual bool      deactiveWindow() ; //Add for Mozilla

	public:
		bool              __createWindow(WIN_CREATE_STRUCT& cs,const xCfgNode* params);
	public :
		void              saveWindowMsgHandler();
		void              attach(HWND hWnd);
		HWND              getHWND();

	protected:
		bool              __createFullScreenWindow(WIN_CREATE_STRUCT& createStruct,HWND hParentWnd);
		bool              __createNormalWindow(WIN_CREATE_STRUCT& createStruct,HWND hParentWnd);
		bool              __createWindow(WIN_CREATE_STRUCT& createStruct,HWND hParentWnd);
		bool              __createWindow(HWND hWnd);
		
	protected:
		HWND              m_hWindow;
		bool              m_hAttached;
		WNDPROC           m_wProc;
		long              m_pWndOldProc;
	};
}

#endif
