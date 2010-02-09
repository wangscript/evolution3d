#ifndef _BASIC_MATERIAL_H__
#define _BASIC_MATERIAL_H__

#include "Renderer/xMaterial.h"
#include "RenderApi/xShaderManager.h"
#include "Renderer/xDrawElement.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASRD_API_ csMaterial : public xMaterial
{
    DECL_BASE_OBJECT_DLLSAFE(csMaterial);
public:
    csMaterial(IRenderApi* pRenderApi);
    ~csMaterial();
    virtual bool begin(IGpuProgramParamTable* pConstTable = NULL);
    virtual bool end(IGpuProgramParamTable* pConstTable = NULL);
    virtual bool load(const wchar_t* matName , const int8* buf , int len);

public:
	virtual bool load(xXmlNode* pNode);
protected:
    HGpuProgram            m_hGpuPrograme;
    IGpuProgramParamTable* m_pOldGpuCbTable;
    IDepthStencilState*    m_pDepthStencil;
    IBlenderState*         m_pBlender;
    IRasterizerState*      m_pRasterizer;
};

class _XEVOL_BASRD_API_ csRenderTech
{
public:
    csRenderTech(IRenderApi* pRenderApi);
    virtual ~csRenderTech();
};

class _XEVOL_BASRD_API_ csRenderPass    : public xRenderPass
{
public:
    csRenderPass(IRenderApi* pRenderApi);
    virtual ~csRenderPass();
};

class _XEVOL_BASRD_API_ csRenderEffect : public IRenderEffect
{
public:
    csRenderEffect(IRenderApi* pRenderApi);
    virtual ~csRenderEffect();
};

END_NAMESPACE_XEVOL3D
#endif


