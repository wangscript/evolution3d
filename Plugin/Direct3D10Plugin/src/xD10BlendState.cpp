#include "stdafx.h"
#include "xD10BlendState.h"
#include "xD10ConstLexer.h"
#include "xDirect3D10API.h"
#include <BaseLib/xXmlDocument.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD10BlendState  , IBlenderState);
bool xD10BlendState::_destory()
{
	m_RefCount=1;
	XSAFE_RELEASE(m_pD10Blend);
	m_pD10Blend = NULL;
	return true;
}

float* xD10BlendState::factor()
{
	return (float*)&m_color;
}

bool xD10BlendState::_load(xXmlNode* node)
{
	/*
	<?xml version ="1.0" encoding="unicode" ?>
	<Blend name=Default AlphaToCoverage="true" mask = "-1" >
	   <color  src="" dest="" op=""/>
	   <alpha  src="" dest="" op=""/>
	   <factor r = "1.0"  g = "1.0" b = "1.0" a = "1.0" />
	   <rendertarget index = "0" blend="true" write="0x0f" />
	   <rendertarget index = "1" blend="true" write="0x0f" />
	   <rendertarget index = "2" blend="true" write="0x0f" />
	   <rendertarget index = "3" blend="true" write="0x0f" />
	   <rendertarget index = "4" blend="true" write="0x0f" />
	   <rendertarget index = "5" blend="true" write="0x0f" />
	   <rendertarget index = "6" blend="true" write="0x0f" />
	   <rendertarget index = "7" blend="true" write="0x0f" />
	</Blend>
	*/
	m_name = node->value(L"name");
	D3D10_BLEND_DESC desc;
	desc.SrcBlend  = D3D10_BLEND_ONE;
	desc.DestBlend = D3D10_BLEND_ZERO;
	desc.BlendOp   = D3D10_BLEND_OP_ADD;
	desc.SrcBlendAlpha  = D3D10_BLEND_ONE;
	desc.DestBlendAlpha = D3D10_BLEND_ZERO;
	desc.BlendOpAlpha   = D3D10_BLEND_OP_ADD;
	xXmlNode* pNode = node->findNode(L"color");
	if(pNode)
	{
		desc.SrcBlend  = xD10ConstLexer::GetBlendFactor(pNode->value(L"src"));
		desc.DestBlend = xD10ConstLexer::GetBlendFactor(pNode->value(L"dest"));
		desc.BlendOp   = xD10ConstLexer::GetBlendOp(pNode->value(L"op"));
	}
	desc.AlphaToCoverageEnable = node->bool_value(L"AlphaToCoverage");
	m_mask = node->hex_value(L"mask");
	pNode = node->findNode(L"alpha");
	if(pNode)
	{
		desc.SrcBlendAlpha  = xD10ConstLexer::GetBlendFactor(pNode->value(L"src"));
		desc.DestBlendAlpha = xD10ConstLexer::GetBlendFactor(pNode->value(L"dest"));
		desc.BlendOpAlpha   = xD10ConstLexer::GetBlendOp(pNode->value(L"op"));
	}

	m_color.r = 1.0f; 
	m_color.g = 1.0f;
	m_color.b = 1.0f;
	m_color.a = 1.0f;  
	pNode = node->findNode(L"factor");
	if(pNode)
	{
		m_color.r = pNode->float_value(L"r");
		m_color.g = pNode->float_value(L"g");
		m_color.b = pNode->float_value(L"b");
		m_color.a = pNode->float_value(L"a");
	}

	xXmlNode::XmlNodes nodes;
	node->findNode(L"rendertarget",nodes);
	size_t _nNodes = nodes.size() ;
	for(size_t i = 0 ; i < _nNodes ; i ++)
	{
		xXmlNode* pRTNode = nodes[i];
		int  idx = pRTNode->int_value(L"index");
		bool bEnable = pRTNode->bool_value(L"blend");
		int  mask    = pRTNode->hex_value(L"write");
		desc.RenderTargetWriteMask[idx] = mask;
		desc.BlendEnable[idx] = bEnable;
	}
	D3D10_COLOR_WRITE_ENABLE_ALL;
	m_pD10API->d10Device()->CreateBlendState(&desc, &m_pD10Blend);
	return m_pD10Blend != NULL;
}

xD10BlendState::xD10BlendState(xD3D10RenderApi* pAPI  )
:IBlenderState(pAPI)
{
	m_pD10API = pAPI;
	m_pD10Blend = NULL;
}

xD10BlendState::~xD10BlendState()
{
	_destory();
}

bool xD10BlendState::load(const wchar_t* fileName , unsigned long  arg)
{
	xXmlDocument doc;
	if(false == doc.load(fileName , true) )
		return false;
	return _load(doc.root() );
}

bool xD10BlendState::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	if(buf == 0 || bufLen == 0)
		return load(fileName , arg);
	xXmlDocument doc;
	if(false == doc.load((const wchar_t* )buf, bufLen ,  true) )
		return false;
	return _load(doc.root() );
}

bool xD10BlendState::isLoaded()
{
	return m_pD10Blend != NULL;
}

bool xD10BlendState::unload()
{
	return _destory();
}

unsigned long xD10BlendState::memUsage()
{
	return 1;
}


END_NAMESPACE_XEVOL3D
