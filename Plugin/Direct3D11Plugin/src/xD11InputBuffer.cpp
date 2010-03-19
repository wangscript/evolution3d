#include "stdafx.h"
#include "xD3DInputBuffer.h"
#include "xD11InputBuffer.h"
#include "xDirect3D11API.h"
#include "xD11ConstLexer.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD11InputBuffer  , IInputBuffer);

static void conv(const xInputBufferDesc& from , D3D11_BUFFER_DESC& to)
{
	to.BindFlags = 0;
	
	if( from.m_bindtype&BIND_AS_VERTEX_BUFFER )
	{
		to.BindFlags |= D3D11_BIND_VERTEX_BUFFER;
	}

	if( from.m_bindtype&BIND_AS_INDEX_BUFFER )
	{
		to.BindFlags |= D3D11_BIND_INDEX_BUFFER;
	}

	if( from.m_bindtype&BIND_AS_CONSTANT_BUFFER )
	{
		to.BindFlags |= D3D11_BIND_CONSTANT_BUFFER ;
	}

	if( from.m_bindtype&BIND_AS_SHADER_RESOURCE )
	{
		to.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	}

	if( from.m_bindtype&BIND_AS_STREAM_OUTPUT )
	{
		to.BindFlags |= D3D11_BIND_STREAM_OUTPUT;
	}

	if( from.m_bindtype&BIND_AS_RENDER_TARGET )
	{
		to.BindFlags |= D3D11_BIND_RENDER_TARGET;
	}
	

	switch(from.m_accessFlage)
	{
	case RESOURCE_ACCESS_READ:
		to.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		break;
	case RESOURCE_ACCESS_NONE:
		to.CPUAccessFlags = 0;
		break;
	case RESOURCE_ACCESS_WRITE:
		to.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case RESOURCE_ACCESS_READWRITE:
		to.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		break;
	}

	switch(from.m_usage)
	{
	case RESOURCE_USAGE_DEFAULT:
		to.Usage = D3D11_USAGE_DEFAULT;
		break;
	case RESOURCE_USAGE_IMMUTABLE:
		to.Usage = D3D11_USAGE_IMMUTABLE;
		break;
	case RESOURCE_USAGE_DYNAMIC:
		to.Usage = D3D11_USAGE_DYNAMIC;
		break;
	case RESOURCE_USAGE_STAGING:
		to.Usage = D3D11_USAGE_STAGING;
		break;
	}
	to.MiscFlags = 0;
	if(from.m_bShared ) to.MiscFlags =  D3D11_RESOURCE_MISC_SHARED;
}



xD11InputBuffer::xD11InputBuffer(xD3D11RenderApi* pAPI  ) :m_pD11RenderAPI(pAPI) , IInputBuffer(pAPI)
{
    m_RefCount = 1;
	m_iBufLen = 0;
	m_nBytePerVertex = 0;
	m_nElement = 0;
    m_pBuffer = NULL;
	m_pRSV = NULL;
	m_pReflection = NULL;
}

xD11InputBuffer::~xD11InputBuffer()
{
	_destory();
}

IInputBufferReflection* xD11InputBuffer::reflection()
{
    return m_pReflection;
}

IInputBufferReflection* xD11InputBuffer::createReflection(bool bSysBuffer)
{
	if(m_pReflection == NULL)
	{
		m_pReflection = new xD3DInputBufReflection(this , bSysBuffer , (int)m_iBufLen);
	}
	return m_pReflection;
}

ID3D11ShaderResourceView*  xD11InputBuffer::toShaderResourceView(ePIXEL_FORMAT fmt)
{
    if(m_pRSV) return m_pRSV;

    xD11GIFormatInfo* info = xD11ConstLexer::singleton()->GetPixelFormat(fmt);
    int elementSize   = info->m_compont * info->m_bytePerComponent;
	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	desc.Format = info->m_dxfmt;
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	desc.Buffer.ElementOffset = 0;
	desc.Buffer.ElementWidth  = (UINT)( m_iBufLen/ elementSize );
    /*
    Members
    ElementOffset 
       The offset of the first element in the view to access, relative to element 0. 
    ElementWidth 
       The total number of elements in the view. 
    */
	m_pD11RenderAPI->d11Device()->CreateShaderResourceView(m_pBuffer, &desc , &m_pRSV);
	return m_pRSV;
}

