#include "xStdPch.h"
#include "Application/xWindow.h"
#include <Windows.h>
#include "xWin32Platform.h"
BEGIN_NAMESPACE_XEVOL3D


void translateKbMsg(xWindowMsg& msg)
{
	//×ª»»³ÉÉ¨ÃèÂë
	unsigned short charCode;
	msg.Keyboard.nScanCode = MapVirtualKey(msg.Keyboard.nSysVKey , 0);
	BYTE KeyState[256];
	GetKeyboardState(KeyState);
	int nChar = ToAscii(msg.Keyboard.nSysVKey , msg.Keyboard.nScanCode,KeyState,&charCode,0);
	switch(nChar)
	{
	case 0:
		msg.Keyboard.cKeyChar = 0;
		break;
	case 1:
		msg.Keyboard.cKeyChar  = (char)charCode;
		break;
	case 2:
		msg.Keyboard.cKeyChar  = (char)charCode;

		break;   
	}
	//unsigned int keyChar = MapVirtualKey(msg.Keyboard.nSysVKey , 2);
	msg.Keyboard.nKey = MAPVKEY( msg.Keyboard.nSysVKey );;
}

#define LOSHORT(l)           ((short)(((DWORD_PTR)(l)) & 0xffff))
#define HISHORT(l)           ((short)((((DWORD_PTR)(l)) >> 16) & 0xffff))

void  processMouseMsg(xWindowMsg& msg,WPARAM wParam,LPARAM lParam)
{
	int x, y;
	y=HISHORT(lParam);
	x=LOSHORT(lParam);
	msg.Mouse.x = x;
	msg.Mouse.y = y;
	switch( LOWORD(wParam) )
	{
	case MK_LBUTTON:
		msg.Mouse.nParam  = MKEY_LBUTTON;
		break;
	case MK_RBUTTON:
		msg.Mouse.nParam = MKEY_RBUTTON;
		break;
	case MK_SHIFT:
		msg.Mouse.nParam =  MKEY_RBUTTON;
		break;  
	case MK_CONTROL :
		msg.Mouse.nParam = MKEY_CONTROL;
		break;
	case MK_MBUTTON:
		msg.Mouse.nParam =  MKEY_MBUTTON;
		break;
	default:
		msg.Mouse.nParam = 0;
	}

}

bool ConvertToXEvolMsg(xWindowMsg& WinMsg , HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	WinMsg.eMsgType = XMT_System;
	//Fill the Windows System Message;
	WinMsg.sSysMsg.hWnd   = (unsigned int)(hWnd);
	WinMsg.sSysMsg.nMsg   = (unsigned long)(msg);
	WinMsg.sSysMsg.lParam = (unsigned long)(lParam);
	WinMsg.sSysMsg.wParam = (unsigned long)(wParam);
	switch(msg)
	{
	case WM_ERASEBKGND:
		return 1;
	case WM_LBUTTONDBLCLK:
		WinMsg.MsgID = WIN_LMOUSE_DB_CLICK;
		processMouseMsg(WinMsg,wParam,lParam);
		return true;

	case WM_LBUTTONDOWN:
		WinMsg.MsgID = WIN_LMOUSE_DOWN;
		processMouseMsg(WinMsg,wParam,lParam);
		return true;

	case WM_LBUTTONUP:
		WinMsg.MsgID = WIN_LMOUSE_RELEASE;
		processMouseMsg(WinMsg,wParam,lParam);
		return true;

	case WM_RBUTTONDBLCLK:
		WinMsg.MsgID = WIN_RMOUSE_DB_CLICK;
		processMouseMsg(WinMsg,wParam,lParam);
		return true;

	case WM_RBUTTONDOWN:
		WinMsg.MsgID = WIN_RMOUSE_DOWN;
		processMouseMsg(WinMsg,wParam,lParam);
		return true;

	case WM_RBUTTONUP:
		WinMsg.MsgID = WIN_RMOUSE_RELEASE;
		processMouseMsg(WinMsg,wParam,lParam);
		return true;

	case WM_MBUTTONDBLCLK:
		WinMsg.MsgID = WIN_MMOUSE_DB_CLICK;
		processMouseMsg(WinMsg,wParam,lParam);
		return true;

	case WM_MBUTTONDOWN:
		WinMsg.MsgID = WIN_MMOUSE_DOWN;
		processMouseMsg(WinMsg,wParam,lParam);
		return true;

	case WM_MBUTTONUP:
		WinMsg.MsgID = WIN_MMOUSE_RELEASE;
		processMouseMsg(WinMsg,wParam,lParam);
		return true;

	case WM_MOUSEWHEEL:
		WinMsg.MsgID = WIN_MOUSE_WHEEL;
		WinMsg.Mouse.nValue = HIWORD(wParam);
		processMouseMsg(WinMsg,wParam,lParam);
		return true;

	case WM_MOUSEMOVE :
		WinMsg.MsgID = WIN_MOUSE_MOVE;
		processMouseMsg(WinMsg,wParam,lParam);
		return true;
		break;

	case WM_SIZE:
		WinMsg.MsgID = WIN_RESIZE;
		WinMsg.Common.x = 0;
		WinMsg.Common.y = 0;
		WinMsg.Common.w = LOWORD(lParam);
		WinMsg.Common.h = HIWORD(lParam);
		return true;


	case WM_KEYDOWN:
		WinMsg.Keyboard.nSysVKey  = (unsigned char)wParam;
		WinMsg.MsgID = WIN_KEYDOWN;
		translateKbMsg(WinMsg);
		return true;

	case WM_KEYUP:
		WinMsg.Keyboard.nSysVKey = (unsigned char)wParam;
		WinMsg.MsgID          = WIN_KEYUP;
		translateKbMsg(WinMsg);
		return true;

	case WM_CLOSE:
		WinMsg.MsgID = WIN_CLOSE;
		return true;

	case WM_DESTROY:
		WinMsg.MsgID = WIN_CLOSED;
		return true;

	case WM_QUIT:
		WinMsg.MsgID = WIN_QUIT;
		return true;
	}
	return false;

}

END_NAMESPACE_XEVOL3D