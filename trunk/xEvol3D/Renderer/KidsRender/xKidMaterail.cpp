#include "xKidRenderer.h"
#include "xKidMaterail.h"
#include "../xRendererQueue.h"
#include "../xDrawElement.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_DLLSAFE(xKidBasicMaterial , xMaterial);

xKidBasicMaterial::xKidBasicMaterial(IRenderApi* pRenderApi)
:xMaterial(pRenderApi)
{
	m_pOldGpuCbTable = NULL;
	m_pDepthStencil = NULL;
	m_pBlender = NULL;
	m_pRasterizer = NULL;
}

xKidBasicMaterial::~xKidBasicMaterial()
{
	XSAFE_RELEASEOBJECT(m_pDepthStencil);
	XSAFE_RELEASEOBJECT(m_pBlender     );
	XSAFE_RELEASEOBJECT(m_pRasterizer  );
}

bool xKidBasicMaterial::load(const wchar_t* matName , const int8* buf , int len)
{
	xXmlDocument doc;
	if(doc.load(matName , true) == false)
		return false;

	return load( doc.root() );
}

bool xKidBasicMaterial::load(xXmlNode* pNode)
{
	if(pNode == NULL )
		return false;

	//shader配置
	xXmlNode* pXMLShader = pNode->findNode(L"shader");
	if(pXMLShader)
	{
		m_hGpuPrograme = m_pRenderApi->gpuProgramManager()->load(pXMLShader->value(L"name"));
	}
	xXmlNode* pXMLRenderState = pNode->findNode(L"state");

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

bool xKidBasicMaterial::begin(IGpuProgramParamTable* pConstTable )
{
	if(m_hGpuPrograme.getResource() == NULL)
		return false;
	if(pConstTable)
	{
		m_pOldGpuCbTable = m_hGpuPrograme->getParamTable();
		m_hGpuPrograme->setParamTable(pConstTable);
	}
	m_pRenderApi->pushGpuProgram(m_hGpuPrograme.getResource() );

	m_pRenderApi->setDepthStencilState( m_pDepthStencil )  ;
	m_pRenderApi->setBlendState       ( m_pBlender      )  ;
	m_pRenderApi->setRasterizerState  ( m_pRasterizer   )  ;  

	return true;
}

bool xKidBasicMaterial::end(IGpuProgramParamTable* pConstTable )
{
	if(pConstTable)
	{
		m_hGpuPrograme->setParamTable(m_pOldGpuCbTable);
		m_pOldGpuCbTable = NULL;
	}
	return m_pRenderApi->popGpuProgram();
}

//=================
IMPL_BASE_OBJECT_DLLSAFE(xKidBasicRenderEffect , IRenderEffect);
xKidBasicRenderEffect::xKidBasicRenderEffect(IRenderApi* pRenderApi , xKidRenderer* pRenderer)
: IRenderEffect(pRenderApi , pRenderer)
{
	 setQueueName(L"default");
	 m_pRenderApi = pRenderApi;
	 m_pRenderer  = pRenderer;
	 m_pRenderPass = m_pRenderer->createPass(L"model");
}

xKidBasicRenderEffect::~xKidBasicRenderEffect()
{
	m_pRenderPass->ReleaseObject();
	m_pRenderPass = NULL;
}
bool xKidBasicRenderEffect::setQueueName(const wchar_t* queueName)
{
	m_QueueName = queueName;
	return true;
}
bool xKidBasicRenderEffect::draw(IDrawElement* pObject , unsigned int passedTime)
{
	m_pRenderPass->setDrawable(pObject);
	return m_pRenderer->drawPass(m_pRenderPass , passedTime , true);
}

bool xKidBasicRenderEffect::draw(IDrawElement* pObject)
{
	m_pRenderPass->setDrawable(pObject);
	
	int queueID = xStringHash(m_QueueName.c_str() );
    if( true != m_pRenderer->drawPass( queueID , m_pRenderPass) )
	{
		XEVOL_LOG(eXL_ERROR_NORMAL , L"Draw pass failed , queue=");
		XEVOL_LOG(eXL_ERROR_NORMAL , m_QueueName.c_str() );
		XEVOL_LOG(eXL_ERROR_NORMAL , L"\n");
	}
	return true;
}

bool xKidBasicRenderEffect::setMaterial(xMaterial* pMaterial)
{
	m_pRenderPass->setMaterial(pMaterial);
	return true;
}

bool xKidBasicRenderEffect::save(xXmlNode* pXml)
{
	//加载一些Effect的参数
	XEVOL_WARNNING_NOT_IMPLEMENT_INFO( L"Need to save effect's param\n" );
    return true;
}

bool xKidBasicRenderEffect::load(xXmlNode* pXml)
{
	//加载一些Effect的参数
	XEVOL_WARNNING_NOT_IMPLEMENT_INFO( L"Need to load effect's param\n" );
	return true;
}

END_NAMESPACE_XEVOL3D
