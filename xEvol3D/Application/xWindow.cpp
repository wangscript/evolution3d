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
#include "../xStdPch.h"
#include "xWindow.h"
#include "xListener.h"

BEGIN_NAMESPACE_XEVOL3D

IWindow::IWindow()
{

}
IWindow::~IWindow()
{
}
bool IWindow::onMessage(xWindowMsg& msg)
{
	return false;
}

bool IWindow::dispatchMsg(xWindowMsg& msg)
{
	size_t num = m_MsgListeners.size();

    if(onMessage(msg) == true)
		return true;

	for(size_t i = 0 ; i < num ; i ++)
	{
		IMsgListener* pListener = m_MsgListeners[i];
		if(pListener->dispatchMsg(msg) == false)
			continue;
		return true;
	}
	return false;
}


//?加入一个Listener．
void   IWindow::addMsgListener(IMsgListener* msgListenter)
{
	m_MsgListeners.push_back(msgListenter);
}

//删除一个Listener.
bool   IWindow::removeMsgListener(IMsgListener* msgListenter)
{
	vector<IMsgListener*> ::iterator i  = m_MsgListeners.begin();
	for(; i!= m_MsgListeners.end(); i ++)
	{
		if( *(i) == msgListenter)
		{
			m_MsgListeners.erase(i);
			return true;
		}
	}
	return false;
}

END_NAMESPACE_XEVOL3D
