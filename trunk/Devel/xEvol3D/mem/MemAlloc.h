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

#ifndef __MEM_ALLOC_H__
#define __MEM_ALLOC_H__

#include "memdef.h"
#include "MemState.h"
//基础配置类的包含
#include "MemPool.h"
#include "ObjectPool.h"

//第一种分配策略：快速内存分配，适用于内存并不是非常小的地方
#include "FastMemAlloc.h"
#include "FastSmallMemAlloc.h"

//第二种分配策略：无消耗的内存分配，在对内存浪费比较严格的地方非常适用
#include "FixMemAlloc.h"
#include "FixSmallMemAlloc.h"


#ifndef FIX_MEM_ALLOC   
#define MemAlloc                FastMemAlloc
#define SmallMemAlloc           FastSmallMemAlloc
#define IS_SMALL_MEM(size)      ( (size)<=MAX_SMALLMEM_SIZE)
#define IS_SMALL_MEM_POINTER(p) ( SMALLMEMALLOC()->is_ptr(p))
#else
#define MemAlloc                FixMemAlloc
#define SmallMemAlloc           FixSmallMemAlloc
#define IS_SMALL_MEM(size)      ( (size)<=MAX_SMALLMEM_SIZE)
#define IS_SMALL_MEM_POINTER(p) ( (*( (int*)(p) - 1 )) <=MAX_SMALLMEM_SIZE)
#endif

#endif
