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

#ifndef __X_STL_malloc_allocator__
#define __X_STL_malloc_allocator__

#include  <limits>
#include  <new>
#include  <stdlib.h>
#ifdef max
#undef max
#endif

namespace std 
{
    template <typename T>
    class malloc_allocator 
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


        //rebind malloc_allocator to type U
        template <typename U>
        struct rebind 
        {
            typedef malloc_allocator<U> other;
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
        *-nothing to do because the malloc_allocator has no state
        */
        malloc_allocator() throw() 
        {
        }
        malloc_allocator(const malloc_allocator&) throw() 
        {
        }
        template <class U>
            malloc_allocator (const malloc_allocator<U>&) throw() 
        {
        }
        ~malloc_allocator() throw() 
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
            return (pointer) (malloc(num*sizeof(T)));
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
            free((void*)p);
        }
    };



    //return that all specializations of this malloc_allocator are interchangeable
    template <typename T1, typename T2>    bool operator== (const malloc_allocator<T1>&,
        const malloc_allocator<T2>&) throw() 
    {
        return true;
    }
    template <typename T1, typename T2>
        bool operator!= (const malloc_allocator<T1>&,
        const malloc_allocator<T2>&) throw() 
    {
        return false;
    }
}

#endif

