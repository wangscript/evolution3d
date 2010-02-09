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

#ifndef __XMATRIX_FUNC_ASM_H__
#define __XMATRIX_FUNC_ASM_H__
#include "xmathbasedef.h"
//#define _MATRIX_FUNC_ASM

#ifdef _MATRIX_FUNC_ASM
BEGIN_NAMESPACE_XMATHLIB
__FORCE_INLINE__ void _MATH_LIB_EXPORT_  XM_Sub(const xmat4& m1,const xmat4& m2,xmat4& mOut)
{
	mOut.m00 = m1.m00 - m2.m00;    mOut.m01 = m1.m01 - m2.m01;    mOut.m02 = m1.m02 - m2.m02;   mOut.m03 = m1.m03 - m2.m03;
	mOut.m10 = m1.m10 - m2.m10;    mOut.m11 = m1.m11 - m2.m11;    mOut.m12 = m1.m12 - m2.m12;   mOut.m13 = m1.m13 - m2.m13;
	mOut.m20 = m1.m20 - m2.m20;    mOut.m21 = m1.m21 - m2.m21;    mOut.m22 = m1.m22 - m2.m22;   mOut.m23 = m1.m23 - m2.m23;
	mOut.m30 = m1.m30 - m2.m30;    mOut.m31 = m1.m31 - m2.m31;    mOut.m32 = m1.m32 - m2.m32;   mOut.m33 = m1.m33 - m2.m33;
}

__FORCE_INLINE__ void _MATH_LIB_EXPORT_  XM_Add(xmat4& m1,xmat4& m2,xmat4& mOut)
{
	mOut.m00 = m1.m00 + m2.m00;    mOut.m01 = m1.m01 + m2.m01;    mOut.m02 = m1.m02 + m2.m02;   mOut.m03 = m1.m03 + m2.m03;
	mOut.m10 = m1.m10 + m2.m10;    mOut.m11 = m1.m11 + m2.m11;    mOut.m12 = m1.m12 + m2.m12;   mOut.m13 = m1.m13 + m2.m13;
	mOut.m20 = m1.m20 + m2.m20;    mOut.m21 = m1.m21 + m2.m21;    mOut.m22 = m1.m22 + m2.m22;   mOut.m23 = m1.m23 + m2.m23;
	mOut.m30 = m1.m30 + m2.m30;    mOut.m31 = m1.m31 + m2.m31;    mOut.m32 = m1.m32 + m2.m32;   mOut.m33 = m1.m33 + m2.m33;
}

__FORCE_INLINE__ void _MATH_LIB_EXPORT_  XM_Mul(const xmat4& m1,float s,xmat4& mOut)
{

	mOut.m00 = m1.m00 * s; mOut.m01 = m1.m01 * s; mOut.m02 = m1.m02 * s;  mOut.m03 = m1.m03 * s;
	mOut.m10 = m1.m10 * s; mOut.m11 = m1.m11 * s; mOut.m12 = m1.m12 * s;  mOut.m13 = m1.m13 * s;
	mOut.m20 = m1.m20 * s; mOut.m21 = m1.m21 * s; mOut.m22 = m1.m22 * s;  mOut.m23 = m1.m23 * s;
	mOut.m30 = m1.m30 * s; mOut.m31 = m1.m31 * s; mOut.m32 = m1.m32 * s;  mOut.m33 = m1.m33 * s;
}

//---------------------------------------------------------------------------------
//  This function is tested by Stanly Lee at 2003-11
//---------------------------------------------------------------------------------
__FORCE_INLINE__ void _MATH_LIB_EXPORT_ XM_Mul(const xmat4& m1,const xmat4& m2,xmat4& mOut)
{
	for(int r =0 ; r < 4 ; r++)
	{
		mOut.m[r][0] = m1.m[r][0] * m2.m[0][0] + m1.m[r][1] * m2.m[1][0] + m1.m[r][2] * m2.m[2][0] + m1.m[r][3] * m2.m[3][0] ;

		mOut.m[r][1] = m1.m[r][0] * m2.m[0][1] +
			m1.m[r][1] * m2.m[1][1] +
			m1.m[r][2] * m2.m[2][1] +
			m1.m[r][3] * m2.m[3][1] ;

		mOut.m[r][2] = m1.m[r][0] * m2.m[0][2] +
			m1.m[r][1] * m2.m[1][2] +
			m1.m[r][2] * m2.m[2][2] +
			m1.m[r][3] * m2.m[3][2] ;


		mOut.m[r][3] = m1.m[r][0] * m2.m[0][3] +
			m1.m[r][1] * m2.m[1][3] +
			m1.m[r][2] * m2.m[2][3] +
			m1.m[r][3] * m2.m[3][3] ;

	}

	return ;
}


