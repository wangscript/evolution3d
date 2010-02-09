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

#ifndef __FAST_SMALL_MEM_ALLOC_H__
#define __FAST_SMALL_MEM_ALLOC_H__

#include "MemPool.h"
#include "memdef.h"
#include <iostream>
using namespace std;
namespace XEvol3D
{ 
	namespace Mem
	{

		class _XEVOL_BASE_API_  FastSmallMemAlloc
		{
			enum {MAX_POOL_NUM = MAX_SMALLMEM_SIZE/8 + 1};
			MemPool m_MemPools[MAX_POOL_NUM];
		public:   
			FastSmallMemAlloc();
			//-------------------------------
			//½Ó¿Ú
			//-------------------------------
			bool  init();
			void* alloc(size_t size);
			void  dealloc(void* p);
			bool  is_ptr(void* p);
			void* alloc(size_t size,char* fileName,int lineNo);
			void  diagnostic();
			void  diagBound();
			template<typename T>      void   diagnostic(T& out){}
			template<typename T>      void   diagBound(T& out){}

		};

	};//End Name Space Mem
};//End Name Space XEvol3D

#endif//__FAST_MEM_ALLOC_H__
