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

#ifndef __XMATRIX2_FUNC_INLINE_H__
#define __XMATRIX2_FUNC_INLINE_H__
#include "xmathbasedef.h"

BEGIN_NAMESPACE_XMATHLIB

__FORCE_INLINE__ void _XEVOL_BASE_API_  XM_Sub(const xmat2& m1,const xmat2& m2,xmat2& mOut)
{
	mOut.m00 = m1.m00 - m2.m00 ;   mOut.m01 = m1.m01 - m2.m01;
	mOut.m10 = m1.m10 - m2.m10 ;   mOut.m11 = m1.m11 - m2.m11;
}

__FORCE_INLINE__ void XM_Add(const xmat2& m1,const xmat2& m2,xmat2& mOut)
{
	mOut.m00 = m1.m00 + m2.m00;    mOut.m01 = m1.m01 + m2.m01;
	mOut.m10 = m1.m10 + m2.m10;    mOut.m11 = m1.m11 + m2.m11;
}

__FORCE_INLINE__ void  XM_Mul(const xmat2& m1,float s,xmat2& mOut)
{
	mOut.m00 = m1.m00 * s; mOut.m01 = m1.m01 * s; 
	mOut.m10 = m1.m10 * s; mOut.m11 = m1.m11 * s; 
}


//MAT2
__FORCE_INLINE__ void	XM_Mul(const xmat2& m1,const xmat2& m2,xmat2& mOut)
{
    for(int r =0 ; r < 2 ; r++)
    {
        mOut.m[r][0] = m1.m[r][0] * m2.m[0][0] +  m1.m[r][1] * m2.m[1][0];
        mOut.m[r][1] = m1.m[r][0] * m2.m[0][1] +  m1.m[r][1] * m2.m[1][1];
    }
}

__FORCE_INLINE__ void	XM_Mul(const xvec2& v,const xmat2& m,xvec2& vOut)
{
    vOut.x = v.x * m.m[0][0] + v.y * m.m[1][0];
    vOut.y = v.x * m.m[0][1] + v.y * m.m[1][1];
}

__FORCE_INLINE__ void	XM_Mul(const xmat2& m,const xvec2& v,xvec2& vOut)
{
    vOut.x = m.m[0][0] * v.x + m.m[0][1] * v.y;
    vOut.y = m.m[1][0] * v.x + m.m[1][1] * v.y;
}

__FORCE_INLINE__ void	XM_Transpos(const xmat2&m , xmat2& mOut)
{
	for(int r = 0 ;  r < 2 ; r ++)
	{
		mOut.m[0][r] = m.m[r][0];
		mOut.m[1][r] = m.m[r][1];
	}

}

__FORCE_INLINE__ xmat2 _XEVOL_BASE_API_ operator *(const xmat2& m1,float s)
{
	xmat2 mOut;
	XM_Mul(m1,s,mOut);
	return mOut;
}

__FORCE_INLINE__ xmat2 _XEVOL_BASE_API_  operator *(float s,const xmat2& m1)
{
	xmat2 mOut;
	XM_Mul(m1,s , mOut);
	return mOut;
}

//---------------------------------------------------------------------------------
//matrix +- matrix
//---------------------------------------------------------------------------------
__FORCE_INLINE__ xmat2 _XEVOL_BASE_API_  operator -(const xmat2& m1,const xmat2& m2)
{
	xmat2 mOut;
	XM_Sub(m1,m2,mOut);
	return mOut;
}

__FORCE_INLINE__ xmat2 _XEVOL_BASE_API_  operator +(const xmat2& m1,const xmat2& m2)
{
	xmat2 mOut;
	XM_Add(m1,m2,mOut);
	return mOut;
}

//---------------------------------------------------------------------------------
//Matrix2 x Matrix2
//---------------------------------------------------------------------------------
__FORCE_INLINE__ xmat2 _XEVOL_BASE_API_  operator *(const xmat2& m1,const xmat2& m2)
{
    xmat2 mOut;
    XM_Mul(m1,m2,mOut);
    return mOut;
}

//---------------------------------------------------------------------------------
//Matrix2 x Vec2
//---------------------------------------------------------------------------------
__FORCE_INLINE__ xvec2 _XEVOL_BASE_API_  operator *(const xmat2& m,const xvec2& v)
{
    xvec2 vOut;
    XM_Mul(m, v,vOut);
    return vOut;
}


//---------------------------------------------------------------------------------
//Matrix2 x Vec2
//---------------------------------------------------------------------------------
__FORCE_INLINE__ xvec2 _XEVOL_BASE_API_  operator *(const xvec2& v,const xmat2& m)
{
    xvec2 vOut;
    XM_Mul( v,m,vOut);
    return vOut;
}

END_NAMESPACE_XMATHLIB

#endif
