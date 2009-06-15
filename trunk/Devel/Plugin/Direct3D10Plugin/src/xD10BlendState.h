#ifndef _XEVOL_DIRECT3D10_BLEND_STATE_H_
#define _XEVOL_DIRECT3D10_BLEND_STATE_H_
#include <RenderApi/xRenderApi.h>
#include <d3d10.h>
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xD3D10RenderApi;
class xD10BlendState : public IBlenderState
{
	IMPL_BASE_OBJECT_INTERFACE(xD10BlendState);
	xD3D10RenderApi*          m_pD10API;
	ID3D10BlendState *        m_pD10Blend;
	std::wstring              m_name;
	xColor_4f                 m_color;
	unsigned int               m_mask;
	bool                      _destory() ; 
	bool                      _load(xXmlNode* node);
public:
	void*        handle(){ return m_pD10Blend ; }
	xD10BlendState(xD3D10RenderApi* pAPI  );
	~xD10BlendState();
	bool          load(const wchar_t* fileName , unsigned long  arg);
	bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool          isLoaded();
	bool          unload();
	unsigned long memUsage();
	float*        factor();
	void          set_factor(xColor_4f& cl) {m_color = cl ;}
	void          set_factor(float r , float g , float b , float a)	{m_color.r = r ; m_color.g = g ; m_color.b = b ; m_color.a = a ;	}
	void          set_samplemask(unsigned int mask){m_mask = mask;}
	unsigned int  samplemask(){return m_mask ; }
};

END_NAMESPACE_XEVOL3D

#endif

