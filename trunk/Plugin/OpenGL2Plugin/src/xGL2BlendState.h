#ifndef _XEVOL_OPENGL20_BLEND_STATE_H_
#define _XEVOL_OPENGL20_BLEND_STATE_H_
#include <RenderApi/xRenderApi.h>
#include "../GL2.h"
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xGL2RenderApi;


struct GL2_BLEND_DESC
{
   GLenum SrcBlend;
   GLenum DestBlend;
   GLenum BlendOp;
   GLenum SrcBlendAlpha;
   GLenum DestBlendAlpha;
   GLenum BlendOpAlpha;
   bool   AlphaToCoverageEnable;
   int    RenderTargetWriteMask[MAX_RENDER_TARGET];
   bool   BlendEnable[MAX_RENDER_TARGET];
};

class xGL2BlendState : public IBlenderState
{
	IMPL_BASE_OBJECT_INTERFACE(xGL2BlendState);
	IRenderApi*             m_pGL2Api;
	GL2_BLEND_DESC          m_Desc;
	std::wstring            m_name;
	xColor_4f               m_color;
	unsigned int            m_mask;
	bool                    _destory() ; 
	bool                    _load(xXmlNode* node);
public:
	void*        handle(){ return NULL ; }
	xGL2BlendState(IRenderApi* pAPI  );
	~xGL2BlendState();
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
    void          apply();
};

END_NAMESPACE_XEVOL3D
#endif

