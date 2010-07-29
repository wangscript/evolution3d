#include "xKidRenderer.h"
#include "xKidMaterail.h"
#include "Renderer/xRendererQueue.h"
#include "Renderer/xDrawElement.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_DLLSAFE(xKidBasicMaterial , xMaterial);

xKidBasicMaterial::xKidBasicMaterial(IBaseRenderer* pRenderer)
:xMaterial(pRenderer)
{
	m_pOldGpuCbTable = NULL;
	m_pDepthStencil = NULL;
	m_pBlender = NULL;
	m_pRasterizer = NULL;
	m_pRenderApi = pRenderer->renderApi();
	m_AlphaRefValue = -1.0f;
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

IGpuProgram*  xKidBasicMaterial::gpuProgram() 
{
    return m_hGpuPrograme.getResource();
}

bool  xKidBasicMaterial::setGpuProgram(xGpuProgramName& _name)
{
    HGpuProgram hGpuProgram = m_pRenderer->renderApi()->gpuProgramManager()->load(_name);
    if(hGpuProgram.getResource() == NULL )
        return false;

    m_hGpuPrograme = hGpuProgram;
    return true;
}

bool xKidBasicMaterial::setDepthState(const wchar_t* _name) 
{
	XSAFE_RELEASEOBJECT(m_pDepthStencil);
    m_pDepthStencil = m_pRenderApi->createDepthStencilState(_name);
	return true;
}

bool  xKidBasicMaterial::setRazState(const wchar_t* _name) 
{
	return true;
}


bool xKidBasicMaterial::setBlendState(const wchar_t* _name)
{
	return true;
}

bool xKidBasicMaterial::load(xXmlNode* pNode)
{
	if(pNode == NULL )
		return false;

	m_AlphaRefValue = -1.0f;
	if(pNode->float_value(L"AlphaRefValue"))
	   m_AlphaRefValue = pNode->float_value(L"AlphaRefValue");
	//shader配置
	xXmlNode* pXMLShader = pNode->findNode(L"shader");
	if(pXMLShader)
	{
        const wchar_t* shaderName = pXMLShader->value(L"name");
		m_hGpuPrograme = m_pRenderer->renderApi()->gpuProgramManager()->load( shaderName );
        //if(m_hGpuPrograme.getResource() )
        //{
        //    xGpuProgNameParser _parser;
        //    m_hGpuPrograme->getName( _parser );
        //    xGpuProgramName _name;
        //    _parser.toName(_name);
        //    xShaderName* pShaderName = _parser.getShaderName(eShader_VertexShader);

        //    
        //}
	}
	xXmlNode* pXMLRenderState = pNode->findNode(L"state");

	//渲染状态
	if(pXMLRenderState)
	{
		const wchar_t* depthStencil = pXMLRenderState->value(L"depth");
		const wchar_t* blender      = pXMLRenderState->value(L"blend");
		const wchar_t* rasterizer   = pXMLRenderState->value(L"raster");
		if(depthStencil && std::wstring(L"null") != depthStencil ) m_pDepthStencil = m_pRenderApi->createDepthStencilState(depthStencil);
		if(blender      && std::wstring(L"null") != blender      ) m_pBlender      = m_pRenderApi->createBlendState(blender);
		if(rasterizer   && std::wstring(L"null") != rasterizer   ) m_pRasterizer   = m_pRenderApi->createRasterizerState(rasterizer);
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
    m_pRenderApi->alphaTestRef(m_AlphaRefValue);
	return true;
}

bool xKidBasicMaterial::end(IGpuProgramParamTable* pConstTable )
{
	if(pConstTable)
	{
		m_hGpuPrograme->setParamTable(m_pOldGpuCbTable);
		m_pOldGpuCbTable = NULL;
	}
	m_pRenderApi->alphaTestRef(-1.0f);
	return m_pRenderApi->popGpuProgram();
}

//=================
IMPL_BASE_OBJECT_DLLSAFE(xKidBasicRenderEffect , IRenderEffect);
xKidBasicRenderEffect::xKidBasicRenderEffect(xKidRenderer* pRenderer, const wchar_t* _name)
: IRenderEffect(pRenderer)
{
	 setQueueName(L"default");
	 m_pRenderApi = pRenderer->renderApi();
	 m_pKidRenderer = pRenderer;
	 m_pRenderPass = m_pKidRenderer->createPass(L"model");
	 m_Name = _name;
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
bool xKidBasicRenderEffect::drawImm(IDrawElement* pObject , IRenderPassArg* pArg , unsigned int passedTime)
{
	m_pRenderPass->setDrawable(pObject);
	m_pRenderPass->setRenderPassArg(pArg);
	return m_pRenderer->drawPass(m_pRenderPass , passedTime , true);
}

bool xKidBasicRenderEffect::draw(IDrawElement* pObject  , IRenderPassArg* pArg )
{
	m_pRenderPass->setRenderPassArg(pArg);
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

bool  xKidBasicRenderEffect::setArg(const wchar_t* _argName , const wchar_t* _argVal)
{
    if(_argName == std::wstring(L"queue"))
    {
        m_QueueName = _argVal;
        return true;
    }
    return false;
}

const wchar_t* xKidBasicRenderEffect::getArg(const wchar_t* _argName) 
{
      if(_argName == std::wstring(L"queue"))
      {
          return m_QueueName.c_str();
      }
      return NULL;
}

bool xKidBasicRenderEffect::setMaterial(xMaterial* pMaterial)
{
	m_pRenderPass->setMaterial(pMaterial);
	return true;
}

bool xKidBasicRenderEffect::save(xXmlNode* pXml)
{
	//加载一些Effect的参数
	XEVOL_WARNNING_NOT_IMPLEMENT_INFO( "Need to save effect's param\n" );
    if(m_shaderName.length() > 0)
    {
        pXml->setValue(L"shader" , m_shaderName.c_str() );
    }
	pXml->setValue(L"queue" , m_QueueName.c_str() );
    return true;
}

bool xKidBasicRenderEffect::load(xXmlNode* pXml)
{
	//加载一些Effect的参数
	XEVOL_WARNNING_NOT_IMPLEMENT_INFO( "Need to load effect's param\n" );
    m_shaderName = L"";
    if(pXml && pXml->value(L"shader") && m_pRenderPass->getMaterial() )
    {
         m_shaderName = pXml->value(L"shader");
         xGpuProgramName _name(m_shaderName.c_str() , true);
         m_pRenderPass->getMaterial()->setGpuProgram( _name );
    }
	if(pXml && pXml->value(L"queue"))
	{
		m_QueueName = pXml->value(L"queue");
	}

	return true;
}
const wchar_t* xKidBasicRenderEffect::name()
{
	return m_Name.c_str();
}

END_NAMESPACE_XEVOL3D
