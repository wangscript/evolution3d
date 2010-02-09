#include "xGL2DepthStencilState.h"
#include "xGL2ConstLexer.h"
#include <BaseLib/xXmlDocument.h>

BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(xGL2ZStencilState  , IDepthStencilState);
bool xGL2ZStencilState::_destory()
{
	m_RefCount=1;
	return true;
}

void fillStencilOp(GL2_DEPTH_STENCILOP_DESC& opDesc, xXmlNode* pOpNode)
{
	if(pOpNode == NULL)
	{
		opDesc.StencilFunc        = GL_ALWAYS;
		opDesc.StencilDepthFailOp = GL_KEEP;
		opDesc.StencilFailOp      = GL_KEEP;
		opDesc.StencilPassOp      = GL_KEEP;
		return ;
	}
	opDesc.StencilFunc        = xGL2ConstLexer::GetCompareFunc( pOpNode->value(L"compare") );
	opDesc.StencilDepthFailOp = xGL2ConstLexer::GetStencilOp( pOpNode->value(L"zfial") );
	opDesc.StencilFailOp      = xGL2ConstLexer::GetStencilOp( pOpNode->value(L"fail") );
	opDesc.StencilPassOp      = xGL2ConstLexer::GetStencilOp( pOpNode->value(L"pass") );
}


bool xGL2ZStencilState::_load(xXmlNode* node)
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
	m_Desc.DepthEnable              = true;
	m_Desc.DepthWriteMask           = 1;
	m_Desc.DepthFunc                = GL_LESS ;
	m_stecilref                   = 1;
	m_Desc.StencilEnable            = false;
	m_Desc.StencilReadMask          = 1;
	m_Desc.StencilWriteMask         = 1;
	fillStencilOp(m_Desc.FrontFace , NULL);
	fillStencilOp(m_Desc.BackFace  , NULL);

	//----------------------------------
	xXmlNode* pNode = node->findNode(L"depth");
	if(pNode)
	{
		m_Desc.DepthEnable             = pNode->bool_value(L"enable");
		m_Desc.DepthWriteMask          = pNode->int_value(L"writemask");
		m_Desc.DepthFunc               = xGL2ConstLexer::GetCompareFunc( pNode->value(L"compare") );
	}
    pNode = node->findNode(L"stencil");

	if(pNode)
	{
		m_stecilref                   = pNode->int_value(L"ref");
		m_Desc.StencilEnable            = pNode->bool_value(L"enable");
		m_Desc.StencilReadMask          = pNode->hex_value(L"readmask");
		m_Desc.StencilWriteMask         = pNode->hex_value(L"writemask");
        xXmlNode* pOpNode = pNode->findNode(L"front");
		fillStencilOp(m_Desc.FrontFace , pOpNode);
		pOpNode = pNode->findNode(L"back");
		fillStencilOp(m_Desc.BackFace  , pOpNode);
	}
	return true;
}



xGL2ZStencilState::xGL2ZStencilState(IRenderApi* pAPI  )
:IDepthStencilState(pAPI)
{
	m_pGL2Api = pAPI;
}


xGL2ZStencilState::~xGL2ZStencilState()
{
	_destory();
}

bool xGL2ZStencilState::load(const wchar_t* fileName , unsigned long  arg)
{
	xXmlDocument doc;
	if(false == doc.load(fileName , true) )
		return false;
	return _load(doc.root() );
}

bool xGL2ZStencilState::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	if(buf == 0 || bufLen == 0)
		return load(fileName , arg);
	xXmlDocument doc;
	if(false == doc.load((const wchar_t* )buf, bufLen ,  true) )
		return false;
	return _load(doc.root() );
}

bool xGL2ZStencilState::isLoaded()
{
	return true;
}

bool xGL2ZStencilState::unload()
{
	return _destory();
}

unsigned long xGL2ZStencilState::memUsage()
{
	return 1;
}
void xGL2ZStencilState::apply()
{
    if(m_Desc.StencilEnable)
    {
        glStencilMask(m_Desc.StencilWriteMask);
        if(glActiveStencilFaceEXT)
        {
            glEnable(GL_STENCIL_TEST);
            glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT);
            glActiveStencilFaceEXT(GL_BACK);
            glStencilOp(m_Desc.BackFace.StencilFailOp , m_Desc.BackFace.StencilDepthFailOp , m_Desc.BackFace.StencilPassOp);
            glStencilFunc(m_Desc.BackFace.StencilFunc , m_stecilref , m_Desc.StencilReadMask );

            glActiveStencilFaceEXT(GL_FRONT);
            glStencilOp(m_Desc.FrontFace.StencilFailOp , m_Desc.FrontFace.StencilDepthFailOp , m_Desc.FrontFace.StencilPassOp);
            glStencilFunc(m_Desc.FrontFace.StencilFunc , m_stecilref , m_Desc.StencilReadMask );
        }
        else
        {
            glStencilOp(m_Desc.FrontFace.StencilFailOp , m_Desc.FrontFace.StencilDepthFailOp , m_Desc.FrontFace.StencilPassOp);
            glStencilFunc(m_Desc.FrontFace.StencilFunc , m_stecilref , m_Desc.StencilReadMask );
        }
    }
    else
    {
        glDisable(GL_STENCIL_TEST);
        glDisable(GL_STENCIL_TEST_TWO_SIDE_EXT);
        glActiveStencilFaceEXT(GL_BACK);
    }

    glDepthMask(m_Desc.DepthWriteMask);
    if(m_Desc.DepthEnable)
    {
        glEnable(GL_DEPTH_TEST);        
        glDepthFunc(m_Desc.DepthFunc);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
}

END_NAMESPACE_XEVOL3D
