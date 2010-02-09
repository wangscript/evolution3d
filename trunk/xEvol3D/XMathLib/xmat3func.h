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

#ifndef __XMATRIX3x3_FUNC_INLINE_H__
#define __XMATRIX3x3_FUNC_INLINE_H__
#include "xmathbasedef.h"

#ifdef _MSC_VER
#include "xmat3x3_asm_vc.h"
#endif

#ifdef _GNUC_
#include "xmat3x3_asm_gcc.h"
#endif

#ifndef _MATRIX3x3_FUNC_ASM

BEGIN_NAMESPACE_XMATHLIB

__FORCE_INLINE__ void _XEVOL_BASE_API_  XM_Sub(const xmat3& m1,const xmat3& m2,xmat3& mOut)
{
	mOut.m00 = m1.m00 - m2.m00 ;   mOut.m01 = m1.m01 - m2.m01;    mOut.m02 = m1.m02 - m2.m02;
	mOut.m10 = m1.m10 - m2.m10 ;   mOut.m11 = m1.m11 - m2.m11;    mOut.m12 = m1.m12 - m2.m12;
	mOut.m20 = m1.m20 - m2.m20 ;   mOut.m21 = m1.m21 - m2.m21;    mOut.m22 = m1.m22 - m2.m22;
}
__FORCE_INLINE__ void _XEVOL_BASE_API_  XM_Add(const xmat3& m1,const xmat3& m2,xmat3& mOut)
{
	mOut.m00 = m1.m00 + m2.m00;    mOut.m01 = m1.m01 + m2.m01;    mOut.m02 = m1.m02 + m2.m02;
	mOut.m10 = m1.m10 + m2.m10;    mOut.m11 = m1.m11 + m2.m11;    mOut.m12 = m1.m12 + m2.m12;
	mOut.m20 = m1.m20 + m2.m20;    mOut.m21 = m1.m21 + m2.m21;    mOut.m22 = m1.m22 + m2.m22;
}

__FORCE_INLINE__ void _XEVOL_BASE_API_  XM_Mul(const xmat3& m1,float s,xmat3& mOut)
{
	mOut.m00 = m1.m00 * s; mOut.m01 = m1.m01 * s; mOut.m02 = m1.m02 * s;
	mOut.m10 = m1.m10 * s; mOut.m11 = m1.m11 * s; mOut.m12 = m1.m12 * s;
	mOut.m20 = m1.m20 * s; mOut.m21 = m1.m21 * s; mOut.m22 = m1.m22 * s;
}


__FORCE_INLINE__ void	_XEVOL_BASE_API_ XM_Mul(const xmat3& m1,const xmat3& m2,xmat3& mOut)
{
	for(int r =0 ; r < 4 ; r++)
	{
		mOut.m[r][0] = m1.m[r][0] * m2.m[0][0] +
			m1.m[r][1] * m2.m[1][0] +
			m1.m[r][2] * m2.m[2][0] ;

		mOut.m[r][1] = m1.m[r][0] * m2.m[0][1] +
			m1.m[r][1] * m2.m[1][1] +
			m1.m[r][2] * m2.m[2][1];

		mOut.m[r][2] = m1.m[r][0] * m2.m[0][2] +
			m1.m[r][1] * m2.m[1][2] +
			m1.m[r][2] * m2.m[2][2];
	}
}

__FORCE_INLINE__ void	_XEVOL_BASE_API_ XM_Mul(const xvec3& v,const xmat3& m,xvec3& vOut)
{
	vOut.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
	vOut.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
	vOut.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];
}

__FORCE_INLINE__ void	_XEVOL_BASE_API_ XM_Mul(const xmat3& m,const xvec3& v,xvec3& vOut)
{
	vOut.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z ;
	vOut.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z ;
	vOut.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z ;
}

END_NAMESPACE_XMATHLIB

#endif //_MATRIX3x3_FUNC_ASM

//不需要用汇编优化的版本
BEGIN_NAMESPACE_XMATHLIB

__FORCE_INLINE__ void	_XEVOL_BASE_API_ XM_Transpos(const xmat3&m , xmat3& mOut)
{
	for(int r = 0 ;  r < 3 ; r ++)
	{
		mOut.m[0][r] = m.m[r][0];
		mOut.m[1][r] = m.m[r][1];
		mOut.m[2][r] = m.m[r][2];
	}

}

__FORCE_INLINE__ xmat3 _XEVOL_BASE_API_ operator *(const xmat3& m1,float s)
{
	xmat3 mOut;
	XM_Mul(m1,s);
	return mOut;
}

__FORCE_INLINE__ xmat3 _XEVOL_BASE_API_  operator *(float s,const xmat3& m1)
{
	xmat3 mOut;
	XM_Mul(m1,s);
	return mOut;
}

//---------------------------------------------------------------------------------
//matrix +- matrix
//---------------------------------------------------------------------------------
__FORCE_INLINE__ xmat3 _XEVOL_BASE_API_  operator -(const xmat3& m1,const xmat3& m2)
{
	xmat3 mOut;
	XM_Sub(m1,m2,mOut);
	return mOut;
}

__FORCE_INLINE__ xmat3 _XEVOL_BASE_API_  operator +(const xmat3& m1,const xmat3& m2)
{
	xmat3 mOut;
	XM_Add(m1,m2,mOut);
	return mOut;
}

//---------------------------------------------------------------------------------
//Matrix3 x Matrix3
//---------------------------------------------------------------------------------
__FORCE_INLINE__ xmat3 _XEVOL_BASE_API_  operator *(const xmat3& m1,const xmat3& m2)
{
	xmat3 mOut;
	XM_Mul(m1,m2,mOut);
	return mOut;
}

//---------------------------------------------------------------------------------
//Matrix3 x Vec3
//---------------------------------------------------------------------------------
__FORCE_INLINE__ xvec3 _XEVOL_BASE_API_  operator *(const xmat3& m,const xvec3& v)
{
	xvec3 vOut;
	XM_Mul(m, v,vOut);
	return vOut;
}


//---------------------------------------------------------------------------------
//Matrix3 x Vec3
//---------------------------------------------------------------------------------
__FORCE_INLINE__ xvec3 _XEVOL_BASE_API_  operator *(const xvec3& v,const xmat3& m)
{
	xvec3 vOut;
	XM_Mul( v,m,vOut);
	return vOut;
}

END_NAMESPACE_XMATHLIB

#endif
