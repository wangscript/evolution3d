#include "stdafx.h"
#include "xD11DepthStencilState.h"
#include "xD11ConstLexer.h"
#include "xDirect3D11API.h"
#include <BaseLib/xXmlDocument.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD11ZStencilState  , IDepthStencilState);
bool xD11ZStencilState::_destory()
{
	m_RefCount=1;
	XSAFE_RELEASE(m_pD10RasterizerState);
	m_pD10RasterizerState = NULL;
	return true;
}

void fillStencilOp(D3D11_DEPTH_STENCILOP_DESC& opDesc, xXmlNode* pOpNode)
{
	if(pOpNode == NULL)
	{
		opDesc.StencilFunc        = D3D11_COMPARISON_ALWAYS;
		opDesc.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		opDesc.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
		opDesc.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
		return ;
	}
	opDesc.StencilFunc        = xD11ConstLexer::GetCompareFunc( pOpNode->value(L"compare") );
	opDesc.StencilDepthFailOp = xD11ConstLexer::GetStencilOp( pOpNode->value(L"zfial") );
	opDesc.StencilFailOp      = xD11ConstLexer::GetStencilOp( pOpNode->value(L"fail") );
	opDesc.StencilPassOp      = xD11ConstLexer::GetStencilOp( pOpNode->value(L"pass") );
}


bool xD11ZStencilState::_load(xXmlNode* node)
{
	/*
	<?xml version ="1.0" encoding="unicode" ?>
	<ZStencil name=Default   >
	   <depth    enable="true" writemask="1" compare = "less" />
	   <stencil ref="1" enable="true" readmask ="0xff" writemask="0xff" >
	       <front fail="keep"  zfail="keep" pass="keep" />
		   <back  fail="keep"  zfail="keep" pass="keep" />
	   </stencil>
	</ZStencil>
	*/
	m_name = node->value(L"name");
    D3D11_DEPTH_STENCIL_DESC desc;
	desc.DepthEnable             = true;
	desc.DepthWriteMask          = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc               = D3D11_COMPARISON_LESS ;
	m_stecilref                   = 1;
	desc.StencilEnable            = false;
	desc.StencilReadMask          = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask         = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	fillStencilOp(desc.FrontFace , NULL);
	fillStencilOp(desc.BackFace  , NULL);

	//----------------------------------
	xXmlNode* pNode = node->findNode(L"depth");
	if(pNode)
	{
		desc.DepthEnable             = pNode->bool_value(L"enable");
		desc.DepthWriteMask          = (D3D11_DEPTH_WRITE_MASK)pNode->int_value(L"writemask");
		desc.DepthFunc               = xD11ConstLexer::GetCompareFunc( pNode->value(L"compare") );
	}
    pNode = node->findNode(L"stencil");

	if(pNode)
	{
		m_stecilref                   = pNode->int_value(L"ref");
		desc.StencilEnable            = pNode->bool_value(L"enable");
		desc.StencilReadMask          = pNode->hex_value(L"readmask");
		desc.StencilWriteMask         = pNode->hex_value(L"writemask");
        xXmlNode* pOpNode = pNode->findNode(L"front");
		fillStencilOp(desc.FrontFace , pOpNode);
		pOpNode = pNode->findNode(L"back");
		fillStencilOp(desc.BackFace  , pOpNode);
	}
	m_pD11RenderAPI->d11Device()->CreateDepthStencilState(&desc,&m_pD10RasterizerState);
	return m_pD10RasterizerState != NULL;
}



xD11ZStencilState::xD11ZStencilState(xD3D11RenderApi* pAPI  )
:IDepthStencilState(pAPI)
{
	m_pD11RenderAPI = pAPI;
	m_pD10RasterizerState = NULL;
}


xD11ZStencilState::~xD11ZStencilState()
{
	_destory();
}

bool xD11ZStencilState::load(const wchar_t* fileName , unsigned long  arg)
{
	xXmlDocument doc;
	if(false == doc.load(fileName , true) )
		return false;
	return _load(doc.root() );
}

bool xD11ZStencilState::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	if(buf == 0 || bufLen == 0)
		return load(fileName , arg);
	xXmlDocument doc;
	if(false == doc.load((const wchar_t* )buf, bufLen ,  true) )
		return false;
	return _load(doc.root() );
}

bool xD11ZStencilState::isLoaded()
{
	return m_pD10RasterizerState != NULL;
}

bool xD11ZStencilState::unload()
{
	return _destory();
}

unsigned long xD11ZStencilState::memUsage()
{
	return 1;
}


END_NAMESPACE_XEVOL3D
