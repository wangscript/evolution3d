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

#ifndef __XMATRIX_FUNC_ASM_H__
#define __XMATRIX_FUNC_ASM_H__
#include "XMathBaseDef.h"

#pragma  warning (disable : 4537)
#ifdef __XMATHLIB_ASM__
BEGIN_NAMESPACE_XMATHLIB

__FORCE_INLINE__ void _XEVOL_BASE_API_  XM_Sub(const xmat4& m1,const xmat4& m2,xmat4& mOut)
{
	__asm
	{
		mov    eax,       m1;
		mov    ecx,       m2;
		mov    edx,       mOut;

		movups xmm0,                 dword ptr[eax];
		movups xmm1,                 dword ptr[ecx];
		subps  xmm0,                 xmm1;
		movups dword ptr[edx],       xmm0;

		movups xmm0,                 dword ptr[eax + 16];
		movups xmm1,                 dword ptr[ecx + 16];
		subps  xmm0,                 xmm1;
		movups dword ptr[edx + 16],  xmm0;

		movups xmm0,                 dword ptr[eax + 32];
		movups xmm1,                 dword ptr[ecx + 32];
		subps  xmm0,                 xmm1;
		movups dword ptr[edx + 32],  xmm0;

		movups xmm0,                 dword ptr[eax + 48];
		movups xmm1,                 dword ptr[ecx + 48];
		subps  xmm0,                 xmm1;
		movups dword ptr[edx + 48],  xmm0;
	}
}

__FORCE_INLINE__ void _XEVOL_BASE_API_  XM_Add(const xmat4& m1,const xmat4& m2,xmat4& mOut)
{

	__asm
	{
		mov    eax,       m1;
		mov    ecx,       m2;
		mov    edx,       mOut;

		movups xmm0,                 dword ptr[eax];
		movups xmm1,                 dword ptr[ecx];
		addps  xmm0,                 xmm1;
		movups dword ptr[edx],       xmm0;

		movups xmm0,                 dword ptr[eax + 16];
		movups xmm1,                 dword ptr[ecx + 16];
		addps  xmm0,                 xmm1;
		movups dword ptr[edx + 16],  xmm0;

		movups xmm0,                 dword ptr[eax + 32];
		movups xmm1,                 dword ptr[ecx + 32];
		addps  xmm0,                 xmm1;
		movups dword ptr[edx + 32],  xmm0;

		movups xmm0,                 dword ptr[eax + 48];
		movups xmm1,                 dword ptr[ecx + 48];
		addps  xmm0,                 xmm1;
		movups dword ptr[edx + 48],  xmm0;

	}


}

__FORCE_INLINE__ void _XEVOL_BASE_API_  XM_Mul(const xmat4& m1,float s,xmat4& mOut)
{
	float s4[4];
	s4[0] = s4[1] = s4[2] = s4[3] = s;
	__asm
	{
		lea    ecx,       s4;
		mov    eax,       m1;
		mov    edx,       mOut;
		movups xmm1,                 dword ptr[ecx];

		movups xmm0,                 dword ptr[eax];
		mulps  xmm0,                 xmm1;
		movups dword ptr[edx],       xmm0;

		movups xmm0,                 dword ptr[eax + 16];
		mulps  xmm0,                 xmm1;
		movups dword ptr[edx + 16],  xmm0;

		movups xmm0,                 dword ptr[eax + 32];
		mulps  xmm0,                 xmm1;
		movups dword ptr[edx + 32],  xmm0;

		movups xmm0,                 dword ptr[eax + 48];
		mulps  xmm0,                 xmm1;
		movups dword ptr[edx + 48],  xmm0;
	}
}

//---------------------------------------------------------------------------------
//  This function is tested by Stanly Lee at 2003-11
//---------------------------------------------------------------------------------
__FORCE_INLINE__ void _XEVOL_BASE_API_ XM_Mul(const xmat4& m1,const xmat4& m2,xmat4& mOut)
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


