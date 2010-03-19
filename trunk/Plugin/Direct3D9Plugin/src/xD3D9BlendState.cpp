#include "stdafx.h"
#include "xD3D9BlendState.h"
#include "xD3D9ConstLexer.h"
#include "xDIRECT3D9API.h"
#include <BaseLib/xXmlDocument.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD3D9BlendState  , IBlenderState);
bool xD3D9BlendState::_destory()
{
    return true;
}
void xD3D9BlendState::set_factor(xColor_4f& cl) 
{
    m_fBlendFactor = cl ;
    xColor_4ub _cl = m_fBlendFactor;
    m_BlendFactor = _cl.color;
}

void xD3D9BlendState::set_factor(float r , float g , float b , float a)	
{
    m_fBlendFactor.r = r ; m_fBlendFactor.g = g ; m_fBlendFactor.b = b ; m_fBlendFactor.a = a ;
    xColor_4ub cl = m_fBlendFactor;
    m_BlendFactor = cl.color;
}

float* xD3D9BlendState::factor()
{
	return (float*)&m_fBlendFactor;
}

bool xD3D9BlendState::_load(xXmlNode* pBlendNode)
{
	/*
		<?xml version ="1.0" encoding="utf-16" ?>
		<Blend name="AlphaBlend" AlphaToCoverage="false" IndependentBlendEnable="false" enable="true" >
		  <factor r = "1.0"  g = "1.0" b = "1.0" a = "1.0" />
          <color  src="src_alpha" dest="inv_src_alpha" op="add"/>
		  <alpha  src="one" dest="one" op="add"/>    
		</Blend>
	*/
    m_name = pBlendNode->value(L"name");

    xXmlNode* pNode = pBlendNode->findNode(L"color");
    if(pNode)
    {
        m_RGBSrc  = xD3D9ConstLexer::GetBlendFactor(pNode->value(L"src"));
        m_RGBDest = xD3D9ConstLexer::GetBlendFactor(pNode->value(L"dest"));
        m_RGBOp   = xD3D9ConstLexer::GetBlendOp(pNode->value(L"op"));
    }
    pNode = pBlendNode->findNode(L"alpha");
    if(pNode)
    {
        m_AlphaSrc  =  xD3D9ConstLexer::GetBlendFactor(pNode->value(L"src"));
        m_AlphaDest =  xD3D9ConstLexer::GetBlendFactor(pNode->value(L"dest"));
        m_AlphaOp   =  xD3D9ConstLexer::GetBlendOp(pNode->value(L"op"));
    }


    m_Enable = pBlendNode->bool_value(L"enable");

    xXmlNode* pFactorNode = pBlendNode->findNode(L"factor");
    if(pFactorNode)
    {
        xColor_4f cl;
        cl.r = pFactorNode->float_value(L"r");
        cl.g = pFactorNode->float_value(L"g");
        cl.b = pFactorNode->float_value(L"b");
        cl.a = pFactorNode->float_value(L"a");
        set_factor(cl);
    }

	return true;
}

xD3D9BlendState::xD3D9BlendState(xD3D9RenderApi* pAPI  )
:IBlenderState(pAPI)
{
	m_pD3D9RenderApi = pAPI;
	m_name = L"";
    m_RefCount=1;
    m_BlendFactor = 0xffffffff; //D3DRS_BLENDFACTOR;
    m_fBlendFactor = xColor_4f(1.0f , 1.0f , 1.0f , 1.0f );
    m_Enable = FALSE; //D3DRS_ALPHABLENDENABLE;

    m_RGBSrc = D3DBLEND_ONE  ;// D3DRS_SRCBLEND;
    m_RGBDest= D3DBLEND_ZERO ;// D3DRS_DESTBLEND;        
    m_RGBOp  = D3DBLENDOP_ADD;// D3DRS_BLENDOP ;
    


    //*DWORD     m_   */D3DRS_SEPARATEALPHABLENDENABLE;
    m_AlphaSrc = D3DBLEND_ONE  ;;// D3DRS_SRCBLENDALPHA;
    m_AlphaDest= D3DBLEND_ZERO ;;// D3DRS_DESTBLENDALPHA;
    m_AlphaOp  = D3DBLENDOP_ADD;;// D3DRS_BLENDOPALPHA;

    m_SupportSeparateAlpha = true;
}

