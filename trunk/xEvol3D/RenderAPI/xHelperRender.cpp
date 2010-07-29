#include "../xStdPch.h"
#include "xHelperRender.h"
#include "xMemBuffer.h"
#include "xRenderAPI.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D

xHelpRender::xHelpRender(IRenderApi* pRenderApi)
{
     m_pRenderApi  = pRenderApi;
	 m_pAssembler  = NULL;
	 m_pVertStream = NULL;
	 m_type = ePrimtiveType_Unknown;
}

void xHelpRender::begin(ePrimtiveType _type)
{
	//在begin和end之间不能嵌套begin
	if(m_type != ePrimtiveType_Unknown)
		return ;
    m_type = _type;
}

bool  xHelpRender::queryInputBuffer()
{
	if(m_pInputBuffer)
	{
		int len = (int)m_pInputBuffer->bufLen();
		int lenNeed = (int)m_Vertexs.size() * (int)sizeof(xHelperVertex);
		if(lenNeed <= len)
		{
			m_pInputBuffer->update(eLock_WriteDiscard , (void*)&m_Vertexs[0] , lenNeed ) ;
			m_pVertStream->setInputBuffer(0 , m_pInputBuffer);
			return true;
		}

		m_pInputBuffer->ReleaseObject();
	}
	m_pInputBuffer = m_pVertStream->createInputBuffer(0 , m_Vertexs.size() , (void*)&m_Vertexs[0] );
	return true;
}

void xHelpRender::end()
{
	//没有调用begin了。
	if(m_type == ePrimtiveType_Unknown)
		return ;

    queryInputBuffer();
	m_pRenderApi->setVertexStream(m_pVertStream);
	m_pRenderApi->draw(m_Vertexs.size() , 0 , m_type);
	
	m_Vertexs.clear();
	m_type = ePrimtiveType_Unknown;
}

void xHelpRender::vertex3f(float x , float y , float z)
{
	xvec4  vert(x , y , z , 1.0f);
	vertex4v( vert.v);
}

void xHelpRender::vertex4v(float v[4])
{
	xvec4& vert = *((xvec4*)v);
	xHelperVertex vertex;
	vertex.pos  = vert;
	vertex.nor = m_Normal;
	vertex.tan = m_Tangent;
	vertex.tex = m_TexCoord;
    m_Vertexs.push_back(vertex);
}

void xHelpRender::normal3f(float x , float y , float z)
{
	m_Normal.x = x;
	m_Normal.y = y;
	m_Normal.z = z;
	m_Normal.w = 1.0f;
}

void xHelpRender::normal4v(float v[4])
{
    m_Normal.x = v[0];
	m_Normal.y = v[1];
	m_Normal.z = v[2];
	m_Normal.w = v[3];
}

void xHelpRender::texCoord2f(float u , float v)
{
    m_TexCoord.x = u;
	m_TexCoord.y = v;
	m_TexCoord.z = 1.0f;
	m_TexCoord.w = 1.0f;
}

void xHelpRender::texCoord3f(float u , float v , float s )
{
	m_TexCoord.x = u;
	m_TexCoord.y = v;
	m_TexCoord.z = s;
	m_TexCoord.w = 1.0f;
}

void xHelpRender::texCoord4f(float u , float v , float s , float t)
{
	m_TexCoord.x = u;
	m_TexCoord.y = v;
	m_TexCoord.z = s;
	m_TexCoord.w = t;
}

void xHelpRender::texCoord2v(float v[2])
{
	m_TexCoord.x = v[0];
	m_TexCoord.y = v[1];
	m_TexCoord.z = 1.0f;
	m_TexCoord.w = 1.0f;
}

void xHelpRender::texCoord3v(float v[3])
{
	m_TexCoord.x = v[0];
	m_TexCoord.y = v[1];
	m_TexCoord.z = v[2];
	m_TexCoord.w = 1.0f;
}
void xHelpRender::texCoord4v(float v[4])
{
	m_TexCoord.x = v[0];
	m_TexCoord.y = v[1];
	m_TexCoord.z = v[2];
	m_TexCoord.w = v[3];
}

void xHelpRender::drawLine(xMathLib::xvec4* pVertex , int nVertex , xColor_4f& cl)
{

}


END_NAMESPACE_XEVOL3D
