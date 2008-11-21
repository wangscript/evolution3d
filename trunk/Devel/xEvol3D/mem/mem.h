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

#ifndef __DGE_MEM_H__
#define __DGE_MEM_H__

#include <new>

#include "MemAlloc.h"
#include "MemState.h"


namespace XEvol3D
{ 
	namespace Mem
	{

//#ifdef _DEBUG
		_XEVOL_BASE_API_  void *  Alloc(size_t size , char* filename, int line);
//#endif


		_XEVOL_BASE_API_  void * Alloc(size_t size);
		_XEVOL_BASE_API_  void   Dealloc(void* p);


      
        _XEVOL_BASE_API_  MemAlloc*      MEMALLOC();
		_XEVOL_BASE_API_  SmallMemAlloc* SMALLMEMALLOC();

		_XEVOL_BASE_API_  void           diagnostic();
		_XEVOL_BASE_API_  void           diagBound();

#ifdef _DEBUG
		template<typename T>      void   diagnostic(T& out)
		{
			getMemState().diagnostic(out);
			MEMALLOC()->diagnostic(out);
		}

		template<typename T>      void   diagBound(T& out)
		{
			getMemState().diagBound(out);
			MEMALLOC()->diagBound(out);

		}
#else
		template<typename T>      void   diagnostic(T& out)
		{out<<"在非调试模式下，不能记录内存的分配情况\n";}
		template<typename T>      void   diagBound(T& out)
		{out<<"在非调试模式下，不能记录内存的分配情况\n";}
#endif

		template<typename T> T* XNEW_T(size_t nCount,char* fileName,int lineNo)
		{
			void* pdata = Alloc(nCount*sizeof(T),fileName,lineNo);
			for(size_t i = 0 ; i < nCount ; ++i)
			{
				new( (void*) ( (char*)pdata + sizeof(T) * i ) ) T;
			}
			return (T*)pdata;
		}

		template<typename T> T* XNEW_T(char* fileName,int lineNo)
		{
			void* pdata = Alloc(sizeof(T),fileName,lineNo);
			new (pdata) T ;
			return (T*)pdata;
		}
		template<typename T> T* XNEW(size_t nCount,char* fileName,int lineNo)
		{
			void* pdata = Alloc(nCount*sizeof(T),fileName,lineNo);
			return (T*)pdata;
		}


		template<typename T> T* XNEW(char* fileName,int lineNo)
		{
			void* pdata = Alloc(sizeof(T),fileName,lineNo);
			return (T*)pdata;
		}

#ifndef FIX_MEM_ALLOC
		template<typename T> void xdelete_t(T* p)
		{
			int Size = *(((int*)p) - 1);
			Size/=sizeof(T);
			for(size_t i = 0 ; i < (size_t)Size ;++i)
			{
				(p+i)->~T();
			}
			Dealloc(p); 
		}

#else
		template<typename T> void xdelete_t(T* p)
		{
			Dealloc(p); 
		}
#endif

		template<typename T> void xdelete(T* p)
		{
			Dealloc(p); 
		}

	};//End Name Space Mem
};//End Name Space XEvol3D


#ifdef _DEBUG
#define xnew(T,nCount)   XNEW<T>(nCount,__FILE__,__LINE__)
#define xnew_t(T,nCount) XNEW_T<T>(nCount,__FILE__,__LINE__)
#else
#define xnew   XNEW   
#define xnew_t XNEW_T   
#endif

#endif
