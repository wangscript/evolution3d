#include "stdafx.h"
#include "xDirect3D10API.h"
#include "xD10ConstLexer.h"
#include "xD10InputBuffer.h"
BEGIN_NAMESPACE_XEVOL3D
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
    m_RefCount = 1;
	m_iBufLen = 0;
	m_nBytePerVertex = 0;
	m_nElement = 0;
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
	{
		m_pReflection = new xD3DInputBufReflection(this , bSysBuffer , (int)m_iBufLen);
	}
	return m_pReflection;
}

ID3D10ShaderResourceView*  xD10InputBuffer::toShaderResourceView(ePIXEL_FORMAT fmt)
{
    if(m_pRSV) return m_pRSV;

    xD10GIFormatInfo* info = xD10ConstLexer::singleton()->GetPixelFormat(fmt);
    int elementSize   = info->m_compont * info->m_bytePerComponent;
	D3D10_SHADER_RESOURCE_VIEW_DESC desc;
	desc.Format = info->m_dxfmt;
	desc.ViewDimension = D3D10_SRV_DIMENSION_BUFFER;
	desc.Buffer.ElementOffset = 0;
	desc.Buffer.ElementWidth  = (UINT)( m_iBufLen/ elementSize );
    /*
    Members
    ElementOffset 
       The offset of the first element in the view to access, relative to element 0. 
    ElementWidth 
       The total number of elements in the view. 
    */
	m_pD10API->d10Device()->CreateShaderResourceView(m_pBuffer, &desc , &m_pRSV);
	return m_pRSV;
}

bool xD10InputBuffer::create(size_t nElement , size_t bytePerVertex , const xInputBufferDesc* pDesc , void* pData  )
{
	_destory();
    m_iBufLen = nElement * bytePerVertex;
	bool ret= create(m_iBufLen , pDesc , pData);
	m_nElement = nElement;
	m_nBytePerVertex = bytePerVertex;
	return ret;
}

bool xD10InputBuffer::create(size_t iBufLen , const xInputBufferDesc* pDesc , void* pData  )
{
	_destory();
	m_iBufLen = iBufLen ;
	m_nElement = 1;
	m_nBytePerVertex = iBufLen;

	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	D3D10_SUBRESOURCE_DATA* pInitData = pData ? &InitData:NULL;   

	ID3D10Device* pD10Device = m_pD10API->d10Device();

	
	m_buffDesc.ByteWidth = (UINT)m_iBufLen ; 

	conv( *pDesc , m_buffDesc);
	pD10Device->CreateBuffer(&m_buffDesc , pInitData , &m_pBuffer);
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
    if(m_pReflection) m_pReflection->setDirty(true);
	return true;
}

bool xD10InputBuffer::update(eLockPolicy lockPolicy , void* pData , size_t dataLen)
{
	if(m_pBuffer == NULL)
		return false;
    HRESULT hr = E_FAIL;
    //如果是UsageDefault。则必须用UpdateSubResource啦。
    if(m_buffDesc.Usage == D3D10_USAGE_DEFAULT )
    {
        m_pD10API->d10Device()->UpdateSubresource(m_pBuffer , 0 , NULL , pData , (UINT)m_iBufLen , (UINT)m_iBufLen);
        if(m_pReflection) m_pReflection->setDirty(true);
        return true;
    }


	void* pDstData = NULL;
	hr = m_pBuffer->Map( (D3D10_MAP)lockPolicy , 0/*D3D10_MAP_FLAG_DO_NOT_WAIT*/ , &pDstData);
	if(SUCCEEDED(hr))
	{
		memcpy(pDstData , pData , dataLen);
		m_pBuffer->Unmap();
        if(m_pReflection) m_pReflection->setDirty(true);
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


END_NAMESPACE_XEVOL3D
