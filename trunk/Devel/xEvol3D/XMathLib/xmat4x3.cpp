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


#include "XMathLib.h"

BEGIN_NAMESPACE_XMATHLIB
/****
transform 3x3 for Matrix4x3
**/
void  xmat4x3::transform_l_3x3(const xvec3& v,xvec3& vOut)
{
	vOut.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0];
	vOut.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1];
	vOut.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2];
}

void  xmat4x3::transform_l_3x3(const xvec4& v,xvec3& vOut)
{
	vOut.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0];
	vOut.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1];
	vOut.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2];
}

void  xmat4x3::transform_l_3x3(const xvec3& v,xvec4& vOut)
{
	vOut.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0];
	vOut.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1];
	vOut.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2];
}

void  xmat4x3::transform_l_3x3(const xvec4& v,xvec4& vOut)
{
	vOut.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0];
	vOut.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1];
	vOut.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2];
}


void  xmat4x3::transform_r_3x3(const xvec3& v,xvec3& vOut)
{
	vOut.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z ;
	vOut.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z ;
	vOut.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z ;

}

void  xmat4x3::transform_r_3x3(const xvec4& v,xvec3& vOut)
{
	vOut.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z ;
	vOut.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z ;
	vOut.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z ;
}

void  xmat4x3::transform_r_3x3(const xvec3& v,xvec4& vOut)
{
	vOut.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z ;
	vOut.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z ;
	vOut.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z ;

}

void  xmat4x3::transform_r_3x3(const xvec4& v,xvec4& vOut)
{
	vOut.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z ;
	vOut.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z ;
	vOut.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z ;

}

void  xmat4x3::from(const xmat4& _m)
{
	m[0][0] = _m.m[0][0];    m[0][1] = _m.m[0][1];    m[0][2] = _m.m[0][2];
	m[1][0] = _m.m[1][0];    m[1][1] = _m.m[1][1];    m[1][2] = _m.m[1][2];
	m[2][0] = _m.m[2][0];    m[2][1] = _m.m[2][1];    m[2][2] = _m.m[2][2];
	m[3][0] = _m.m[3][0];    m[3][1] = _m.m[3][1];    m[3][2] = _m.m[3][2];
}

void  xmat4x3::from(const xmat3& _m)
{
	identity();
	m[0][0] = _m.m[0][0];    m[0][1] = _m.m[0][1];    m[0][2] = _m.m[0][2];
	m[1][0] = _m.m[1][0];    m[1][1] = _m.m[1][1];    m[1][2] = _m.m[1][2];
	m[2][0] = _m.m[2][0];    m[2][1] = _m.m[2][1];    m[2][2] = _m.m[2][2];
	m[3][0] = 0.0f      ;    m[3][1] = 0.0f      ;    m[3][2] = 0.0f      ;
}


bool  xmat4x3::inverse()
{
	xmat4 mat;
	xmat4 mati;
	mat.from(*this);
	bool ret = XM_Inv(mat,mati);
	if(ret)
	{
		from(mati);
		return true;
	}
	return false;
}


bool	_XEVOL_BASE_API_ XM_Inv(const xmat4x3& m1 , xmat4x3& mOut)
{
	xmat4 mat;
	xmat4 mati;
	mat.from(m1);
	bool ret = XM_Inv(mat,mati);
	mOut.from(mati); 
	return ret;

}

END_NAMESPACE_XMATHLIB
