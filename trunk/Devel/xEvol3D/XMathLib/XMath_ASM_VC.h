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

#ifndef __XMATH_ASM_H__
#define __XMATH_ASM_H__
#include "XMathBaseDef.h"

#define __fpu_cos(a,c)   \
	_asm fld a ;         \
	_asm fcos ;          \
	_asm fstp c;         \

#define __fpu_sin(a,s)   \
	_asm fld a ;         \
	_asm fsin ;          \
	_asm fstp s ;        \


#define __fpu_sin_cos(a,s,c)\
	_asm fld a ;            \
	_asm fsincos ;          \
	_asm fstp s ;           \
	_asm fstp c ;           \

#define _fpu_tan(a,t)     \
	_asm fld a ;            \
	_asm fptan ;            \
	_asm fstp t ;          \

#define _fpu_atan(a,at)     \
	_asm fld a ;            \
	_asm fpatan ;            \
	_asm fstp at ;          \
	_asm fstp at ;          \

#ifdef __XMATHLIB_ASM__
#pragma warning(push)
#pragma warning(disable : 4100)
BEGIN_NAMESPACE_XMATHLIB

__FORCE_INLINE__ void  XM_SinCosD(float angle,float& s,float& c)
{
	angle = (float)angle / 180 * (float)XM_PI;
	__fpu_sin_cos(angle,s,c);
}

__FORCE_INLINE__ void  XM_SinCosR(float angle,float& s,float& c)
{
	__fpu_sin_cos(angle,s,c);
}

__FORCE_INLINE__ float  XM_CosD(float angle)
{
	angle = (float)angle / 180 * (float)XM_PI;
	__asm
	{
		fld angle;
		fcos;
		fstp angle;
	}
	return angle;
}

__FORCE_INLINE__ float  XM_SinD(float angle)
{
	angle = (float)angle / 180 * (float)XM_PI;
	__asm
	{
		fld angle;
		fsin;
		fstp angle;
	}
	return angle;
}

__FORCE_INLINE__ float  XM_CosR(float angle)
{
	__asm
	{
		fld angle;
		fcos;
		fstp angle;
	}
	return angle;
}

__FORCE_INLINE__ float  XM_SinR(float angle)
{
	__asm
	{
		fld  angle;
		fsin;
		fstp angle;
	}
	return angle;
}


inline __declspec(naked) unsigned __fastcall XM_CeilPowerOf2(unsigned x) 
{
	__asm {
		xor eax,eax ;
		dec ecx ;
		bsr ecx,ecx ;
		cmovz ecx,eax ;
		setnz al ;
		inc eax ;
		shl eax,cl ;
		ret ;
	}
}

// Returns the greatest power of 2 less than or equal to "x".
// Note that for x=0 this returns 0!

inline __declspec(naked) unsigned __fastcall XM_FloorPowerOf2(unsigned x) 
{
	__asm {
		xor eax,eax ;
		bsr ecx,ecx ;
		setnz al ;
		shl eax,cl ;
		ret ;
	}
}

END_NAMESPACE_XMATHLIB

#pragma warning(pop)
#endif
#endif

