/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
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

#include "stl_dllalloc.h"
#include "../mem/mem.h"
namespace std
{

	xDllSafeMemAlloc& xDllSafeMemAlloc::singleton()
	{
		static xDllSafeMemAlloc  _alloc;
		return _alloc;
	}
	void* xDllSafeMemAlloc::alloc(size_t _size)
	{
		//return ::malloc(_size);
		//std::allocator<char> _alloc;
		//return _alloc.allocate(_size);
		return XEvol3D::Mem::Alloc(_size);
	}
	void  xDllSafeMemAlloc::dealloc(void* pdata,size_t _size)
	{
		//::free(pdata);
		//std::allocator<char> _alloc;
		//return _alloc.deallocate((char*)pdata , _size);

		return XEvol3D::Mem::Dealloc(pdata);
	}

}




