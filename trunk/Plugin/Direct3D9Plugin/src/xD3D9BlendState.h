#ifndef _XEVOL_DIRECT3D10_BLEND_STATE_H_
#define _XEVOL_DIRECT3D10_BLEND_STATE_H_
#include <RenderApi/xRenderApi.h>
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xD3D9RenderApi;
class xD3D9BlendState : public IBlenderState
{
	IMPL_BASE_OBJECT_INTERFACE(xD3D9BlendState);
	xD3D9RenderApi*           m_pD3D9RenderApi;
    D3DCOLOR    m_BlendFactor ;
    xColor_4f   m_fBlendFactor;
    DWORD       m_Enable ; //D3DRS_ALPHABLENDENABLE;
    
    D3DBLEND    m_RGBSrc ;// D3DRS_SRCBLEND;
    D3DBLEND    m_RGBDest;// D3DRS_DESTBLEND;        
    D3DBLENDOP  m_RGBOp  ;// D3DRS_BLENDOP ;
         //D3DRS_BLENDFACTOR;


     //*DWORD     m_   */D3DRS_SEPARATEALPHABLENDENABLE;

     D3DBLEND    m_AlphaSrc ;// D3DRS_SRCBLENDALPHA;
     D3DBLEND    m_AlphaDest;// D3DRS_DESTBLENDALPHA;
     D3DBLENDOP  m_AlphaOp  ;// D3DRS_BLENDOPALPHA;



    bool                      m_SupportSeparateAlpha;
	std::wstring              m_name;
	xColor_4f                 m_color;
	unsigned int              m_mask;
	bool                      _destory() ; 
	bool                      _load(xXmlNode* node);
public:
	void*        handle(){ return NULL ; }
	xD3D9BlendState(xD3D9RenderApi* pAPI  );
	~xD3D9BlendState();
	bool          load(const wchar_t* fileName , unsigned long  arg);
	bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool          isLoaded();
	bool          unload();
	unsigned long memUsage();
	float*        factor();
	void          set_factor(xColor_4f& cl);
	void          set_factor(float r , float g , float b , float a);
	void          set_samplemask(unsigned int mask){;}
	unsigned int  samplemask(){return 0xffffffff ; }

public:
    void Install(xD3D9BlendState* pBlend);
};

END_NAMESPACE_XEVOL3D

#endif

