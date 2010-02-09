#include "xGL2BlendState.h"
#include "xGL2ConstLexer.h"
#include <BaseLib/xXmlDocument.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xGL2BlendState  , IBlenderState);
bool xGL2BlendState::_destory()
{
	m_RefCount=1;
	return true;
}

float* xGL2BlendState::factor()
{
	return (float*)&m_color;
}

bool xGL2BlendState::_load(xXmlNode* node)
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
	m_Desc.SrcBlend  = GL_ONE;
	m_Desc.DestBlend = GL_ZERO;
	m_Desc.BlendOp   = GL_ADD;
	m_Desc.SrcBlendAlpha  = GL_ONE;
	m_Desc.DestBlendAlpha = GL_ZERO;
	m_Desc.BlendOpAlpha   = GL_ADD;
	xXmlNode* pNode = node->findNode(L"color");
	if(pNode)
	{
		m_Desc.SrcBlend  = xGL2ConstLexer::GetBlendFactor(pNode->value(L"src"));
		m_Desc.DestBlend = xGL2ConstLexer::GetBlendFactor(pNode->value(L"dest"));
		m_Desc.BlendOp   = xGL2ConstLexer::GetBlendOp(pNode->value(L"op"));
	}
	m_Desc.AlphaToCoverageEnable = node->bool_value(L"AlphaToCoverage");
	m_mask = node->hex_value(L"mask");
	pNode = node->findNode(L"alpha");
	if(pNode)
	{
		m_Desc.SrcBlendAlpha  = xGL2ConstLexer::GetBlendFactor(pNode->value(L"src"));
		m_Desc.DestBlendAlpha = xGL2ConstLexer::GetBlendFactor(pNode->value(L"dest"));
		m_Desc.BlendOpAlpha   = xGL2ConstLexer::GetBlendOp(pNode->value(L"op"));
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
	for(size_t i = 0 ; i < nodes.size() ; i ++)
	{
		xXmlNode* pRTNode = nodes[i];
		int  idx = pRTNode->int_value(L"index");
		bool bEnable = pRTNode->bool_value(L"blend");
		int  mask    = pRTNode->hex_value(L"write");
		m_Desc.RenderTargetWriteMask[idx] = mask;
		m_Desc.BlendEnable[idx] = bEnable;
	}
	
	return true;
}


xGL2BlendState::xGL2BlendState(IRenderApi* pAPI  )
:IBlenderState(pAPI)
{
	m_pGL2Api = pAPI;
}

xGL2BlendState::~xGL2BlendState()
{
	_destory();
}

bool xGL2BlendState::load(const wchar_t* fileName , unsigned long  arg)
{
	xXmlDocument doc;
	if(false == doc.load(fileName , true) )
		return false;
	return _load(doc.root() );
}

bool xGL2BlendState::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	if(buf == 0 || bufLen == 0)
		return load(fileName , arg);
	xXmlDocument doc;
	if(false == doc.load((const wchar_t* )buf, bufLen ,  true) )
		return false;
	return _load(doc.root() );
}

bool xGL2BlendState::isLoaded()
{
	return true;
}

bool xGL2BlendState::unload()
{
	return _destory();
}

unsigned long xGL2BlendState::memUsage()
{
	return 1;
}

void xGL2BlendState::apply()
{
     if(m_Desc.BlendEnable[0])
     {
         glEnable(GL_BLEND);
         glBlendEquationSeparate(m_Desc.BlendOp , m_Desc.BlendOpAlpha);
         glBlendFuncSeparate(m_Desc.SrcBlend , m_Desc.DestBlend , m_Desc.SrcBlendAlpha , m_Desc.DestBlendAlpha);
     }
     else
     {
         glDisable(GL_BLEND);
     }
     
     xColor_4ub cl ;
     cl.color = m_Desc.RenderTargetWriteMask[0];
     glColorMask(cl.r != 0 , cl.g != 0 , cl.b != 0 , cl.a != 0);
}

END_NAMESPACE_XEVOL3D
