#include "xGL2RazState.h"
#include "xGL2ConstLexer.h"
#include "xOpenGL2API.h"
#include <BaseLib/xXmlDocument.h>

BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xGL2RazState  , IRasterizerState);
bool xGL2RazState::_destory()
{
	m_RefCount=1;
	XSAFE_RELEASE(m_pD10RasterizerState);
	m_pD10RasterizerState = NULL;
	return true;
}


bool xGL2RazState::_load(xXmlNode* node)
{


	/*
	<?xml version ="1.0" encoding="unicode" ?>
	<Rasterizer name=Default cull="none" fill="fill" front="ccw" depthclip="true" scissor="false"  antialis_line="true"  multisample="true" >
	   <depthbias  value = ""  clamp = "" scale="" />
	</Rasterizer>
	*/
	m_name = node->value(L"name");

	m_Desc.FillMode = GL_FILL;
	m_Desc.CullMode =  GL_BACK;
	m_Desc.FrontCounterClockwise = GL_TRUE;
	m_Desc.DepthBias = 0;
	m_Desc.DepthBiasClamp = 0.0f;
	m_Desc.SlopeScaledDepthBias = 0.0f;
	m_Desc.DepthClipEnable = GL_TRUE;
	m_Desc.ScissorEnable = GL_FALSE;
	m_Desc.MultisampleEnable = GL_FALSE;
	m_Desc.AntialiasedLineEnable = GL_FALSE;
	m_Desc.FillMode = xGL2ConstLexer::GetFillMode( node->value(L"fill"));
	m_Desc.CullMode = xGL2ConstLexer::GetCullMode( node->value(L"cull"));
	ds_wstring front = node->value(L"front");
	if(front == L"ccw") m_Desc.FrontCounterClockwise = true;
	else m_Desc.FrontCounterClockwise = false;

	xXmlNode* pNode = node->findNode(L"depthbias");
	if(pNode)
	{
		m_Desc.DepthBias             = pNode->int_value(L"value");
		m_Desc.DepthBiasClamp        = pNode->float_value(L"clamp");
		m_Desc.SlopeScaledDepthBias  = pNode->float_value(L"scale");
	}

	m_Desc.DepthClipEnable = node->bool_value(L"depthclip");
	m_Desc.ScissorEnable = node->bool_value(L"scissor");
	m_Desc.MultisampleEnable = node->bool_value(L"multisample");
	m_Desc.AntialiasedLineEnable = node->bool_value(L"antialis_line");

	return true;
}

xGL2RazState::xGL2RazState(IRenderApi* pAPI  )
:IRasterizerState(pAPI)
{
	m_pGL2Api = pAPI;
}


xGL2RazState::~xGL2RazState()
{
	_destory();
}

bool xGL2RazState::load(const wchar_t* fileName , unsigned long  arg)
{
	xXmlDocument doc;
	if(false == doc.load(fileName , true) )
		return false;
	return _load(doc.root() );
}

bool xGL2RazState::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	if(buf == 0 || bufLen == 0)
		return load(fileName , arg);
	xXmlDocument doc;
	if(false == doc.load((const wchar_t* )buf, bufLen ,  true) )
		return false;
	return _load(doc.root() );
}

bool xGL2RazState::isLoaded()
{
	return true;
}

bool xGL2RazState::unload()
{
	return _destory();
}

unsigned long xGL2RazState::memUsage()
{
	return 1;
}

void xGL2RazState::apply()
{
    glPolygonMode(GL_FRONT_AND_BACK , m_Desc.FillMode);
    if(m_Desc.CullMode == GL_NONE)
    {
        glDisable(GL_CULL_FACE);
    }
    else
    {
        glFrontFace(m_Desc.FrontCounterClockwise ? GL_CCW : GL_CW);
        glEnable(GL_CULL_FACE);
        glCullFace(m_Desc.CullMode);
    }

}

END_NAMESPACE_XEVOL3D
