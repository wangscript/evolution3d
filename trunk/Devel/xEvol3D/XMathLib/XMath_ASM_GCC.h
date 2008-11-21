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

#ifndef __XMATH_ASM_H__
#define __XMATH_ASM_H__

BEGIN_NAMESPACE_XMATHLIB
inline unsigned XM_CeilPowerOf2(unsigned x) 
{
	unsigned eax;
	__asm__(
		"xor eax,eax\n"
		"dec ecx\n"
		"bsr ecx,ecx\n"
		"cmovz ecx,eax\n"
		"setnz al\n"
		"inc eax\n"
		"shl eax,cl\n"
		: "=a" (eax)
		: "c" (x)
		);
	return eax;
}

inline unsigned XM_FloorPowerOf2(unsigned x) 
{
	unsigned eax;
	__asm__(
		"xor eax,eax\n"
		"bsr ecx,ecx\n"
		"setnz al\n"
		"shl eax,cl\n"
		: "=a" (eax)
		: "c" (x)
		);
	return eax;
}
END_NAMESPACE_XMATHLIB
#endif
