#include "xGL2InputBuffer.h"

BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xGL2VertexStream , IVertexStream);
IMPL_BASE_OBJECT_CLASSID(xGL2InputBuffer  , IInputBuffer);


xGL2InputBuffer::xGL2InputBuffer(IRenderApi* pAPI  ) :m_pGL2Api(pAPI) , IInputBuffer(pAPI)
{

}

xGL2InputBuffer::~xGL2InputBuffer()
{
}


bool xGL2HWVertexBuffer::__destory()
{
    glDeleteBuffers(1,&m_BufferObj);
    return true;
}


//===========Hardware vertex buffer=============================
xGL2HWVertexBuffer::xGL2HWVertexBuffer(IRenderApi* pAPI  )
:xGL2InputBuffer(pAPI)
{
    m_RefCount = 1;
    m_iBufLen = 0;
    m_nVertex = 0;
    m_pReflection = NULL;
    m_BufferObj = 0;
    m_nBytePerVertex = 0; 
}

xGL2HWVertexBuffer::~xGL2HWVertexBuffer()
{
    __destory();
}

void* xGL2HWVertexBuffer::handle()
{
    return (void*)m_BufferObj;
}



GLenum xGL2HWVertexBuffer::__getVBOType(xInputBufferDesc* pDesc)
{
    if(pDesc->m_bindtype == BIND_AS_INDEX_BUFFER)
        return GL_ELEMENT_ARRAY_BUFFER;
    if(pDesc->m_bindtype == BIND_AS_VERTEX_BUFFER)
        return GL_ARRAY_BUFFER;
    if(pDesc->m_bindtype == BIND_AS_STREAM_OUTPUT)
    {
        XEVOL_LOG(eXL_DEBUG_HIGH , "Use OpenGL2.0's Transform feedback\n");
        return GL_TRANSFORM_FEEDBACK_BUFFER_EXT;
    }
    if(pDesc->m_bindtype == BIND_AS_CONSTANT_BUFFER)
    {
        XEVOL_LOG(eXL_DEBUG_HIGH , "Use OpenGL2.0's Bindable uniform \n");
        return GL_UNIFORM_BUFFER_EXT;
    }
    return GL_NONE;
}

GLenum xGL2HWVertexBuffer::__getVBOUsage(xInputBufferDesc* pDesc)
{
    switch(pDesc->m_usage)
    {
    case RESOURCE_USAGE_DEFAULT:
        return GL_STATIC_DRAW ;

    case RESOURCE_USAGE_IMMUTABLE:
        return GL_STATIC_DRAW ;

    case RESOURCE_USAGE_DYNAMIC:
        return GL_DYNAMIC_DRAW;

    case RESOURCE_USAGE_STAGING:
        break;
    }
    return GL_STREAM_DRAW_ARB;
}

bool xGL2HWVertexBuffer::create(size_t nVertex , size_t bytePerVertex ,const xInputBufferDesc* pDesc , void* pInitData  )
{
    m_nBytePerVertex = bytePerVertex;
    m_nVertex  = nVertex;
    GLenum bufferType = __getVBOType(&m_Desc);
    if(bufferType == GL_NONE)
        return false;

    __destory();
    m_Desc = *pDesc;
    m_iBufLen = m_nBytePerVertex * m_nVertex;
    glGenBuffers(1,&m_BufferObj);
    GLenum BUFFER_LOC = __getVBOUsage(&m_Desc);

    glNamedBufferDataEXT(bufferType, m_iBufLen , pInitData , BUFFER_LOC);// _DRAW_ARB);//GL_STREAM_DRAW_ARB);
    return m_BufferObj != 0;
}

void* xGL2HWVertexBuffer::buffer_data_addr(int offset)
{
     return (void*)(offset);
}

bool xGL2HWVertexBuffer::create(size_t iBufLen , const xInputBufferDesc* pDesc , void* pInitData  )
{
     return create(iBufLen , 1 , pDesc , pInitData);
}

bool xGL2HWVertexBuffer::lock(eLockPolicy lockPolicy , void** pData)
{
    void * pLockeData = NULL;
    GLenum bufferType = __getVBOType(&m_Desc);
    switch( lockPolicy)
    {
    case eLock_WriteDiscard:
    case eLock_Write:
    case eLock_WriteNoOverwrite:
        pLockeData = glMapNamedBufferEXT(m_BufferObj ,GL_WRITE_ONLY);
        break;
    case eLock_Read:
        pLockeData = glMapNamedBufferEXT(m_BufferObj ,GL_READ_ONLY);
        break;
    case eLock_ReadWrite:
        pLockeData = glMapNamedBufferEXT(m_BufferObj ,GL_READ_WRITE);
        break;
    }
    *pData = pLockeData;
    return pLockeData != NULL;
}

