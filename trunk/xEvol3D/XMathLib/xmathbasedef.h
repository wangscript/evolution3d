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

#ifndef __XREALMATH_BASE_DEF_H__
#define __XREALMATH_BASE_DEF_H__

#include "../BaseLib/xEvol3DBaseInc.h"

/*
#ifdef _WIN32
#ifdef   _XEVOL_BUILD_DLL_ 
#define  _XEVOL_BASE_API_  __declspec( dllexport )
#else
#define  _XEVOL_BASE_API_  __declspec( dllimport )

#endif
#else
#undef    _XEVOL_BASE_API_ 
#define   _XEVOL_BASE_API_ 
#endif



#pragma warning(disable : 4251)
#pragma warning(disable : 4799)

#define __FORCE_INLINE__ inline

#ifdef _MSC_VER
#undef  __FORCE_INLINE__
#define __FORCE_INLINE__ __inline
#endif 




//------------------------
#ifdef _MSC_VER
#  ifndef _WIN32_WCE
#define __XMATHLIB_ASM__

#  endif
#endif

*/

#define BEGIN_NAMESPACE_XMATHLIB namespace XEvol3D { namespace XMathLib {
#define END_NAMESPACE_XMATHLIB }}


#define BEGIN_NAMESPACE_XGEOMLIB namespace XEvol3D { namespace XGeomLib {
#define END_NAMESPACE_XGEOMLIB }}

#endif
