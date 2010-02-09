#ifndef _XEVOL_OPENGL20_STENCIL_DEPTH_STATE_H_
#define _XEVOL_OPENGL20_STENCIL_DEPTH_STATE_H_
#include <RenderApi/xRenderApi.h>
#include "../GL2.h"
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D

struct GL2_DEPTH_STENCILOP_DESC
{
    GLenum StencilFailOp;
    GLenum StencilDepthFailOp;
    GLenum StencilPassOp;
    GLenum StencilFunc;
};


struct GL2_DEPTH_STENCIL_DESC
{
    GLboolean                DepthEnable;
    int                      DepthWriteMask;
    GLenum                   DepthFunc;
    GLboolean                StencilEnable;
    int                      StencilReadMask;
    int                      StencilWriteMask;
    GL2_DEPTH_STENCILOP_DESC FrontFace;
    GL2_DEPTH_STENCILOP_DESC BackFace;
};

class xGL2ZStencilState : public IDepthStencilState
{
	IMPL_BASE_OBJECT_INTERFACE(xGL2ZStencilState);
	IRenderApi*               m_pGL2Api;
	std::wstring              m_name;
    GL2_DEPTH_STENCIL_DESC    m_Desc;
	unsigned     int          m_stecilref;
	bool                      _destory() ; 
	bool                      _load(xXmlNode* node);
public:
	void         stencil_ref(unsigned int ref) {m_stecilref = ref;}
	unsigned int stencil_ref(){return m_stecilref;}
	void*        handle(){ return NULL ; }
	xGL2ZStencilState(IRenderApi* pAPI  );
	~xGL2ZStencilState();
	bool          load(const wchar_t* fileName , unsigned long  arg);
	bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool          isLoaded();
	bool          unload();
	unsigned long memUsage();
    void          apply();
};

END_NAMESPACE_XEVOL3D

#endif

