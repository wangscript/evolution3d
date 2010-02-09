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
#include "xPlacement.h"
BEGIN_NAMESPACE_XMATHLIB

xPlacement::xPlacement(): m_Position(0,0,0) , m_Scale(1,1,1)
{
	xvec3 axis(0,0,1);
	setRotate(axis,0);
	m_bIsValidate = true;
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
	m_bIsValidate = false;
	m_Position = position;
}

void xPlacement::invalidate()
{
	m_bIsValidate = false;
}

void xPlacement::setRotate(xvec3& axis, float angle)
{
	m_bIsValidate = false;
	m_Rotation.m_Angle = angle;
	m_Rotation.m_Axis  = axis;
}

void xPlacement::setRotate(xquat& quat)
{
	m_bIsValidate = false;
	quat.toAxis(m_Rotation.m_Axis,m_Rotation.m_Angle);
}

void xPlacement::setScale(xvec3& scale)
{
	m_Scale = scale;
	m_bIsValidate = false;
}

void xPlacement::getPosition(xvec3& position)
{
	position = m_Position;
}

void xPlacement::getRotate(xvec3& axis, float& angle)
{
	axis  = m_Rotation.m_Axis ;
    angle = m_Rotation.m_Angle;
}

void xPlacement::getRotate(xquat& quat)
{
	quat.set(m_Rotation.m_Axis,m_Rotation.m_Angle);
}

void xPlacement::getScale(xvec3& scale)
{
	scale = m_Scale;
}

void xPlacement::getTransform(xmat4& mat)
{
	validate();
	mat = m_Matrix;
}

void  xPlacement::validate()
{
	if(!m_bIsValidate)
	{
		xmat4 _mat1;
		_mat1.identity();
		_mat1.scale(m_Scale.x , m_Scale.y , m_Scale.z);
		xmat4 _mat_rot;
		xquat quat ;
		quat.set(m_Rotation.m_Axis , m_Rotation.m_Angle );
		quat.toMatrix(_mat_rot);
		XM_Mul(_mat1,_mat_rot,m_Matrix);
		m_Matrix.setTransform(m_Position.x, m_Position.y, m_Position.z);
		m_bIsValidate = true;
	}
}
xmat4*  xPlacement::toMatrix()
{
	validate();
	return &m_Matrix;
}

END_NAMESPACE_XMATHLIB
