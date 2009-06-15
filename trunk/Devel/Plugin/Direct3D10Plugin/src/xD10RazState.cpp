#include "xD10RazState.h"
#include "xD10ConstLexer.h"
#include "xDirect3D10API.h"
#include <BaseLib/xXmlDocument.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD10RazState  , IRasterizerState);
bool xD10RazState::_destory()
{
	m_RefCount=1;
	XSAFE_RELEASE(m_pD10RasterizerState);
	m_pD10RasterizerState = NULL;
	return true;
}


bool xD10RazState::_load(xXmlNode* node)
{


	/*
	<?xml version ="1.0" encoding="unicode" ?>
	<Rasterizer name=Default cull="none" fill="fill" front="ccw" depthclip="true" scissor="false"  antialis_line="true"  multisample="true" >
	   <depthbias  value = ""  clamp = "" scale="" />
	</Rasterizer>
	*/
	m_name = node->value(L"name");
    D3D10_RASTERIZER_DESC desc;

	desc.FillMode = D3D10_FILL_SOLID;
	desc.CullMode =  D3D10_CULL_BACK;
	desc.FrontCounterClockwise = FALSE;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthClipEnable = TRUE;
	desc.ScissorEnable = FALSE;
	desc.MultisampleEnable = FALSE;
	desc.AntialiasedLineEnable = FALSE;
	desc.FillMode = xD10ConstLexer::GetFillMode( node->value(L"fill"));
	desc.CullMode = xD10ConstLexer::GetCullMode( node->value(L"cull"));
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

	m_pD10API->d10Device()->CreateRasterizerState(&desc,&m_pD10RasterizerState);
	return m_pD10RasterizerState != NULL;
}

xD10RazState::xD10RazState(xD3D10RenderApi* pAPI  )
:IRasterizerState(pAPI)
{
	m_pD10API = pAPI;
	m_pD10RasterizerState = NULL;
}


xD10RazState::~xD10RazState()
{
	_destory();
}

bool xD10RazState::load(const wchar_t* fileName , unsigned long  arg)
{
	xXmlDocument doc;
	if(false == doc.load(fileName , true) )
		return false;
	return _load(doc.root() );
}

bool xD10RazState::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	if(buf == 0 || bufLen == 0)
		return load(fileName , arg);
	xXmlDocument doc;
	if(false == doc.load((const wchar_t* )buf, bufLen ,  true) )
		return false;
	return _load(doc.root() );
}

bool xD10RazState::isLoaded()
{
	return m_pD10RasterizerState != NULL;
}

bool xD10RazState::unload()
{
	return _destory();
}

unsigned long xD10RazState::memUsage()
{
	return 1;
}


END_NAMESPACE_XEVOL3D
