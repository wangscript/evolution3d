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
#include <stdlib.h>


#ifndef _DEBUG
#include "MemAlloc.h"
#include "mem.h"


/*
Release 版本的内存分配函数.
分成两个文件主要是为了方便阅读

*/

BEGIN_XEVOL3D_MEM

MemAlloc*      MEMALLOC()
{
	static MemAlloc* gs_MemAlloc = NULL;
	if(gs_MemAlloc == NULL ) gs_MemAlloc = new MemAlloc;
	return gs_MemAlloc;

}

SmallMemAlloc* SMALLMEMALLOC()
{
	static SmallMemAlloc* gs_MemAlloc = NULL;
	if(gs_MemAlloc == NULL ) gs_MemAlloc = new SmallMemAlloc;
	return gs_MemAlloc;
}


void * Alloc(size_t size)
{
	if(IS_SMALL_MEM(size))
	{
		return SMALLMEMALLOC()->alloc(size);
	}
	else
	{
		return MEMALLOC()->alloc(size);
	}
}

void Dealloc(void* p)
{
	if(p == NULL)
		return ;

	if(IS_SMALL_MEM_POINTER(p))
	{
		return SMALLMEMALLOC()->dealloc(p);
	}
	else
	{
		return MEMALLOC()->dealloc(p);
	}
}


void diagnostic()
{
	getMemState().diagnostic();
	MEMALLOC()->diagnostic();
}

void diagBound()
{
	getMemState().diagBound();
	MEMALLOC()->diagBound();
}

END_XEVOL3D_MEM

#endif
