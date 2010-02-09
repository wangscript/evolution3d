#include "stdafx.h"
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <stack>
#include <deque>
#include <new>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <iostream>
#include <math.h>
#include <assert.h>
#include "RenderApi/xRenderApi.h"
#include "RenderApi/xRenderApiImp.h"
#include "BaseLib/xStringHash.h"
#include "RenderApi/xShaderManager.h"
#include "RenderApi/xBaseShader.h"
#include "RenderApi/xRenderStateObjManager.h"
#include "RenderApi/xRenderApiManager.h"
#include "RenderApi/xRenderView.h"
#include "RenderApi/xBaseTexture.h"
#include "BaseLib/xI18N.h"
#include "BaseLib/xBaseMath.h"
#include "OperationSys/xOperationSys.h"
#include "RenderAPI/xShaderDefine.h"
#include "RenderAPI/xBaseTexture.h"

#include "xD3DInputBuffer.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD3DVertexStream , IVertexStream);
 IMPL_BASE_OBJECT_CLASSID(xD3DInputBufReflection , IInputBufferReflection);
xD3DVertexStream::xD3DVertexStream(IRenderApi* pRenderApi , IInputAssembler* pAssembler)
:IVertexStream(pRenderApi),m_pAssembler(pAssembler) //, m_pD11RenderAPI(pRenderApi)
{
     m_RefCount = 1;
	 m_pAssembler->AddRef();
	 xInputLayoutDesc&  layoutDesc =      m_pAssembler->layoutDesc();
	 m_VertexBuffers.resize(layoutDesc.nBuffer() );
}

xD3DVertexStream::~xD3DVertexStream()
{
	size_t _nVB = m_VertexBuffers.size() ;
    for(size_t i = 0 ; i < _nVB ; i ++)
	{
		XSAFE_RELEASEOBJECT(m_VertexBuffers[i].m_pBuffer);
	}
	XSAFE_RELEASEOBJECT( m_pAssembler );
}

IInputAssembler* xD3DVertexStream::getAssembler()
{
     return m_pAssembler;
}

IInputBuffer*    xD3DVertexStream::getInputBuffer(size_t nSlot)
{
     if(nSlot < m_VertexBuffers.size() )
		 return m_VertexBuffers[nSlot].m_pBuffer;
	 return NULL;
}

size_t            xD3DVertexStream::nInputSlot()
{
     return m_VertexBuffers.size();
}

//Create Vertex Buffers with inite Data;

bool xD3DVertexStream::createBuffers(size_t nVertex , void** pInitDatas ,const  xInputBufferDesc** pDescs)
{
	size_t _nVB = m_VertexBuffers.size() ;
     for(size_t i = 0 ; i < _nVB ; i ++ )
	 {
		 size_t iSlot = i;
		 const xInputBufferDesc* pDesc = pDescs? pDescs[ iSlot ] : &m_pAssembler->layoutDesc().inputSlotDesc( iSlot ) ;

		 if(pDesc&& pDesc->m_bCreateBuffer )
		 {
			 void* pInitData =  pInitDatas? pInitDatas[ iSlot ] : NULL;
			 createInputBuffer(iSlot , nVertex , pInitData , pDesc );
		 }
	 }
	 return true;
}

IInputBuffer* xD3DVertexStream::createInputBuffer(size_t nSlot , size_t nVertex , void* pData , const xInputBufferDesc* pDesc)
{
	if(nSlot >= m_VertexBuffers.size() )
		return NULL;

	const xInputBufferDesc* bufferDesc = pDesc ? pDesc : &m_pAssembler->layoutDesc().inputSlotDesc(nSlot); 
	size_t                nBufferStride = m_pAssembler->layoutDesc().nStrideOfBuffer(nSlot);
	if(nBufferStride <= 0 ) return NULL;

	IInputBuffer* vBuffer =  m_pRenderApi->createInputBuffer(nVertex , nBufferStride , bufferDesc , pData);
	m_VertexBuffers[nSlot].m_pBuffer = vBuffer;
	return vBuffer;
}

bool xD3DVertexStream::setInputBuffer(size_t nSlot , IInputBuffer* pIB)
{
	if(nSlot >= m_VertexBuffers.size() )
		return false;
	
	m_VertexBuffers[nSlot].m_pBuffer = pIB;
	pIB->AddRef();
	return m_VertexBuffers[nSlot].m_pBuffer != NULL;
}

//描述一个Buffer的Reflection
//Shander常量的Reflection
xD3DShaderConstReflection::xD3DShaderConstReflection()
{
	m_pBinder = NULL;
	m_pBufferReflection = NULL;
}

xD3DShaderConstReflection::~xD3DShaderConstReflection()
{
	XSAFE_RELEASEOBJECT(m_pBinder);
}

bool xD3DShaderConstReflection::setData(void* _data , int dataLen)
{
	return m_pBufferReflection->setData(m_bufOffset , _data , dataLen);
}

bool xD3DShaderConstReflection::setData(int _offset , void* _data , int dataLen)
{
	return m_pBufferReflection->setData(m_bufOffset + _offset , _data , dataLen);
}

//Buffer的Reflection
xD3DInputBufReflection::xD3DInputBufReflection(IInputBuffer* pBuf , bool bSysBuffer , int bufLen)
{
    m_vConstant.reserve(16);
	m_pInputBuffer = pBuf;
	m_pSysMem = new int8[bufLen];
	m_bDirty  = false;
	m_BufLen  = bufLen;
}
void* xD3DInputBufReflection::systemBuffer()
{
	return m_pSysMem;
}

xD3DInputBufReflection::~xD3DInputBufReflection()
{
	size_t _nCst = m_vConstant.size() ;
	for(size_t i = 0 ; i != _nCst ; i ++)
	{
		IShaderConstantReflection* pDesc = m_vConstant[i] ;
		XSAFE_DELETE(pDesc);
	}
    m_vConstant.clear();
	XSAFE_DELETE_ARRAY(m_pSysMem);
}

size_t xD3DInputBufReflection::lenght()
{
	return m_BufLen;
}

size_t xD3DInputBufReflection::nConstant()
{
	return m_vConstant.size();
}

IShaderConstantReflection*   xD3DInputBufReflection::constantDesc(size_t idx)
{
	if(idx >= nConstant())
		return NULL;
	return m_vConstant[idx];        
}


xD3DShaderConstReflection*  xD3DInputBufReflection::addConstant()
{
	xD3DShaderConstReflection* pDesc = new xD3DShaderConstReflection;
	m_vConstant.push_back(pDesc);
	return pDesc;    
}


bool xD3DInputBufReflection::setData(int _off , void* _data , int dataLen)
{
	memcpy( m_pSysMem + _off , _data , dataLen);
	m_bDirty = true;
	return true;
}

bool xD3DInputBufReflection::flushToBuffer()
{
	if(m_pSysMem == NULL || m_pInputBuffer == NULL)
		return false;

	if(m_bDirty )
	{
		void* pWritePos = NULL;
		m_pInputBuffer->lock(eLock_WriteDiscard,  &pWritePos);
		memcpy(pWritePos, m_pSysMem , m_BufLen);
		m_pInputBuffer->unlock();
	}
	m_bDirty = false;
	return true;
}

END_NAMESPACE_XEVOL3D
