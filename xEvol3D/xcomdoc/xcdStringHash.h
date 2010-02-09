/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) .
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

#ifndef __CDAPI_CORE_ALGORITHM_H__
#define __CDAPI_CORE_ALGORITHM_H__
#include "xcomdocImp.h"
/*
以下函数为Hash函数。用来指定Hash Value A , Hash Value B, 和 Index Hash
*/
BEGIN_NAMESPACE_XEVOL3D
struct HashValue
{
	int indexHash;
	int valueA;
	int valueB;
};




enum eHashCodePage
{
	HASH_INDEX   = 0,
	HASH_VALUE_1 = 1,
	HASH_VALUE_2 = 2,
	HASH_ENCRYPT = 3,
	HASH_DECRYPT = 4,
};

class xHashCodeTable
{
	bool     m_bCodeTableInited;
	int      m_CodeTable[5][256];
	xHashCodeTable()
	{
		m_bCodeTableInited = false;
	}

public:



	int* table(eHashCodePage codePage)
	{
		assert(codePage < 5);
		assert(codePage >= 0);
		return m_CodeTable[codePage];
	}

	int* operator [](eHashCodePage codePage)
	{
		assert(codePage < 5);
		assert(codePage >= 0);
		return m_CodeTable[codePage];
	}

	void            string_hash(const wchar_t* str, HashValue& hashValue);
	bool            init();
	int             value(eHashCodePage codePage,int index);
	static          xHashCodeTable& singleton();      
};

END_NAMESPACE_XEVOL3D

#endif

