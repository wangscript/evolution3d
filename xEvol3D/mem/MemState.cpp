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
#include "../xStdPch.h"

#include "MemState.h"
#ifdef _WIN32
#include <windows.h>
#endif

#pragma warning (disable:4311)
#include "MemAlloc.h"
BEGIN_XEVOL3D_MEM

size_t MemState::m_AllocedSize = 0;
size_t MemState::m_AllocID = 0;
static MemState* s_MemState = NULL;
MemState& getMemState()
{
    if(s_MemState == NULL) 
	{
		s_MemState = (MemState*)malloc(sizeof(MemState));
		s_MemState = new (s_MemState) MemState;
	}
	return  *s_MemState;
}


size_t MemState::diagnostic()
{
#ifdef _WIN32
	OutputDebugStringW(L"\nMemState class Beiging check the memory leak\n");
#else
	XEVOL_LOG(eXL_DEBUG_NORMAL,"\nMemState class Beiging check the memory leak\n");
#endif
	size_t n_Block = m_MemAllocedBlock.size();
	wchar_t buf[512];
	MemBlockList::iterator i = m_MemAllocedBlock.begin();
	for(size_t n = 0; n < n_Block ; n++)
	{
		MemUsedBlock* pBlock = i->second;
		i++;
		swprintf(buf,L"%s(%d): at 0x%Xf %d int8 unfreed! memory leaked!! AllocID = %ld\n",pBlock->m_pFileName,pBlock->m_LineNumber,(unsigned int)(pBlock->m_pointerValue),pBlock->size,pBlock->m_AllocID);
#ifdef _WIN32
		OutputDebugStringW(buf);
#else
		XEVOL_LOG(eXL_DEBUG_NORMAL,buf);
#endif
	}
	swprintf(buf,L"Total %d small memory block not freed!\n", n_Block);
#ifdef _WIN32
	OutputDebugStringW(buf);
#else
	XEVOL_LOG(eXL_DEBUG_NORMAL,buf);
#endif
	return n_Block;
}

size_t MemState::diagBound()
{
#ifdef _WIN32
	OutputDebugStringW(L"\nMemState class Beiging check the memory bound\n");
#else
	XEVOL_LOG(eXL_DEBUG_NORMAL,"\nMemState class Beiging check the memory bound\n");
#endif
	size_t n_Block = m_MemAllocedBlock.size();
	wchar_t buf[512];
	MemBlockList::iterator i = m_MemAllocedBlock.begin();
	int nBlockLeaked = 0;
	for(size_t n = 0; n < n_Block ; n++)
	{
		MemUsedBlock* pBlock = i->second;
		i++;

		unsigned short * pGuard = (unsigned short * )((char*)pBlock->m_pointerValue + pBlock->size);
		if(*pGuard != GUARD_BYTE)
		{
			swprintf(buf,L"%s(%d): at 0x%Xf %d byste(s) small memory bound overflow!! AllocID = %ld\n",	pBlock->m_pFileName,pBlock->m_LineNumber, (unsigned int)pBlock->m_pointerValue,pBlock->size,pBlock->m_AllocID);
#ifdef _WIN32
			OutputDebugStringW(buf);
#else
			XEVOL_LOG(eXL_DEBUG_NORMAL,buf);
#endif
			nBlockLeaked ++;
		}
	}
	swprintf(buf,L"Total %d small memory block has overflow !\n", nBlockLeaked);
#ifdef _WIN32
	OutputDebugStringW(buf);
#else
	XEVOL_LOG(eXL_DEBUG_NORMAL,buf);
#endif
	return n_Block;
}

void MemState::addMemAllocBlock(void* p,size_t size,char* fileName , int line)
{

	unsigned short * pGuard = (unsigned short * )((char*)p + size);
	*pGuard = GUARD_BYTE;
	//增加一条记录
	MemUsedBlock* pBlock = (MemUsedBlock*)malloc(sizeof(MemUsedBlock));
	pBlock->m_pFileName=fileName;
	pBlock->m_LineNumber = line;
	pBlock->m_pointerValue = p;
	pBlock->size = size;
	pBlock->m_AllocID = useAllocID();
 
	m_MemAllocedBlock.insert(MemBlockList::value_type(p , pBlock));
}

void MemState::addMemAllocBlock(void* p,size_t size)
{
	addMemAllocBlock(p,size,"at Unknown postion",0); 
}

void MemState::removeAllocBlock(void* p)
{
	MemBlockList::iterator i = m_MemAllocedBlock.find(p);
	if(i != m_MemAllocedBlock.end())
	{
		MemUsedBlock* pBlock = i->second;
		if(pBlock->m_pointerValue == p)
		{
			m_AllocedSize-=pBlock->size;
			m_MemAllocedBlock.erase(i);
			free((void*)pBlock);
			return ;
		}
	}
	wchar_t buf[512];
	swprintf(buf,L"double free detected: at 0x%Xf (s)\n", (unsigned int)p);
#ifdef _WIN32
	OutputDebugStringW(buf);
#else
	XEVOL_LOG(eXL_DEBUG_NORMAL,buf);
#endif
}

MemState::~MemState()
{
	MemBlockList::iterator i = m_MemAllocedBlock.begin();
	while(i != m_MemAllocedBlock.end())
	{
		MemUsedBlock* pBlock = i->second;
		free((void*)pBlock);
		i++; 
	}
	m_MemAllocedBlock.clear();
}

END_XEVOL3D_MEM