__FORCE_INLINE__ void _MATH_LIB_EXPORT_ XM_Lerp(xmat4& m1, xmat4& m2 , float t , xmat4& mOut)
{

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
__FORCE_INLINE__ void _MATH_LIB_EXPORT_ XM_Mul(const xmat4& m,const xvec4& v,xvec3& vOut)
{
	float w =  v.x * m.m[3][0] + v.y * m.m[3][1] + v.z * m.m[3][2] + v.w * m.m[3][3] ;
	vOut.x =  (v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + v.w * m.m[0][3])/w ;
	vOut.y =  (v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + v.w * m.m[1][3])/w ;
	vOut.z =  (v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + v.w * m.m[2][3])/w ;
}

__FORCE_INLINE__ void _MATH_LIB_EXPORT_ XM_Mul(const xvec4& v,const xmat4& m,xvec3& vOut)
{
	float w =  v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3] ;
	vOut.x =  (v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0])/w ;
	vOut.y =  (v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1])/w ;
	vOut.z =  (v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2])/w ;
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
__FORCE_INLINE__ void _MATH_LIB_EXPORT_ XM_Mul(const xvec3& v,const xmat4& m,xvec3& vOut)
{
	vOut.x =  v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
	vOut.y =  v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
	vOut.z =  v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
}
//---------------------------------------------------------------------------------
//This function is tested by Stanly Lee at 2003-11
//---------------------------------------------------------------------------------
__FORCE_INLINE__ void _MATH_LIB_EXPORT_ XM_Mul(const xmat4& m,const xvec3& v,xvec3& vOut)
{
	vOut.x =  v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + m.m[0][3];
	vOut.y =  v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + m.m[1][3];
	vOut.z =  v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + m.m[2][3];
}

//---------------------------------------------------------------------------------
//This function is tested by Stanly Lee at 2003-11
//---------------------------------------------------------------------------------
__FORCE_INLINE__ void _MATH_LIB_EXPORT_ XM_Mul(const xmat4& m,const xvec4& v,xvec4& vOut)
{ 
	vOut.x =  v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + v.w * m.m[0][3] ;
	vOut.y =  v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + v.w * m.m[1][3] ;
	vOut.z =  v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + v.w * m.m[2][3] ;
	vOut.w =  v.x * m.m[3][0] + v.y * m.m[3][1] + v.z * m.m[3][2] + v.w * m.m[3][3] ;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
__FORCE_INLINE__ void _MATH_LIB_EXPORT_ XM_Mul(const xvec4& v,const xmat4& m,xvec4& vOut)
{
	vOut.x =  v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0] ;
	vOut.y =  v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1] ;
	vOut.z =  v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2] ;
	vOut.w =  v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3] ;
}



__FORCE_INLINE__ void _MATH_LIB_EXPORT_  XM_Sub(const xmat3& m1,const xmat3& m2,xmat3& mOut)
{
	mOut.m00 = m1.m00 - m2.m00 ;   mOut.m01 = m1.m01 - m2.m01;    mOut.m02 = m1.m02 - m2.m02;
	mOut.m10 = m1.m10 - m2.m10 ;   mOut.m11 = m1.m11 - m2.m11;    mOut.m12 = m1.m12 - m2.m12;
	mOut.m20 = m1.m20 - m2.m20 ;   mOut.m21 = m1.m21 - m2.m21;    mOut.m22 = m1.m22 - m2.m22;
}
__FORCE_INLINE__ void _MATH_LIB_EXPORT_  XM_Add(const xmat3& m1,const xmat3& m2,xmat3& mOut)
{
	mOut.m00 = m1.m00 + m2.m00;    mOut.m01 = m1.m01 + m2.m01;    mOut.m02 = m1.m02 + m2.m02;
	mOut.m10 = m1.m10 + m2.m10;    mOut.m11 = m1.m11 + m2.m11;    mOut.m12 = m1.m12 + m2.m12;
	mOut.m20 = m1.m20 + m2.m20;    mOut.m21 = m1.m21 + m2.m21;    mOut.m22 = m1.m22 + m2.m22;
}

