#include "xD10InputBuffer.h"
#include "xDirect3D10API.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD10VertexStream , IVertexStream);
IMPL_BASE_OBJECT_CLASSID(xD10InputBuffer  , IInputBuffer);

static void conv(const xInputBufferDesc& from , D3D10_BUFFER_DESC& to)
{
	to.BindFlags = 0;
	
	if( from.m_bindtype&BIND_AS_VERTEX_BUFFER )
	{
		to.BindFlags |= D3D10_BIND_VERTEX_BUFFER;
	}

	if( from.m_bindtype&BIND_AS_INDEX_BUFFER )
	{
		to.BindFlags |= D3D10_BIND_INDEX_BUFFER;
	}

	if( from.m_bindtype&BIND_AS_CONSTANT_BUFFER )
	{
		to.BindFlags |= D3D10_BIND_CONSTANT_BUFFER ;
	}

	if( from.m_bindtype&BIND_AS_SHADER_RESOURCE )
	{
		to.BindFlags |= D3D10_BIND_SHADER_RESOURCE;
	}

	if( from.m_bindtype&BIND_AS_STREAM_OUTPUT )
	{
		to.BindFlags |= D3D10_BIND_STREAM_OUTPUT;
	}

	if( from.m_bindtype&BIND_AS_RENDER_TARGET )
	{
		to.BindFlags |= D3D10_BIND_RENDER_TARGET;
	}
	

	switch(from.m_accessFlage)
	{
	case RESOURCE_ACCESS_READ:
		to.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
		break;
	case RESOURCE_ACCESS_NONE:
		to.CPUAccessFlags = 0;
		break;
	case RESOURCE_ACCESS_WRITE:
		to.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		break;
	case RESOURCE_ACCESS_READWRITE:
		to.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
		break;
	}

	switch(from.m_usage)
	{
	case RESOURCE_USAGE_DEFAULT:
		to.Usage = D3D10_USAGE_DEFAULT;
		break;
	case RESOURCE_USAGE_IMMUTABLE:
		to.Usage = D3D10_USAGE_IMMUTABLE;
		break;
	case RESOURCE_USAGE_DYNAMIC:
		to.Usage = D3D10_USAGE_DYNAMIC;
		break;
	case RESOURCE_USAGE_STAGING:
		to.Usage = D3D10_USAGE_STAGING;
		break;
	}
	to.MiscFlags = 0;
	if(from.m_bShared ) to.MiscFlags =  D3D10_RESOURCE_MISC_SHARED;
}



xD10InputBuffer::xD10InputBuffer(xD3D10RenderApi* pAPI  ) :m_pD10API(pAPI) , IInputBuffer(pAPI)
{
	m_iBufLen = 0;
	m_nBytePerVertex = 0;
	m_nVertex = 0;
    m_pBuffer = NULL;
	m_pRSV = NULL;
	m_pReflection = NULL;
}

xD10InputBuffer::~xD10InputBuffer()
{
	_destory();
}

IInputBufferReflection* xD10InputBuffer::reflection()
{
    return m_pReflection;
}

IInputBufferReflection* xD10InputBuffer::createReflection(bool bSysBuffer)
{
	if(m_pReflection == NULL)
		return new xD10InputBufReflection(this , bSysBuffer , (int)m_iBufLen);
	return m_pReflection;
}

ID3D10ShaderResourceView*  xD10InputBuffer::toShaderResourceView()
{
    if(m_pRSV) return m_pRSV;

	D3D10_SHADER_RESOURCE_VIEW_DESC desc;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.ViewDimension = D3D10_SRV_DIMENSION_BUFFER;
	desc.Buffer.ElementOffset = 0;
	desc.Buffer.ElementWidth  = (UINT)( m_iBufLen/ (sizeof(float) * 4) );
	m_pD10API->d10Device()->CreateShaderResourceView(m_pBuffer, &desc , &m_pRSV);
	return m_pRSV;
}

