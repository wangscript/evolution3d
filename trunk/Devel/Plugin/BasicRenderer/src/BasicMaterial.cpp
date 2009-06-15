#include "../include/BasicRenderer.h"
#include "../include/BasicMaterial.h"

BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_DLLSAFE(xBasicMaterial , xMaterial);
xBasicMaterial::xBasicMaterial(IRenderApi* pRenderApi)
:xMaterial(pRenderApi)
{
    m_pOldGpuCbTable = NULL;
    m_pDepthStencil = NULL;
    m_pBlender = NULL;
    m_pRasterizer = NULL;
}

xBasicMaterial::~xBasicMaterial()
{
    XSAFE_RELEASEOBJECT(m_pDepthStencil);
    XSAFE_RELEASEOBJECT(m_pBlender     );
    XSAFE_RELEASEOBJECT(m_pRasterizer  );
}

bool xBasicMaterial::load(const wchar_t* matName , const int8* buf , int len)
{
    xXmlDocument doc;
    if(doc.load(matName , true) == false)
        return false;

	//shader配置
    xXmlNode* pXMLShader = doc.findNode(L"shader");
    if(pXMLShader)
    {
        m_hGpuPrograme = m_pRenderApi->gpuProgramManager()->load(pXMLShader->value(L"name"));
    }
    xXmlNode* pXMLRenderState = doc.findNode(L"state");

    //渲染状态
    if(pXMLRenderState)
    {
        const wchar_t* depthStencil = pXMLRenderState->value(L"depth");
        const wchar_t* blender      = pXMLRenderState->value(L"blend");
        const wchar_t* rasterizer   = pXMLRenderState->value(L"raster");
        if(depthStencil) m_pDepthStencil = m_pRenderApi->createDepthStencilState(depthStencil);
        if(blender     ) m_pBlender      = m_pRenderApi->createBlendState(blender);
        if(rasterizer  ) m_pRasterizer   = m_pRenderApi->createRasterizerState(rasterizer);
    }

    return true;
}

bool xBasicMaterial::begin(IGpuProgramParamTable* pConstTable )
{
    if(m_hGpuPrograme.getResource() == NULL)
        return false;
    if(pConstTable)
    {
        m_pOldGpuCbTable = m_hGpuPrograme->getParamTable();
        m_hGpuPrograme->setParamTable(pConstTable);
    }
    m_pRenderApi->pushGpuProgram(m_hGpuPrograme.getResource() );

    if(m_pDepthStencil ) m_pRenderApi->setDepthStencilState( m_pDepthStencil )  ;
    if(m_pBlender      ) m_pRenderApi->setBlendState       ( m_pBlender      )  ;
    if(m_pRasterizer   ) m_pRenderApi->setRasterizerState  ( m_pRasterizer   )  ;  

    return true;
}

bool xBasicMaterial::end(IGpuProgramParamTable* pConstTable )
{
    if(pConstTable)
    {
        m_hGpuPrograme->setParamTable(m_pOldGpuCbTable);
        m_pOldGpuCbTable = NULL;
    }
    return m_pRenderApi->popGpuProgram();
}



//////////////////////////////////////////////////////////////////////////
//控制所有的Queue怎么绘制
xBasicRenderTech::xBasicRenderTech(IRenderApi* pRenderApi)
{

}

xBasicRenderTech::~xBasicRenderTech()
{

}

///绘制的一个Pass
xBasicRenderPass::xBasicRenderPass(IRenderApi* pRenderApi) : xRenderPass(pRenderApi)
{
}
xBasicRenderPass::~xBasicRenderPass()
{

}

//控制一个物体如何绘制，共有几个pass.，
xBasicRenderEffect::xBasicRenderEffect(IRenderApi* pRenderApi) : xRenderEffect(pRenderApi)
{

}
xBasicRenderEffect::~xBasicRenderEffect()
{

}



END_NAMESPACE_XEVOL3D
