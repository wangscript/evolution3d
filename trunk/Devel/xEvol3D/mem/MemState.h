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
#ifndef __XEVOL3D_MEM_STATE_H__
#define __XEVOL3D_MEM_STATE_H__

#include <iostream>
#include <map>
#include "stl_alloc.h"
#pragma warning(disable : 4996)

using namespace std;

#ifdef _WIN32
#include <Windows.h>
#endif

#include "memdef.h"

namespace XEvol3D
{ 
	namespace Mem
	{
		//----------------------------------------------------------
		//MemState类，主要用来记录内存分配的．
		//----------------------------------------------------------
		class _XEVOL_BASE_API_  MemState
		{
		private:
			MemState(){}
		public:
			~MemState();
			friend MemState& getMemState();
		public:
			struct MemUsedBlock
			{
				char*   m_pFileName;
				int     m_LineNumber;
				size_t  size;
				void*   m_pointerValue;
				size_t  m_AllocID;
			};
		public:
			/*记录一块已经分配了的内存*/
			void addMemAllocBlock(void* p,size_t size,char* fileName , int line);
			void addMemAllocBlock(void* p,size_t size);

			/*一块分配了的内存被释放了，必须被删除掉*/
			void removeAllocBlock(void* p);

			/*报告内存的使用情况*/
			size_t diagnostic();
			/*报告内存越界*/
			size_t diagBound();

			/*内部使用，分配一个AllocID*/
			size_t useAllocID(){return m_AllocID++ ;}
			/*内存分配后，需要将分配下来的容量记录下来。*/
			void   incAllocSize(size_t size){m_AllocedSize += size;}
			void   decAllocSize(size_t size){m_AllocedSize -= size;}

			/*
			Dump 出内存的使用状况来．
			因为是个模板，所以可以使用任何的out类型．只要支持cout类型的<<操作就可以
			*/
			template <typename T> void diagnostic(T& out)
			{
				out<<"\nMemState class Beiging check the memory leak\n";
				size_t n_Block = m_MemAllocedBlock.size();
				char buf[512];
				MemBlockList::iterator i = m_MemAllocedBlock.begin();
				for(size_t n = 0; n < n_Block ; n++)
				{
					MemUsedBlock* pBlock = *i;
					i++;
					sprintf(buf,"%s(%d): at 0x%Xf %d int8 unfreed! memory leaked!! AllocID = %ld\n"
						,pBlock->m_pFileName,pBlock->m_LineNumber,(unsigned int)pBlock->m_pointerValue,pBlock->size,pBlock->m_AllocID);
					out<<buf;
				}
#ifdef _WIN32
				OutputDebugString("\n");
#else
				XEVOL_LOG(eXL_DEBUG_NORMAL,"\n");
#endif
				return ;
			}

			template <typename T> void diagBound(T& out)
			{
				out<<"\nMemState class Beiging check the memory bound\n";
				size_t n_Block = m_MemAllocedBlock.size();
				char buf[512];
				MemBlockList::iterator i = m_MemAllocedBlock.begin();
				for(size_t n = 0; n < n_Block ; n++)
				{
					MemUsedBlock* pBlock = *i;
					i++;

					unsigned short * pGuard = (unsigned short * )((char*)pBlock->m_pointerValue + pBlock->size);
					if(*pGuard != GUARD_BYTE)
					{
						sprintf(buf,"%s(%d): at 0x%Xf %d byste(s) memory bound overflow!! AllocID = %ld\n",
							pBlock->m_pFileName,pBlock->m_LineNumber, (unsigned int)pBlock->m_pointerValue,pBlock->size,pBlock->m_AllocID);

						out<<buf;
					}
				}

#ifdef _WIN32
				OutputDebugString("\n");
#else
				XEVOL_LOG(eXL_DEBUG_NORMAL,"\n");
#endif
				return ;
			}

		private :
			typedef map<void* , MemUsedBlock* , std::less<void*> , malloc_allocator< pair<void* , MemUsedBlock*> >  > MemBlockList;
			MemBlockList         m_MemAllocedBlock;
			static size_t m_AllocedSize;
			static size_t m_AllocID;
		};

		MemState& getMemState();

	};//End Name Space Mem
};//End Name Space XEvol3D



#endif 
