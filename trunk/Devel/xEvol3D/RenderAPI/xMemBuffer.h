#ifndef __XEVOL_MEM_BUFFER_H__
#define __XEVOL_MEM_BUFFER_H__
#include "xRenderObject.h"
#include "../BaseLib/stl_dllalloc.h"
#include "xRenderAPIDef.h"
#include <vector>

BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_  xInputElementTypeInfo
{
public:
	eShaderVarType type;//输入元素的类型
	int                m_nComponent;//分量数
	int                m_stride;//字节跨度
	int                m_bytePerComponent;//每分量字节数
	int                m_dataType;//数据类型
	xInputElementTypeInfo(const eShaderVarType& vet);
};


struct xInputBufferDesc
{
     eResourceUsage       m_usage;
	 eResourceAccessFlage m_accessFlage;
	 eResourceBindType    m_bindtype;//Vertex Buffer or Index buffer;
	 bool                 m_bShared;//Shared between two device;
	 bool                 m_bCreateBuffer;
public:
	 xInputBufferDesc()
	 {
		 m_usage       = RESOURCE_USAGE_DYNAMIC;
		 m_accessFlage = RESOURCE_ACCESS_WRITE;
		 m_bindtype    = BIND_AS_VERTEX_BUFFER;//Vertex Buffer or Index buffer;
		 m_bShared     = false;//Shared between two device;
		 m_bCreateBuffer = true;
	 }
};

struct xInputElement
{
    wchar_t            m_Semantic[16];
	size_t             m_SemanticIdx;
	eShaderVarType m_Type;
	size_t             m_BufferIdx;
	size_t             m_offset;
	eInputPerVertUsage m_InputUsage;
	size_t             m_InstanceDataStepRate;

};

class _XEVOL_BASE_API_  xInputLayoutDesc
{
public:
	typedef ds_vector(xInputElement     ) xInputElements_t;
	typedef ds_vector(xInputBufferDesc  ) xInputBufferDescs_t;
protected:
	xInputElements_t     m_vInputElemets;
	xInputBufferDescs_t  m_vInputBufferDescs;
public:
	xInputLayoutDesc();
	~xInputLayoutDesc();
	bool addBufferDesc(xInputBufferDesc& desc) ;
	bool addBufferDesc(eResourceUsage       usage         = RESOURCE_USAGE_DYNAMIC,
	                   eResourceAccessFlage accessFlage   = RESOURCE_ACCESS_WRITE,
	                   eResourceBindType         type          = BIND_AS_VERTEX_BUFFER,//Vertex Buffer or Index buffer;
	                   bool                    bShared       = false,//Shared between two device;
					   bool                    bCreateBuffer = true // default is create the buffer;
					);
	bool addElement(xInputElement& element);
	bool addElement(wchar_t* semantic ,eShaderVarType veType , 
		            size_t sematicIdx  = 0 /*default semantic idx is 0*/, 
		            size_t bufIdx = 0 , /*Default is first buffer*/ 
		            int    bufOffset =  -1, /*Auto set by addElement*/
					eInputPerVertUsage inputUsage = INPUT_USAGE_PER_VERTEX_DATA,
					size_t      instanceDataStepRate = 0
					);
	xInputElements_t&           inputElements()  {return m_vInputElemets ; };
	xInputBufferDescs_t&        inputSlotDescs()  {return m_vInputBufferDescs ; }
    xInputBufferDesc&           inputSlotDesc(size_t slotIdx);
	const size_t                nStrideOfBuffer(size_t bufIdx)  const;
	const size_t                nBuffer();
};


class _XEVOL_BASE_API_  IInputBuffer : public IRenderObject
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IInputBuffer);
public:
	IInputBuffer(IRenderApi* pOwner):IRenderObject(pOwner){}
	virtual ~IInputBuffer(){};
	virtual bool                    create(size_t nVertex , size_t bytePerVertex , const xInputBufferDesc* pDesc , void* pInitData = NULL  ) = 0;
	virtual bool                    create(size_t bufLen , const xInputBufferDesc* pDesc , void* pInitData = NULL  ) = 0;
	virtual bool                    lock(eLockPolicy lockPolicy , void** pData) = 0;
	virtual bool                    unlock() = 0;
	virtual bool                    update(eLockPolicy lockPolicy , void* pData , size_t dataLen) = 0;
	virtual IInputBufferReflection* reflection() = 0;
};


class _XEVOL_BASE_API_  IVertexStream : public IRenderObject
{
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IVertexStream);
	IVertexStream(IRenderApi* pOwner):IRenderObject(pOwner){}
	virtual ~IVertexStream(){};


    virtual IInputAssembler* getAssembler() = 0 ;
	virtual IInputBuffer*    getInputBuffer(size_t nSlot) = 0;
    virtual size_t           nInputSlot()  = 0;
	//Create Vertex Buffers with inite Data;
	virtual bool             createBuffers(size_t nVertex , void** pInitData = NULL, const xInputBufferDesc** pDesc = NULL) = 0 ; 
	virtual IInputBuffer*    createInputBuffer(size_t nSlot , size_t nVertex , void* pData ,const  xInputBufferDesc* pDesc = NULL) = 0;
	virtual bool             setInputBuffer(size_t nSlot , IInputBuffer* pIB) = 0;
};

class _XEVOL_BASE_API_  IInputAssembler : public IRenderObject
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IInputAssembler);
public:
	IInputAssembler(IRenderApi* pOwner):IRenderObject(pOwner){}
	virtual ~IInputAssembler(){};
	virtual const xInputLayoutDesc& desc() const = 0;
	virtual const wchar_t*          name() const = 0;
	virtual const size_t            hash_name() const = 0;

	//Create a group of input Buffers; 
	virtual IVertexStream*          createVertexStream() = 0;
    virtual xInputLayoutDesc&       layoutDesc() = 0;
};

END_NAMESPACE_XEVOL3D


#endif