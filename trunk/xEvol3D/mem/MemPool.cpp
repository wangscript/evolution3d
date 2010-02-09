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

#include "../xStdPch.h"
#include "MemPool.h"
#include <cassert>
#include <algorithm>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <iostream>
using namespace std;
/******
内存池分配器．
也就是一个固定大小的内存分配器.
分配算法为自创部分，外加Loki的启发，所以现在算法基本为Loki中的FixedAlloctor

******/

BEGIN_XEVOL3D_MEM

/*
m_pData--->        [X X X X X ...]
第一个可用块	-->  [3 | _____ ...]
[X X X X X ...]
[  | _____ ...]
[X X X X X ...]
*/
bool  MemPool::MemChunck::init(size_t blockSize, unsigned short blocks)
{
	assert(blockSize > 0);
	assert(blocks > 0);
	// Overflow check
	assert((blockSize * blocks) / blockSize == blocks);

	m_pData = (unsigned char*) malloc(blockSize * blocks);
	if(m_pData == NULL)
		return false;
	reset(blockSize, blocks);
	return true;
}

void* MemPool::MemChunck::alloc(size_t blockSize)
{
	if (!m_blocksAvailable) return 0;

	//检查内存是不是依然对齐的
	assert((m_firstAvailableBlock * blockSize) / blockSize == m_firstAvailableBlock);

	//把第一个可用返回给拥护
	unsigned char* pResult = m_pData + (m_firstAvailableBlock * blockSize);
	m_firstAvailableBlock = *((unsigned short*)pResult);
	-- m_blocksAvailable;

	return pResult;
}

void  MemPool::MemChunck::dealloc(void* p, size_t blockSize)
{
	assert(p >= m_pData);

	unsigned char* toRelease = static_cast<unsigned char*>(p);
	//内存对齐检查
	assert((toRelease - m_pData) % blockSize == 0);

	//把释放掉的块加入到表头里．新建一个表头,表头下一个块指向原来的第一个可用块
	* ((unsigned short*)toRelease) = m_firstAvailableBlock;
	//第一个可用块指向表头
	m_firstAvailableBlock = static_cast<unsigned short>( (toRelease - m_pData) / blockSize   )  ;
	//块对齐检查
	assert(m_firstAvailableBlock == (toRelease - m_pData) / blockSize);

	++ m_blocksAvailable;
}

void  MemPool::MemChunck::reset(size_t blockSize, unsigned short blocks)
{
	assert(blockSize > 0);
	assert(blocks > 0);
	//诣出检查
	assert((blockSize * blocks) / blockSize == blocks);

	m_firstAvailableBlock = 0;
	m_blocksAvailable = blocks;

	//填充内存块的链
	unsigned short i = 0;
	unsigned char* p = m_pData;
	for (; i != blocks; p += blockSize)
	{
		unsigned short * pNext =(unsigned short*) p;
		*pNext = ++i;
	}

}

void  MemPool::MemChunck::Release()
{
	free((void*)m_pData);
}

bool  MemPool::MemChunck::is_ptr(void* p,size_t blockSize, unsigned short blocks)
{
	if( p < m_pData) return false;
	//内存不在这个里面。也不是他分配的。
	if( p > m_pData + blockSize * blocks) return false;

	//指针没在blockSize边界上对齐．肯定不是由这个MemChunck分配的
	if( ((unsigned char*)p - m_pData)%blockSize != 0) return false;
	return true;
}


//======================================================================================
//　以下为MemPool的类．
//  函数的实现．
//======================================================================================

MemPool::MemPool(size_t block_size, size_t block_reserved,size_t chunck_size)
{
	init(block_size,block_reserved,chunck_size);
}

bool MemPool::init(size_t block_size, size_t block_reserved,size_t chunck_size)
{
	if(block_size < sizeof(unsigned short) )
		block_size = sizeof(unsigned short) ;
	m_blockSize = block_size;
	assert(chunck_size > block_size);
	m_blocksPerChunk  = chunck_size / block_size;

	m_avaliableBlocks = 0;
	size_t nChuncks = (block_reserved + 1 ) / m_blocksPerChunk;

	for(size_t i = 0 ; i < nChuncks ; i ++ )
	{
		MemChunck chunck;
		if( chunck.init(m_blockSize,(unsigned short)m_blocksPerChunk) == false)
			return false;
		m_Chuncks.push_back(chunck);
		m_avaliableBlocks += m_blocksPerChunk;
	}

	m_lastChunk = 0;

	return true;
}

void MemPool::clear()
{
	size_t nChuncks = m_Chuncks.size();
	for(size_t i = 0 ; i < nChuncks ; i ++ )
	{
		m_Chuncks[i].Release();
	}
	m_avaliableBlocks = 0;
}

