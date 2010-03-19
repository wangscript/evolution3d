#include "stdafx.h"
#include "xD3D9Sampler.h"
#include "xD3D9ConstLexer.h"
#include "xDIRECT3D9API.h"
#include <BaseLib/xXmlDocument.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD3D9Sampler  , ISamplerState);
bool xD3D9Sampler::_destory()
{
	m_RefCount=1;
	return true;
}


bool xD3D9Sampler::_load(xXmlNode* node)
{
	/*
	<?xml version ="1.0" encoding="unicode" ?>
	<Sampler name=Default border = "0x00000000">
		<filter  min = "linear" mag = "linear"  mip = "point"/>
		<address u = "clamp" v = "clamp" w = "clamp"/>
		<border  r = "1.0" g = "0.0" b = "0.0" a = "0.0" />
	</Sampler>
	*/

    m_name = node->value(L"name");
	//Filter;
	xXmlNode* pNode = node->findNode(L"filter");
	if(pNode)
	{
		const wchar_t* min_val = pNode->value(L"min");
		const wchar_t* mag_val = pNode->value(L"mag");
		const wchar_t* mip_val = pNode->value(L"mip");
		m_MagFilter =  xD3D9ConstLexer::GetFilter(mag_val);
        m_MinFilter =  xD3D9ConstLexer::GetFilter(min_val);
        m_MipFilter =  xD3D9ConstLexer::GetFilter(mip_val);
	}

	pNode = node->findNode(L"address");
	if(pNode)
	{
		const wchar_t* u_val = pNode->value(L"u");
		const wchar_t* v_val = pNode->value(L"v");
		const wchar_t* w_val = pNode->value(L"w");
		m_AddressU = xD3D9ConstLexer::GetAdress(u_val);
		m_AddressV = xD3D9ConstLexer::GetAdress(v_val);
		m_AddressW = xD3D9ConstLexer::GetAdress(w_val);
	}
    m_BorderColor = node->hex_value(L"border");

    return true;
}

xD3D9Sampler::xD3D9Sampler(xD3D9RenderApi* pAPI  ) :ISamplerState(pAPI)
{
	m_pD3D9RenderApi = pAPI;
    m_AddressU       = D3DTADDRESS_WRAP;
    m_AddressV       = D3DTADDRESS_WRAP;
    m_AddressW       = D3DTADDRESS_WRAP;
    m_BorderColor    = 0x00000000;
    m_MinFilter      = D3DTEXF_POINT;
    m_MagFilter      = D3DTEXF_POINT;
    m_MipFilter      = D3DTEXF_NONE ;
}
xD3D9Sampler::~xD3D9Sampler()
{
	_destory();
}

bool          xD3D9Sampler::load(const wchar_t* fileName , unsigned long  arg)
{
	xXmlDocument doc;
	if(false == doc.load(fileName , true) )
		return false;
	return _load(doc.root() );
}

bool          xD3D9Sampler::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	if(buf == 0 || bufLen == 0)
		return load(fileName , arg);
	xXmlDocument doc;
	if(false == doc.load((const wchar_t* )buf, bufLen ,  true) )
		return false;
	return _load(doc.root() );
}

bool          xD3D9Sampler::isLoaded()
{
	return true;
}

bool          xD3D9Sampler::unload()
{
	return _destory();
}

unsigned long xD3D9Sampler::memUsage()
{
	return 0;
}
#define GUID_VALUE(v)  if(  (v != pOldSampler-> v)  ) 
void xD3D9Sampler::Install(int iSampler , xD3D9Sampler* pOldSampler)
{
    IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();
    if(pOldSampler)
    {
        GUID_VALUE(m_AddressU   ) pDevice->SetSamplerState(iSampler , D3DSAMP_ADDRESSU    , m_AddressU    );   //D3DSAMP_ADDRESSU      = 1,
        GUID_VALUE(m_AddressV   ) pDevice->SetSamplerState(iSampler , D3DSAMP_ADDRESSV    , m_AddressV    );   //D3DSAMP_ADDRESSV      = 2,
        GUID_VALUE(m_AddressW   ) pDevice->SetSamplerState(iSampler , D3DSAMP_ADDRESSW    , m_AddressW    );   //D3DSAMP_ADDRESSW      = 3,
        GUID_VALUE(m_BorderColor) pDevice->SetSamplerState(iSampler , D3DSAMP_BORDERCOLOR , m_BorderColor );   //D3DSAMP_BORDERCOLOR   = 4,
        GUID_VALUE(m_MagFilter  ) pDevice->SetSamplerState(iSampler , D3DSAMP_MAGFILTER   , m_MagFilter   );   //D3DSAMP_MAGFILTER     = 5,
        GUID_VALUE(m_MinFilter  ) pDevice->SetSamplerState(iSampler , D3DSAMP_MINFILTER   , m_MinFilter   );   //D3DSAMP_MINFILTER     = 6,
        GUID_VALUE(m_MipFilter  ) pDevice->SetSamplerState(iSampler , D3DSAMP_MIPFILTER   , m_MipFilter   );   //D3DSAMP_MIPFILTER     = 7,
    }
    else 
    {
        pDevice->SetSamplerState(iSampler , D3DSAMP_ADDRESSU    , m_AddressU    );  
        pDevice->SetSamplerState(iSampler , D3DSAMP_ADDRESSV    , m_AddressV    );  
        pDevice->SetSamplerState(iSampler , D3DSAMP_ADDRESSW    , m_AddressW    );  
        pDevice->SetSamplerState(iSampler , D3DSAMP_BORDERCOLOR , m_BorderColor );  
        pDevice->SetSamplerState(iSampler , D3DSAMP_MAGFILTER   , m_MagFilter   );  
        pDevice->SetSamplerState(iSampler , D3DSAMP_MINFILTER   , m_MinFilter   );  
        pDevice->SetSamplerState(iSampler , D3DSAMP_MIPFILTER   , m_MipFilter   ); 
    }
}

END_NAMESPACE_XEVOL3D
