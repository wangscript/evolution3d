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

#ifdef _WIN32

#pragma warning (disable:4311)
#pragma warning (disable:4312)
#include "xWindowImp.h"
#include <Application/xPlatform.h>
BEGIN_NAMESPACE_XEVOL3D
Win32WindowImp::Win32WindowImp()
{
	m_RefCount    = 1;
	m_wProc       = NULL;
	m_hWindow     = NULL;
	m_hAttached   = false;
	m_pWndOldProc = NULL;
}

void     Win32WindowImp::getMousePosition(x2DPoint& point)
{
	::GetCursorPos((POINT*)&point);
	return ;
}

bool Win32WindowImp::activeWindow()
{
	if(m_hWindow)
	{
		::SendMessage(m_hWindow , WM_ACTIVATE , (WPARAM)WA_ACTIVE , (LPARAM)m_hWindow);	
		return true;
	}
	return false;
}

bool Win32WindowImp::deactiveWindow()
{
	if(m_hWindow)
	{
		::SendMessage(m_hWindow , WM_ACTIVATE , (WPARAM)WA_INACTIVE , (LPARAM)m_hWindow);	
		return true;
	}
	return false;
}


bool Win32WindowImp::resetWindowMsgHandler()
{
	::SendMessage(m_hWindow , WM_ACTIVATE , (WPARAM)WA_ACTIVE , (LPARAM)m_hWindow);	
	if(m_pWndOldProc)
		::SetWindowLong(m_hWindow , GWL_WNDPROC , m_pWndOldProc);

	return true;
}

void Win32WindowImp::saveWindowMsgHandler()
{
	m_pWndOldProc =  ::GetWindowLong(m_hWindow , GWL_WNDPROC);
}
void     Win32WindowImp::getWindRect(x2DRect& rect)
{
	RECT r;
	GetWindowRect(m_hWindow,&r);
	rect.x = r.left;
	rect.y = r.top ;
	rect.w = r.right - r.left;
	rect.h = r.bottom - r.top;
	return ;
}

void     Win32WindowImp::getClientRect(x2DRect& rect)
{
	RECT r;
	GetClientRect(m_hWindow,&r);
	rect.x = r.left;
	rect.y = r.top ;
	rect.w = r.right - r.left;
	rect.h = r.bottom - r.top ;
	return;
}

void  Win32WindowImp::setWindowCaption(const wchar_t* text)
{
	SetWindowText(m_hWindow,text);
}

int Win32WindowImp::getWindowCaption(wchar_t*buff,int len)
{
	return GetWindowText(m_hWindow,buff,len);
}

void  Win32WindowImp::attach(HWND hWnd)
{
	m_hWindow = hWnd;
	m_hAttached = true;
}

bool  Win32WindowImp::close()
{
	//如果一个窗口是托管的，不应该删除它
	if(m_hAttached == true)
		return false;

	if(IsWindow( m_hWindow ) )
		::DestroyWindow( m_hWindow );
	m_hWindow = NULL;
	return true;
}

long  Win32WindowImp::getHandle()
{
	return (long) m_hWindow;
}

bool  Win32WindowImp::show(bool bShow)
{
	if(bShow)
		::ShowWindow(m_hWindow,SW_SHOW);
	else
		::ShowWindow(m_hWindow,SW_HIDE);

	return true;
}

HWND Win32WindowImp::getHWND()
{
	return m_hWindow;
}


bool Win32WindowImp::__createFullScreenWindow(WIN_CREATE_STRUCT& createStruct,HWND hParentWnd)
{
	long wndStyle = WS_POPUP;

	RECT _rect={ createStruct.m_WinRect.x,  
		createStruct.m_WinRect.y,   
		createStruct.m_WinRect.x + createStruct.m_WinRect.w,	 
		createStruct.m_WinRect.y + createStruct.m_WinRect.h	};

	IPlatform::singleton()->setDisplayMode(createStruct.m_WinRect.w , createStruct.m_WinRect.h , 32 , 0);
	AdjustWindowRect(&_rect,wndStyle,FALSE);

	m_hWindow = CreateWindowEx(
		WS_EX_TOPMOST,  // Extended style
		createStruct.m_WinName,
		createStruct.m_Caption,
		wndStyle,       // Window style
		_rect.left,                             // Horizontal origin  
		_rect.top ,                             // Vertical origin
		_rect.right  - _rect.left,//GetSystemMetrics(SM_CXSCREEN), // x size
		_rect.bottom - _rect.top ,//GetSystemMetrics(SM_CYSCREEN), // y size
		hParentWnd,            // Handle of parent
		NULL,            // Handle to menu
		GetModuleHandle(NULL),       // Application instance
		NULL);           // Additional data

	long error = GetLastError();
	if (!m_hWindow)
		return false;
	if (!m_hWindow)
	{
		return false;
	}
	::ShowWindow(m_hWindow,SW_HIDE);
	return true;
}

