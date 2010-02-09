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

#ifndef __XEVOL_REFCOUNT_OBJECT_PTR_H__
#define __XEVOL_REFCOUNT_OBJECT_PTR_H__

#include "xEvol3DBaseInc.h"

BEGIN_NAMESPACE_XEVOL3D

template<typename _T > class xRefCountAutoPtr
{
	_T* m_ptr;
public :
	xRefCountAutoPtr(_T* pType)
	{
		m_ptr = pType;
		m_ptr->AddRef();
	}

	~xRefCountAutoPtr()
	{
		if(m_ptr) m_ptr->Release();
	}

	xRefCountAutoPtr(const xRefCountAutoPtr<_T>& rhv)
	{
		m_ptr = rhv.m_pTR;
		m_ptr->AddRef();
	}

	void operator = (const xRefCountAutoPtr<_T>& rhv)
	{
		m_ptr = rhv.m_pTR;
		m_ptr->AddRef();
	}

	operator  _T* () 
	{
		return m_ptr;
	}
	_T* operator ->() const
	{
		return m_ptr;
	}
	_T* operator *() const
	{
		return m_ptr;
	}
};


template<typename _T > class xAutoReleaser
{
	_T* m_ptr;
public :
	xAutoReleaser(_T* pType)
	{
		m_ptr = pType;
	}

	~xAutoReleaser()
	{
		if(m_ptr) m_ptr->Release();
	}

	xAutoReleaser(const xAutoReleaser<_T>& rhv)
	{
		m_ptr = rhv.m_ptr;
		m_ptr->AddRef();
	}

	void operator = (const xAutoReleaser<_T>& rhv)
	{
		m_ptr = rhv.m_ptr;
		m_ptr->AddRef();
	}

	operator  _T* () 
	{
		return m_ptr;
	}
	_T* operator ->() const
	{
		return m_ptr;
	}
	_T* operator *() const
	{
		return m_ptr;
	}
};

END_NAMESPACE_XEVOL3D

#endif



