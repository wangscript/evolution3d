#ifndef _XEVOL_DIRECT3D10_SAMPLER_STATE_H_
#define _XEVOL_DIRECT3D10_SAMPLER_STATE_H_
#include <RenderApi/xRenderApi.h>
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xD3D9RenderApi;
class xD3D9Sampler : public ISamplerState
{
	IMPL_BASE_OBJECT_INTERFACE(xD3D9Sampler);
	xD3D9RenderApi*          m_pD3D9RenderApi;
	std::wstring             m_name;


protected:
    D3DTEXTUREADDRESS    m_AddressU;   //D3DSAMP_ADDRESSU      = 1,
    D3DTEXTUREADDRESS    m_AddressV;   //D3DSAMP_ADDRESSV      = 2,
    D3DTEXTUREADDRESS    m_AddressW;   //D3DSAMP_ADDRESSW      = 3,
    D3DCOLOR             m_BorderColor;//D3DSAMP_BORDERCOLOR   = 4,
    D3DTEXTUREFILTERTYPE m_MagFilter;  //D3DSAMP_MAGFILTER     = 5,
    D3DTEXTUREFILTERTYPE m_MinFilter;  //D3DSAMP_MINFILTER     = 6,
    D3DTEXTUREFILTERTYPE m_MipFilter;  //D3DSAMP_MIPFILTER     = 7,
                                       //D3DSAMP_MIPMAPLODBIAS = 8,
                                       //D3DSAMP_MAXMIPLEVEL   = 9,
                                       //D3DSAMP_MAXANISOTROPY = 10,
                                       //D3DSAMP_SRGBTEXTURE   = 11,
                                       //D3DSAMP_ELEMENTINDEX  = 12,
                                       //D3DSAMP_DMAPOFFSET    = 13,
protected:
    bool          _destory() ; 
    bool          _load(xXmlNode* node);
public:
	void*        handle(){ return this ; }
	xD3D9Sampler(xD3D9RenderApi* pAPI  );
	~xD3D9Sampler();
	bool          load(const wchar_t* fileName , unsigned long  arg);
	bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool          isLoaded();
	bool          unload();
	unsigned long memUsage();
public:
    void          Install(int iSampler , xD3D9Sampler* pOldSampler);
};

END_NAMESPACE_XEVOL3D

#endif

