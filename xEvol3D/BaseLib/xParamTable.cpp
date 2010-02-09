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
#include "../xStdPch.h"
#include "xParamTable.h"
#include "xStringHash.h"

BEGIN_NAMESPACE_XEVOL3D


xParamTable::~xParamTable()
{
    clear();
}

xParamTable::xParamTable()
{

}


xProperty*        xParamTable::findValue(HASHVALUE valueName)  const
{
	xParamMap::const_iterator it =   m_Values.find(valueName);
	if(it == m_Values.end() )
		return NULL;
	return it->second;
}

xProperty*        xParamTable::insertValue(const wchar_t* valueName)
{
	HASHVALUE hName = toHash(valueName);
	xProperty* pValue = findValue(hName);
	if(pValue)
		return pValue;

	pValue = new xProperty( valueName );
	m_Values.insert(xParamMap::value_type(hName , pValue) );
	return pValue;
}

void           xParamTable::setValue(const wchar_t* valueName, size_t newValue)
{
	xProperty* pParam = insertValue(valueName);
	pParam->setValue( (int)newValue);
}

void           xParamTable::setValue(const wchar_t* valueName, int    newValue)
{
	xProperty* pParam = insertValue(valueName);
	pParam->setValue( (int)newValue);
}

void           xParamTable::setValue(const wchar_t* valueName, float  newValue)
{
	xProperty* pParam = insertValue(valueName);
	pParam->setValue( newValue);
}

void           xParamTable::setValue(const wchar_t* valueName, bool   newValue)
{
	xProperty* pParam = insertValue(valueName);
	pParam->setValue( newValue);
}

void           xParamTable::setValue(const wchar_t* valueName, const wchar_t* newValue)
{
	xProperty* pParam = insertValue(valueName);
	pParam->setValue( newValue);
}

void xParamTable::setValue(const wchar_t* valueName, const void* newValue , size_t dataLen)
{
	xProperty* pParam = insertValue(valueName);
	pParam->setValue( newValue , dataLen);
}

int xParamTable::int_value(HASHVALUE valueName) const
{
      xProperty* pValue = findValue(valueName);
	  if(pValue == NULL)  
		  return 0;
	  return pValue->int_value();
}

bool           xParamTable::bool_value(HASHVALUE valueName ) const
{
	xProperty* pValue = findValue(valueName);
	if(pValue == NULL)  
		return 0;
	return pValue->bool_value();
}

float          xParamTable::float_value(HASHVALUE valueName) const
{
	xProperty* pValue = findValue(valueName);
	if(pValue == NULL)  
		return 0;
	return pValue->float_value();
}

const wchar_t* xParamTable::string_value(HASHVALUE valueName) const
{
	xProperty* pValue = findValue(valueName);
	if(pValue == NULL)  
		return 0;
	return pValue->string_value();
}

const void* xParamTable::value(HASHVALUE valueName) const
{
	xProperty* pValue = findValue(valueName);
	if(pValue == NULL)  
		return 0;
	return pValue->value();
}

void           xParamTable::clear()
{
	for(xParamMap::iterator it = m_Values.begin(); it != m_Values.end() ; ++it)
	{
		xProperty* pValue = it->second;
		delete pValue;
	}
	m_Values.clear();
}

xParamTable::HASHVALUE xParamTable::toHash(const wchar_t* pName)
{
	return xStringHash(pName);
}

END_NAMESPACE_XEVOL3D


