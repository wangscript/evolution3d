#ifndef _XEVOL_DIRECT3D10_STENCIL_DEPTH_STATE_H_
#define _XEVOL_DIRECT3D10_STENCIL_DEPTH_STATE_H_
#include <RenderApi/xRenderApi.h>
#include <d3d10.h>
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xD3D10RenderApi;
class xD10ZStencilState : public IDepthStencilState
{
	IMPL_BASE_OBJECT_INTERFACE(xD10ZStencilState);
	xD3D10RenderApi*          m_pD10API;
	ID3D10DepthStencilState*  m_pD10RasterizerState;
	std::wstring              m_name;
	unsigned     int          m_stecilref;

	bool                      _destory() ; 
	bool                      _load(xXmlNode* node);
public:
	void         stencil_ref(unsigned int ref) {m_stecilref = ref;}
	unsigned int stencil_ref(){return m_stecilref;}
	void*        handle(){ return m_pD10RasterizerState ; }
	xD10ZStencilState(xD3D10RenderApi* pAPI  );
	~xD10ZStencilState();
	bool          load(const wchar_t* fileName , unsigned long  arg);
	bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool          isLoaded();
	bool          unload();
	unsigned long memUsage();

};

END_NAMESPACE_XEVOL3D

#endif

