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

#ifndef __XEVOL3D_ENGINE_BASE_INC_H__
#define __XEVOL3D_ENGINE_BASE_INC_H__
#include <string.h>
#include "xEvol3DAPI.h"
#include "xBaseObject.h"
#include "stl_dllalloc.h"
#include "xLogger.h"
#include "../mem/mem_operator.h"

#ifdef _WIN32
#define PATH_SPLITTER '\\'
#endif

#ifdef _LINUX
#define PATH_SPLITTER '/'
#endif

//#define ENUM_PUBLIC 
//#define ENUM_CLASS 

#undef  ENUM_CLASS
#define ENUMCLASS( v )  enum

#define PRIVATE 
#define PUBLIC 

BEGIN_NAMESPACE_XEVOL3D

template <typename T> void T_XEvolSwap(T& a , T& b)
{
	T t = a ;
	a = b;
	b = t;
}

template <typename _TSTR> bool XEvol_IsAbsPath(_TSTR strResName)
{
	//Case /usr/hello.jpg (UNIX)
	if(strResName[0] == '/')
		return true;

	if(strResName.length() <3)
		return false;

	//Case C:\\Hello.jpg
	if(strResName[1] == ':' &&(strResName[2] == '\\' || strResName[2] == '/') )
		return true;
	//Case \\192.168.0.1\ddd.jpg
	if(strResName[0] == '\\' && strResName[1] == '\\'   )
		return true;
	return false;
}

inline bool XEvol_IsAbsPath(const wchar_t* strResName)
{
	//Case /usr/hello.jpg (UNIX)
	if(strResName[0] == '/')
		return true;

	if( wcslen(strResName) <3)
		return false;

	//Case C:\\Hello.jpg
	if(strResName[1] == ':' &&(strResName[2] == '\\' || strResName[2] == '/') )
		return true;
	//Case \\192.168.0.1\ddd.jpg
	if(strResName[0] == '\\' && strResName[1] == '\\'   )
		return true;
	return false;
}


template<typename T> class TAutoLocker
{
	T* m_pLocker;
public:
	TAutoLocker(T* pT){	m_pLocker=pT;if(m_pLocker) m_pLocker->lock();};
	~TAutoLocker()	 {if(m_pLocker)	m_pLocker->unlock();};
};


template <typename T, typename _TSTREAM > void Ts_Write(_TSTREAM& _out ,const T& data)
{
	_out.write( (const char*) &data,sizeof(T));
}


template <typename T, typename _TSTREAM , typename _SIZE> void  Ts_Write(_TSTREAM& _out , const T* data, _SIZE nObject)
{
	_out.write( (const char*)data,sizeof(T)*nObject);
}

template <typename T, typename _TSTREAM > void  Ts_Read(_TSTREAM& _in , T& data)
{
	_in.read( (char*) &data,sizeof(T));
}

template <typename T , typename _TSTREAM , typename _SIZE> void  Ts_Read(_TSTREAM& _in ,  T* data, _SIZE nObject)
{
	_in.read( (char*)data , ( (_SIZE)sizeof(T) ) * nObject);
}

#define __FORCE_INLINE__ inline

#ifdef _MSC_VER
#undef  __FORCE_INLINE__
#define __FORCE_INLINE__ __inline
#endif 

END_NAMESPACE_XEVOL3D

#include "xstring.h"
#endif
