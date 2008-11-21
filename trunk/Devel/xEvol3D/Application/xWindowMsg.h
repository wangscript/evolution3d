// ***************************************************************
//	XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
//	Stanly.Lee 2006
//	This program is free software; you can redistribute it and/or
//	modify it under the terms of the GNU General Public License
//	as published by the Free Software Foundation; either version 2
//	of the License, or (at your option) any later version.
//	
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//	
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// ***************************************************************
#ifndef  __XWINDOW_MSG_H__
#define  __XWINDOW_MSG_H__
#include "xVirtualKey.h"
//------------------------------------------------------------------------------------------
//消息的定义．
//第一部分．窗口消息
enum eXrMessageID
{
	WIN_CREATE          =   1001,
	WIN_INIT_UPDATE     =   1002,
	WIN_REPAINT         =   1003,
	WIN_CLOSE           =   1004,
	WIN_CLOSED          =   1005,
	WIN_RESIZE          =   1006,
	WIN_SETCURSOR       =   1007,

	//第二部分：鼠标消息
	WIN_MOUSE_MESSAGE_BEGIN = 2000,
	WIN_LMOUSE_CLICK    =   2001,
	WIN_RMOUSE_CLICK    =   2002,
	WIN_MMOUSE_CLICK    =   2003,

	WIN_LMOUSE_DOWN     =   2004,
	WIN_RMOUSE_DOWN     =   2005,
	WIN_MMOUSE_DOWN     =   2006,

	WIN_LMOUSE_DB_CLICK  =  2007,
	WIN_RMOUSE_DB_CLICK   = 2008,
	WIN_MMOUSE_DB_CLICK  =  2009,

	WIN_LMOUSE_RELEASE   =  2010,
	WIN_RMOUSE_RELEASE   =  2011,
	WIN_MMOUSE_RELEASE   =  2012,

	WIN_MOUSE_WHEEL      =  2013,
	WIN_MOUSE_MOVE       =  2014,
	WIN_MOUSE_LEAVE      =  2015,

	WIN_MOUSE_MESSAGE_END = 2999,

	//第三部分：键盘消息
	WIN_KEYBOARD_MESSAGE_BEGIN = 3000,
	WIN_KEYDOWN          =  3001,
	WIN_KEYUP            =  3002,

	WIN_SYS_KEYDOWN      =  3003,
	WIN_SYS_KEYUP        =  3004,

	WIN_CHAR             =  3005,
	WIN_KEYBOARD_MESSAGE_END = 3999,

	//第四部分：应用程序消息
	WIN_QUIT            =   4001,
	WIN_ACTIVATEAPP		=   4002,
	WIN_INPUTLANGCHANGE =   4003,
};

#define IsMouseMsg(id) (id >= WIN_LMOUSE_CLICK && id <= WIN_MOUSE_MOVE)
#define IsKeybdMsg(id) (id >= WIN_KEYDOWN && id <= WIN_CHAR)

//------------------------------------------------------------------------------------------
enum eXrMsgType
{
	XMT_System      ,
	XMT_Application ,
};

struct sXrMouseMsg
{
	eXrMessageID eMsgID;
	int         x;
	int         y;
	int         nParam;
	int         nValue;
};

struct sXrKeyboardMsg
{
	eXrMessageID   eMsgID;
	VIRTUAL_KEY    nKey;
	bool           bIsCharKey;
	char           cKeyChar;
	wchar_t		   nSysVKey;
	int            nScanCode;
};

struct sXrCommonMsg
{
	eXrMessageID	eMsgID;
	union
	{
		struct 
		{
			int             nParam1;
			int             nParam2;
		};
		struct 
		{
			int x , y , w , h;
		};
	};

	unsigned int    nEventID; //Event ID.
};


struct sXrBaseMsg
{
public:
	union
	{
		eXrMessageID	eMsgID;
		sXrCommonMsg	Common;
		sXrKeyboardMsg	Keyboard;
		sXrMouseMsg		Mouse; 
	};

	struct _sSysMsg
	{
		int             nMsg;
		int             lParam;
		int             wParam;
		int             hWnd;
	}sSysMsg;

	eXrMsgType          eMsgType;
};

//------------------------------------------------------------------------------------------
#define WIN_STYLE_COMMON      0x00000000
#define WIN_STYLE_NORESIZE    0x00000001
#define WIN_STYLE_NOMAXBOX    0x00000002
#define WIN_STYLE_NOMINBOX    0x00000004
#define WIN_STYLE_DIALOG      0x80000000
#define WIN_STYLE_NOCAPTION   0x00000008
#define WIN_STYLE_RENDERABLE  0x00000010
//------------------------------------------------------------------------------------------

#endif
