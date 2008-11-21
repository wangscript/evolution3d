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

#include <stdlib.h>
#include "FixMemAlloc.h"

#ifdef _DEBUG
#include "MemState.h"
#endif

BEGIN_XEVOL3D_MEM

FixMemAlloc::FixMemAlloc()
{

}
bool   FixMemAlloc::is_ptr(void* p)
{
	return true;
}

void   FixMemAlloc::diagnostic()
{

}

#ifndef _DEBUG

void*  FixMemAlloc::alloc(size_t size)
{
	return ::malloc(size);    
}

void   FixMemAlloc::dealloc(void* p)
{
	::free(p);
}
#else

void*  FixMemAlloc::alloc(size_t size)
{
	return alloc(size,"Unknown postion",0); 
}

void   FixMemAlloc::dealloc(void* p)
{
	getMemState().removeAllocBlock(p);
	::free(p);
}



void*  FixMemAlloc::alloc(size_t size,char* fileName,int lineNo)
{
	void* ret = ::malloc(size + 2);
	getMemState().addMemAllocBlock(ret,size,fileName,lineNo);
	return ret;
}


#endif
END_XEVOL3D_MEM
