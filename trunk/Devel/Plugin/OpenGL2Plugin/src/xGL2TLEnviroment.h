#ifndef _XEVOL_OPENGL20_TLENVIROMENT_H_
#define _XEVOL_OPENGL20_TLENVIROMENT_H_
#include <RenderApi/xRenderApi.h>
#include <RenderApi/xTLEnvioroment.h>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xGL2RenderApi;
class xGL2InputBufReflection;
#define  XD10_MAX_LIGHT 8

struct xGL2LightEnvBuffer
{
	int        nLight[4];
	xLightDesc m_Light[XD10_MAX_LIGHT];
};

class xGL2LightingEnv : public ILightingEnv
{
	IInputBuffer*           m_pConstBuffer;
	xGL2InputBufReflection* m_pReflection;
	xGL2LightEnvBuffer*     m_pEnvBuffer;
	std::wstring            m_name;
	IMPL_BASE_OBJECT_INTERFACE(xGL2LightingEnv);
public:
	~xGL2LightingEnv();
	xGL2LightingEnv(xGL2RenderApi* pD10RenderApi , const wchar_t* _name);
public:
	xLightDesc*    lightDesc(size_t idx);
	void           enableLight(size_t nLight);
	size_t         nLightEnabled();
	const wchar_t* name();
	void           applyChange() ;
};

END_NAMESPACE_XEVOL3D

#endif

