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

#ifndef __XMATH_PLACEMENT_H__
#define __XMATH_PLACEMENT_H__
#include "xMathLib.h"
BEGIN_NAMESPACE_XMATHLIB
using namespace XEvol3D::XMathLib;

struct xRotation
{
	xvec3 m_Axis;
	float m_Angle;
	xRotation()	: m_Angle(0.0f),	m_Axis(0,0,1){	}
};

class  _XEVOL_BASE_API_ xPlacement
{
public:
	xPlacement();
	void    setPosition(xvec3& position);
	void    setRotate(xvec3& axis, float angle);
	void    setRotate(xquat& quat);
	void    setScale(xvec3& scale);

	void    getPosition(xvec3& position);
	void    getRotate(xvec3& axis, float& angle);
	void    getRotate(xquat& quat);
	void    getScale(xvec3& scale);

	void    getTransform(xmat4& _mat);
	void    setInUsed(bool bIsUsed);
	bool    isInUsed();
	xmat4*  toMatrix();
	void    validate();
	void    invalidate();

public:
	xvec3       m_Position;
	xvec3       m_Scale;
	xRotation   m_Rotation;

protected:
	xmat4       m_Matrix;
	bool        m_bIsValidate;
	bool        m_bIsUsed;
};

END_NAMESPACE_XMATHLIB
#endif
