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
#pragma warning (disable:4311)
#pragma warning (disable:4312)

#include "FastMemAlloc.h"
#include "MemAlloc.h"

#ifdef _DEBUG
#include <iostream>

using namespace std;
#endif
#include <assert.h>

#if (defined(_WIN32) || defined(_WIN64) )
#include <windows.h>
#endif
#include "MemState.h"
using namespace std;

BEGIN_XEVOL3D_MEM

//非调试版本
#ifndef _DEBUG
void* FastMemAlloc::alloc(size_t size)
{
	int* pAlloc = (int*) malloc(size + 4);    
	* pAlloc = size;
	return (void*)(pAlloc + 1);
}

void  FastMemAlloc::dealloc(void* p)
{
	int* pMem = (int*)p - 1;
	if(*pMem > MAX_SMALLMEM_SIZE)
		free((void*)pMem);
}

bool  FastMemAlloc::is_ptr(void* p)
{
	int* pMem = (int*)p - 1;
	if(*pMem > MAX_SMALLMEM_SIZE)
		return true;
	else
		return false;
}
FastMemAlloc::FastMemAlloc()
{

}
void  FastMemAlloc::diagnostic()
{

}
void  FastMemAlloc::diagBound()
{
}
void* FastMemAlloc::alloc(size_t size,char* fileName,int lineNo)
{
	return alloc(size);
}

//调试版本
#else

FastMemAlloc::FastMemAlloc()
{
	m_pFirstMemNode = NULL;
}

void  FastMemAlloc::diagnostic()
{
#ifdef _WIN32
	OutputDebugStringW(L"\nFastMemAlloc class Beiging check the memory leak \n");
#else
	XEVOL_LOG(eXL_DEBUG_NORMAL,"\nFastMemAlloc class Beiging check the memory leak\n");
#endif
	MemNode* pNode = m_pFirstMemNode;
	wchar_t buf[512];
	int n_block = 0;
	while(pNode)
	{
		int size = *( (int*)pNode + MemNodeSizeInInt);
		swprintf(buf,L"%s(%d): at 0x%Xf %d int8 unfreed! memory leaked!! AllocID = %ld\n"
			,pNode->m_pFileName,pNode->m_LineNo,(unsigned int)pNode,size,pNode->m_AllocID);
#ifdef _WIN32
		OutputDebugStringW(buf);
#else
		XEVOL_LOG(eXL_DEBUG_NORMAL,buf);
#endif
		n_block ++;
		pNode = pNode->m_pNext;
	}
	swprintf(buf,L"Total %d memory block not freed !\n" , n_block);
#ifdef _WIN32
	OutputDebugStringW(buf);
#else
	cout<<buf;
#endif
}

void  FastMemAlloc::diagBound()
{
#ifdef _WIN32
	OutputDebugStringW(L"\nFastMemAlloc class Beiging check the memory bound\n");
#else
	XEVOL_LOG(eXL_DEBUG_NORMAL,"\nFastMemAlloc class Beiging check the memory bound\n");
#endif

	/*[MemNode   size  <UseData> guard data]*/
	MemNode* pNode = m_pFirstMemNode;
	wchar_t buf[512];
	int n_block = 0;
	while(pNode)
	{
		int size = *( (int*)pNode + MemNodeSizeInInt);
		unsigned short* pGuard = (unsigned short*)( (char*)pNode + size + sizeof(MemNode) + sizeof(int) ) ;

		if(*pGuard != GUARD_BYTE)
		{
			swprintf(buf,L"%s(%d): at 0x%Xf %d memory bound overflow!! AllocID = %ld\n"
				,pNode->m_pFileName,pNode->m_LineNo,(unsigned int)pNode,size,pNode->m_AllocID);
#ifdef _WIN32
			OutputDebugStringW(buf);
#else
			XEVOL_LOG(eXL_DEBUG_NORMAL,buf);
#endif
			n_block ++;
		}

		pNode = pNode->m_pNext;
	}
	swprintf(buf,L"Total %d memory block has overflow !\n" , n_block);
#ifdef _WIN32
	OutputDebugStringW(buf);
#else
	XEVOL_LOG(eXL_DEBUG_NORMAL,buf);
#endif
}

void* FastMemAlloc::alloc(size_t size)
{
	return alloc(size,"Unknown postion",0); 
}

void* FastMemAlloc::alloc(size_t size,char* fileName,int lineNo)
{
	/*[MemNode   size  <UseData> guard data]*/
	char* p = (char*)malloc(sizeof(MemNode) + sizeof(int) + size +  2);
	assert(p);
	int* pSize = (int*) (p + sizeof(MemNode));
	*pSize = (int)size;
	getMemState().incAllocSize(size);

	unsigned short* pGuard = (unsigned short*  )( p + size + sizeof(MemNode) + sizeof(int) );
	*pGuard = GUARD_BYTE;

	MemNode* pMemNode = (MemNode*)(p);

	pMemNode->m_pFileName = fileName;
	pMemNode->m_LineNo = lineNo;
	pMemNode->m_AllocID = (long)getMemState().useAllocID();

	pMemNode->m_pNext = m_pFirstMemNode;
	if(m_pFirstMemNode)
	{
		m_pFirstMemNode->m_pPrev = pMemNode;
	}
	m_pFirstMemNode = pMemNode;
	pMemNode->m_pPrev = NULL;

	return (void*) ( p+sizeof(MemNode) + sizeof(int) );

}

void  FastMemAlloc::dealloc(void* p)
{
	/*[MemNode   size(pSize)  <UseData p> guard data]*/
	int* pSize = (int*)p - 1;
	if(*pSize > MAX_SMALLMEM_SIZE)
	{
		MemNode* pMemNode = (MemNode*) ( pSize - MemNodeSizeInInt);
		//列表重新建立
		if( pMemNode->m_pPrev )
			pMemNode->m_pPrev->m_pNext = pMemNode->m_pNext ;
		else
			m_pFirstMemNode = pMemNode->m_pNext;
		if( pMemNode->m_pNext )
			pMemNode->m_pNext->m_pPrev = pMemNode->m_pPrev;

		getMemState().decAllocSize(*pSize);
		char* pMem = (char*)p- (sizeof(int)+sizeof(MemNode));
		free((void*) (pMem) );
	}
}

bool  FastMemAlloc::is_ptr(void* p)
{
	/*[MemNode   size(pSize)  <UseData p> guard data]*/
	int* pSize = (int*)p -  1 ;
	if(*pSize > MAX_SMALLMEM_SIZE)
		return true;
	else
		return false;
}

#endif

END_XEVOL3D_MEM
