#include "stdafx.h"
#include "xD11RazState.h"
#include "xD11ConstLexer.h"
#include "xDirect3D11API.h"
#include <BaseLib/xXmlDocument.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD11RazState  , IRasterizerState);
bool xD11RazState::_destory()
{
	m_RefCount=1;
	XSAFE_RELEASE(m_pD10RasterizerState);
	m_pD10RasterizerState = NULL;
	return true;
}


bool xD11RazState::_load(xXmlNode* node)
{


	/*
	<?xml version ="1.0" encoding="unicode" ?>
	<Rasterizer name=Default cull="none" fill="fill" front="ccw" depthclip="true" scissor="false"  antialis_line="true"  multisample="true" >
	   <depthbias  value = ""  clamp = "" scale="" />
	</Rasterizer>
	*/
	m_name = node->value(L"name");
    D3D11_RASTERIZER_DESC desc;

	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode =  D3D11_CULL_BACK;
	desc.FrontCounterClockwise = FALSE;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthClipEnable = TRUE;
	desc.ScissorEnable = FALSE;
	desc.MultisampleEnable = FALSE;
	desc.AntialiasedLineEnable = FALSE;
	desc.FillMode = xD11ConstLexer::GetFillMode( node->value(L"fill"));
	desc.CullMode = xD11ConstLexer::GetCullMode( node->value(L"cull"));
	ds_wstring front = node->value(L"front");
	if(front == L"ccw") desc.FrontCounterClockwise = true;
	else desc.FrontCounterClockwise = false;

	xXmlNode* pNode = node->findNode(L"depthbias");
	if(pNode)
	{
		desc.DepthBias             = pNode->int_value(L"value");
		desc.DepthBiasClamp        = pNode->float_value(L"clamp");
		desc.SlopeScaledDepthBias  = pNode->float_value(L"scale");
	}

	desc.DepthClipEnable = node->bool_value(L"depthclip");
	desc.ScissorEnable = node->bool_value(L"scissor");
	desc.MultisampleEnable = node->bool_value(L"multisample");
	desc.AntialiasedLineEnable = node->bool_value(L"antialis_line");

	m_pD11RenderAPI->d11Device()->CreateRasterizerState(&desc,&m_pD10RasterizerState);
	return m_pD10RasterizerState != NULL;
}

xD11RazState::xD11RazState(xD3D11RenderApi* pAPI  )
:IRasterizerState(pAPI)
{
	m_pD11RenderAPI = pAPI;
	m_pD10RasterizerState = NULL;
}


xD11RazState::~xD11RazState()
{
	_destory();
}

bool xD11RazState::load(const wchar_t* fileName , unsigned long  arg)
{
	xXmlDocument doc;
	if(false == doc.load(fileName , true) )
		return false;
	return _load(doc.root() );
}

bool xD11RazState::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	if(buf == 0 || bufLen == 0)
		return load(fileName , arg);
	xXmlDocument doc;
	if(false == doc.load((const wchar_t* )buf, bufLen ,  true) )
		return false;
	return _load(doc.root() );
}

bool xD11RazState::isLoaded()
{
	return m_pD10RasterizerState != NULL;
}

bool xD11RazState::unload()
{
	return _destory();
}

unsigned long xD11RazState::memUsage()
{
	return 1;
}


END_NAMESPACE_XEVOL3D
