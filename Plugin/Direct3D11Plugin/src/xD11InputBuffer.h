#ifndef _XEVOL_DIRECT3D11_INPUT_BUFFER_H_
#define _XEVOL_DIRECT3D11_INPUT_BUFFER_H_
#include <RenderApi/xRenderApi.h>
#include "RenderAPI/xBaseShader.h"
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xD3D11RenderApi;
class xD3DInputBufReflection;
class xD11InputBuffer : public IInputBuffer
{
	IMPL_BASE_OBJECT_INTERFACE(xD11InputBuffer);
	xD3D11RenderApi*          m_pD11RenderAPI;
	ID3D11Buffer *            m_pBuffer;
	ID3D11ShaderResourceView* m_pRSV;
	xD3DInputBufReflection*   m_pReflection;
	void*                     m_pLockData;
	eLockPolicy               m_lockPolicy;
	D3D11_BUFFER_DESC         m_buffDesc;

	size_t              m_nElement;
	size_t              m_nBytePerVertex;  
	size_t              m_iBufLen;
	bool _destory() ; 
public:
	void* handle(){ return m_pBuffer ; }
	ID3D11ShaderResourceView* toShaderResourceView(ePIXEL_FORMAT fmt);
	xD11InputBuffer(xD3D11RenderApi* pAPI  );
	~xD11InputBuffer();
	bool create(size_t nElement , size_t bytePerVertex ,const xInputBufferDesc* pDesc , void* pInitData  ) ;
	bool create(size_t iBufLen , const xInputBufferDesc* pDesc , void* pInitData  ) ;
	bool lock(eLockPolicy lockPolicy , void** pData);
	bool unlock();
	bool update(eLockPolicy lockPolicy , void* pData , size_t dataLen);
	IInputBufferReflection* reflection();
	IInputBufferReflection* createReflection(bool bSysBuffe);
	virtual size_t          nElement(){ return m_nElement ; }
	virtual size_t          bufLen(){ return m_iBufLen ; }
};

END_NAMESPACE_XEVOL3D

#endif