__FORCE_INLINE__ void _MATH_LIB_EXPORT_  XM_Mul(const xmat3& m1,float s,xmat3& mOut)
{
	mOut.m00 = m1.m00 * s; mOut.m01 = m1.m01 * s; mOut.m02 = m1.m02 * s;
	mOut.m10 = m1.m10 * s; mOut.m11 = m1.m11 * s; mOut.m12 = m1.m12 * s;
	mOut.m20 = m1.m20 * s; mOut.m21 = m1.m21 * s; mOut.m22 = m1.m22 * s;
}


__FORCE_INLINE__ void	_MATH_LIB_EXPORT_ XM_Mul(const xmat3& m1,const xmat3& m2,xmat3& mOut)
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

__FORCE_INLINE__ void	_MATH_LIB_EXPORT_ XM_Mul(const xvec3& v,const xmat3& m,xvec3& vOut)
{
	vOut.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
	vOut.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
	vOut.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];
}

__FORCE_INLINE__ void	_MATH_LIB_EXPORT_ XM_Mul(const xmat3& m,const xvec3& v,xvec3& vOut)
{
	vOut.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z ;
	vOut.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z ;
	vOut.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z ;
}

__FORCE_INLINE__ void _MATH_LIB_EXPORT_  XM_Mul(const xmat4x3& m1, const xmat4x3& m2,xmat4x3& mOut)
{
	for(int r =0 ; r < 4 ; r++)
	{
		mOut.m[r][0] = m1.m[r][0] * m2.m[0][0] + 
			m1.m[r][1] * m2.m[1][0] + 
			m1.m[r][2] * m2.m[2][0] + 
			m2.m[3][0] ;

		mOut.m[r][1] = m1.m[r][0] * m2.m[0][1] +
			m1.m[r][1] * m2.m[1][1] +
			m1.m[r][2] * m2.m[2][1] +
			m2.m[3][1] ;

		mOut.m[r][2] = m1.m[r][0] * m2.m[0][2] +
			m1.m[r][1] * m2.m[1][2] +
			m1.m[r][2] * m2.m[2][2] +
			m2.m[3][2] ;
	}
}

__FORCE_INLINE__ void _MATH_LIB_EXPORT_  XM_Sub(const xmat4x3& m1, const xmat4x3& m2,xmat4x3& mOut)
{
	mOut.m00 = m1.m00 - m2.m00;    mOut.m01 = m1.m01 - m2.m01;    mOut.m02 = m1.m02 - m2.m02;   
	mOut.m10 = m1.m10 - m2.m10;    mOut.m11 = m1.m11 - m2.m11;    mOut.m12 = m1.m12 - m2.m12;   
	mOut.m20 = m1.m20 - m2.m20;    mOut.m21 = m1.m21 - m2.m21;    mOut.m22 = m1.m22 - m2.m22;   
	mOut.m30 = m1.m30 - m2.m30;    mOut.m31 = m1.m31 - m2.m31;    mOut.m32 = m1.m32 - m2.m32;   
}

__FORCE_INLINE__ void _MATH_LIB_EXPORT_  XM_Add(const xmat4x3& m1, const xmat4x3& m2,xmat4x3& mOut)
{
	mOut.m00 = m1.m00 + m2.m00;    mOut.m01 = m1.m01 + m2.m01;    mOut.m02 = m1.m02 + m2.m02;   
	mOut.m10 = m1.m10 + m2.m10;    mOut.m11 = m1.m11 + m2.m11;    mOut.m12 = m1.m12 + m2.m12;   
	mOut.m20 = m1.m20 + m2.m20;    mOut.m21 = m1.m21 + m2.m21;    mOut.m22 = m1.m22 + m2.m22;   
	mOut.m30 = m1.m30 + m2.m30;    mOut.m31 = m1.m31 + m2.m31;    mOut.m32 = m1.m32 + m2.m32;   
}


__FORCE_INLINE__ void _MATH_LIB_EXPORT_  XM_Mul(const xmat4x3& m1,float s,xmat4x3& mOut)
{
	mOut.m00 = m1.m00 * s; mOut.m01 = m1.m01 * s; mOut.m02 = m1.m02 * s;
	mOut.m10 = m1.m10 * s; mOut.m11 = m1.m11 * s; mOut.m12 = m1.m12 * s;
	mOut.m20 = m1.m20 * s; mOut.m21 = m1.m21 * s; mOut.m22 = m1.m22 * s;
	mOut.m30 = m1.m30 * s; mOut.m31 = m1.m31 * s; mOut.m32 = m1.m32 * s;
}

