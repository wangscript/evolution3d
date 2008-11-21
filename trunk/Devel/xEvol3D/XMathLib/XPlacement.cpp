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

#include "xPlacement.h"
BEGIN_NAMESPACE_XGEOMLIB

xPlacement::xPlacement(): m_Position(0,0,0) , m_Scale(1,1,1)
{
	xvec3 axis(0,0,1);
	setRotate(axis,0);
	m_bIsVialidate = true;
	m_Matrix.identity();
	m_bIsUsed = true;
}

void xPlacement::setInUsed(bool bIsUsed)
{
	m_bIsUsed = bIsUsed;
}
bool xPlacement::isInUsed()
{
	return m_bIsUsed;
}
void xPlacement::setPosition(xvec3& position)
{
	m_bIsVialidate = false;
	m_Position = position;
}

void xPlacement::setRotate(xvec3& axis, float angle)
{
	m_bIsVialidate = false;
	m_Rotation.m_Angle = angle;
	m_Rotation.m_Axis  = axis;
	m_Rotation.m_Quat.set(axis,angle);

}

void xPlacement::setRotate(xquat& quat)
{
	m_bIsVialidate = false;
	m_Rotation.m_Quat = quat;
	m_Rotation.m_Quat.toAxis(m_Rotation.m_Axis,m_Rotation.m_Angle);
}

void xPlacement::setScale(xvec3& scale)
{
	m_Scale = scale;
}

void xPlacement::getPosition(xvec3& position)
{
	position = m_Position;
}

void xPlacement::getRotate(xvec3& axis, float& angle)
{
	m_Rotation.m_Quat.toAxis(axis,angle); 
}

void xPlacement::getRotate(xquat& quat)
{
	quat = m_Rotation.m_Quat;
}

void xPlacement::getScale(xvec3& scale)
{
	scale = m_Scale;
}

void xPlacement::getTransform(xmat4& mat)
{
	if(m_bIsVialidate)
	{
		mat = m_Matrix;		
		return ;
	}
	xmat4 _mat1;
	_mat1.identity();
	_mat1.scale(m_Scale.x , m_Scale.y , m_Scale.z);
	xmat4 _mat_rot;
	m_Rotation.m_Quat.toMatrix(_mat_rot);
	XM_Mul(_mat_rot,_mat1,mat);
	mat.setTransform(m_Position.x, m_Position.y, m_Position.z);

	m_Matrix = mat;
	m_bIsVialidate = true;
}

END_NAMESPACE_XGEOMLIB
