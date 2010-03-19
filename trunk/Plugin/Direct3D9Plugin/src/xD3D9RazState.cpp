#include "stdafx.h"
#include "xD3D9RazState.h"
#include "xD3D9ConstLexer.h"
#include "xDIRECT3D9API.h"
#include <BaseLib/xXmlDocument.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD3D9RazState  , IRasterizerState);
bool xD3D9RazState::_destory()
{
	m_RefCount=1;
	return true;
}


bool xD3D9RazState::_load(xXmlNode* node)
{


	/*
	<?xml version ="1.0" encoding="unicode" ?>
	<Rasterizer name=Default cull="none" fill="fill" alphaFunc="always" antialis_line="true" >
	   <depthbias  value = ""  scale="" />
	</Rasterizer>
	*/
    if(node == NULL)
        return false;

	m_name = node->value(L"name");
    m_CullMode  = xD3D9ConstLexer::GetCullMode(node->value(L"cull"));
    m_FillMode  = xD3D9ConstLexer::GetFillMode(node->value(L"fill"));
    m_AlphaFunc = xD3D9ConstLexer::GetCompareFunc(node->value(L"alphaFunc"));
    m_LineAntiAlias = node->bool_value(L"antialis_line");
	xXmlNode* pNode = node->findNode(L"depthbias");
	if(pNode)
	{
		m_ZBias           = pNode->float_value(L"value");
		m_SlopScaleZbias  = pNode->int_value(L"scale");
	}
    return true;
}

xD3D9RazState::xD3D9RazState(xD3D9RenderApi* pAPI  )
:IRasterizerState(pAPI)
{
	m_pD3D9RenderApi = pAPI;
    
    m_FillMode      = D3DFILL_SOLID;// D3DRS_FILLMODE           ;
    m_ShadeMode     = D3DSHADE_GOURAUD;// D3DRS_SHADEMODE          ;
    m_CullMode      = D3DCULL_NONE;// D3DRS_CULLMODE           ;
    m_SlopScaleZbias=0;// D3DRS_SLOPESCALEDEPTHBIAS;   bias = (max * D3DRS_SLOPESCALEDEPTHBIAS) + D3DRS_DEPTHBIAS.
    m_ZBias         =0.0f;// D3DRS_DEPTHBIAS          ;
    m_AlphaFunc     = D3DCMP_ALWAYS;
    m_LineAntiAlias = TRUE;
}


xD3D9RazState::~xD3D9RazState()
{
	_destory();
}
#define GUID_VALUE(v)  if(  (v != pOldRazState-> v)  ) 
void xD3D9RazState::Install(xD3D9RazState* pOldRazState)
{
     IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();
     if(pOldRazState)
     {
          GUID_VALUE(m_FillMode      )    pDevice->SetRenderState( D3DRS_FILLMODE             , m_FillMode       ) ;
          GUID_VALUE(m_ShadeMode     )    pDevice->SetRenderState( D3DRS_SHADEMODE            , m_ShadeMode      ) ;
          GUID_VALUE(m_CullMode      )    pDevice->SetRenderState( D3DRS_CULLMODE             , m_CullMode       ) ;
          GUID_VALUE(m_SlopScaleZbias)    pDevice->SetRenderState( D3DRS_SLOPESCALEDEPTHBIAS  , m_SlopScaleZbias ) ;
          GUID_VALUE(m_ZBias         )    pDevice->SetRenderState( D3DRS_DEPTHBIAS            , *((DWORD*) (&m_ZBias)));
          GUID_VALUE(m_AlphaFunc     )    pDevice->SetRenderState( D3DRS_ALPHAFUNC            , m_AlphaFunc      );
          GUID_VALUE(m_LineAntiAlias )    pDevice->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, m_LineAntiAlias  );
     }
     else
     {
         pDevice->SetRenderState( D3DRS_FILLMODE             , m_FillMode       ) ;
         pDevice->SetRenderState( D3DRS_SHADEMODE            , m_ShadeMode      ) ;
         pDevice->SetRenderState( D3DRS_CULLMODE             , m_CullMode       ) ;
         pDevice->SetRenderState( D3DRS_SLOPESCALEDEPTHBIAS  , m_SlopScaleZbias ) ;
         pDevice->SetRenderState( D3DRS_DEPTHBIAS            , *((DWORD*) (&m_ZBias)));
         pDevice->SetRenderState( D3DRS_ALPHAFUNC            , m_AlphaFunc      );
     }
}

bool xD3D9RazState::load(const wchar_t* fileName , unsigned long  arg)
{
	xXmlDocument doc;
	if(false == doc.load(fileName , true) )
		return false;
	return _load(doc.root() );
}

bool xD3D9RazState::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	if(buf == 0 || bufLen == 0)
		return load(fileName , arg);
	xXmlDocument doc;
	if(false == doc.load((const wchar_t* )buf, bufLen ,  true) )
		return false;
	return _load(doc.root() );
}

bool xD3D9RazState::isLoaded()
{
	return true;
}

bool xD3D9RazState::unload()
{
	return _destory();
}

unsigned long xD3D9RazState::memUsage()
{
	return 0;
}


END_NAMESPACE_XEVOL3D