bool xGL2HWVertexBuffer::unlock()
{
    glUnmapNamedBufferEXT(m_BufferObj);
    return m_BufferObj != 0;
}

bool xGL2HWVertexBuffer::update(eLockPolicy lockPolicy , void* pData , size_t dataLen)
{
    if(dataLen > m_iBufLen)
    {
        XEVOL_LOG(eXL_DEBUG_HIGH,"Out of memory int xGL2HWVertexBuffer::update() \n " );
        return false;
    }

    if( dataLen == 0) dataLen = m_iBufLen;

    GLenum bufferType = __getVBOType(&m_Desc);
    bool bUpLoad = true;
    int  _offset = 0;
    if(bUpLoad)
    {
        glNamedBufferSubDataEXT   (m_BufferObj , _offset , (GLsizeiptr)dataLen , pData);
        return true;
    }
    else
    {
        glGetNamedBufferSubDataEXT(m_BufferObj , _offset , (GLsizeiptr)dataLen , pData);
        return true;
    }
}

IInputBufferReflection* xGL2HWVertexBuffer::reflection()
{
    return m_pReflection;
}

IInputBufferReflection* xGL2HWVertexBuffer::createReflection(bool bSysBuffer)
{
    if(m_pReflection == NULL)
    {
        m_pReflection = new xGL2InputBufReflection(this , bSysBuffer , (int)m_iBufLen);
    }
    return m_pReflection;
}

//=================System memory vertex array=============================
xGL2VertexBuffer::xGL2VertexBuffer(IRenderApi* pAPI  )
:xGL2InputBuffer(pAPI)
{
    m_RefCount = 1;
    m_iBufLen = 0;
    m_nVertex = 0;
    m_pReflection = NULL;
    m_Buffer = NULL;
    m_nBytePerVertex = 0; 
}

xGL2VertexBuffer::~xGL2VertexBuffer()
{
    if(m_Buffer)
    {
        ::free(m_Buffer);
        m_Buffer = NULL;
    }
}

void* xGL2VertexBuffer::handle()
{
    return (void*)m_Buffer;
}


bool xGL2VertexBuffer::create(size_t nVertex , size_t bytePerVertex ,const xInputBufferDesc* pDesc , void* pInitData  )
{
    m_nBytePerVertex = bytePerVertex;
    m_nVertex  = nVertex;
    m_Buffer = (char*)malloc(m_nVertex * m_nBytePerVertex);
    return m_Buffer != NULL;
}


bool xGL2VertexBuffer::create(size_t iBufLen , const xInputBufferDesc* pDesc , void* pInitData  )
{
    return create(iBufLen , 1 , pDesc , pInitData);
}

bool xGL2VertexBuffer::lock(eLockPolicy lockPolicy , void** pData)
{
    *pData = m_Buffer;
    return m_Buffer != NULL;
}

bool xGL2VertexBuffer::unlock()
{
    return m_Buffer != NULL;
}

void* xGL2VertexBuffer::buffer_data_addr(int offset)
{
    return (void*)( (char*)m_Buffer + offset);
}

bool xGL2VertexBuffer::update(eLockPolicy lockPolicy , void* pData , size_t dataLen)
{
    if(dataLen > m_iBufLen)
    {
        XEVOL_LOG(eXL_ERROR_FALT,"Out of memory int xGL2VertexBuffer::update() \n " );
        return false;
    }

    bool bUpLoad = true;
    if( dataLen == 0) dataLen = m_iBufLen;
    int _offset = 0;
    if(bUpLoad)
    {
        memcpy(m_Buffer , ((char*)pData + _offset) , dataLen);
    }
    else
    {
        memcpy(((char*)pData + _offset) , m_Buffer ,  dataLen);
    }
    return true;
}

IInputBufferReflection* xGL2VertexBuffer::reflection()
{
    return m_pReflection;
}

IInputBufferReflection* xGL2VertexBuffer::createReflection(bool bSysBuffer)
{
    if(m_pReflection == NULL)
    {
        m_pReflection = new xGL2InputBufReflection(this , bSysBuffer , (int)m_iBufLen);
    }
    return m_pReflection;
}

//===========Vertex stream==================================
xGL2VertexStream::xGL2VertexStream(IRenderApi* oGL2API , IInputAssembler* D10Assembler)
:IVertexStream(oGL2API),m_pAssembler(D10Assembler) , m_pGL2Api(oGL2API)
{
     m_RefCount = 1;
	 m_pAssembler->AddRef();
	 xInputLayoutDesc&  layoutDesc =      D10Assembler->layoutDesc();
	 m_VertexBuffers.resize(layoutDesc.nBuffer() );
}

