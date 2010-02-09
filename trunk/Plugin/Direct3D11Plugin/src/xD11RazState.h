#ifndef _XEVOL_DIRECT3D10_RAZ_STATE_H_
#define _XEVOL_DIRECT3D10_RAZ_STATE_H_
#include <RenderApi/xRenderApi.h>
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xD3D11RenderApi;
class xD11RazState : public IRasterizerState
{
	IMPL_BASE_OBJECT_INTERFACE(xD11RazState);
	xD3D11RenderApi*          m_pD11RenderAPI;
	ID3D11RasterizerState *   m_pD10RasterizerState;
	std::wstring              m_name;

	bool                      _destory() ; 
	bool                      _load(xXmlNode* node);
public:
	void*        handle(){ return m_pD10RasterizerState ; }
	xD11RazState(xD3D11RenderApi* pAPI  );
	~xD11RazState();
	bool          load(const wchar_t* fileName , unsigned long  arg);
	bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool          isLoaded();
	bool          unload();
	unsigned long memUsage();

};

END_NAMESPACE_XEVOL3D

#endif

