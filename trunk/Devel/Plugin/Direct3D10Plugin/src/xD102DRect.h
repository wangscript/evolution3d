#ifndef _XEVOL_DIRECT3D10_2DRECT_API_
#define _XEVOL_DIRECT3D10_2DRECT_API_
#include <d3d10.h>
#include <map>
#include <RenderApi/xRenderApi.h>
BEGIN_NAMESPACE_XEVOL3D
class xD10GpuProgram;
class xD10VertexStream;
class xD3D10RenderApi;
#define MAX_UV_CHANEL   2
#define MAX_2DTEXTURE   4
class xD102DRectEnv
{
	friend class xD102DRect;
protected:
	IVertexStream*   m_pVertexStream;
	IInputBuffer*    m_pIdxBuffer;
	IInputAssembler* m_pAss[MAX_UV_CHANEL + 1];
	xD3D10RenderApi* m_pRenderApi;
public:
	xD102DRectEnv(xD3D10RenderApi* pApi);
	~xD102DRectEnv();
	bool        create();
	float       zValue();
};

class xD102DRect : public I2DRectObject
{
public:
	struct xD102DVertex
	{
		xvec4       m_pos;
		xColor_4f   m_color;
		xvec2       m_uv[MAX_UV_CHANEL];
	};
public:
	xD102DRect(xD102DRectEnv*  pRectEnv);
	~xD102DRect();
	void             setUVChanel(int iUVChanel);
	int              nUVChanel();

	void             setColor(xColor_4f& cl){m_cl = cl;}
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
	xD102DVertex     m_Vertex[4];
	IInputBuffer*    m_pVertexBuffer;
	xD102DRectEnv*   m_pRectEnv;
public:
};

END_NAMESPACE_XEVOL3D

#endif

