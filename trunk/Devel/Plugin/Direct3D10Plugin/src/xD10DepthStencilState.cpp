#include "xD10DepthStencilState.h"
#include "xD10ConstLexer.h"
#include "xDirect3D10API.h"
#include <BaseLib/xXmlDocument.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD10ZStencilState  , IDepthStencilState);
bool xD10ZStencilState::_destory()
{
	m_RefCount=1;
	XSAFE_RELEASE(m_pD10RasterizerState);
	m_pD10RasterizerState = NULL;
	return true;
}

void fillStencilOp(D3D10_DEPTH_STENCILOP_DESC& opDesc, xXmlNode* pOpNode)
{
	if(pOpNode == NULL)
	{
		opDesc.StencilFunc        = D3D10_COMPARISON_ALWAYS;
		opDesc.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
		opDesc.StencilFailOp      = D3D10_STENCIL_OP_KEEP;
		opDesc.StencilPassOp      = D3D10_STENCIL_OP_KEEP;
		return ;
	}
	opDesc.StencilFunc        = xD10ConstLexer::GetCompareFunc( pOpNode->value(L"compare") );
	opDesc.StencilDepthFailOp = xD10ConstLexer::GetStencilOp( pOpNode->value(L"zfial") );
	opDesc.StencilFailOp      = xD10ConstLexer::GetStencilOp( pOpNode->value(L"fail") );
	opDesc.StencilPassOp      = xD10ConstLexer::GetStencilOp( pOpNode->value(L"pass") );
}


bool xD10ZStencilState::_load(xXmlNode* node)
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
    D3D10_DEPTH_STENCIL_DESC desc;
	desc.DepthEnable             = true;
	desc.DepthWriteMask          = D3D10_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc               = D3D10_COMPARISON_LESS ;
	m_stecilref                   = 1;
	desc.StencilEnable            = false;
	desc.StencilReadMask          = D3D10_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask         = D3D10_DEFAULT_STENCIL_WRITE_MASK;
	fillStencilOp(desc.FrontFace , NULL);
	fillStencilOp(desc.BackFace  , NULL);

	//----------------------------------
	xXmlNode* pNode = node->findNode(L"depth");
	if(pNode)
	{
		desc.DepthEnable             = pNode->bool_value(L"enable");
		desc.DepthWriteMask          = (D3D10_DEPTH_WRITE_MASK)pNode->int_value(L"writemask");
		desc.DepthFunc               = xD10ConstLexer::GetCompareFunc( pNode->value(L"compare") );
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
	m_pD10API->d10Device()->CreateDepthStencilState(&desc,&m_pD10RasterizerState);
	return m_pD10RasterizerState != NULL;
}



xD10ZStencilState::xD10ZStencilState(xD3D10RenderApi* pAPI  )
:IDepthStencilState(pAPI)
{
	m_pD10API = pAPI;
	m_pD10RasterizerState = NULL;
}


xD10ZStencilState::~xD10ZStencilState()
{
	_destory();
}

bool xD10ZStencilState::load(const wchar_t* fileName , unsigned long  arg)
{
	xXmlDocument doc;
	if(false == doc.load(fileName , true) )
		return false;
	return _load(doc.root() );
}

bool xD10ZStencilState::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	if(buf == 0 || bufLen == 0)
		return load(fileName , arg);
	xXmlDocument doc;
	if(false == doc.load((const wchar_t* )buf, bufLen ,  true) )
		return false;
	return _load(doc.root() );
}

bool xD10ZStencilState::isLoaded()
{
	return m_pD10RasterizerState != NULL;
}

bool xD10ZStencilState::unload()
{
	return _destory();
}

unsigned long xD10ZStencilState::memUsage()
{
	return 1;
}


END_NAMESPACE_XEVOL3D
