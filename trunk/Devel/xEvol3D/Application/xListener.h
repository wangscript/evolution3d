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

#ifndef __WIN_MSG_LISTNER_H__
#define __WIN_MSG_LISTNER_H__
#include "xWindow.h"

BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_  IMsgListener
{
	virtual bool onMessage(xWindowMsg& msg) = 0;

public:
	//--------------------------------------------------
	//onMessage 函数为消息处理函数．
	//如果onMessage 函数返回了true．表示，消息已经被处理．
	//否则消息是没有被处理．则需要进一步进行处理
	//--------------------------------------------------
	virtual bool translator(xWindowMsg& msgIn , xWindowMsg& _out)
	{
		return false;
	}
	virtual bool hasTranslator()
	{
		return false;
	}

	bool         isOnListen()
	{
		return m_isOnListening;
	}
	void         turnOn(bool isOn = true)
	{
		m_isOnListening = isOn;
	}

	bool dispatchMsg(xWindowMsg& msg );
private:
	bool m_isOnListening;
};

END_NAMESPACE_XEVOL3D

#endif

