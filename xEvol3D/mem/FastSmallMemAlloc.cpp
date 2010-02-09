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
#include <assert.h>
#include "MemAlloc.h"
#include "FastSmallMemAlloc.h"

#ifdef _DEBUG
#include "MemState.h"
#endif

using namespace std;

BEGIN_XEVOL3D_MEM

#ifdef _DEBUG
#define FIND_POOL(size) (  ((size)+5)/8   )
#else
#define FIND_POOL(size) (  ((size)+3)/8   )
#endif

FastSmallMemAlloc::FastSmallMemAlloc()
{
	init();
}

bool  FastSmallMemAlloc::init()
{
	int size = MAX_POOL_NUM * 8;
	int n = MAX_POOL_NUM -1;
	while(size > 0)
	{
		m_MemPools[n].init(size,DEFUALT_CHUNK_SIZE/size);
		size -= 8;
		n--;
	}
	return true;
}

bool  FastSmallMemAlloc::is_ptr(void* p)
{
	if(p == NULL)
		return false;
	int* pdata = (int*) p - 1;
	if(*pdata > MAX_SMALLMEM_SIZE)
	{
		return false;
	}
	int index = FIND_POOL( (*pdata) );
	return m_MemPools[index].is_ptr(pdata);
}


#ifndef _DEBUG//非调试版本的接口

void* FastSmallMemAlloc::alloc(size_t size)
{
	if(size == 0 )
	{
		size = 1;
	}
	int index = FIND_POOL(size);
	assert(index < MAX_POOL_NUM);
	int* ret = (int*) m_MemPools[index].alloc();
	*ret = size;
	return (void*)(ret+1);
}

void  FastSmallMemAlloc::dealloc(void* p)
{
	int* pdata = (int*) p - 1;
	if(*pdata > MAX_SMALLMEM_SIZE)
	{
		return ;
	}
	int index = FIND_POOL( (*pdata) );
	assert(index < MAX_POOL_NUM);
	assert(m_MemPools[index].is_ptr(pdata));
	m_MemPools[index].dealloc(pdata);
}




void* FastSmallMemAlloc::alloc(size_t size,char* fileName,int lineNo)
{
	return alloc(size);
}

void  FastSmallMemAlloc::diagnostic()
{

}
#else//调式版本的接口

//------------------------------------
//接口
//------------------------------------
void* FastSmallMemAlloc::alloc(size_t size)
{
	return alloc(size,"Unknown postion",0); 
}

void  FastSmallMemAlloc::dealloc(void* p)
{
	if(p == NULL)
		return ;
	int* pdata = (int*) p - 1;
	if(*pdata > MAX_SMALLMEM_SIZE)
	{
		return ;
	}
	int index = FIND_POOL( (*pdata) );
	assert(index < MAX_POOL_NUM);
	assert(m_MemPools[index].is_ptr(pdata));
	getMemState().removeAllocBlock(p);
	m_MemPools[index].dealloc(pdata);
}


void* FastSmallMemAlloc::alloc(size_t size,char* fileName,int lineNo)
{
	if(size == 0 )
	{
		size = 1;
	}
	size_t index = FIND_POOL(size);
	assert(index < MAX_POOL_NUM);
	int* ret = (int*) m_MemPools[index].alloc();
	*ret = (int)size;
	getMemState().addMemAllocBlock((void*)(ret+1),size,fileName,lineNo);
	return (void*)(ret+1);
}



void  FastSmallMemAlloc::diagnostic()
{
#ifdef _WIN32
	OutputDebugStringA("Fast Small MemAlloc begin diagnostic mem pool state\n");
#else
	XEVOL_LOG(eXL_DEBUG_NORMAL,"Fast Small MemAlloc begin diagnostic mem pool state\n");
#endif
    for(size_t i = 0 ; i < MAX_POOL_NUM ; ++i)
	{
	   m_MemPools[i].diagnostic();
	}
}
#endif

END_XEVOL3D_MEM
