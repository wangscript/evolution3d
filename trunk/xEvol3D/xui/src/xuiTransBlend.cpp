/*
XReal3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xreal3d/) 
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

#include "../../xStdPch.h"
#include "../xuiWindow.h"
BEGIN_NAMESPACE_XUI
void xuiWindowTransform::setName(const wchar_t* _name)
{

}

xuiWindowTransform::xuiWindowTransform()
{

}

void xuiWindowTransform::begin()
{

}

void xuiWindowTransform::end()
{

}

xuiWindowTransform  xuiWindowTransform::XUITRANFORM_IDENTITY;//µ¥Î»±ä»»


xuiWindowTransBlender::xuiWindowTransBlender(xuiWindow* pWindow)
{
    return ;
}

xuiWindowTransBlender::~xuiWindowTransBlender()
{
    return ;
}

bool xuiWindowTransBlender::update(long passedTime)
{
    return true;
}

bool xuiWindowTransBlender::isInBlending() const
{
    return true;
}

bool xuiWindowTransBlender::stop()
{
    return true;
}

bool xuiWindowTransBlender::setTrans(xuiWindowTransform* pStartState , xuiWindowTransform* pEndState , long _time)
{
    return true;
}

bool xuiWindowTransBlender::setTrans(xuiWindowTransform* pEndState , long _time)
{
    return true;
}

bool xuiWindowTransBlender::getTrans(xuiWindowTransform& tans)
{
    return false;
}

END_NAMESPACE_XUI

