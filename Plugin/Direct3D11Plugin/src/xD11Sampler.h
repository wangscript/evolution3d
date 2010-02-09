#ifndef _XEVOL_DIRECT3D10_SAMPLER_STATE_H_
#define _XEVOL_DIRECT3D10_SAMPLER_STATE_H_
#include <RenderApi/xRenderApi.h>
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xD3D11RenderApi;
class xD11Sampler : public ISamplerState
{
	IMPL_BASE_OBJECT_INTERFACE(xD11Sampler);
	xD3D11RenderApi*          m_pD11RenderAPI;
	ID3D11SamplerState *      m_pD10Sampler;
	std::wstring              m_name;
	bool                      _destory() ; 
	bool                      _load(xXmlNode* node);
public:
	void*        handle(){ return m_pD10Sampler ; }
	xD11Sampler(xD3D11RenderApi* pAPI  );
	~xD11Sampler();
	bool          load(const wchar_t* fileName , unsigned long  arg);
	bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool          isLoaded();
	bool          unload();
	unsigned long memUsage();
};

END_NAMESPACE_XEVOL3D

#endif

