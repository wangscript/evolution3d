#include "stdafx.h"
#include "xD3DInputBuffer.h"
#include "xD3D9InputBuffer.h"
#include "xDIRECT3D9API.h"
#include "xD3D9ConstLexer.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD3D9VertexBuffer  , IInputBuffer);
IMPL_BASE_OBJECT_CLASSID(xD3D9IndexBuffer   , IInputBuffer);

xD3D9VertexBuffer::xD3D9VertexBuffer(xD3D9RenderApi* pAPI  ) :m_pD3D9RenderAPI(pAPI) , IInputBuffer(pAPI)
{
    m_RefCount = 1;
	m_iBufLen = 0;
	m_nBytePerVertex = 0;
	m_nElement = 0;
    m_pBuffer = NULL;
    m_pD3D9RenderAPI = pAPI;
    REGISTER_DEVICE_LOST_RESOURCE(m_pD3D9RenderAPI);
}

xD3D9VertexBuffer::~xD3D9VertexBuffer()
{
    REMOVE_DEVICE_LOST_RESOURCE(m_pD3D9RenderAPI);
	_destory();
}

void xD3D9VertexBuffer::onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action)
{
    if( !bNewDevice && m_Pool != D3DPOOL_DEFAULT)
        return ;

    if(_Action == ID3D9DeviceLostListener::eReset_Begin)
        XSAFE_RELEASE(m_pBuffer);

    if(_Action == ID3D9DeviceLostListener::eReset_Finish)
    {
        //新创建的纹理是不会有内容的
        create(m_nElement , m_nBytePerVertex , NULL, NULL);
    }
}

bool xD3D9VertexBuffer::create(size_t nElement , size_t bytePerVertex , const xInputBufferDesc* pDesc , void* pData  )
{
	_destory();
	m_nElement = nElement;
	m_iBufLen = m_nElement * bytePerVertex;
	m_nBytePerVertex = bytePerVertex;

	IDirect3DDevice9* pDevice = m_pD3D9RenderAPI->getDevice();
    
    if(pDesc)
    {
        m_Usage = 0;//xD3D9ConstLexer::singleton()->GetD3DUsage(pDesc->m_accessFlage , pDesc->m_bindtype , pDesc->m_usage);
        m_Pool  = xD3D9ConstLexer::singleton()->GetD3DPool(pDesc->m_usage);
        RESET_D3DPOOL(m_Pool);
        if(m_Pool == D3DPOOL_DEFAULT ) m_Usage |= D3DUSAGE_WRITEONLY;
    }


    pDevice->CreateVertexBuffer( (UINT)m_iBufLen , m_Usage , 0 , m_Pool , &m_pBuffer , NULL );
    if(m_pBuffer == NULL)
        return false;
    if(pData) 
    {
        update(eLock_WriteDiscard , pData , m_iBufLen);
    }
	return true;

}

bool xD3D9VertexBuffer::create(size_t iBufLen , const xInputBufferDesc* pDesc , void* pData  )
{
    return create(iBufLen , 1 , pDesc , pData);
}

DWORD GetLockFlag(eLockPolicy lockPolicy , D3DPOOL _Pool)
{

      if(_Pool == D3DPOOL_DEFAULT) return 0;
      return 0;

      if(lockPolicy == eLock_WriteDiscard)    return D3DLOCK_DISCARD ;
      if(lockPolicy == eLock_Read)             return D3DLOCK_READONLY ;
      if(lockPolicy == eLock_WriteNoOverwrite) return D3DLOCK_NOOVERWRITE;
      return 0; 
}

bool xD3D9VertexBuffer::lock(eLockPolicy lockPolicy , void** pData)
{
	 if(m_pBuffer == NULL)
		 return false;

     IDirect3DDevice9* pDevice = m_pD3D9RenderAPI->getDevice();
     HRESULT hr = m_pBuffer->Lock(0 , (UINT)m_iBufLen , pData , GetLockFlag(lockPolicy , m_Pool) );
	 return SUCCEEDED(hr);
}

bool xD3D9VertexBuffer::unlock()
{
	if(m_pBuffer == NULL)
		return false;
	m_pBuffer->Unlock();
	return true ;
}

bool xD3D9VertexBuffer::update(eLockPolicy lockPolicy , void* pSrcData , size_t dataLen)
{
    if(m_pBuffer == NULL)
        return false;

    void * pData = NULL;
    IDirect3DDevice9* pDevice = m_pD3D9RenderAPI->getDevice();
    HRESULT hr = m_pBuffer->Lock(0 , (UINT)dataLen , &pData , GetLockFlag(lockPolicy , m_Pool) );
    if(FAILED(hr) )
        return false;
    memcpy(pData , pSrcData , dataLen);
    m_pBuffer->Unlock();
    return SUCCEEDED(hr);
}

