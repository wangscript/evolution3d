#include "stdafx.h"
#include "xD3D9DepthStencilState.h"
#include "xD3D9ConstLexer.h"
#include "xDIRECT3D9API.h"
#include <BaseLib/xXmlDocument.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD3D9ZStencilState  , IDepthStencilState);
bool xD3D9ZStencilState::_destory()
{
	m_RefCount=1;
	m_Stencil.m_Ref = 0;
	return true;
}

void fillStencilOp(xStencilOp& opDesc, xXmlNode* pOpNode)
{
	if(pOpNode == NULL)
	{
		return ;
	}
    opDesc.m_Func      = xD3D9ConstLexer::GetCompareFunc( pOpNode->value(L"compare") );
	opDesc.m_zFail     = xD3D9ConstLexer::GetStencilOp( pOpNode->value(L"zfial") );
	opDesc.m_Fail      = xD3D9ConstLexer::GetStencilOp( pOpNode->value(L"fail") );
	opDesc.m_Pass      = xD3D9ConstLexer::GetStencilOp( pOpNode->value(L"pass") );
}


bool xD3D9ZStencilState::_load(xXmlNode* node)
{
	/*
	<?xml version ="1.0" encoding="unicode" ?>
	<ZStencil name=Default   >
	   <depth    enable="true" writeable="true" compare = "less" />
	   <stencil ref="1" enable="true" readmask ="0xff" writemask="0xff" >
	       <front fail="keep"  zfail="keep" pass="keep" compare = "always"/>
		   <back  fail="keep"  zfail="keep" pass="keep" compare = "always"/>
	   </stencil>
	</ZStencil>
	*/
	m_name = node->value(L"name");

	//----------------------------------
	xXmlNode* pNode = node->findNode(L"depth");
	if(pNode)
	{
		m_zEnable             = pNode->bool_value(L"enable");
		m_zWriteable          = pNode->bool_value(L"writeable");
		m_zFunc               = xD3D9ConstLexer::GetCompareFunc( pNode->value(L"compare") );
	}
    pNode = node->findNode(L"stencil");

	if(pNode)
	{
		m_Stencil.m_Ref               = pNode->int_value(L"ref");
		m_Stencil.m_Enable            = pNode->bool_value(L"enable");
		m_Stencil.m_Mask              = pNode->hex_value(L"readmask");
		m_Stencil.m_WriteMask         = pNode->hex_value(L"writemask");
        xXmlNode* pOpNode = pNode->findNode(L"cw");
		fillStencilOp(m_Stencil.m_OpCW   , pOpNode);
		pOpNode = pNode->findNode(L"ccw");
		fillStencilOp(m_Stencil.m_OpCCW  , pOpNode);
	}
	return true;
}

