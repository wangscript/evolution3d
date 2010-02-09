#ifndef _XEVOL_DIRECT3D1X_TLENVIROMENT_H_
#define _XEVOL_DIRECT3D1X_TLENVIROMENT_H_
#include <RenderApi/xRenderApi.h>
#include <RenderApi/xTLEnvioroment.h>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xD3DInputBufReflection;
#define  XD3D_MAX_LIGHT 8

struct xD1xLightEnvBuffer
{
	int        nLight[4];
	xLightDesc m_Light[XD3D_MAX_LIGHT];
};

class xD1xLightingEnv : public ILightingEnv
{
	IInputBuffer*           m_pConstBuffer;
	IInputBufferReflection* m_pReflection;
	xD1xLightEnvBuffer*     m_pEnvBuffer;
	std::wstring            m_name;
	IMPL_BASE_OBJECT_INTERFACE(xD1xLightingEnv);
public:
	~xD1xLightingEnv();
	xD1xLightingEnv(IRenderApi* pD10RenderApi , const wchar_t* _name);
public:
	xLightDesc*    lightDesc(size_t idx);
	void           enableLight(size_t nLight);
	size_t         nLightEnabled();
	const wchar_t* name();
	void           applyChange() ;
};

END_NAMESPACE_XEVOL3D

#endif

