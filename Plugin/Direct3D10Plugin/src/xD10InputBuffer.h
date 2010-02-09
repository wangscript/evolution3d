#ifndef _XEVOL_DIRECT3D10_INPUT_BUFFER_H_
#define _XEVOL_DIRECT3D10_INPUT_BUFFER_H_
#include <RenderApi/xRenderApi.h>
#include "RenderAPI/xBaseShader.h"
#include "xDirect3D10API.h"
#include "xD3DInputBuffer.h"
#include <d3d10.h>
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class   xD3D10RenderApi;
class   xD10InputAssembler;
typedef xD3DVertexStream          xD3DVertexStream;
typedef xD3DShaderConstReflection xD10ShaderConstReflection;
typedef xD3DInputBufReflection    xD3DInputBufReflection;

class xD10InputBuffer : public IInputBuffer
{
	IMPL_BASE_OBJECT_INTERFACE(xD10InputBuffer);
	xD3D10RenderApi*          m_pD10API;
	ID3D10Buffer *            m_pBuffer;
	ID3D10ShaderResourceView* m_pRSV;
	xD3DInputBufReflection*   m_pReflection;

	size_t              m_nElement;
	size_t              m_nBytePerVertex;  
	size_t              m_iBufLen;
    D3D10_BUFFER_DESC   m_buffDesc;
	bool _destory() ; 
public:
	void* handle(){ return m_pBuffer ; }
	ID3D10ShaderResourceView* toShaderResourceView(ePIXEL_FORMAT fmt);
	xD10InputBuffer(xD3D10RenderApi* pAPI  );
	~xD10InputBuffer();
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

