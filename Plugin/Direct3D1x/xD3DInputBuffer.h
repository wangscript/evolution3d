#ifndef _XEVOL_DIRECT3D1x_INPUT_BUFFER_H_
#define _XEVOL_DIRECT3D1x_INPUT_BUFFER_H_
#include <RenderApi/xRenderApi.h>
#include "RenderAPI/xBaseShader.h"
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xD3DInputBufReflection;

struct xD3DBufferSlot
{
   IInputBuffer*     m_pBuffer;
   xD3DBufferSlot(){ m_pBuffer = NULL; }
};
typedef ds_vector(xD3DBufferSlot)  xD3DBufferSlots_t;

class xD3DVertexStream : public IVertexStream
{
	IInputAssembler*    m_pAssembler;
	xD3DBufferSlots_t   m_VertexBuffers;
	IMPL_BASE_OBJECT_INTERFACE(xD3DVertexStream);
public:
	xD3DVertexStream(IRenderApi* d10API , IInputAssembler* D10Assembler);
	~xD3DVertexStream();
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
enum eD3DResourceBindType
{
	eRBT_CBuffer,
	eRBT_TBuffer,
	eRBT_Buffer,
	eRBT_Texture,
	eRBT_Sampler,
};

class  xD3DInputBufReflection;
struct xD3DShaderConstReflection : public IShaderConstantReflection
{
	eD3DResourceBindType    m_bindType;
	xD3DInputBufReflection* m_pBufferReflection;
    std::wstring            m_Name;
public:
	xD3DShaderConstReflection(const wchar_t* _name);
	~xD3DShaderConstReflection();
	bool setData(int _offset , void* _data , int dataLen);
	bool setData(void* _data , int dataLen);
};

typedef ds_vector(xD3DShaderConstReflection*)     vecShaderConstantDescs;
typedef ds_map(int , xD3DShaderConstReflection*)  mapShaderConstantDescs;

class xD3DInputBufReflection : public IInputBufferReflection
{
    IMPL_BASE_OBJECT_INTERFACE(xD3DInputBufReflection);
public:
	xD3DInputBufReflection(IInputBuffer* pBuf , bool bSysBuffer , int bufLen);
	~xD3DInputBufReflection();
	size_t                       lenght();
	size_t                       nConstant();
	IShaderConstantReflection*   constantDesc(size_t idx);
	//void                         addConstant(xD3DShaderConstReflection* cd);
	xD3DShaderConstReflection*   addConstant(const wchar_t* _name);
    void*                        systemBuffer();
	void                         setDirty(bool bDirty = true) {m_bDirty = bDirty; }
public:
	bool                         setData(int _off , void* _data , int dataLen);
	bool                         flushToBuffer();
protected:
    vecShaderConstantDescs       m_vConstant;
    int8*                        m_pSysMem;
    int                          m_bindPoint;
    bool                         m_bDirty;
    size_t                       m_BufLen;
    IInputBuffer*                m_pInputBuffer;
};


END_NAMESPACE_XEVOL3D

#endif

