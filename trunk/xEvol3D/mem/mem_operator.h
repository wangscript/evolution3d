/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) .
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

#ifndef __DGE_MEM_OPERATOR_H__
#define __DGE_MEM_OPERATOR_H__

#define XNEW_OP new

#ifdef _USE_XEVOL3D_NEW_

#include <new>

#include "MemAlloc.h"
#include "MemState.h"
#include "mem.h"

inline void  operator delete(void* p)
{
	XEvol3D::Mem::Dealloc(p);
}

inline void  operator delete[](void* p)
{
	XEvol3D::Mem::Dealloc(p);
}

inline void  operator delete[](void* p,size_t)
{
	XEvol3D::Mem::Dealloc(p);
}


#ifndef _DEBUG

inline void* operator new(size_t size)
{
	return XEvol3D::Mem::Alloc(size);
}

inline void* operator new[](size_t size)
{
	return XEvol3D::Mem::Alloc(size);
}

#else

inline void* operator new(size_t size)
{
	return XEvol3D::Mem::Alloc(size,"Unkown postion",0);
}

inline void* operator new[](size_t size)
{
	return XEvol3D::Mem::Alloc(size,"Unkown postion",0);
}

inline void* operator new(size_t size,const char* fileName,int lineNo)
{
	return XEvol3D::Mem::Alloc(size,(char*)fileName,lineNo);
}

inline void* operator new[](size_t size,const char* fileName,int lineNo)
{
	return XEvol3D::Mem::Alloc(size,(char*)fileName,lineNo);
}

inline void operator delete(void* p,const char* fileName,int lineNo)
{
	XEvol3D::Mem::Dealloc(p);
}

inline void operator delete(void* p,size_t size,const char* fileName,int lineNo)
{
	XEvol3D::Mem::Dealloc(p);
}

inline void  operator delete[](void* p,const char* fileName,int lineNo)
{
	XEvol3D::Mem::Dealloc(p);
}

inline void  operator delete[](void* p,size_t,const char* fileName,int lineNo)
{
	XEvol3D::Mem::Dealloc(p);
}


#ifdef XNEW_OP
#undef XNEW_OP
#endif

#define XNEW_OP new(__FILE__, __LINE__)
#define new XNEW_OP


#endif//_DEBUG


#endif
#endif
