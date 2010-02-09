#ifndef _XEVOL_OPENGL20_RAZ_STATE_H_
#define _XEVOL_OPENGL20_RAZ_STATE_H_
#include <RenderApi/xRenderApi.h>
#include <d3d10.h>
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D


struct GL2_RASTERIZER_DESC
{
    GLenum FillMode;
    GLenum CullMode;
    GLboolean FrontCounterClockwise;
    int DepthBias;
    float DepthBiasClamp;
    float SlopeScaledDepthBias;
    GLboolean DepthClipEnable;
    GLboolean ScissorEnable;
    GLboolean MultisampleEnable;
    GLboolean AntialiasedLineEnable;
};


class xGL2RazState : public IRasterizerState
{
	IMPL_BASE_OBJECT_INTERFACE(xGL2RazState);
	IRenderApi*             m_pGL2Api;
	GL2_RASTERIZER_DESC     m_Desc;
	std::wstring            m_name;

	bool                      _destory() ; 
	bool                      _load(xXmlNode* node);
public:
	void*        handle(){ return NULL ; }
	xGL2RazState(IRenderApi* pAPI  );
	~xGL2RazState();
	bool          load(const wchar_t* fileName , unsigned long  arg);
	bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool          isLoaded();
	bool          unload();
	unsigned long memUsage();
    void          apply();
};

END_NAMESPACE_XEVOL3D

#endif