bool xD10InputBuffer::create(size_t nVertex , size_t bytePerVertex , const xInputBufferDesc* pDesc , void* pData  )
{
	_destory();
    m_iBufLen = nVertex * bytePerVertex;
	bool ret= create(m_iBufLen , pDesc , pData);
	m_nVertex = nVertex;
	m_nBytePerVertex = bytePerVertex;
	return ret;
}

bool xD10InputBuffer::create(size_t iBufLen , const xInputBufferDesc* pDesc , void* pData  )
{
	_destory();
	m_iBufLen = iBufLen ;
	m_nVertex = 1;
	m_nBytePerVertex = iBufLen;

	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	D3D10_SUBRESOURCE_DATA* pInitData = pData ? &InitData:NULL;   

	ID3D10Device* pD10Device = m_pD10API->d10Device();

	D3D10_BUFFER_DESC buffDesc;
	buffDesc.ByteWidth = (UINT)m_iBufLen ; 

	conv( *pDesc , buffDesc);
	pD10Device->CreateBuffer(&buffDesc , pInitData , &m_pBuffer);
	return m_pBuffer != NULL;
}

bool xD10InputBuffer::lock(eLockPolicy lockPolicy , void** pData)
{
	 if(m_pBuffer == NULL)
		 return false;
     HRESULT hr = m_pBuffer->Map( (D3D10_MAP)lockPolicy , 0/*D3D10_MAP_FLAG_DO_NOT_WAIT*/ , pData);
	 if(SUCCEEDED(hr))
	 {
		 return true;
	 }
	 return false;
}

bool xD10InputBuffer::unlock()
{
	if(m_pBuffer == NULL)
		return false;
	m_pBuffer->Unmap();
	return true;
}

bool xD10InputBuffer::update(eLockPolicy lockPolicy , void* pData , size_t dataLen)
{
	if(m_pBuffer == NULL)
		return false;
	void* pDstData = NULL;
	HRESULT hr = m_pBuffer->Map( (D3D10_MAP)lockPolicy , 0/*D3D10_MAP_FLAG_DO_NOT_WAIT*/ , &pDstData);
	if(SUCCEEDED(hr))
	{
		memcpy(pDstData , pData , dataLen);
		m_pBuffer->Unmap();
		return true;
	}
	return false;
}

bool xD10InputBuffer::_destory()
{
	XSAFE_RELEASE(m_pRSV);
	XSAFE_RELEASEOBJECT(m_pReflection);
	XSAFE_RELEASE(m_pBuffer);	
	return true;
}

xD10VertexStream::xD10VertexStream(xD3D10RenderApi* d10API , xD10InputAssembler* D10Assembler)
:IVertexStream(d10API),m_pAssembler(D10Assembler) , m_pD10API(d10API)
{
	 m_pAssembler->AddRef();
	 xInputLayoutDesc&  layoutDesc =      D10Assembler->layoutDesc();
	 m_VertexBuffers.resize(layoutDesc.nBuffer() );
}

xD10VertexStream::~xD10VertexStream()
{
    for(size_t i = 0 ; i < m_VertexBuffers.size() ; i ++)
	{
		XSAFE_RELEASEOBJECT(m_VertexBuffers[i].m_pBuffer);
	}
	XSAFE_RELEASEOBJECT( m_pAssembler );
}

IInputAssembler* xD10VertexStream::getAssembler()
{
     return m_pAssembler;
}

IInputBuffer*    xD10VertexStream::getInputBuffer(size_t nSlot)
{
     if(nSlot < m_VertexBuffers.size() )
		 return m_VertexBuffers[nSlot].m_pBuffer;
	 return NULL;
}

size_t            xD10VertexStream::nInputSlot()
{
     return m_VertexBuffers.size();
}

//Create Vertex Buffers with inite Data;