bool Win32WindowImp::__createNormalWindow(WIN_CREATE_STRUCT& createStruct,HWND hParentWnd)
{
	long wndStyle = WS_OVERLAPPEDWINDOW;
	int k = createStruct.m_Style & WIN_STYLE_NORESIZE;
	if( (createStruct.m_Style & WIN_STYLE_NORESIZE ))
	{
		wndStyle = wndStyle & (~ WS_MAXIMIZEBOX );
		wndStyle = wndStyle & (~ WS_MINIMIZEBOX );
		wndStyle = wndStyle & (~ WS_SIZEBOX );
	}
	if( (createStruct.m_Style & WIN_STYLE_NOCAPTION))
	{
		wndStyle = wndStyle & (~ WS_CAPTION);
		wndStyle = wndStyle & (~ WS_SYSMENU);
	}
	if( (createStruct.m_Style & WIN_STYLE_NOMAXBOX ))
	{
		wndStyle = wndStyle & (~ WS_MAXIMIZEBOX );
	}
	if( (createStruct.m_Style & WIN_STYLE_NOMINBOX ))
	{
		wndStyle = wndStyle & (~ WS_MINIMIZEBOX );
	}

    if(hParentWnd)
	{
		wndStyle = WS_CHILD;
	}
	else
	{
		wndStyle = WS_OVERLAPPEDWINDOW;
	}
	
	RECT _rect={ createStruct.m_WinRect.x,  
		createStruct.m_WinRect.y,   
		createStruct.m_WinRect.x + createStruct.m_WinRect.w,	 
		createStruct.m_WinRect.y + createStruct.m_WinRect.h	};

	AdjustWindowRect(&_rect,wndStyle,FALSE);
	this->m_hWindow = CreateWindowEx(
		0,  // Extended style
		createStruct.m_WinName,
		createStruct.m_Caption,
		wndStyle,       // Window style
		createStruct.m_WinRect.x,              // Horizontal origin  
		createStruct.m_WinRect.y,              // Vertical origin
		_rect.right  - _rect.left,//GetSystemMetrics(SM_CXSCREEN), // x size
		_rect.bottom - _rect.top,//GetSystemMetrics(SM_CYSCREEN), // y size
		hParentWnd,            // Handle of parent
		NULL,            // Handle to menu
		GetModuleHandle(NULL),//hInstance,       // Application instance
		NULL);           // Additional data

	if (!m_hWindow)
	{
		HRESULT hr= GetLastError();
		XEVOL_LOG(eXL_ERROR_FALT,L"->error<- create window failed\n");
		return false;

	}
	::ShowWindow(m_hWindow,SW_HIDE);
	return true;
}


bool Win32WindowImp::__createWindow(WIN_CREATE_STRUCT& createStruct,HWND hParentWnd)
{
	if(createStruct.m_isFullScreen == true)
	{
		return __createFullScreenWindow(createStruct, hParentWnd);
	}
	else
	{
		return __createNormalWindow(createStruct, hParentWnd);
	}
}

void  Win32WindowImp::setWindowPosition(int x , int y)
{
	SetWindowPos(m_hWindow , NULL , x , y , 0 , 0 , SWP_NOSIZE);
}
void  Win32WindowImp::setWindowSize(int w , int h)
{
    SetWindowPos(m_hWindow , NULL , 0 , 0 , w , h , SWP_NOMOVE);
}
bool Win32WindowImp::__createWindow(HWND hWnd)
{
	attach((HWND)hWnd);
	return true;
}

bool Win32WindowImp::__createWindow(WIN_CREATE_STRUCT& cs,const xXmlNode* params)
{
	//没有父窗口
	bool ret = false;
	if(params != NULL && params->int_value( L"parentWindow") != NULL)
	{
		HWND hParent = (HWND) params->int_value( L"parentWindow");
		ret = __createWindow(cs,hParent);
		goto rt;
	}
	else if(params != NULL && params->int_value( L"externWindow"))
	{
		HWND hWnd = (HWND) params->int_value( L"externWindow");
		ret = __createWindow(hWnd);
		goto rt;
	}
	else
	{
		ret = __createWindow(cs,(HWND)NULL);
	}

rt:
	return ret;
}
END_NAMESPACE_XEVOL3D

#endif