bool xD11InputBuffer::create(size_t nElement , size_t bytePerVertex , const xInputBufferDesc* pDesc , void* pData  )
{
	_destory();
	m_nElement = nElement;
	m_iBufLen = m_nElement * bytePerVertex;
	m_nBytePerVertex = bytePerVertex;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	D3D11_SUBRESOURCE_DATA* pInitData = pData ? &InitData:NULL;   

	ID3D11Device* pD10Device = m_pD11RenderAPI->d11Device();

	m_buffDesc.ByteWidth = (UINT)m_iBufLen ; 
	m_buffDesc.StructureByteStride = (UINT)bytePerVertex;
	conv( *pDesc , m_buffDesc);
	pD10Device->CreateBuffer(&m_buffDesc , pInitData , &m_pBuffer);
	return m_pBuffer != NULL;

}

bool xD11InputBuffer::create(size_t iBufLen , const xInputBufferDesc* pDesc , void* pData  )
{
    return create(iBufLen , 1 , pDesc , pData);
}


bool xD11InputBuffer::lock(eLockPolicy lockPolicy , void** pData)
{
	 if(m_pBuffer == NULL)
		 return false;

	 D3D11_MAPPED_SUBRESOURCE MapSubRes;
	 UINT subResource = 0;
     HRESULT hr = m_pD11RenderAPI->d11DeviceContext()->Map( m_pBuffer ,subResource , (D3D11_MAP)lockPolicy , 0 , &MapSubRes);
	 if(SUCCEEDED(hr))
	 {
		 *pData = MapSubRes.pData;
		 return true;
	 }
	 return false;
}

bool xD11InputBuffer::unlock()
{
	if(m_pBuffer == NULL)
		return false;
	UINT subResource = 0;
	m_pD11RenderAPI->d11DeviceContext()->Unmap(m_pBuffer , subResource);
    if(m_pReflection) m_pReflection->setDirty(true);
	return true ;
}

bool xD11InputBuffer::update(eLockPolicy lockPolicy , void* pSrcData , size_t dataLen)
{
	if(m_pBuffer == NULL)
		return false;

    HRESULT hr = E_FAIL;
	//如果是UsageDefault。则必须用UpdateSubResource啦。
	if(m_buffDesc.Usage == D3D11_USAGE_DEFAULT )
	{
		m_pD11RenderAPI->d11DeviceContext()->UpdateSubresource(m_pBuffer , 0 , NULL , pSrcData , (UINT)m_iBufLen , (UINT)m_iBufLen);
        if(m_pReflection) m_pReflection->setDirty(true);
		return true;
	}

	D3D11_MAPPED_SUBRESOURCE MapSubRes;
	UINT subResource = 0;
	hr = m_pD11RenderAPI->d11DeviceContext()->Map( m_pBuffer ,subResource , (D3D11_MAP)lockPolicy , 0 , &MapSubRes);
	if( SUCCEEDED(hr) )
	{
		memcpy(MapSubRes.pData , pSrcData , dataLen);
		m_pD11RenderAPI->d11DeviceContext()->Unmap(m_pBuffer , subResource);
        if(m_pReflection) m_pReflection->setDirty(true);
		return true;
	}
	return false;
}

bool xD11InputBuffer::_destory()
{
	XSAFE_RELEASE(m_pRSV);
	XSAFE_RELEASEOBJECT(m_pReflection);
	XSAFE_RELEASE(m_pBuffer);	
	return true;
}

END_NAMESPACE_XEVOL3D
