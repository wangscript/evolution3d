#ifndef _XEVOL_OPENGL20_SAMPLER_STATE_H_
#define _XEVOL_OPENGL20_SAMPLER_STATE_H_
#include <RenderApi/xRenderApi.h>
#include <d3d10.h>
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xGL2RenderApi;
class xGL2Sampler : public ISamplerState
{
	IMPL_BASE_OBJECT_INTERFACE(xGL2Sampler);
	xGL2RenderApi*          m_pGL2Api;
	ID3D10SamplerState *      m_pD10Sampler;
	std::wstring              m_name;
	bool                      _destory() ; 
	bool                      _load(xXmlNode* node);
public:
	void*        handle(){ return m_pD10Sampler ; }
	xGL2Sampler(xGL2RenderApi* pAPI  );
	~xGL2Sampler();
	bool          load(const wchar_t* fileName , unsigned long  arg);
	bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool          isLoaded();
	bool          unload();
	unsigned long memUsage();
};

END_NAMESPACE_XEVOL3D

#endif

