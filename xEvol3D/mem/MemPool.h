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

#ifndef   __MEM_POOL_H__
#define   __MEM_POOL_H__

/***************************************************************************************************************
Fixed Alloctor:
DGE project 

内存池分配器，所有的小内存分配的基础.
这是一个没有空间浪费的　Fixed Allocator．给MemPool类指定一个blockSize后，
这个MemPool就只能分配blockSize那么大的内存.该类可以通过is_ptr判断一个指针是不是由该类分配的。

架构在MemPool上的内存分配器：
SmallAlloc : 小内存分配器．以下为两种策略，
SmallAllocFast: 　　快速分配．有浪费．用Coockie记录了SmallMem的大小．很容易知道这块多大．
SmallAllocNoCookie．无浪费的分配策略　完全依赖MemPool的is_ptr来判断内存是由哪个分配器分配的．
ObjectPool<T>: 对象池，一个模板


基本算法和Loki的FixedAllocator非常接近.

类名:     MemPool
名字空间: DGE

2004-10-19. 潘李亮　．
***************************************************************************************************************/

#include <vector>
#include "stl_alloc.h"
#include "memdef.h"

namespace XEvol3D
{ 
	namespace Mem
	{

		//MemChunck　默认的大小．　取4096的原因是x86和Alpha平台上是8k分页的。
#define DEFUALT_CHUNK_SIZE 8192

		class MemPool
		{
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//一块内存．内存块的大小为,blockSize * blocks.
			class MemChunck
			{
				friend class MemPool;

				bool  init(size_t blockSize, unsigned short blocks);
				void  reset(size_t blockSize, unsigned short blocks);
				void  Release();


				void* alloc(size_t blockSize);
				void  dealloc(void* p, size_t blockSize);

				bool  is_ptr(void* p,size_t blockSize, unsigned short blocks);

			private://成员
				//内存块的地址
				unsigned char* m_pData;
				//第一个可用块，一个可用块的大小是由blockSize指定的．
				//所以说一个MemChunck的大小是　blockSize * blocks
				unsigned short m_firstAvailableBlock;
				unsigned short m_blocksAvailable;
			};

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		public:
			/*
			创建一个MemPool
			*/
			MemPool(){}
			MemPool(size_t block_size, size_t block_reserved,size_t chunck_size = DEFUALT_CHUNK_SIZE);
			bool   init(size_t block_size, size_t block_reserved,size_t chunck_size = DEFUALT_CHUNK_SIZE);
			void clear();
			void clear_unused();

			//----------------------------------------------------
			//为这个MemPool保留多少个字节的空间
			//----------------------------------------------------
			void        reserve(size_t block_reserved);
			//----------------------------------------------------
			//释放没有用过的空间．
			//----------------------------------------------------
			size_t unused_block();

			//----------------------------------------------------
			//查看还有多少空间可以用
			//----------------------------------------------------
			size_t capacity();

			//----------------------------------------------------
			//分配一个内存和释放一个内存
			//----------------------------------------------------
			void*     alloc();
			void      dealloc(void* p);

			//----------------------------------------------------
			//判断一个指针是不是由这个Pool分配的。
			//----------------------------------------------------
			bool      is_ptr(void* p);

			//********************************************************
			//
			//********************************************************
#ifdef _DEBUG
			template<typename T>      void  diagnostic(T& out)
			{

			}
#endif
			void  diagnostic();

			/**
			内存池塘需要记录的信息
			1: 每一个块的大小，即FixedAlloctor能分配的大小. 当m_blockSize不能为1.
			2: 每个MemChunck中，Block的个数.
			3: 内存池中，空闲的块的个数
			*/

		private:
			size_t      m_blockSize;       //每一个块的大小，即FixedAlloctor能分配的大小. 当m_blockSize不能为1.
			size_t      m_blocksPerChunk;  //每个MemChunck中，Block的个数.
			size_t      m_avaliableBlocks; // 内存池中，空闲的块的个数

			typedef std::vector<MemChunck,std::malloc_allocator<MemChunck> > MemChunks;
			MemChunks        m_Chuncks;

			int              m_lastChunk;



			//-------------------------------------------------------
			//End of the class MemPool
		};

	};//End Name Space Mem
};//End Name Space XEvol3D



#endif