bool xD3D9VertexBuffer::_destory()
{
	XSAFE_RELEASE(m_pBuffer);	
	return true;
}


//=========
xD3D9IndexBuffer::xD3D9IndexBuffer(xD3D9RenderApi* pAPI  ) :m_pD3D9RenderAPI(pAPI) , IInputBuffer(pAPI)
{
    m_RefCount = 1;
    m_iBufLen = 0;
    m_nBytePerVertex = 0;
    m_nElement = 0;
    m_pBuffer = NULL;
    m_pD3D9RenderAPI = pAPI;
    REGISTER_DEVICE_LOST_RESOURCE(m_pD3D9RenderAPI);
}

xD3D9IndexBuffer::~xD3D9IndexBuffer()
{
    REMOVE_DEVICE_LOST_RESOURCE(m_pD3D9RenderAPI);
    _destory();
}

void xD3D9IndexBuffer::onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action)
{
    if( !bNewDevice && m_Pool != D3DPOOL_DEFAULT)
        return ;

    if(_Action == ID3D9DeviceLostListener::eReset_Begin)
        XSAFE_RELEASE(m_pBuffer);

    if(_Action == ID3D9DeviceLostListener::eReset_Finish)
    {
        //新创建的纹理是不会有内容的
        create(m_nElement , m_nBytePerVertex , NULL, NULL);
    }
}
bool xD3D9IndexBuffer::create(size_t nElement , size_t bytePerVertex , const xInputBufferDesc* pDesc , void* pData  )
{
    if(bytePerVertex != 4 && bytePerVertex != 2)
        return false;

    _destory();
    m_nElement = nElement;
    m_iBufLen = m_nElement * bytePerVertex;
    m_nBytePerVertex = bytePerVertex;

    IDirect3DDevice9* pDevice = m_pD3D9RenderAPI->getDevice();
    if(pDesc)
    {
        m_Usage = 0;//xD3D9ConstLexer::singleton()->GetD3DUsage(pDesc->m_accessFlage , pDesc->m_bindtype , pDesc->m_usage);
        m_Pool  = xD3D9ConstLexer::singleton()->GetD3DPool(pDesc->m_usage);
        RESET_D3DPOOL(m_Pool);
        if(m_Pool == D3DPOOL_DEFAULT ) m_Usage |= D3DUSAGE_WRITEONLY;
    }
    D3DFORMAT fmt = bytePerVertex == 4 ? D3DFMT_INDEX32 : D3DFMT_INDEX16;
    pDevice->CreateIndexBuffer((UINT)m_iBufLen , m_Usage , fmt , m_Pool , &m_pBuffer , NULL );
    if(m_pBuffer == NULL)
        return false;
    if(pData) 
    {
        update(eLock_WriteDiscard , pData , m_iBufLen);
    }
    return true;

}

bool xD3D9IndexBuffer::create(size_t iBufLen , const xInputBufferDesc* pDesc , void* pData  )
{
    return create(iBufLen/4 , 4 , pDesc , pData);
}

bool xD3D9IndexBuffer::lock(eLockPolicy lockPolicy , void** pData)
{
    if(m_pBuffer == NULL)
        return false;

    IDirect3DDevice9* pDevice = m_pD3D9RenderAPI->getDevice();
    HRESULT hr = m_pBuffer->Lock(0 , (UINT)m_iBufLen , pData , GetLockFlag(lockPolicy , m_Pool) );
    return SUCCEEDED(hr);
}

bool xD3D9IndexBuffer::unlock()
{
    if(m_pBuffer == NULL)
        return false;
    m_pBuffer->Unlock();
    return true ;
}

bool xD3D9IndexBuffer::update(eLockPolicy lockPolicy , void* pSrcData , size_t dataLen)
{
    if(m_pBuffer == NULL)
        return false;

    void * pData = NULL;
    IDirect3DDevice9* pDevice = m_pD3D9RenderAPI->getDevice();
    HRESULT hr = m_pBuffer->Lock(0 , (UINT)dataLen , &pData , GetLockFlag(lockPolicy , m_Pool) );
    if(FAILED(hr) )
        return false;
    memcpy(pData , pSrcData , dataLen);
    m_pBuffer->Unlock();
    return SUCCEEDED(hr);
}

bool xD3D9IndexBuffer::_destory()
{
    XSAFE_RELEASE(m_pBuffer);	
    return true;
}

END_NAMESPACE_XEVOL3D
