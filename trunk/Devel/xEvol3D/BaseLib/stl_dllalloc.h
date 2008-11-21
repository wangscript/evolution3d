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

#ifndef __X_STL_DLL_MEM_ALLOCATOR_H__
#define __X_STL_DLL_MEM_ALLOCATOR_H__
#include "xEvol3DBaseInc.h"

#include  <limits>
#include  <new>
#include  <string>
#include  <vector>
#include  <stdlib.h>
#ifdef max
#undef max
#endif

namespace std 
{
	class _XEVOL_BASE_API_  xDllSafeMemAlloc
	{
	public:
		static xDllSafeMemAlloc& singleton();
		void* alloc(size_t _size) ;
		void  dealloc(void* pdata,size_t _size);
	};
    template <typename T> class _dllsafe_alloc 
    {
    public:
        //type definitions
        typedef size_t    size_type;
        typedef ptrdiff_t difference_type;
        typedef T*        pointer;
        typedef const T*  const_pointer;
        typedef T&        reference;
        typedef const T&  const_reference;
        typedef T         value_type;
        
		//rebind _dllsafe_alloc to type U
        template <typename U>    struct rebind 
        {
            typedef _dllsafe_alloc<U> other;
        };

        //return address of values
        pointer address (reference value) const 
        {
            return &value;
        }
        const_pointer address (const_reference value) const 
        {
            return &value;
        }
        /*constructors and destructor
        *-nothing to do because the _dllsafe_alloc has no state
        */
        _dllsafe_alloc() throw() 
        {
        }

        _dllsafe_alloc(const _dllsafe_alloc&) throw() 
        {
        }
        template <class U> _dllsafe_alloc (const _dllsafe_alloc<U>&) throw() 
        {
        }
        ~_dllsafe_alloc() throw() 
        {
        }


        //return maximum number of elements that can be allocated
        size_type max_size () const// throw() 
        {
            //for numeric_limits see Section 4.3, page 59
            return std::numeric_limits<size_t>::max() / sizeof(T);
            //return 0x7fffff/sizeof(T);
        }


        //allocate but don't initialize num elements of type T
        pointer allocate (size_type num,const T* hint = NULL) 
        {
            //allocate memory with global new
			return (pointer) (xDllSafeMemAlloc::singleton().alloc(num*sizeof(T)));
        }


        //initialize elements of allocated storage p with value value
        void construct (pointer p, const T& value) 
        {
            //initialize memory with placement new
            new((void*)p) T(value);
        }


        //destroy elements of initialized storage p
        void destroy (pointer p) 
        {
            // destroy objects by calling their destructor
            p->~T();
        }
        //deallocate storage p of deleted elements
        void deallocate (pointer p, size_type num) 
        {
            //deallocate memory with global delete
             xDllSafeMemAlloc::singleton().dealloc((void*)p , num);
        }
		void deallocate (const_pointer p, size_type num) 
		{
			//deallocate memory with global delete
			xDllSafeMemAlloc::singleton().dealloc((void*)p , num);
		}

    };



    //return that all specializations of this _dllsafe_alloc are interchangeable
    template <typename T1, typename T2>    bool operator== (const _dllsafe_alloc<T1>&,
        const _dllsafe_alloc<T2>&) throw() 
    {
        return true;
    }

    template <typename T1, typename T2>
        bool operator!= (const _dllsafe_alloc<T1>&,
        const _dllsafe_alloc<T2>&) throw() 
    {
        return false;
    }

  
typedef basic_string<char    , char_traits<char>    , _dllsafe_alloc<char>    > ds_string;
typedef basic_string<wchar_t , char_traits<wchar_t> , _dllsafe_alloc<wchar_t> > ds_wstring;
#define dllsafe_alloc _dllsafe_alloc

#define ds_vector(_Type)            std::vector  <_Type, std::_dllsafe_alloc<_Type> >
#define ds_map(_TKey , _Type)       std::map     < _TKey , _Type , std::less<_TKey>    , std::_dllsafe_alloc<std::pair<const _TKey, _Type> > > 
#define ds_hash_map(_TKey , _Type)  stdext::hash_map< _TKey , _Type , stdext::hash_compare<_TKey> , std::_dllsafe_alloc<std::pair<const _TKey, _Type> > >


typedef vector<ds_wstring, _dllsafe_alloc<ds_wstring> >   ds_wstring_vector;
typedef vector<ds_string, _dllsafe_alloc<ds_string> >     ds_string_vector;

//#define dllsafe_alloc allocator
//#define ds_string     string
//#define ds_wstring    wstring

//#endif

}

#endif

