#include "stdafx.h"
#include "xD3D9ConstantBuffer.h"
#include "xDirect3D9API.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD3D9ConstantBuffer, IInputBuffer);
IMPL_BASE_OBJECT_CLASSID(xD3D9ConstBufferReflection , IInputBufferReflection);

xD3D9ConstantBuffer::xD3D9ConstantBuffer(xD3D9RenderApi* pAPI  ) :m_pD3D9RenderAPI(pAPI) , IInputBuffer(pAPI)
{
    m_RefCount = 1;
    m_iBufLen = 0;
    m_nBytePerVertex = 0;
    m_nElement = 0;
    m_pBuffer = NULL;
    m_pReflection = NULL;
}

xD3D9ConstantBuffer::~xD3D9ConstantBuffer()
{
    _destory();
}


bool xD3D9ConstantBuffer::create(size_t nElement , size_t bytePerVertex , const xInputBufferDesc* pDesc , void* pData  )
{
    _destory();
    m_nElement = nElement;
    m_iBufLen = m_nElement * bytePerVertex;
    m_nBytePerVertex = bytePerVertex;

    m_pBuffer = new char[m_iBufLen];
    if(m_pBuffer == NULL)
        return false;
    if(pData) 
    {
        update(eLock_WriteDiscard , pData , m_iBufLen);
    }

    m_pReflection = new xD3D9ConstBufferReflection(this , true , m_iBufLen);
    return true;

}

bool xD3D9ConstantBuffer::create(size_t iBufLen , const xInputBufferDesc* pDesc , void* pData  )
{
    return create(iBufLen , 1 , pDesc , pData);
}
IInputBufferReflection* xD3D9ConstantBuffer::reflection()
{ 
    return m_pReflection; 
}

IInputBufferReflection* xD3D9ConstantBuffer::createReflection(bool bSysBuffe)
{ 
    return m_pReflection;
}

bool xD3D9ConstantBuffer::lock(eLockPolicy lockPolicy , void** pData)
{
    if(m_pBuffer == NULL)
        return false;
    *pData = (void*)m_pBuffer;
    return true;
}

bool xD3D9ConstantBuffer::unlock()
{
    if(m_pBuffer == NULL)
        return false;
    if(m_pReflection) m_pReflection->setDirty(true);
    return true ;
}

bool xD3D9ConstantBuffer::update(eLockPolicy lockPolicy , void* pSrcData , size_t dataLen)
{
    if(m_pBuffer == NULL)
        return false;
    memcpy(m_pBuffer , pSrcData , dataLen);
    if(m_pReflection) m_pReflection->setDirty(true);
    return true;
}

bool xD3D9ConstantBuffer::_destory()
{
    XSAFE_DELETE(m_pReflection);
    XSAFE_DELETE_ARRAY(m_pBuffer);	
    return true;
}

//描述一个Buffer的Reflection
//Shander常量的Reflection
xD3D9ShaderConstReflection::xD3D9ShaderConstReflection(const wchar_t* _name)
{
    m_Name = _name;
    m_pBinder = NULL;
    m_pBufferReflection = NULL;
    m_hConstHandle = NULL;
    m_pConstTable = NULL;
    m_pD3D9Api = NULL;
}

xD3D9ShaderConstReflection::~xD3D9ShaderConstReflection()
{
    XSAFE_RELEASEOBJECT(m_pBinder);
}

bool xD3D9ShaderConstReflection::setData(void* _data , int dataLen)
{
    if(m_pBufferReflection) return m_pBufferReflection->setData(m_bufOffset , _data , dataLen);
    if(m_pConstTable) m_pConstTable->SetValue(m_pD3D9Api->getDevice() , m_hConstHandle , _data , dataLen );

    return true;
}

bool xD3D9ShaderConstReflection::setData(int _offset , void* _data , int dataLen)
{
    if(m_pBufferReflection) return m_pBufferReflection->setData(m_bufOffset + _offset , _data , dataLen);

    assert(0);
    return true;
}

//Buffer的Reflection
xD3D9ConstBufferReflection::xD3D9ConstBufferReflection(IInputBuffer* pBuf , bool bSysBuffer , int bufLen)
{
    xD3D9ConstantBuffer * pConstBuffer = dynamic_cast<xD3D9ConstantBuffer *>(pBuf);
    if(pConstBuffer == NULL)
         return ;
    m_vConstant.reserve(16);
    m_pInputBuffer = pBuf;
    m_pSysMem = (char*)pBuf->handle();
    m_bDirty  = false;
    m_BufLen  = bufLen;
}
void* xD3D9ConstBufferReflection::systemBuffer()
{
    return m_pSysMem;
}

xD3D9ConstBufferReflection::~xD3D9ConstBufferReflection()
{
    size_t _nCst = m_vConstant.size() ;
    for(size_t i = 0 ; i != _nCst ; i ++)
    {
        IShaderConstantReflection* pDesc = m_vConstant[i] ;
        XSAFE_DELETE(pDesc);
    }
    m_vConstant.clear();
}

size_t xD3D9ConstBufferReflection::lenght()
{
    return m_BufLen;
}

size_t xD3D9ConstBufferReflection::nConstant()
{
    return m_vConstant.size();
}

IShaderConstantReflection*   xD3D9ConstBufferReflection::constantDesc(size_t idx)
{
    if(idx >= nConstant())
        return NULL;
    return m_vConstant[idx];        
}


xD3D9ShaderConstReflection*  xD3D9ConstBufferReflection::addConstant(const wchar_t* _name)
{
    xD3D9ShaderConstReflection* pDesc = new xD3D9ShaderConstReflection(_name);
    m_vConstant.push_back(pDesc);
    return pDesc;    
}


bool xD3D9ConstBufferReflection::setData(int _off , void* _data , int dataLen)
{
    memcpy( m_pSysMem + _off , _data , dataLen);
    m_bDirty = true;
    return true;
}

bool xD3D9ConstBufferReflection::flushToBuffer()
{
    m_bDirty = false;
    return true;
}


END_NAMESPACE_XEVOL3D