#define GUID_VALUE(v)  if(v != pOldStencil-> v ) 
void xD3D9ZStencilState::Install(xD3D9ZStencilState* pOldStencil)
{
    IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();
    if(pOldStencil) 
    {
        GUID_VALUE(m_zEnable            ) pDevice->SetRenderState(D3DRS_ZENABLE          , m_zEnable         );
        GUID_VALUE(m_zWriteable         ) pDevice->SetRenderState(D3DRS_ZWRITEENABLE     , m_zWriteable      );
        GUID_VALUE(m_zFunc              ) pDevice->SetRenderState(D3DRS_ZFUNC            , m_zFunc           );
        GUID_VALUE(m_Stencil.m_Enable   ) pDevice->SetRenderState(D3DRS_STENCILENABLE    , m_Stencil.m_Enable   );
        GUID_VALUE(m_Stencil.m_Ref      ) pDevice->SetRenderState(D3DRS_STENCILREF       , m_Stencil.m_Ref      );
        GUID_VALUE(m_Stencil.m_Mask     ) pDevice->SetRenderState(D3DRS_STENCILMASK      , m_Stencil.m_Mask     );
        GUID_VALUE(m_Stencil.m_WriteMask) pDevice->SetRenderState(D3DRS_STENCILWRITEMASK , m_Stencil.m_WriteMask);
        
        GUID_VALUE(m_Stencil.m_OpCW.m_Fail )  pDevice->SetRenderState(D3DRS_STENCILFAIL  , m_Stencil.m_OpCW.m_Fail );
        GUID_VALUE(m_Stencil.m_OpCW.m_zFail)  pDevice->SetRenderState(D3DRS_STENCILZFAIL , m_Stencil.m_OpCW.m_zFail);
        GUID_VALUE(m_Stencil.m_OpCW.m_Pass )  pDevice->SetRenderState(D3DRS_STENCILPASS  , m_Stencil.m_OpCW.m_Pass );
        GUID_VALUE(m_Stencil.m_OpCW.m_Func )  pDevice->SetRenderState(D3DRS_STENCILFUNC  , m_Stencil.m_OpCW.m_Func );

        GUID_VALUE(m_Stencil.m_OpCCW.m_Fail )  pDevice->SetRenderState(D3DRS_CCW_STENCILFAIL  , m_Stencil.m_OpCCW.m_Fail );
        GUID_VALUE(m_Stencil.m_OpCCW.m_zFail)  pDevice->SetRenderState(D3DRS_CCW_STENCILZFAIL , m_Stencil.m_OpCCW.m_zFail);
        GUID_VALUE(m_Stencil.m_OpCCW.m_Pass )  pDevice->SetRenderState(D3DRS_CCW_STENCILPASS  , m_Stencil.m_OpCCW.m_Pass );
        GUID_VALUE(m_Stencil.m_OpCCW.m_Func )  pDevice->SetRenderState(D3DRS_CCW_STENCILFUNC  , m_Stencil.m_OpCCW.m_Func );
    }
    else
    {
        pDevice->SetRenderState(D3DRS_ZENABLE          , m_zEnable         );
        pDevice->SetRenderState(D3DRS_ZWRITEENABLE     , m_zWriteable      );
        pDevice->SetRenderState(D3DRS_ZFUNC            , m_zFunc           );

        pDevice->SetRenderState(D3DRS_STENCILENABLE    , m_Stencil.m_Enable   );
        pDevice->SetRenderState(D3DRS_STENCILREF       , m_Stencil.m_Ref      );
        pDevice->SetRenderState(D3DRS_STENCILMASK      , m_Stencil.m_Mask     );
        pDevice->SetRenderState(D3DRS_STENCILWRITEMASK , m_Stencil.m_WriteMask);

        pDevice->SetRenderState(D3DRS_STENCILFAIL      , m_Stencil.m_OpCW.m_Fail );
        pDevice->SetRenderState(D3DRS_STENCILZFAIL     , m_Stencil.m_OpCW.m_zFail);
        pDevice->SetRenderState(D3DRS_STENCILPASS      , m_Stencil.m_OpCW.m_Pass );
        pDevice->SetRenderState(D3DRS_STENCILFUNC      , m_Stencil.m_OpCW.m_Func );

        pDevice->SetRenderState(D3DRS_CCW_STENCILFAIL  , m_Stencil.m_OpCCW.m_Fail );
        pDevice->SetRenderState(D3DRS_CCW_STENCILZFAIL , m_Stencil.m_OpCCW.m_zFail);
        pDevice->SetRenderState(D3DRS_CCW_STENCILPASS  , m_Stencil.m_OpCCW.m_Pass );
        pDevice->SetRenderState(D3DRS_CCW_STENCILFUNC  , m_Stencil.m_OpCCW.m_Func );
    }
}


xD3D9ZStencilState::xD3D9ZStencilState(xD3D9RenderApi* pAPI  )
:IDepthStencilState(pAPI)
{
	m_pD3D9RenderApi = pAPI;
    m_zEnable       = D3DZB_TRUE ; //D3DRS_ZENABLE;
    m_zWriteable    = TRUE; //D3DRS_ZWRITEENABLE;
    m_zFunc         = D3DCMP_LESSEQUAL; //D3DRS_ZFUNC ;

}


xD3D9ZStencilState::~xD3D9ZStencilState()
{
	_destory();
}

bool xD3D9ZStencilState::load(const wchar_t* fileName , unsigned long  arg)
{
	xXmlDocument doc;
	if(false == doc.load(fileName , true) )
		return false;
	return _load(doc.root() );
}

bool xD3D9ZStencilState::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	if(buf == 0 || bufLen == 0)
		return load(fileName , arg);
	xXmlDocument doc;
	if(false == doc.load((const wchar_t* )buf, bufLen ,  true) )
		return false;
	return _load(doc.root() );
}

bool xD3D9ZStencilState::isLoaded()
{
	return true;
}

bool xD3D9ZStencilState::unload()
{
	return _destory();
}

unsigned long xD3D9ZStencilState::memUsage()
{
	return 1;
}


END_NAMESPACE_XEVOL3D
