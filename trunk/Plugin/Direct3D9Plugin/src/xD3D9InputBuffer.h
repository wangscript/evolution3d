#ifndef _XEVOL_DIRECT3D9_INPUT_BUFFER_H_
#define _XEVOL_DIRECT3D9_INPUT_BUFFER_H_
#include <RenderApi/xRenderApi.h>
#include "RenderAPI/xBaseShader.h"
#include "xDirect3D9APIResource.h"
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xD3D9RenderApi;
class xD3DInputBufReflection;
class xD3D9VertexBuffer : public IInputBuffer 
{
	IMPL_BASE_OBJECT_INTERFACE(xD3D9VertexBuffer);
    DECLARE_DEVICE_LOST(xD3D9VertexBuffer);
	xD3D9RenderApi*           m_pD3D9RenderAPI;
	IDirect3DVertexBuffer9 *  m_pBuffer;
	void*                     m_pLockData;
	eLockPolicy               m_lockPolicy;
    D3DPOOL                   m_Pool; 
    DWORD                     m_Usage; 

	size_t                    m_nElement;
	size_t                    m_nBytePerVertex;  
	size_t                    m_iBufLen;
	bool _destory() ; 
public:
	void* handle(){ return m_pBuffer ; }
	xD3D9VertexBuffer(xD3D9RenderApi* pAPI  );
	~xD3D9VertexBuffer();
	bool create(size_t nElement , size_t bytePerVertex ,const xInputBufferDesc* pDesc , void* pInitData  ) ;
	bool create(size_t iBufLen , const xInputBufferDesc* pDesc , void* pInitData  ) ;
	bool lock(eLockPolicy lockPolicy , void** pData);
	bool unlock();
	bool update(eLockPolicy lockPolicy , void* pData , size_t dataLen);
    IInputBufferReflection* reflection(){ return NULL; }
	IInputBufferReflection* createReflection(bool bSysBuffe){ return NULL; }
	virtual size_t          nElement(){ return m_nElement ; }
	virtual size_t          bufLen(){ return m_iBufLen ; }
    void onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action);
    
};

class xD3D9IndexBuffer : public IInputBuffer
{
    IMPL_BASE_OBJECT_INTERFACE(xD3D9IndexBuffer);
    DECLARE_DEVICE_LOST(xD3D9IndexBuffer);
    xD3D9RenderApi*           m_pD3D9RenderAPI;
    IDirect3DIndexBuffer9 *   m_pBuffer;
    void*                     m_pLockData;
    eLockPolicy               m_lockPolicy;
    D3DPOOL                   m_Pool; 
    DWORD                     m_Usage; 

    size_t                    m_nElement;
    size_t                    m_nBytePerVertex;  
    size_t                    m_iBufLen;
    bool _destory() ; 
public:
    void* handle(){ return m_pBuffer ; }
    xD3D9IndexBuffer(xD3D9RenderApi* pAPI  );
    ~xD3D9IndexBuffer();
    bool create(size_t nElement , size_t bytePerVertex ,const xInputBufferDesc* pDesc , void* pInitData  ) ;
    bool create(size_t iBufLen , const xInputBufferDesc* pDesc , void* pInitData  ) ;
    bool lock(eLockPolicy lockPolicy , void** pData);
    bool unlock();
    bool update(eLockPolicy lockPolicy , void* pData , size_t dataLen);
    IInputBufferReflection* reflection(){ return NULL; }
    IInputBufferReflection* createReflection(bool bSysBuffe){ return NULL; }
    virtual size_t          nElement(){ return m_nElement ; }
    virtual size_t          bufLen(){ return m_iBufLen ; }
    void onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action);
};

END_NAMESPACE_XEVOL3D

#endif

