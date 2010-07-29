#ifndef _X_AXIS_HELPER_H_
#define _X_AXIS_HELPER_H_

#include <RenderApi/xRenderApi.h>
#include <RenderAPI/xBaseShader.h>
#include <RenderAPI/xShaderNode.h>
#include <RenderAPI/xShaderManager.h>
#include <tchar.h>
#include <XMathLib/xMathlib.h>
#include <Image/xILImage.h>
#include <RenderAPI/xBaseTexture.h>
#include "Renderer/xDrawableHelper.h"
#include "../xPluginBaseInc.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_PLUGIN_API_ xAxisDrawable : public IDrawableHelper
{
	IVertexStream*   m_pVertexStream;
	IInputBuffer*    m_pIdxBuffer;
	xInputLayoutDesc m_InputDesc;
	IInputAssembler* m_pAss;
	IRenderApi*      m_pRenderApi;
	IBaseTexture*    m_pTexture;
	HGpuProgram      m_hProgram;

	IBaseTexture*    m_pMaskTexture;
	IBlenderState*   m_pBlendState;
	IDepthStencilState* m_pDepthState;

	float            m_Radios;
	IMPL_BASE_OBJECT_INTERFACE(xAxisDrawable);

public:
	enum PLANE_AXIS
	{
		X_AXIS = 0,
		Y_AXIS = 1,
		Z_AXIS = 2,
	};
public:
	xAxisDrawable(IBaseRenderer* pRenderer, int arg);
	~xAxisDrawable();
	bool           init(xBaseTextureMgr* pTexMgr);
	bool           begin();
	bool           end();

	void           center(XMathLib::xvec3&   _center);
	void           aabb(xGeomLib::xaabb&     _aabb);
	void           shpere(xGeomLib::xshpere& _shpere);
	IRenderApi*    renderApi() { return m_pRenderApi ; }
	bool           render(unsigned long passedTime);
};

END_NAMESPACE_XEVOL3D

#endif

