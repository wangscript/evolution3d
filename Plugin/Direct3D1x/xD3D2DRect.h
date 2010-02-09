#ifndef _XEVOL_DIRECT3D1X_2DRECT_API_
#define _XEVOL_DIRECT3D1X_2DRECT_API_
#include <map>
#include <RenderApi/xRenderApi.h>
BEGIN_NAMESPACE_XEVOL3D
class xD3DVertexStream;
using namespace  xMathLib;
#define MAX_UV_CHANEL   2
#define MAX_2DTEXTURE   4
class xD3D2DRectEnv
{
	friend class xD3D2DRect;
public:
	xD3D2DRectEnv(IRenderApi* pApi);
	~xD3D2DRectEnv();
	bool        create();
	float       zValue();
protected:
    IVertexStream*   m_pVertexStream;
    IInputBuffer*    m_pIdxBuffer;
    IInputAssembler* m_pAss[MAX_UV_CHANEL + 1];
    IRenderApi*      m_pRenderApi;
};

class xD3D2DRect : public I2DRectObject
{
public:
	struct xD3D2DVertex
	{
		xvec4       m_pos;
		xColor_4f   m_color;
		xvec2       m_uv[MAX_UV_CHANEL];
	};
public:
    IMPL_REFCOUNT_OBJECT_INTERFACE(xD3D2DRect);
	xD3D2DRect(xD3D2DRectEnv*  pRectEnv);
	~xD3D2DRect();
	void             setUVChanel(int iUVChanel);
	int              nUVChanel();

	void             setColor(const xColor_4f& cl){m_cl = cl;}
	void             setRect(float destRect[4]);
	void             setTexture(int iStage , IBaseTexture* pTexture);
	void             setTexture(int iStage , IBaseTexture* pTexture , float srcRect[4]);
	void             commit();
	void             create();
	bool             draw();

private:
	int              m_nUVChanel;
	IInputAssembler* m_pInputAss;

	bool             m_bDirty;
	xColor_4f        m_cl;
	xvec4            m_rectLD;
	xvec4            m_rectRU;
	xvec4            m_SrcRect[MAX_2DTEXTURE];
	IBaseTexture*    m_Texture[MAX_2DTEXTURE];
	xD3D2DVertex     m_Vertex[4];
	IInputBuffer*    m_pVertexBuffer;
	xD3D2DRectEnv*   m_pRectEnv;
public:
};

END_NAMESPACE_XEVOL3D

#endif

