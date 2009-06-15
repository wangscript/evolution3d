#ifndef _XEVOL_DIRECT3D10_TLENVIROMENT_H_
#define _XEVOL_DIRECT3D10_TLENVIROMENT_H_
#include <RenderApi/xRenderApi.h>
#include <RenderApi/xTLEnvioroment.h>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xD3D10RenderApi;
class xD10InputBufReflection;
#define  XD10_MAX_LIGHT 8

struct xD10LightEnvBuffer
{
	int        nLight[4];
	xLightDesc m_Light[XD10_MAX_LIGHT];
};

class xD10LightingEnv : public ILightingEnv
{
	IInputBuffer*           m_pConstBuffer;
	xD10InputBufReflection* m_pReflection;
	xD10LightEnvBuffer*     m_pEnvBuffer;
	std::wstring            m_name;
	IMPL_BASE_OBJECT_INTERFACE(xD10LightingEnv);
public:
	~xD10LightingEnv();
	xD10LightingEnv(xD3D10RenderApi* pD10RenderApi , const wchar_t* _name);
public:
	xLightDesc*    lightDesc(size_t idx);
	void           enableLight(size_t nLight);
	size_t         nLightEnabled();
	const wchar_t* name();
	void           applyChange() ;
};

END_NAMESPACE_XEVOL3D

#endif

