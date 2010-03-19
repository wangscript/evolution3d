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

#ifndef _XEVOL_BASE_API_H_
#define _XEVOL_BASE_API_H_

#ifdef _WIN32
   #ifdef   _XEVOL_BUILD_DLL_ 
       #define  _XEVOL_BASE_API_   __declspec( dllexport )
       #define EXPIMP_TEMPLATE
   #else
       #define  _XEVOL_BASE_API_   __declspec( dllimport )
       #define EXPIMP_TEMPLATE       
       #ifdef   _DEBUG 
          //#pragma  comment(lib,"xEvol3D_Debug.lib")
       #else
          //#pragma  comment(lib,"xEvol3D_Release.lib")
       #endif

   #endif
#else
   #undef    _XEVOL_BASE_API_  
   #define   _XEVOL_BASE_API_ 
   #define EXPIMP_TEMPLATE
#endif

#ifdef _LINUX
#undef    _XEVOL_BASE_API_  
#define   _XEVOL_BASE_API_ 
#define EXPIMP_TEMPLATE
#endif

#ifdef _XEVOL_BUILD_STATIC_
#undef   _XEVOL_BASE_API_   
#define  _XEVOL_BASE_API_ 
#define EXPIMP_TEMPLATE
#endif


#ifndef BEGIN_NAMESPACE_XEVOL3D
#define BEGIN_NAMESPACE_XEVOL3D namespace XEvol3D{
#define END_NAMESPACE_XEVOL3D  }
#endif

#define USING_NS_XEVOL3D using namespace XEvol3D;
#define NS_XEVOL3D XEvol3D

#ifndef MSCVER
#define int8 char  
#define int32 int
#define int16 short
#else   
#define int8  __int8
#define int32 __int32
#define int16 __int16
#endif

#pragma warning(disable : 4251)
#pragma warning(disable : 4799)
#pragma warning(disable : 4201)
#pragma warning(disable : 6011)
#pragma warning(disable : 6031)
#pragma warning(disable : 6385)
#pragma warning(disable : 4819)

#endif
