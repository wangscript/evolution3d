#include "stdafx.h"
#include "xD11BlendState.h"
#include "xD11ConstLexer.h"
#include "xDirect3D11API.h"
#include <BaseLib/xXmlDocument.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD11BlendState  , IBlenderState);
bool xD11BlendState::_destory()
{
	m_RefCount=1;
	XSAFE_RELEASE(m_pD10Blend);
	m_pD10Blend = NULL;
    m_name = L"";
    m_color= xColor_4f(1.0f,1.0f,1.0f,1.0f);
    m_mask = 0xffffffff;
    return true;
}

float* xD11BlendState::factor()
{
	return (float*)&m_color;
}

bool xD11BlendState::_load(xXmlNode* pBlendNode)
{
	/*
		<?xml version ="1.0" encoding="utf-16" ?>
		<Blend name="AlphaBlend" AlphaToCoverage="false" IndependentBlendEnable="false" >
		  <factor r = "1.0"  g = "1.0" b = "1.0" a = "1.0" />
		  <rendertarget index = "0" enable="true"  mask = "0xffffffff" >
            <color  src="src_alpha" dest="inv_src_alpha" op="add"/>
		    <alpha  src="one" dest="one" op="add"/>    
		  </rendertarget>  
		  <rendertarget index = "1" enable="false" mask = "0xffffffff" />
		  <rendertarget index = "2" enable="false" mask = "0xffffffff" />
		  <rendertarget index = "3" enable="false" mask = "0xffffffff" />
		  <rendertarget index = "4" enable="false" mask = "0xffffffff" />
		  <rendertarget index = "5" enable="false" mask = "0xffffffff" />
		  <rendertarget index = "6" enable="false" mask = "0xffffffff" />
		  <rendertarget index = "7" enable="false" mask = "0xffffffff" />
		</Blend>
	*/
	m_name = pBlendNode->value(L"name");
    if(pBlendNode->value(L"SampleMask"))
    {
        m_mask = pBlendNode->hex_value(L"SampleMask");
    }
	D3D11_BLEND_DESC desc;
    ZeroMemory(&desc , sizeof(D3D11_BLEND_DESC) );
	desc.AlphaToCoverageEnable  = pBlendNode->bool_value(L"AlphaToCoverage");
	desc.IndependentBlendEnable = pBlendNode->bool_value(L"IndependentBlendEnable");
    D3D11_RENDER_TARGET_BLEND_DESC RTBlendDesc;
	ZeroMemory(&RTBlendDesc, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
	RTBlendDesc.BlendEnable = FALSE ;
	RTBlendDesc.SrcBlend = D3D11_BLEND_ONE ;
	RTBlendDesc.DestBlend = D3D11_BLEND_ZERO ;
	RTBlendDesc.BlendOp = D3D11_BLEND_OP_ADD ;
	RTBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE ;
	RTBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO ;
	RTBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD ;
	RTBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL ;
	for(int i = 0 ; i < 8 ; i ++)
		desc.RenderTarget[i] = RTBlendDesc;

	xXmlNode::XmlNodes rtDescNodes;
	pBlendNode->findNode(L"rendertarget" , rtDescNodes);
	size_t nRtDescNode = rtDescNodes.size();

	for(size_t i = 0 ; i < nRtDescNode ; i ++)
	{
        ZeroMemory(&RTBlendDesc, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
		RTBlendDesc.BlendEnable = FALSE ;
		RTBlendDesc.SrcBlend = D3D11_BLEND_ONE ;
		RTBlendDesc.DestBlend = D3D11_BLEND_ZERO ;
		RTBlendDesc.BlendOp = D3D11_BLEND_OP_ADD ;
		RTBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE ;
		RTBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO ;
		RTBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD ;
		RTBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL ;

		xXmlNode* pRTNode = rtDescNodes[i];
		xXmlNode* pNode = pRTNode->findNode(L"color");
		if(pNode)
		{
			RTBlendDesc.SrcBlend  = xD11ConstLexer::GetBlendFactor(pNode->value(L"src"));
			RTBlendDesc.DestBlend = xD11ConstLexer::GetBlendFactor(pNode->value(L"dest"));
			RTBlendDesc.BlendOp   = xD11ConstLexer::GetBlendOp(pNode->value(L"op"));
		}
		pNode = pRTNode->findNode(L"alpha");
		if(pNode)
		{
			RTBlendDesc.SrcBlendAlpha  = xD11ConstLexer::GetBlendFactor(pNode->value(L"src"));
			RTBlendDesc.DestBlendAlpha = xD11ConstLexer::GetBlendFactor(pNode->value(L"dest"));
			RTBlendDesc.BlendOpAlpha   = xD11ConstLexer::GetBlendOp(pNode->value(L"op"));
		}

		if(pRTNode->value(L"mask"))
		   RTBlendDesc.RenderTargetWriteMask = pRTNode->hex_value(L"mask");
		RTBlendDesc.BlendEnable = pRTNode->bool_value(L"enable");

		int rtIndx = pRTNode->int_value(L"index");
		desc.RenderTarget[rtIndx] = RTBlendDesc;



	}

	m_color.r = 1.0f; 
	m_color.g = 1.0f;
	m_color.b = 1.0f;
	m_color.a = 1.0f;  
	xXmlNode* pFactorNode = pBlendNode->findNode(L"factor");
	if(pFactorNode)
	{
		m_color.r = pFactorNode->float_value(L"r");
		m_color.g = pFactorNode->float_value(L"g");
		m_color.b = pFactorNode->float_value(L"b");
		m_color.a = pFactorNode->float_value(L"a");
	}

	m_pD11RenderAPI->d11Device()->CreateBlendState(&desc, &m_pD10Blend);
	return m_pD10Blend != NULL;
}

xD11BlendState::xD11BlendState(xD3D11RenderApi* pAPI  )
:IBlenderState(pAPI)
{
	m_pD11RenderAPI = pAPI;
	m_pD10Blend = NULL;
    m_name = L"";
    m_color= xColor_4f(1.0f,1.0f,1.0f,1.0f);
    m_mask = 0xffffffff;
}

xD11BlendState::~xD11BlendState()
{
	_destory();
}

bool xD11BlendState::load(const wchar_t* fileName , unsigned long  arg)
{
	xXmlDocument doc;
	if(false == doc.load(fileName , true) )
		return false;
	return _load(doc.root() );
}

bool xD11BlendState::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	if(buf == 0 || bufLen == 0)
		return load(fileName , arg);
	xXmlDocument doc;
	if(false == doc.load((const wchar_t* )buf, bufLen ,  true) )
		return false;
	return _load(doc.root() );
}

bool xD11BlendState::isLoaded()
{
	return m_pD10Blend != NULL;
}

bool xD11BlendState::unload()
{
	return _destory();
}

unsigned long xD11BlendState::memUsage()
{
	return 1;
}


END_NAMESPACE_XEVOL3D
