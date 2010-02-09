#ifndef _XEVOL_HELPER_RENDER_API_H_
#define _XEVOL_HELPER_RENDER_API_H_
#include "xRenderApiDef.h"
#include "xRenderApiFwDecl.h"
#include "XMathLib/xMathLib.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ IHelperRender
{
public:
	virtual void begin(ePrimtiveType _type) = 0;
	virtual void end() = 0;
	virtual void vertex3f(float x , float y , float z) = 0;
	virtual void vertex4v(float v[4]) = 0;
	virtual void normal3f(float x , float y , float z) = 0;
	virtual void normal4v(float v[4]) = 0;
	virtual void texCoord2f(float u , float v) = 0;
	virtual void texCoord3f(float u , float v , float s ) = 0 ;
	virtual void texCoord4f(float u , float v , float s , float t) = 0;
	virtual void texCoord2v(float v[2]) = 0;
	virtual void texCoord3v(float v[3]) = 0;
	virtual void texCoord4v(float v[4]) = 0;

public:
	virtual void drawLine(xMathLib::xvec4* pVertex , int nVertex , xColor_4f& cl) = 0;
};

class _XEVOL_BASE_API_ xHelpRender : public IHelperRender
{
public:
	xHelpRender(IRenderApi* pRenderApi);
	void begin(ePrimtiveType _type);
	void end();
	void vertex3f(float x , float y , float z);
	void vertex4v(float v[4]);
	void normal3f(float x , float y , float z);
	void normal4v(float v[4]);
	void texCoord2f(float u , float v);
	void texCoord3f(float u , float v , float s ) ;
	void texCoord4f(float u , float v , float s , float t);
	void texCoord2v(float v[2]);
	void texCoord3v(float v[3]);
	void texCoord4v(float v[4]);
	void drawLine(xMathLib::xvec4* pVertex , int nVertex , xColor_4f& cl);

protected:
    xMathLib::xvec4 m_Normal;
	xMathLib::xvec4 m_Tangent;
	xMathLib::xvec4 m_TexCoord;

protected:
	struct xHelperVertex
	{
		xMathLib::xvec4 pos;
		xMathLib::xvec4 nor;
		xMathLib::xvec4 tan;
		xMathLib::xvec4 tex;
	};

	typedef ds_vector(xHelperVertex) xHelperVertexs;
	xHelperVertexs   m_Vertexs;
protected:
	IRenderApi*      m_pRenderApi;
	ePrimtiveType    m_type;
protected:
	typedef ds_vector(IInputBuffer*) vInputBuffers;
	vInputBuffers    m_vInputBuffers;
	IInputBuffer*    m_pInputBuffer;
	IInputAssembler* m_pAssembler;
	IVertexStream*   m_pVertStream;

	bool             queryInputBuffer();
};

END_NAMESPACE_XEVOL3D

#endif
