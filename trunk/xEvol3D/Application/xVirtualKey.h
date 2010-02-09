// ***************************************************************
//	XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
//	Stanly.Lee 2006
// 
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

#ifndef __VIRTUAL_VKEY_H__
#define __VIRTUAL_VKEY_H__

#define  VIRTUAL_KEY         unsigned short
#define VKEY_UNSUPPORT      0x00
#define VKEY_LBUTTON        0x01
#define VKEY_RBUTTON        0x02
#define VKEY_CANCEL         0x03
#define VKEY_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

/*
* 0x07 : unassigned
*/

#define VKEY_BACK           0x08
#define VKEY_TAB            0x09

/*
* 0x0A - 0x0B : reserved
*/

#define VKEY_CLEAR          0x0C
#define VKEY_RETURN         0x0D

#define VKEY_SHIFT          0x10
#define VKEY_CONTROL        0x11
#define VKEY_MENU           0x12
#define VKEY_PAUSE          0x13
#define VKEY_CAPITAL        0x14

#define VKEY_KANA           0x15
#define VKEY_HANGEUL        0x15  /* old name - should be here for compatibility */
#define VKEY_HANGUL         0x15
#define VKEY_JUNJA          0x17
#define VKEY_FINAL          0x18
#define VKEY_HANJA          0x19
#define VKEY_KANJI          0x19

#define VKEY_ESCAPE         0x1B

#define VKEY_CONVERT        0x1C
#define VKEY_NONCONVERT     0x1D
#define VKEY_ACCEPT         0x1E
#define VKEY_MODECHANGE     0x1F

#define VKEY_SPACE          0x20
#define VKEY_PRIOR          0x21
#define VKEY_NEXT           0x22
#define VKEY_END            0x23
#define VKEY_HOME           0x24
#define VKEY_LEFT           0x25
#define VKEY_UP             0x26
#define VKEY_RIGHT          0x27
#define VKEY_DOWN           0x28
#define VKEY_SELECT         0x29
#define VKEY_PRINT          0x2A
#define VKEY_EXECUTE        0x2B
#define VKEY_SNAPSHOT       0x2C
#define VKEY_INSERT         0x2D
#define VKEY_DELETE         0x2E
#define VKEY_HELP           0x2F

/*
* VKEY_0 - VKEY_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
* 0x40 : unassigned
* VKEY_A - VKEY_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
*/

#define VKEY_LWIN           0x5B
#define VKEY_RWIN           0x5C
#define VKEY_APPS           0x5D

/*
* 0x5E : reserved
*/

#define VKEY_SLEEP          0x5F

#define VKEY_NUMPAD0        0x60
#define VKEY_NUMPAD1        0x61
#define VKEY_NUMPAD2        0x62
#define VKEY_NUMPAD3        0x63
#define VKEY_NUMPAD4        0x64
#define VKEY_NUMPAD5        0x65
#define VKEY_NUMPAD6        0x66
#define VKEY_NUMPAD7        0x67
#define VKEY_NUMPAD8        0x68
#define VKEY_NUMPAD9        0x69
#define VKEY_MULTIPLY       0x6A
#define VKEY_ADD            0x6B
#define VKEY_SEPARATOR      0x6C
#define VKEY_SUBTRACT       0x6D
#define VKEY_DECIMAL        0x6E
#define VKEY_DIVIDE         0x6F
#define VKEY_F1             0x70
#define VKEY_F2             0x71
#define VKEY_F3             0x72
#define VKEY_F4             0x73
#define VKEY_F5             0x74
#define VKEY_F6             0x75
#define VKEY_F7             0x76
#define VKEY_F8             0x77
#define VKEY_F9             0x78
#define VKEY_F10            0x79
#define VKEY_F11            0x7A
#define VKEY_F12            0x7B
#define VKEY_F13            0x7C
#define VKEY_F14            0x7D
#define VKEY_F15            0x7E
#define VKEY_F16            0x7F
#define VKEY_F17            0x80
#define VKEY_F18            0x81
#define VKEY_F19            0x82
#define VKEY_F20            0x83
#define VKEY_F21            0x84
#define VKEY_F22            0x85
#define VKEY_F23            0x86
#define VKEY_F24            0x87
/*
* 0x88 - 0x8F : unassigned
*/
#define VKEY_NUMLOCK        0x90
#define VKEY_SCROLL         0x91
/*
* 0x97 - 0x9F : unassigned
*/

/*
* VKEY_L* & VKEY_R* - left and right Alt, Ctrl and Shift virtual keys.
* Used only as parameters to GetAsyncKeyState() and GetKeyState().
* No other API or message will distinguish left and right keys in this way.
*/
#define VKEY_LSHIFT         0xA0
#define VKEY_RSHIFT         0xA1
#define VKEY_LCONTROL       0xA2
#define VKEY_RCONTROL       0xA3
#define VKEY_LMENU          0xA4
#define VKEY_RMENU          0xA5


/*
*/
#define MKEY_LBUTTON          0x0001   //MK_CONTROL
#define MKEY_RBUTTON          0x0002   //MK_LBUTTON
#define MKEY_SHIFT            0x0004   //MK_MBUTTON
#define MKEY_CONTROL          0x0008   //MK_RBUTTON
#define MKEY_MBUTTON          0x0010   //MK_SHIFT


#endif