xGL2VertexStream::~xGL2VertexStream()
{
    for(size_t i = 0 ; i < m_VertexBuffers.size() ; i ++)
	{
		XSAFE_RELEASEOBJECT(m_VertexBuffers[i].m_pBuffer);
	}
	XSAFE_RELEASEOBJECT( m_pAssembler );
}

IInputAssembler* xGL2VertexStream::getAssembler()
{
     return m_pAssembler;
}

IInputBuffer*    xGL2VertexStream::getInputBuffer(size_t nSlot)
{
     if(nSlot < m_VertexBuffers.size() )
		 return m_VertexBuffers[nSlot].m_pBuffer;
	 return NULL;
}

size_t            xGL2VertexStream::nInputSlot()
{
     return m_VertexBuffers.size();
}



//Create Vertex Buffers with inite Data;
bool              xGL2VertexStream::createBuffers(size_t nVertex , void** pInitDatas ,const  xInputBufferDesc** pDescs)
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

IInputBuffer*    xGL2VertexStream::createInputBuffer(size_t nSlot , size_t nVertex , void* pData , const xInputBufferDesc* pDesc)
{
	if(nSlot >= m_VertexBuffers.size() )
		return NULL;

	const xInputBufferDesc* bufferDesc = pDesc ? pDesc : &m_pAssembler->layoutDesc().inputSlotDesc(nSlot); 
	size_t                nBufferStride = m_pAssembler->layoutDesc().nStrideOfBuffer(nSlot);
	if(nBufferStride <= 0 ) return NULL;

	IInputBuffer* vBuffer =  m_pGL2Api->createInputBuffer(nVertex , nBufferStride , bufferDesc , pData);
	m_VertexBuffers[nSlot].m_pBuffer = dynamic_cast<xGL2InputBuffer*>(vBuffer);
	return vBuffer;
}

bool              xGL2VertexStream::setInputBuffer(size_t nSlot , IInputBuffer* pIB)
{
	if(nSlot >= m_VertexBuffers.size() )
		return false;
	
	m_VertexBuffers[nSlot].m_pBuffer = dynamic_cast<xGL2InputBuffer*>(pIB);
	pIB->AddRef();
	return m_VertexBuffers[nSlot].m_pBuffer != NULL;
}

//描述一个Buffer的Reflection
//Shander常量的Reflection
xGL2ShaderConstReflection::xGL2ShaderConstReflection()
{
	m_pBinder = NULL;
	m_pBufferReflection = NULL;
}

xGL2ShaderConstReflection::~xGL2ShaderConstReflection()
{
	XSAFE_RELEASEOBJECT(m_pBinder);
}

bool xGL2ShaderConstReflection::setData(void* _data , int dataLen)
{
	return m_pBufferReflection->setData(m_bufOffset , _data , dataLen);
}

bool xGL2ShaderConstReflection::setData(int _offset , void* _data , int dataLen)
{
	return m_pBufferReflection->setData(m_bufOffset + _offset , _data , dataLen);
}

//Buffer的Reflection
xGL2InputBufReflection::xGL2InputBufReflection(IInputBuffer* pBuf , bool bSysBuffer , int bufLen)
{
	m_pInputBuffer = pBuf;
	m_pSysMem = new int8[bufLen];
	m_bDirty  = false;
	m_BufLen  = bufLen;
}
void* xGL2InputBufReflection::systemBuffer()
{
	return m_pSysMem;
}

xGL2InputBufReflection::~xGL2InputBufReflection()
{
	for(size_t i = 0 ; i != m_vConstant.size() ; i ++)
	{
		IShaderConstantReflection* pDesc = m_vConstant[i] ;
		XSAFE_DELETE(pDesc);
	}
    m_vConstant.clear();
	XSAFE_DELETE_ARRAY(m_pSysMem);
}

size_t xGL2InputBufReflection::lenght()
{
	return m_BufLen;
}

size_t xGL2InputBufReflection::nConstant()
{
	return m_vConstant.size();
}

IShaderConstantReflection*   xGL2InputBufReflection::constantDesc(size_t idx)
{
	if(idx >= nConstant())
		return NULL;
	return m_vConstant[idx];        
}


xGL2ShaderConstReflection*  xGL2InputBufReflection::addConstant()
{
	xGL2ShaderConstReflection* pDesc = new xGL2ShaderConstReflection;
	m_vConstant.push_back(pDesc);
	return pDesc;    
}
void xGL2InputBufReflection::addConstant(xGL2ShaderConstReflection* cd)
{
	m_vConstant.push_back(cd);
}

bool xGL2InputBufReflection::setData(int _off , void* _data , int dataLen)
{
	memcpy( m_pSysMem + _off , _data , dataLen);
	m_bDirty = true;
	return true;
}

bool xGL2InputBufReflection::flushToBuffer()
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
