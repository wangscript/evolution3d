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

#ifndef __XVECTOR_FUNC_INLINE_INCLUDE__
#define __XVECTOR_FUNC_INLINE_INCLUDE__
#include "xmathbasedef.h"
BEGIN_NAMESPACE_XMATHLIB

__FORCE_INLINE__ void   _XEVOL_BASE_API_  XM_Add(const xvec2& v1,const xvec2& v2,xvec2& vOut)
{
	vOut.x = v1.x + v2.x;
	vOut.y = v1.y + v2.y;
}

__FORCE_INLINE__ void _XEVOL_BASE_API_    XM_Sub(const xvec2& v1,const xvec2& v2,xvec2& vOut)
{
	vOut.x = v1.x - v2.x;
	vOut.y = v1.y - v2.y;
}

__FORCE_INLINE__ void  _XEVOL_BASE_API_   XM_Add(const xvec3& v1,const xvec3& v2,xvec3& vOut)
{
	vOut.x = v1.x + v2.x;
	vOut.y = v1.y + v2.y;
	vOut.z = v1.z + v2.z;
}

__FORCE_INLINE__ void _XEVOL_BASE_API_  XM_Sub(const xvec3& v1,const xvec3& v2,xvec3& vOut)
{
	vOut.x = v1.x - v2.x;
	vOut.y = v1.y - v2.y;
	vOut.z = v1.z - v2.z;
}



__FORCE_INLINE__  void _XEVOL_BASE_API_   XM_CP(const xvec3& a,const xvec3& b,xvec3& vOut)
{
	vOut.x = a.y * b.z - b.y * a.z;
	vOut.y = a.z * b.x - b.z * a.x;
	vOut.z = a.x * b.y - b.x * a.y;
}


__FORCE_INLINE__ void  _XEVOL_BASE_API_   XM_Add(const xvec4& a, const xvec4& b, xvec4& vOut)
{
	vOut.x = a.x + b.x;
	vOut.y = a.y + b.y;
	vOut.z = a.z + b.z;
	vOut.w = 1;
}

__FORCE_INLINE__ void  _XEVOL_BASE_API_   XM_Sub(const xvec4& a,const  xvec4& b, xvec4& vOut)
{
	vOut.x = a.x - b.x;
	vOut.y = a.y - b.y;
	vOut.z = a.z - b.z;
	vOut.w = 1;
}

__FORCE_INLINE__ void _XEVOL_BASE_API_    XM_CP(const xvec4& a,const xvec4& b,xvec4& vOut)
{
	vOut.x = a.y * b.z - b.y * a.z;
	vOut.y = a.z * b.x - b.z * a.x;
	vOut.z = a.x * b.y - b.x * a.y;
	vOut.w = 1;
}

__FORCE_INLINE__ void  _XEVOL_BASE_API_   XM_Mul(const xvec4& a, float  s, xvec4& vOut)
{
	vOut.x = a.x * s;
	vOut.y = a.y * s;
	vOut.z = a.z * s;
}

END_NAMESPACE_XMATHLIB
#endif

