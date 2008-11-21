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

#ifndef __OBJECT_POOL__
#define __OBJECT_POOL__

#include "memdef.h"
#include "MemPool.h"

namespace XEvol3D
{ 
	namespace Mem
	{

		template <typename T>
		class ObjectPool
		{
			MemPool m_MemPool;
		public:
			ObjectPool(int ObjectReserve = 1024,size_t chunck_size = DEFUALT_CHUNK_SIZE):m_MemPool(sizeof(T),ObjectReserve,chunck_size){}
			~ObjectPool(){m_MemPool.clear();}
			void*   createObj(){return (void*)m_MemPool.alloc();}
			void    destoryObj(void*p){m_MemPool.dealloc(p);}
			MemPool& getMemPool(){return &m_MemPool;}


		};
	};//End Name Space Mem
};//End Name Space XEvol3D

#define DECL_OBJECT_POOL(Type)         \
	static XEvol3D::Mem::ObjectPool<Type> m_ObjPool;\
	void*  operator new(size_t t){ return m_ObjPool.createObj(); }\
	void   operator delete(void* p) {  m_ObjPool.destoryObj(p); }\
	void   realse_all_object(){m_ObjPool.getMemPool().clear();} \

#define IMP_OBJECT_POOL_DEF(Type,ObjectReserve)\
	XEvol3D::Mem::ObjectPool<Type> Type::m_ObjPool(ObjectReserve);\

#define IMP_OBJECT_POOL(Type,ObjectReserve,chuncksize)\
	XEvol3D::Mem::ObjectPool<Type> Type::m_ObjPool(ObjectReserve,chuncksize);\

#endif

