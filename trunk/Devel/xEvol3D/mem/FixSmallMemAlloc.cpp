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

#include <assert.h>
#include "MemAlloc.h"
#include "FixSmallMemAlloc.h"


#ifdef _DEBUG
#include "MemState.h"
#endif
#include <iostream>
using namespace std;

BEGIN_XEVOL3D_MEM

#ifdef _DEBUG
#define FIND_POOL(size) (  ((size+2)-1)   )
#else
#define FIND_POOL(size) (  ((size)-1)   )
#endif

void  FixSmallMemAlloc::diagnostic()
{

}
bool  FixSmallMemAlloc::init()
{

	int size = MAX_POOL_NUM ;
#ifdef _DEBUG
	while(size > 0)
	{
		m_MemPools[size - 1].init(size + 2,DEFUALT_CHUNK_SIZE/size);
		--size;
	}
#else
	while(size > 0)
	{
		m_MemPools[size - 1].init(size,DEFUALT_CHUNK_SIZE/size);
		--size;
	}
#endif
	return true;
}
FixSmallMemAlloc::FixSmallMemAlloc()
{
	init();
}

bool  FixSmallMemAlloc::is_ptr(void* p)
{
	for(int n = MAX_SMALLMEM_SIZE - 1 ; n >=0 ; --n)
	{
		if(m_MemPools[n].is_ptr(p))
			return true;
	}
	return false;
}


#ifndef _DEBUG//非调试版本的接口

void* FixSmallMemAlloc::alloc(size_t size)
{
	if(size == 0 )
	{
		size = 1;
	}
	size_t index = FIND_POOL(size);
	assert(index < MAX_POOL_NUM);
	return m_MemPools[index].alloc();

}

void* FixSmallMemAlloc::alloc(size_t size,char* fileName,int lineNo)
{
	return alloc(size);
}

void  FixSmallMemAlloc::dealloc(void* p)
{
	for(int n = MAX_SMALLMEM_SIZE - 1 ; n >=0 ; --n)
	{
		if(m_MemPools[n].is_ptr(p))
		{
			m_MemPools[n].dealloc(p);
			return ;
		}
	}
}



#else//调式版本的接口

//------------------------------------
//接口
//------------------------------------
void* FixSmallMemAlloc::alloc(size_t size)
{
	return alloc(size,"Unknown postion",0); 
}
void  FixSmallMemAlloc::dealloc(void* p)
{
	for(int n = MAX_SMALLMEM_SIZE - 1 ; n >=0 ; --n)
	{
		if(m_MemPools[n].is_ptr(p))
		{
			getMemState().removeAllocBlock(p);
			m_MemPools[n].dealloc(p);
			return ;
		}
	}
}


void* FixSmallMemAlloc::alloc(size_t size,char* fileName,int lineNo)
{
	if(size == 0 )
	{
		size = 1;
	}
	size_t index = FIND_POOL(size);
	assert(index < MAX_POOL_NUM);
	void* ret =  m_MemPools[index].alloc();
	getMemState().addMemAllocBlock(ret,size,fileName,lineNo);
	return ret;
}
#endif

END_XEVOL3D_MEM
