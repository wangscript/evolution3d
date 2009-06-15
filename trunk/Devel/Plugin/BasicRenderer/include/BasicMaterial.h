#ifndef _BASIC_MATERIAL_H__
#define _BASIC_MATERIAL_H__

#include "Renderer/xMaterial.h"
#include "RenderApi/xShaderManager.h"
#include "Renderer/xDrawElement.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASRD_API_ xBasicMaterial : public xMaterial
{
    DECL_BASE_OBJECT_DLLSAFE(xBasicMaterial);
public:
    xBasicMaterial(IRenderApi* pRenderApi);
    ~xBasicMaterial();
    virtual bool begin(IGpuProgramParamTable* pConstTable = NULL);
    virtual bool end(IGpuProgramParamTable* pConstTable = NULL);
    virtual bool load(const wchar_t* matName , const int8* buf , int len);
protected:
    HGpuProgram            m_hGpuPrograme;
    IGpuProgramParamTable* m_pOldGpuCbTable;
    IDepthStencilState*    m_pDepthStencil;
    IBlenderState*         m_pBlender;
    IRasterizerState*      m_pRasterizer;
};

class _XEVOL_BASRD_API_ xBasicRenderTech
{
public:
    xBasicRenderTech(IRenderApi* pRenderApi);
    virtual ~xBasicRenderTech();
};

class _XEVOL_BASRD_API_ xBasicRenderPass    : public xRenderPass
{
public:
    xBasicRenderPass(IRenderApi* pRenderApi);
    virtual ~xBasicRenderPass();
};

class _XEVOL_BASRD_API_ xBasicRenderEffect : public xRenderEffect
{
public:
    xBasicRenderEffect(IRenderApi* pRenderApi);
    virtual ~xBasicRenderEffect();
};

END_NAMESPACE_XEVOL3D
#endif


