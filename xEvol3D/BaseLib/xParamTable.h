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

#ifndef __xParamTable_H__
#define __xParamTable_H__

#include "xEvol3DBaseInc.h"
#include "stl_dllalloc.h"
#include <string>
#include <vector>
#include <map>
#include "xProperties.h"
BEGIN_NAMESPACE_XEVOL3D

class  _XEVOL_BASE_API_   xParamTable;


class _XEVOL_BASE_API_  xParamTable
{
public:
	typedef int        HASHVALUE;
public:
	typedef ds_map(HASHVALUE, xProperty*)         xParamMap;

protected:
	xParamMap                        m_Values;
protected:
	xParamTable(const xParamTable& ){};
	xParamTable* operator = (const xParamTable& ){return this;};
public:
	~xParamTable();
	xParamTable();
public:

	xProperty*     findValue(HASHVALUE valueName)  const ;
	xProperty*     insertValue(const wchar_t* valueName) ;

	void           setValue(const wchar_t* valueName, size_t newValue);
	void           setValue(const wchar_t* valueName, int    newValue);
	void           setValue(const wchar_t* valueName, float  newValue);
	void           setValue(const wchar_t* valueName, bool   newValue);
	void           setValue(const wchar_t* valueName, const wchar_t* newValue);
	void           setValue(const wchar_t* valueName, const void* newValue , size_t dataLen);

	int            int_value(HASHVALUE valueName) const ;
	unsigned int   hex_value(HASHVALUE valueName) const ;
	bool           bool_value(HASHVALUE valueName ) const ;
	float          float_value(HASHVALUE valueName) const ;
	const wchar_t* string_value(HASHVALUE valueName) const ;
	const void*    value(HASHVALUE valueName) const ;
	void           clear();
public:
	static HASHVALUE  toHash(const wchar_t* pName);
public:
	template< typename _Type> _Type* value(HASHVALUE valueName , _Type* pNull)
	{
		void* pRet = value(valueName);
		return (_Type*)pRet;
	};

	template< typename _Type> void setValue(const wchar_t* valueName , _Type* pData)
	{
		xProperty* pParam = insertValue(valueName);
		pParam->setValue( (const void*)pData , sizeof(_Type) );
	};

	template< typename _Type> void setValue(HASHVALUE valueName , _Type* pData)
	{
		xProperty* pParam = findValue(valueName);
		if(pParam == NULL)
			return ;

		pParam->setValue( (const void*)pData , sizeof(_Type) );
	};

};

END_NAMESPACE_XEVOL3D
#endif