void MemPool::clear_unused()
{
	MemChunks::iterator i = m_Chuncks.end();
	size_t  n = m_Chuncks.size();
	while( (n--)!=0)
	{
		--i;
		if(i->m_blocksAvailable == m_blocksPerChunk)
		{
			i->Release();
			m_Chuncks.erase(i);
		}
	}
	m_lastChunk = 0;

}

//----------------------------------------------------
//为这个MemPool保留多少个字节的空间
//----------------------------------------------------
void      MemPool::reserve(size_t block_reserved)
{
	if(m_avaliableBlocks >= block_reserved)
		return ;

	size_t nChuncks = (block_reserved - m_avaliableBlocks + 1) / m_blocksPerChunk;
	for(size_t i = 0 ; i < nChuncks ; i ++ )
	{
		MemChunck chunck;
		if( chunck.init(m_blockSize,(unsigned short)m_blocksPerChunk) == false)
			return ;
		m_Chuncks.push_back(chunck);
		m_avaliableBlocks += m_blocksPerChunk;
	}
}

//----------------------------------------------------
//释放没有用过的空间．
//----------------------------------------------------
size_t MemPool::unused_block()
{
	return m_avaliableBlocks;
}

//----------------------------------------------------
//查看还有多少空间可以用
//----------------------------------------------------
size_t MemPool::capacity()
{
	return m_Chuncks.size() * m_blocksPerChunk * m_blockSize;
}

//-------------------------------------------- --------
//分配一个内存和释放一个内存
//----------------------------------------------------
void*     MemPool::alloc()
{
	//先用最后一次分配的MemChunck来分配．
	//如果最后一个块还有，就返回．
	void * ret =  m_Chuncks[m_lastChunk].alloc(m_blockSize);
	if(ret) {m_avaliableBlocks --; return ret;}



	size_t nChuncks = m_Chuncks.size();

	//找一个有空块的MemChunck
	for(size_t i = 0 ; i < nChuncks ; i ++ )
	{
		if(m_Chuncks[i].m_blocksAvailable > 0)
		{
			//把最后一个分配块的游标指向有空闲的块．
			//可用块的计数减少，并分配一个块出去
			m_lastChunk = (int)i;
			m_avaliableBlocks --;
			return m_Chuncks[m_lastChunk].alloc(m_blockSize);
		}
	}
	//建立一个新的Block，放到里面．
	m_Chuncks.push_back( MemChunck() );
	m_lastChunk = (int)nChuncks;
	//如果块初始化失败，则表示没有内存了。返回空．表示失败
	if( false == m_Chuncks[nChuncks].init(m_blockSize,(unsigned short)m_blocksPerChunk))
		return NULL;

	//新建立了一个块．放到最后
	m_avaliableBlocks += (m_blocksPerChunk - 1);
	return m_Chuncks[m_lastChunk].alloc(m_blockSize);

}

void      MemPool::dealloc(void* p)
{
	if(m_Chuncks[m_lastChunk].is_ptr(p,m_blockSize,(unsigned short)m_blocksPerChunk) == true)
	{
		m_Chuncks[m_lastChunk].dealloc(p,m_blockSize);
		m_avaliableBlocks ++;
		return ;
	}
	size_t nChuncks = m_Chuncks.size();

	for(size_t i = 0 ; i < nChuncks ; i ++ )
	{
		if(m_Chuncks[i].is_ptr(p,m_blockSize,(unsigned short)m_blocksPerChunk) == true)
		{
			//当前释放了一个内存．那么这个Chunck肯定是有内存可以用的。
			m_lastChunk =(int) i;
			m_Chuncks[m_lastChunk].dealloc(p,m_blockSize);
			m_avaliableBlocks ++;
			return ;
		}
	}  
}

//----------------------------------------------------
//判断一个指针是不是由这个Pool分配的。
//----------------------------------------------------
bool      MemPool::is_ptr(void* p)
{
	size_t nChuncks = m_Chuncks.size();
	for(size_t i = 0 ; i < nChuncks ; i ++ )
	{
		if(m_Chuncks[i].is_ptr(p,m_blockSize,(unsigned short)m_blocksPerChunk) == true)
		{
			return true;
		}
	} 
	return false;
}
void  MemPool::diagnostic()
{
	size_t  nChunck = m_Chuncks.size();
	size_t totalPoolSize = capacity();
	size_t freeSize = 0;
	for(size_t i = 0 ; i < nChunck ; i ++)
	{
		freeSize += m_Chuncks[i].m_blocksAvailable * m_blockSize;
	}

	wchar_t buf[512];
	swprintf(buf,L"Mempool:Capacity=%d(int8) unused=%d(int8) blockSize=%d nChunk = %d\n ",totalPoolSize,freeSize , m_blockSize, nChunck  );
#ifdef _WIN32
		OutputDebugStringW(buf);
#else
    XEVOL_LOG(eXL_DEBUG_NORMAL,buf);
#endif
}
END_XEVOL3D_MEM
