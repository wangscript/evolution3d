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
#include "xIDManager.h"

BEGIN_NAMESPACE_XEVOL3D

int  xIDManager::getID()
{
	if(m_freeIds.size() > 0 )
	{
		int v = m_freeIds[m_freeIds.size() - 1];
		m_freeIds.pop_back();
		return v;
	}
	int v = m_lastID;
	m_lastID ++ ;
	return v ; 
}

int  xIDManager::getID(int nID)
{
	int v = m_lastID;
	m_lastID += nID  ;
	return v ; 
}

void xIDManager::freeID(int id)
{
	m_freeIds.push_back(id);
}

xIDManager::xIDManager()
{
	m_lastID = 0;
}
xIDManager::~xIDManager()
{

}

END_NAMESPACE_XEVOL3D