__FORCE_INLINE__ void _MATH_LIB_EXPORT_ XM_Mul(const xvec4& v,const xmat4x3& m,xvec3& vOut)
{
	vOut.x =  (v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0])/v.w;
	vOut.y =  (v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1])/v.w;
	vOut.z =  (v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2])/v.w;
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
__FORCE_INLINE__ void _MATH_LIB_EXPORT_ XM_Mul(const xvec3& v,const xmat4x3& m,xvec3& vOut)
{
	vOut.x =  v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
	vOut.y =  v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
	vOut.z =  v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
}

__FORCE_INLINE__ void _MATH_LIB_EXPORT_ XM_Mul(const xmat4x3& m,const xvec4& v,xvec3& vOut)
{ 
	vOut.x =  (v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + m.m[0][3])/v.w;
	vOut.y =  (v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + m.m[1][3])/v.w;
	vOut.z =  (v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + m.m[2][3])/v.w;
}


//---------------------------------------------------------------------------------
//This function is tested by Stanly Lee at 2003-11
//---------------------------------------------------------------------------------
__FORCE_INLINE__ void _MATH_LIB_EXPORT_ XM_Mul(const xmat4x3& m,const xvec3& v,xvec3& vOut)
{
	vOut.x =  v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + m.m[0][3];
	vOut.y =  v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + m.m[1][3];
	vOut.z =  v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + m.m[2][3];
}

//---------------------------------------------------------------------------------
//This function is tested by Stanly Lee at 2003-11
//---------------------------------------------------------------------------------
__FORCE_INLINE__ void _MATH_LIB_EXPORT_ XM_Mul(const xmat4x3& m,const xvec4& v,xvec4& vOut)
{ 
	vOut.x =  (v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + m.m[0][3]);
	vOut.y =  (v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + m.m[1][3]);
	vOut.z =  (v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + m.m[2][3]);
	vOut.w = v.w;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
__FORCE_INLINE__ void _MATH_LIB_EXPORT_ XM_Mul(const xvec4& v,const xmat4x3& m,xvec4& vOut)
{
	vOut.x =  (v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0]) ;
	vOut.y =  (v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1]) ;
	vOut.z =  (v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2]) ;
	vOut.w =  v.w;
}

__FORCE_INLINE__ xmat4x3 _MATH_LIB_EXPORT_  operator +(const xmat4x3& m1,const xmat4x3& m2)
{
	xmat4x3 m;
	m.data[0] = m1.data[0] + m2.data[0]; 
	m.data[1] = m1.data[1] + m2.data[1]; 
	m.data[2] = m1.data[2] + m2.data[2];

	m.data[3] = m1.data[3] + m2.data[3];
	m.data[4] = m1.data[4] + m2.data[4];
	m.data[5] = m1.data[5] + m2.data[5];

	m.data[6] = m1.data[6] + m2.data[6];
	m.data[7] = m1.data[7] + m2.data[7];
	m.data[8] = m1.data[8] + m2.data[8];

	m.data[9] = m1.data[9] + m2.data[9];
	m.data[10] = m1.data[10] + m2.data[10];
	m.data[11] = m1.data[11] + m2.data[11];
	return m;
}

__FORCE_INLINE__ xmat4x3 _MATH_LIB_EXPORT_  operator - (const xmat4x3& m1,const xmat4x3& m2)
{
	xmat4x3 m;
	m.data[0] = m1.data[0] - m2.data[0]; 
	m.data[1] = m1.data[1] - m2.data[1]; 
	m.data[2] = m1.data[2] - m2.data[2];

	m.data[3] = m1.data[3] - m2.data[3];
	m.data[4] = m1.data[4] - m2.data[4];
	m.data[5] = m1.data[5] - m2.data[5];

	m.data[6] = m1.data[6] - m2.data[6];
	m.data[7] = m1.data[7] - m2.data[7];
	m.data[8] = m1.data[8] - m2.data[8];

	m.data[9] = m1.data[9] - m2.data[9];
	m.data[10] = m1.data[10] - m2.data[10];
	m.data[11] = m1.data[11] - m2.data[11];
	return m;
}

END_NAMESPACE_XMATHLIB
#endif

#endif
