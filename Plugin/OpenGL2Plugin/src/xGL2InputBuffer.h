#ifndef _XEVOL_OPENGL20_INPUT_BUFFER_H_
#define _XEVOL_OPENGL20_INPUT_BUFFER_H_
#include "../GL2.h"

#include <RenderApi/xRenderApi.h>
#include "RenderAPI/xBaseShader.h"

#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xGL2RenderApi;
class xGL2InputAssembler;
class xGL2InputBufReflection;

class xGL2InputBuffer : public IInputBuffer
{
	IMPL_BASE_OBJECT_INTERFACE(xGL2InputBuffer);
	IRenderApi*               m_pGL2Api;
	xGL2InputBufReflection*   m_pReflection;
	size_t                    m_iBufLen;
public:

	xGL2InputBuffer(IRenderApi* pAPI  );
	~xGL2InputBuffer();

public:
    virtual void*                   handle() = 0;
	virtual bool                    create(size_t nVertex , size_t bytePerVertex ,const xInputBufferDesc* pDesc , void* pInitData  )  = 0;
	virtual bool                    create(size_t iBufLen , const xInputBufferDesc* pDesc , void* pInitData  )  = 0;
	virtual bool                    lock(eLockPolicy lockPolicy , void** pData) = 0;
	virtual bool                    unlock() = 0;
	virtual bool                    update(eLockPolicy lockPolicy , void* pData , size_t dataLen) = 0;
	virtual IInputBufferReflection* reflection() = 0;
	virtual IInputBufferReflection* createReflection(bool bSysBuffe) = 0;
    virtual void*                   buffer_data_addr(int offset) = 0;
};



//=========Hardware vertex buffor object===========================
class xGL2HWVertexBuffer : public xGL2InputBuffer
{
   
public:
    xGL2HWVertexBuffer(IRenderApi* pAPI  );
    ~xGL2HWVertexBuffer();
    virtual void*                   handle();
    virtual bool                    create(size_t nVertex , size_t bytePerVertex ,const xInputBufferDesc* pDesc , void* pInitData  ) ;
    virtual bool                    create(size_t iBufLen , const xInputBufferDesc* pDesc , void* pInitData  );
    virtual bool                    lock(eLockPolicy lockPolicy , void** pData);
    virtual bool                    unlock();
    virtual bool                    update(eLockPolicy lockPolicy , void* pData , size_t dataLen);
    virtual IInputBufferReflection* reflection();
    virtual IInputBufferReflection* createReflection(bool bSysBuffe);
    virtual void*                   buffer_data_addr(int offset);

protected:
    GLenum                         __getVBOType(xInputBufferDesc* pDesc);
    GLenum                         __getVBOUsage(xInputBufferDesc* pDesc);
    bool                           __destory() ; 
    GLuint                         m_BufferObj;
    size_t                         m_nVertex;
    size_t                         m_nBytePerVertex; 
    xInputBufferDesc               m_Desc;
};

//=============Vertex Buffer In System Memory======================
class xGL2VertexBuffer : public xGL2InputBuffer
{

public:
    xGL2VertexBuffer(IRenderApi* pAPI  );
    ~xGL2VertexBuffer();
    virtual void*                   handle();
    virtual bool                    create(size_t nVertex , size_t bytePerVertex ,const xInputBufferDesc* pDesc , void* pInitData  ) ;
    virtual bool                    create(size_t iBufLen , const xInputBufferDesc* pDesc , void* pInitData  );
    virtual bool                    lock(eLockPolicy lockPolicy , void** pData);
    virtual bool                    unlock();
    virtual bool                    update(eLockPolicy lockPolicy , void* pData , size_t dataLen);
    virtual IInputBufferReflection* reflection();
    virtual IInputBufferReflection* createReflection(bool bSysBuffe);
    virtual void*                   buffer_data_addr(int offset);

protected:
    void*                          m_Buffer;
    size_t                         m_nVertex;
    size_t                         m_nBytePerVertex; 
    xInputBufferDesc               m_Desc;
};

//=============Vertex Stream=======================================
struct xGL2BufferSlot
{
   xGL2InputBuffer * m_pBuffer;
   xGL2BufferSlot(){ m_pBuffer = NULL; }
};

typedef ds_vector(xGL2BufferSlot)  xGL2BufferSlots_t;


class xGL2VertexStream : public IVertexStream
{
	IInputAssembler*    m_pAssembler;
	IRenderApi*         m_pGL2Api;
	xGL2BufferSlots_t   m_VertexBuffers;
	IMPL_BASE_OBJECT_INTERFACE(xGL2VertexStream);
public:
	xGL2VertexStream(IRenderApi* d10API , IInputAssembler* D10Assembler);
	~xGL2VertexStream();
	IInputAssembler* getAssembler() ;
	IInputBuffer*    getInputBuffer(size_t nSlot) ;
	size_t           nInputSlot() ;
	//Create Vertex Buffers with inite Data;
	bool             createBuffers(size_t nVertex , void** pInitData = NULL, const  xInputBufferDesc** pDesc = NULL) ; 
	IInputBuffer*    createInputBuffer(size_t nSlot , size_t nVertex , void* pData ,const   xInputBufferDesc* pDesc = NULL) ;
	bool             setInputBuffer(size_t nSlot , IInputBuffer* pIB) ;
};


//=======================================================
//Buffer Reflection
//=======================================================
enum   eD10ResourceBindType;
class  xGL2InputBufReflection;
struct xGL2ShaderConstReflection : public IShaderConstantReflection
{
	eD10ResourceBindType    m_bindType;
	xGL2InputBufReflection* m_pBufferReflection;
public:
	xGL2ShaderConstReflection();
	~xGL2ShaderConstReflection();
	bool setData(int _offset , void* _data , int dataLen);
	bool setData(void* _data , int dataLen);
};

typedef std::vector<xGL2ShaderConstReflection*>     vecShaderConstantDescs;
typedef std::map<int , xGL2ShaderConstReflection*>  mapShaderConstantDescs;

class xGL2InputBufReflection : public IInputBufferReflection
{
	vecShaderConstantDescs m_vConstant;
	int8*                  m_pSysMem;
	int                    m_bindPoint;
	bool                   m_bDirty;
	size_t                 m_BufLen;
	IInputBuffer*          m_pInputBuffer;
public:
	xGL2InputBufReflection(IInputBuffer* pBuf , bool bSysBuffer , int bufLen);
	~xGL2InputBufReflection();
	size_t                       lenght();
	size_t                       nConstant();
	IShaderConstantReflection*   constantDesc(size_t idx);
	void                         addConstant(xGL2ShaderConstReflection* cd);
	xGL2ShaderConstReflection*   addConstant();
    void*                        systemBuffer();
	void                         setDirty(bool bDirty = true) {m_bDirty = bDirty; }
public:
	bool                         setData(int _off , void* _data , int dataLen);
	bool                         flushToBuffer();
};


END_NAMESPACE_XEVOL3D

#endif

