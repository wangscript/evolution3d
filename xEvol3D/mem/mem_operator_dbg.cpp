#include "../xStdPch.h"
#ifdef _USE_XEVOL3D_NEW_

#ifdef _DEBUG
#include "MemAlloc.h"
#include "MemState.h"
#include "mem_operator.h"
/*
DEBUG 版本的内存分配函数.
分成两个文件主要是为了方便阅读

*/



namespace XEvol3D
{ 
	namespace Mem
	{
		namespace XEvol3D_Mem_Operator
		{
			MemAlloc *      MemAllocator:: __pMemAlloc = NULL;
			SmallMemAlloc * MemAllocator:: __pSmallMemAlloc = NULL;

			void * Alloc_Operator(size_t size , char* filename, int line)
			{
				static volatile MemAllocator __allocator;

				if(IS_SMALL_MEM(size))
				{
					return MemAllocator::getSmallAlloc().alloc(size,filename,line);
				}
				else
				{
					return MemAllocator::getMemAlloc().alloc(size,filename,line);
				}
			}

			void Dealloc_Operator(void* p)
			{
				if(p == NULL)
					return ;

				if(IS_SMALL_MEM_POINTER(p))
				{
					return MemAllocator::getSmallAlloc().dealloc(p);
				}
				else
				{
					return MemAllocator::getMemAlloc().dealloc(p);
				}
			}
		}

	};//End Name Space Mem
};//End Name Space XEvol3D


#endif
#endif
