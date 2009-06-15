#ifndef _XEVOL_DIRECT3D10_INPUT_BUFFER_H_
#define _XEVOL_DIRECT3D10_INPUT_BUFFER_H_
#include <RenderApi/xRenderApi.h>
#include "RenderAPI/xBaseShader.h"
#include <d3d10.h>
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xD3D10RenderApi;
class xD10InputAssembler;
class xD10InputBufReflection;
class xD10InputBuffer : public IInputBuffer
{
	IMPL_BASE_OBJECT_INTERFACE(xD10InputBuffer);
	xD3D10RenderApi*          m_pD10API;
	ID3D10Buffer *            m_pBuffer;
	ID3D10ShaderResourceView* m_pRSV;
	xD10InputBufReflection*   m_pReflection;

	size_t              m_nVertex;
	size_t              m_nBytePerVertex;  
	size_t              m_iBufLen;
	bool _destory() ; 
public:
	void* handle(){ return m_pBuffer ; }
	ID3D10ShaderResourceView* toShaderResourceView(ePIXEL_FORMAT fmt);
	xD10InputBuffer(xD3D10RenderApi* pAPI  );
	~xD10InputBuffer();
	bool create(size_t nVertex , size_t bytePerVertex ,const xInputBufferDesc* pDesc , void* pInitData  ) ;
	bool create(size_t iBufLen , const xInputBufferDesc* pDesc , void* pInitData  ) ;
	bool lock(eLockPolicy lockPolicy , void** pData);
	bool unlock();
	bool update(eLockPolicy lockPolicy , void* pData , size_t dataLen);
	IInputBufferReflection* reflection();
	IInputBufferReflection* createReflection(bool bSysBuffe);
};


struct xD10BufferSlot
{
   xD10InputBuffer * m_pBuffer;
   xD10BufferSlot(){ m_pBuffer = NULL; }
};

typedef ds_vector(xD10BufferSlot)  xD10BufferSlots_t;

class xD10VertexStream : public IVertexStream
{
	xD10InputAssembler* m_pAssembler;
	xD3D10RenderApi*    m_pD10API;
	xD10BufferSlots_t   m_VertexBuffers;
	IMPL_BASE_OBJECT_INTERFACE(xD10VertexStream);
public:
	xD10VertexStream(xD3D10RenderApi* d10API , xD10InputAssembler* D10Assembler);
	~xD10VertexStream();
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
class  xD10InputBufReflection;
struct xD10ShaderConstReflection : public IShaderConstantReflection
{
	eD10ResourceBindType    m_bindType;
	xD10InputBufReflection* m_pBufferReflection;
public:
	xD10ShaderConstReflection();
	~xD10ShaderConstReflection();
	bool setData(int _offset , void* _data , int dataLen);
	bool setData(void* _data , int dataLen);
};

typedef std::vector<xD10ShaderConstReflection*>     vecShaderConstantDescs;
typedef std::map<int , xD10ShaderConstReflection*>  mapShaderConstantDescs;

class xD10InputBufReflection : public IInputBufferReflection
{
	vecShaderConstantDescs m_vConstant;
	int8*                  m_pSysMem;
	int                    m_bindPoint;
	bool                   m_bDirty;
	size_t                 m_BufLen;
	IInputBuffer*          m_pInputBuffer;
public:
	xD10InputBufReflection(IInputBuffer* pBuf , bool bSysBuffer , int bufLen);
	~xD10InputBufReflection();
	size_t                       lenght();
	size_t                       nConstant();
	IShaderConstantReflection*   constantDesc(size_t idx);
	//void                         addConstant(xD10ShaderConstReflection* cd);
	xD10ShaderConstReflection*   addConstant();
    void*                        systemBuffer();
	void                         setDirty(bool bDirty = true) {m_bDirty = bDirty; }
public:
	bool                         setData(int _off , void* _data , int dataLen);
	bool                         flushToBuffer();
};


END_NAMESPACE_XEVOL3D

#endif

