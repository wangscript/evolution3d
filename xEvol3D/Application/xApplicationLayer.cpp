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
#include "xApplicationLayer.h"

BEGIN_NAMESPACE_XEVOL3D

bool IAppLayer::preHandle()
{
	return true;
}


bool IAppLayer::onUnhandle()
{
	return true;
}

bool IAppLayer::preUnhandle()
{
	return true;
}

bool IAppLayer::postHandle()
{
	return true;
}

bool IAppLayer::postUnhandle()
{
	return true;
}

bool IAppLayer::updateFrame(long timePassed )
{
	return true;
}

bool IAppLayer::prevUpdateFrame(long timePassed )
{
	return true;
}

bool IAppLayer::postUpdateFrame(long timePassed )
{
	return true;
}


END_NAMESPACE_XEVOL3D