bool              xD10VertexStream::createBuffers(size_t nVertex , void** pInitDatas ,const  xInputBufferDesc** pDescs)
{
     for(size_t i = 0 ; i < m_VertexBuffers.size() ; i ++ )
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

IInputBuffer*    xD10VertexStream::createInputBuffer(size_t nSlot , size_t nVertex , void* pData , const xInputBufferDesc* pDesc)
{
	if(nSlot >= m_VertexBuffers.size() )
		return NULL;

	const xInputBufferDesc* bufferDesc = pDesc ? pDesc : &m_pAssembler->layoutDesc().inputSlotDesc(nSlot); 
	size_t                nBufferStride = m_pAssembler->layoutDesc().nStrideOfBuffer(nSlot);
	if(nBufferStride <= 0 ) return NULL;

	IInputBuffer* vBuffer =  m_pD10API->createInputBuffer(nVertex , nBufferStride , bufferDesc , pData);
	m_VertexBuffers[nSlot].m_pBuffer = dynamic_cast<xD10InputBuffer*>(vBuffer);
	return vBuffer;
}

bool              xD10VertexStream::setInputBuffer(size_t nSlot , IInputBuffer* pIB)
{
	if(nSlot >= m_VertexBuffers.size() )
		return false;
	
	m_VertexBuffers[nSlot].m_pBuffer = dynamic_cast<xD10InputBuffer*>(pIB);
	pIB->AddRef();
	return m_VertexBuffers[nSlot].m_pBuffer != NULL;
}

//描述一个Buffer的Reflection
//Shander常量的Reflection
xD10ShaderConstReflection::xD10ShaderConstReflection()
{
	m_pBinder = NULL;
	m_pBufferReflection = NULL;
}

xD10ShaderConstReflection::~xD10ShaderConstReflection()
{
	XSAFE_RELEASEOBJECT(m_pBinder);
}

bool xD10ShaderConstReflection::setData(void* _data , int dataLen)
{
	return m_pBufferReflection->setData(m_bufOffset , _data , dataLen);
}

bool xD10ShaderConstReflection::setData(int _offset , void* _data , int dataLen)
{
	return m_pBufferReflection->setData(m_bufOffset + _offset , _data , dataLen);
}

//Buffer的Reflection
xD10InputBufReflection::xD10InputBufReflection(IInputBuffer* pBuf , bool bSysBuffer , int bufLen)
{
	m_pInputBuffer = pBuf;
	m_pSysMem = new int8[bufLen];
	m_bDirty  = false;
	m_BufLen  = bufLen;
}

xD10InputBufReflection::~xD10InputBufReflection()
{
	for(size_t i = 0 ; i != m_vConstant.size() ; i ++)
	{
		IShaderConstantReflection* pDesc = m_vConstant[i] ;
		XSAFE_DELETE(pDesc);
	}
    m_vConstant.clear();
	XSAFE_DELETE_ARRAY(m_pSysMem);
}

size_t xD10InputBufReflection::lenght()
{
	return m_BufLen;
}

size_t xD10InputBufReflection::nConstant()
{
	return m_vConstant.size();
}

IShaderConstantReflection*   xD10InputBufReflection::constantDesc(size_t idx)
{
	if(idx >= nConstant())
		return NULL;
	return m_vConstant[idx];        
}


xD10ShaderConstReflection*  xD10InputBufReflection::addConstant()
{
	xD10ShaderConstReflection* pDesc = new xD10ShaderConstReflection;
	m_vConstant.push_back(pDesc);
	return pDesc;    
}
void xD10InputBufReflection::addConstant(xD10ShaderConstReflection* cd)
{
	m_vConstant.push_back(cd);
}

bool xD10InputBufReflection::setData(int _off , void* _data , int dataLen)
{
	memcpy( m_pSysMem + _off , _data , dataLen);
	m_bDirty = true;
	return true;
}

bool xD10InputBufReflection::flushToBuffer()
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
