#ifndef _XEVOL_OPENGL20_2DRECT_API_
#define _XEVOL_OPENGL20_2DRECT_API_
#include <d3d10.h>
#include <map>
#include <RenderApi/xRenderApi.h>
BEGIN_NAMESPACE_XEVOL3D
class xGL2GpuProgram;
class xGL2VertexStream;
class xGL2RenderApi;
#define MAX_UV_CHANEL   2
#define MAX_2DTEXTURE   4
class xGL2Rect2DEnv
{
	friend class xGL2Rect2D;
protected:
	IVertexStream*   m_pVertexStream;
	IInputBuffer*    m_pIdxBuffer;
	IInputAssembler* m_pAss[MAX_UV_CHANEL + 1];
	xGL2RenderApi* m_pRenderApi;
public:
	xGL2Rect2DEnv(xGL2RenderApi* pApi);
	~xGL2Rect2DEnv();
	bool        create();
	float       zValue();
};

class xGL2Rect2D : public I2DRectObject
{
public:
	struct xGL22DVertex
	{
		xvec4       m_pos;
		xColor_4f   m_color;
		xvec2       m_uv[MAX_UV_CHANEL];
	};
public:
	xGL2Rect2D(xGL2Rect2DEnv*  pRectEnv);
	~xGL2Rect2D();
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
	xGL22DVertex     m_Vertex[4];
	IInputBuffer*    m_pVertexBuffer;
	xGL2Rect2DEnv*   m_pRectEnv;
public:
};

END_NAMESPACE_XEVOL3D

#endif

