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
#include <assert.h>
#include "xcdStringHash.h"
#include "../BaseLib/xStringHash.h"
#ifdef _USE_XEVOL3D_NEW_
#include "../mem/mem_operator.h"
#endif

BEGIN_NAMESPACE_XEVOL3D

void  xHashCodeTable::string_hash(const wchar_t* str, HashValue& hashValue)
{
	wchar_t *key     = (wchar_t *)str;
	unsigned long seed1_i  = 0x7FED7FED, seed2_i = 0xEEEEEEEE;
	unsigned long seed1_va = 0x7FED7FED, seed2_va = 0xEEEEEEEE;
	unsigned long seed1_vb = 0x7FED7FED, seed2_vb = 0xEEEEEEEE;

	hashValue.indexHash = ELFHash(str);
	hashValue.valueA    = RSHash(str);
	hashValue.valueB    = JSHash(str);
	return ;
	/*
	int ch;
	while(*key != 0)
	{ 
		ch = *key;
		if(ch < 255) ch = toupper(*key);
		key++;

		seed1_i = m_CodeTable[HASH_INDEX][ch] ^ (seed1_i + seed2_i);
		seed2_i = ch + seed1_i + seed2_i + (seed2_i << 5) + 3; 

		seed1_va = m_CodeTable[HASH_VALUE_1][ch] ^ (seed1_va + seed2_va);
		seed2_va = ch + seed1_va + seed2_va + (seed2_va << 5) + 3; 


		seed1_vb = m_CodeTable[HASH_VALUE_2][ch] ^ (seed1_vb + seed2_vb);
		seed2_vb = ch + seed1_vb + seed2_vb + (seed2_vb << 5) + 3; 
	}
	hashValue.indexHash = seed1_i;
	hashValue.valueA    = seed1_va;
	hashValue.valueB    = seed1_vb;
	*/
}


bool xHashCodeTable::init()
{
	if(m_bCodeTableInited) return true;

	int dwSeed = 0x100001;
	int p2     = 0x2AAAAB;
	int dwHeight;
	int dwLow;
	for( int i = 0; i < 0x100; i++)
	{
		for(int j = 0; j<5; j++)
		{
			dwSeed   = (dwSeed*125+3)%p2;
			dwHeight = (dwSeed&0xFFFF)<<16;
			dwSeed   = (dwSeed*125+3)%p2;
			dwLow    = (dwSeed&0xFFFF);
			m_CodeTable[j][i] = dwHeight | dwLow ;
		}
	}

	m_bCodeTableInited = true;
	return true;
}

int  xHashCodeTable::value(eHashCodePage codePage,int index)
{
	return table(codePage)[index];
}

xHashCodeTable& xHashCodeTable::singleton()
{
	static xHashCodeTable codeTable;
	codeTable.init();
	return codeTable;
}

END_NAMESPACE_XEVOL3D