__FORCE_INLINE__ void _XEVOL_BASE_API_ XM_Lerp(const xmat4& m1, const xmat4& m2 , float t , xmat4& mOut)
{
	float t4[4];
	t4[0] = t4[1] = t4[2] = t4[3] = t;
	__asm
	{
		//t
		lea    ecx,       t4;
		movups xmm2,                 dword ptr[ecx];

		mov    eax,       m1;
		mov    ecx,       m2;
		mov    edx,       mOut;

		movups xmm0,                 dword ptr[ecx];//m2
		movups xmm1,                 dword ptr[eax];//m1
		subps  xmm0,                 xmm1; // m2 - m1;
		mulps  xmm0,                 xmm2; // (m2 - m1 ) * t;
		addps  xmm0,                 xmm1; // (m2 - m1 ) * t + m1; 
		movups dword ptr[edx],       xmm0;

		movups xmm0,                 dword ptr[ecx + 16];//m2
		movups xmm1,                 dword ptr[eax + 16];//m1
		subps  xmm0,                 xmm1; // m2 - m1;
		mulps  xmm0,                 xmm2; // (m2 - m1 ) * t;
		addps  xmm0,                 xmm1; // (m2 - m1 ) * t + m1; 
		movups dword ptr[edx + 16],  xmm0;

		movups xmm0,                 dword ptr[ecx + 32];//m2
		movups xmm1,                 dword ptr[eax + 32];//m1
		subps  xmm0,                 xmm1; // m2 - m1;
		mulps  xmm0,                 xmm2; // (m2 - m1 ) * t;
		addps  xmm0,                 xmm1; // (m2 - m1 ) * t + m1; 
		movups dword ptr[edx + 32],  xmm0;

		movups xmm0,                 dword ptr[ecx + 48];//m2
		movups xmm1,                 dword ptr[eax + 48];//m1
		subps  xmm0,                 xmm1; // m2 - m1;
		mulps  xmm0,                 xmm2; // (m2 - m1 ) * t;
		addps  xmm0,                 xmm1; // (m2 - m1 ) * t + m1; 
		movups dword ptr[edx + 48],  xmm0;

	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
__FORCE_INLINE__ void _XEVOL_BASE_API_ XM_Mul(const xmat4& m,const xvec4& v,xvec3& vOut)
{
	float w =  v.x * m.m[3][0] + v.y * m.m[3][1] + v.z * m.m[3][2] + v.w * m.m[3][3] ;
	vOut.x =  (v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + v.w * m.m[0][3])/w ;
	vOut.y =  (v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + v.w * m.m[1][3])/w ;
	vOut.z =  (v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + v.w * m.m[2][3])/w ;
}

__FORCE_INLINE__ void _XEVOL_BASE_API_ XM_Mul(const xvec4& v,const xmat4& m,xvec3& vOut)
{

	float w =  v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3] ;
	vOut.x =  (v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0])/w ;
	vOut.y =  (v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1])/w ;
	vOut.z =  (v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2])/w ;
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
__FORCE_INLINE__ void _XEVOL_BASE_API_ XM_Mul(const xvec3& v,const xmat4& m,xvec3& vOut)
{
	vOut.x =  v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
	vOut.y =  v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
	vOut.z =  v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
}
//---------------------------------------------------------------------------------
//This function is tested by Stanly Lee at 2003-11
//---------------------------------------------------------------------------------
__FORCE_INLINE__ void _XEVOL_BASE_API_ XM_Mul(const xmat4& m,const xvec3& v,xvec3& vOut)
{
	vOut.x =  v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + m.m[0][3];
	vOut.y =  v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + m.m[1][3];
	vOut.z =  v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + m.m[2][3];
}

//---------------------------------------------------------------------------------
//This function is tested by Stanly Lee at 2003-11
//---------------------------------------------------------------------------------
__FORCE_INLINE__ void _XEVOL_BASE_API_ XM_Mul(const xmat4& m,const xvec4& v,xvec4& vOut)
{ 
	vOut.x =  v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + v.w * m.m[0][3] ;
	vOut.y =  v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + v.w * m.m[1][3] ;
	vOut.z =  v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + v.w * m.m[2][3] ;
	vOut.w =  v.x * m.m[3][0] + v.y * m.m[3][1] + v.z * m.m[3][2] + v.w * m.m[3][3] ;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
__FORCE_INLINE__ void _XEVOL_BASE_API_ XM_Mul(const xvec4& v,const xmat4& m,xvec4& vOut)
{
	vOut.x =  v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0] ;
	vOut.y =  v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1] ;
	vOut.z =  v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2] ;
	vOut.w =  v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3] ;
}

END_NAMESPACE_XMATHLIB
#endif

#endif
