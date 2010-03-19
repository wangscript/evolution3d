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

#ifndef  __XEVOL3D_API_ENDIAN_H__
#define __XEVOL3D_API_ENDIAN_H__
#include "../BaseLib/xEvol3DBaseInc.h"
BEGIN_NAMESPACE_XEVOL3D

inline void xSwapUShort(unsigned short *s)  
{
#if (defined(_MSC_VER) && !defined(_WIN64) )
	__asm 
	{
		mov ebx, s
		mov al, [ebx+1]
		mov ah, [ebx  ]
		mov [ebx], ax
	}
#else

#ifdef GCC_X86_ASM
	asm("ror $8,%0"
		: 
	: "r"  (*s) );
#else
	*s = ((*s)>>8) | ((*s)<<8);
#endif
#endif
}

inline void xSwapShort(short *s) 
{
	xSwapUShort((unsigned short*)s);
}

inline void xSwapUInt(unsigned int *i) 
{
#if (defined(_MSC_VER) && !defined(_WIN64) )
	__asm {
		mov ebx, i
			mov eax, [ebx]
		bswap eax
			mov [ebx], eax
	}
#else
#ifdef GCC_X86_ASM
	asm("bswap  %0;"
		: "=r" (*i) );
#else
	*i = ((*i)>>24) | (((*i)>>8) & 0xff00) | (((*i)<<8) & 0xff0000) | ((*i)<<24);
#endif //GCC_X86_ASM
#endif //USE_WIN32_ASM
}

inline void xSwapInt(int *i) 
{
	xSwapUInt((unsigned int*)i);
}

inline void xSwapFloat(float *f) {
	xSwapUInt((unsigned int*)f);
}

inline void iSwapDouble(double *d)
{
#ifdef GCC_X86_ASM
	int *t = (int*)d;
	asm("bswap %2    \n"
		"bswap %3    \n"
		"movl  %2,%1 \n"
		"movl  %3,%0 \n"
		: "=g" (t[0]), "=g" (t[1])
		: "r"  (t[0]), "r"  (t[1]));
#else
	unsigned int8 t,*b = (unsigned int8*)d;
#define dswap(x,y) t=b[x];b[x]=b[y];b[y]=b[x];
	dswap(0,7);
	dswap(1,6);
	dswap(2,5);
	dswap(3,4);
#undef dswap
#endif
}


END_NAMESPACE_XEVOL3D

#endif