xD3D9BlendState::~xD3D9BlendState()
{
	_destory();
}

bool xD3D9BlendState::load(const wchar_t* fileName , unsigned long  arg)
{
	xXmlDocument doc;
	if(false == doc.load(fileName , true) )
		return false;
	return _load(doc.root() );
}

bool xD3D9BlendState::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	if(buf == 0 || bufLen == 0)
		return load(fileName , arg);
	xXmlDocument doc;
	if(false == doc.load((const wchar_t* )buf, bufLen ,  true) )
		return false;
	return _load(doc.root() );
}

bool xD3D9BlendState::isLoaded()
{
	return true;
}

bool xD3D9BlendState::unload()
{
	return _destory();
}

unsigned long xD3D9BlendState::memUsage()
{
	return 1;
}
#define GUID_VALUE(v)  if(  (v != pOldBlend-> v)  ) 
void xD3D9BlendState::Install(xD3D9BlendState* pOldBlend)
{
    IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();
    if(pOldBlend)
    {
        if(m_Enable == (DWORD)FALSE )
        {
            if(m_SupportSeparateAlpha) 
            { 
                GUID_VALUE(m_Enable )  pDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE   , m_Enable ); 
            }
            GUID_VALUE(m_Enable )  pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE           , m_Enable );
            return ;
        }

        if(m_SupportSeparateAlpha) 
        { 
            GUID_VALUE(m_Enable )  pDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE   , m_Enable ); 
        }
        GUID_VALUE(m_Enable      )  pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE   , m_Enable );

        GUID_VALUE(m_BlendFactor )  pDevice->SetRenderState(D3DRS_BLENDFACTOR   , m_BlendFactor );
        GUID_VALUE(m_RGBSrc      )  pDevice->SetRenderState(D3DRS_SRCBLEND      , m_RGBSrc      );
        GUID_VALUE(m_RGBDest     )  pDevice->SetRenderState(D3DRS_DESTBLEND     , m_RGBDest     );
        GUID_VALUE(m_RGBOp       )  pDevice->SetRenderState(D3DRS_BLENDOP       , m_RGBOp       );

        if(m_SupportSeparateAlpha)
        {
            GUID_VALUE(m_AlphaSrc )  pDevice->SetRenderState(D3DRS_SRCBLENDALPHA   , m_AlphaSrc );
            GUID_VALUE(m_AlphaDest)  pDevice->SetRenderState(D3DRS_DESTBLENDALPHA  , m_AlphaDest);
            GUID_VALUE(m_AlphaOp  )  pDevice->SetRenderState(D3DRS_BLENDOPALPHA    , m_AlphaOp  );
        }
    }
    else
    {
        if(m_Enable == FALSE )
        {
            if(m_SupportSeparateAlpha) 
            { 
                pDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE   , m_Enable ); 
            }
            pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE           , m_Enable );
            return ;
        }

        if(m_SupportSeparateAlpha) 
        { 
            pDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE   , m_Enable ); 
        }
        pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE   , m_Enable );

        pDevice->SetRenderState(D3DRS_SRCBLEND   , m_RGBSrc );
        pDevice->SetRenderState(D3DRS_DESTBLEND  , m_RGBDest);
        pDevice->SetRenderState(D3DRS_BLENDOP    , m_RGBOp  );
        if(m_SupportSeparateAlpha)
        {
            pDevice->SetRenderState(D3DRS_SRCBLENDALPHA   , m_AlphaSrc );
            pDevice->SetRenderState(D3DRS_DESTBLENDALPHA  , m_AlphaDest);
            pDevice->SetRenderState(D3DRS_BLENDOPALPHA    , m_AlphaOp  );
        }
    }
}

END_NAMESPACE_XEVOL3D
