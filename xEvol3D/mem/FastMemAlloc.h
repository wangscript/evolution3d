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

#ifndef __FAST_MEM_ALLOC_H__
#define __FAST_MEM_ALLOC_H__

#ifdef _WIN32
#include <Windows.h>
#endif
#include <ctype.h>
#include "memdef.h"
#include <iostream>
using namespace std;
namespace XEvol3D
{ 
	namespace Mem
	{
		class  _XEVOL_BASE_API_  FastMemAlloc
		{
		public:   
			FastMemAlloc();
			//-------------------------------
			//接口
			//-------------------------------
			bool  init(){return true;}
			void* alloc(size_t size);
			void  dealloc(void* p);
			bool  is_ptr(void* p);
			void  diagnostic();
			void  diagBound();
			void* alloc(size_t size,char* fileName,int lineNo);

#ifdef _DEBUG//DEBUG版本
			struct MemNode
			{
				MemNode*          m_pPrev;
				MemNode*          m_pNext;
				char*             m_pFileName;
				unsigned int      m_LineNo;
				long              m_AllocID;
			};
#define MemNodeSizeInInt ( sizeof(MemNode)/sizeof(int) )


			template<typename T>      void  diagnostic(T& out) 
			{ 
				out<<"\nFastMemAlloc class Beiging check the memory leak\n";
				MemNode* pNode = m_pFirstMemNode;
				char buf[512]; 
				while(pNode)
				{
					int size = *( (int*)pNode + MemNodeSizeInInt);
					sprintf(buf,"%s(%d): at 0x%Xf %d int8 unfreed! memory leaked!! AllocID = %ld\n"
						,pNode->m_pFileName,pNode->m_LineNo,(unsigned int)pNode,size,pNode->m_AllocID);
					out<<buf;
					pNode = pNode->m_pNext;
				}
			}

			template<typename T>     void  diagBound(T& out)
			{
				out<<"\nFastMemAlloc class Beiging check the memory bound\n";
				MemNode* pNode = m_pFirstMemNode;
				char buf[512];
				while(pNode)
				{
					int size = *( (int*)pNode + MemNodeSizeInInt);
					unsigned short* pGuard = (unsigned short*)( (char*)pNode + size + sizeof(MemNode) + 1 ) ;

					if(*pGuard != GUARD_BYTE)
					{
						sprintf(buf,"%s(%d): at 0x%Xf %d memory bound overflow!! AllocID = %ld\n"
							,pNode->m_pFileName,pNode->m_LineNo,(unsigned int)pNode,size,pNode->m_AllocID);
						out<<buf;
					}

					pNode = pNode->m_pNext;
				}
#ifdef _WIN32
				OutputDebugString("\n");
#else
				out<<"\n";
#endif
			}//End template diagBound
			MemNode* m_pFirstMemNode;

#else

			//非调试版本的代码。空的

			template<typename T>      void   diagnostic(T& out)
			{
			}

			template<typename T>      void   diagBound(T& out)
			{
			}

#endif

		};//End of class

	};//End Name Space Mem
};//End Name Space XEvol3D

#endif//__FAST_MEM_ALLOC_H__

