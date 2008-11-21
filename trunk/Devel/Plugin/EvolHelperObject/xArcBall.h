#ifndef _X_ARCBALL_HELPER_H_
#define _X_ARCBALL_HELPER_H_

#include <RenderApi/xRenderApi.h>
#include <RenderAPI/xBaseShader.h>
#include <RenderAPI/xShaderNode.h>
#include <RenderAPI/xShaderManager.h>
#include <tchar.h>
#include <XMathLib/xMathlib.h>
#include <Image/xILImage.h>
#include <RenderAPI/xBaseTexture.h>
#include "Renderer/xDrawableHelper.h"
#include "xModel/xBaseModel.h"
#include "../xPluginBaseInc.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_PLUGIN_API_ xArcBallDrawable : public IDrawableHelper
{
	IVertexStream*   m_pVertexStream;
	IInputBuffer*    m_pIdxBuffer;
	xInputLayoutDesc m_InputDesc;
	IInputAssembler* m_pAss;
	IRenderApi*      m_pRenderApi;
	IBaseTexture*    m_pTexture;
	IGpuProgram*     m_pGpuProgram;

	IBaseTexture*    m_pMaskTexture;
	IBlenderState*   m_pBlendState;

	float            m_Radios;
	int              m_nSegement;	
	IMPL_BASE_OBJECT_INTERFACE(xArcBallDrawable);

public:
	enum PLANE_AXIS
	{
		XY_PLANE = 0,
		YZ_PLANE = 1,
		XZ_PLANE = 2,
	};
public:
	xArcBallDrawable();
	~xArcBallDrawable();
	bool           init(IRenderApi* pRenderApi , xBaseTextureMgr* pTexMgr);
	bool           begin();
	bool           end();

	void           center(XMathLib::xvec3&   _center);
	void           aabb(xGeomLib::xaabb&     _aabb);
	void           shpere(xGeomLib::xshpere& _shpere);
	IRenderApi*    renderApi() { return m_pRenderApi ; }
	void           render(unsigned long passedTime);
};

END_NAMESPACE_XEVOL3D

#endif

